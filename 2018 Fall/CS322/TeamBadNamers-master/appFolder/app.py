from flask import Flask, render_template, redirect, url_for, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from flask_login import login_required, LoginManager, UserMixin, login_user, logout_user

from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, RadioField
from wtforms.validators import InputRequired, Email, Length

from passlib.hash import pbkdf2_sha256
import sys
import json


app = Flask(__name__, template_folder='templates')
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///user.db'
#app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///testserver.db'
#above is test server
#id: 0
#username: tests  password: passwords  email: student@wsu.edu  fname: student   lname: wsu
#id: 1
#username: testi  password: passwordi  email: instructor@wsu.edu  fname: instructor   lname: wsu

app.config['SECRET_KEY'] = 'secret'
db = SQLAlchemy(app)
login_manager = LoginManager()

login_manager.init_app(app)
#Common User Base Class
class User(db.Model, UserMixin):
    id = db.Column(db.Integer, primary_key = True)
    username = db.Column(db.String(20), unique = True)
    password_hash = db.Column(db.String(80))
    role = db.Column(db.String(20))


class Student(db.Model):
    id = db.Column(db.Integer, primary_key = True)
    username = db.Column(db.String(20), unique = True)
    password_hash = db.Column(db.String(80))

    email = db.Column(db.String(20),)
    first_name = db.Column(db.String(20))
    last_name = db.Column(db.String(20))
    wsuid = db.Column(db.String(20), unique = True, nullable=True)
    phone = db.Column(db.String(20), unique = True, nullable=True)
    major = db.Column(db.String(20), nullable=True)
    gpa = db.Column(db.String(20), nullable=True)
    graduation_date = db.Column(db.String(20), nullable=True)

    preferences = db.relationship('Preferences', backref='Student', lazy = True)

class Instructor(db.Model):
    id = db.Column(db.Integer, primary_key = True)
    username = db.Column(db.String(20), unique = True)
    password_hash = db.Column(db.String(80))

    email = db.Column(db.String(20), unique = True)
    first_name = db.Column(db.String(20))
    last_name = db.Column(db.String(20))
    wsuid = db.Column(db.BigInteger, unique = True, nullable=True)
    phone = db.Column(db.BigInteger, unique = True, nullable=True)

    teaching = db.relationship('Teaching', backref='Instructor', lazy = True)

class Courses(db.Model):
    id = db.Column(db.Integer, primary_key = True)
    name = db.Column(db.String(20))
    title = db.Column(db.String(20))
    description = db.Column(db.String(80))

class Preferences(db.Model):
    id = db.Column(db.Integer, primary_key = True)
    student_id = db.Column(db.Integer, db.ForeignKey('student.id'))
    name = db.Column(db.String(20))
    title = db.Column(db.String(20))
    description = db.Column(db.String(80))

class Teaching(db.Model):
    id = db.Column(db.Integer, primary_key = True)
    instructor_id = db.Column(db.Integer, db.ForeignKey('instructor.id'))
    name = db.Column(db.String(20))
    title = db.Column(db.String(20))
    description = db.Column(db.String(80))

class Applications(db.Model):
    id = db.Column(db.Integer,primary_key = True)
    student_id = db.Column(db.Integer)
    instructor_id = db.Column(db.Integer)
    course_name = db.Column(db.String())
    grade = db.Column(db.String())
    s_taken = db.Column(db.String())
    s_apply = db.Column(db.String())
    prior_TA = db.Column(db.Boolean())
    status = db.Column(db.String())

class LoginForm(FlaskForm):
    username = StringField('username', validators=[InputRequired(), Length(min=4, max=20)],
        render_kw={"placeholder": "Enter Username"})
    password = PasswordField('password', validators=[InputRequired(), Length(min=2, max=80)],
        render_kw={"placeholder": "Enter Password"})

class RegisterForm(FlaskForm):
    username = StringField('username', validators=[InputRequired(), Length(min=4, max=20)],
        render_kw={"placeholder": "Enter Username"})
    password = PasswordField('password', validators=[InputRequired(), Length(min=2, max=80)],
        render_kw={"placeholder": "Enter Password"})
    email = StringField('email', validators=[InputRequired(), Email(), Length(min=4, max=20)],
        render_kw={"placeholder": "Enter Email"})
    role = RadioField('role', choices=[('student','Student'), ('instructor', 'Instructor')],
        default='student')
    first_name = StringField('first_name', validators=[InputRequired(), Length(min=1, max=20)],
        render_kw={"placeholder": "Enter First Name"})
    last_name = StringField('last_name', validators=[InputRequired(), Length(min=1, max=20)],
        render_kw={"placeholder": "Enter Last Name"})

    #Other Contact information like phone number can be set in the users respective home page



@login_manager.user_loader
def load_user(_id):
    return User.query.filter_by(id=_id).first()

@app.route('/')
def index():
    return render_template('landing.html')

@app.route('/student/info/<name>', methods=['GET'])
def getsinfo(name):
    student=Student.query.filter_by(username=name).first()
    return jsonify({'status': 1, 'info': row_to_sinfo(student)}), 200

@app.route('/student/info/<name>/change', methods=['POST'])
def changesinfo(name):
    student=Student(**request.get_json())
    Student.query.filter_by(username=name).first().wsuid=student.wsuid
    Student.query.filter_by(username=name).first().last_name=student.last_name
    Student.query.filter_by(username=name).first().first_name=student.first_name
    Student.query.filter_by(username=name).first().email=student.email
    Student.query.filter_by(username=name).first().phone=student.phone
    Student.query.filter_by(username=name).first().major=student.major
    Student.query.filter_by(username=name).first().gpa=student.gpa
    Student.query.filter_by(username=name).first().graduation_date=student.graduation_date
    db.session.commit()
    row=Student.query.filter_by(username=name).first()
    return jsonify({'status': 1, 'info': row_to_sinfo(row)}), 200



@app.route('/login', methods=['GET', 'POST'])
def login():
    form = LoginForm()
    if form.validate_on_submit():
        user = User.query.filter_by(username=form.username.data.lower()).first()
        if user:
            if pbkdf2_sha256.verify(form.password.data, user.password_hash):
                login_user(user)
                if user.role == 'student':
                    return redirect('student/home')
                elif user.role == 'instructor':
                    return redirect('instructor/home');

    return render_template('login.html', form=form)


@app.route('/create', methods=['GET', 'POST'])
def create():
    form = RegisterForm()
    if form.validate_on_submit():
        if User.query.filter_by(username=form.username.data.lower()).first():
            return render_template('create.html', form=form)

        user = User()
        user.username = form.username.data.lower()
        user.password_hash = pbkdf2_sha256.hash(form.password.data)
        user.role = form.role.data.lower()

        option = request.form['role']

        if option == 'student':
            student = Student()
            student.username = form.username.data.lower()
            student.password_hash = pbkdf2_sha256.hash(form.password.data)
            student.email = form.email.data.lower()
            student.first_name = form.first_name.data.lower()
            student.last_name = form.last_name.data.lower()

            db.session.add(student)
            print('ADDED STUDENT', file=sys.stderr)
        elif option == 'instructor':
            instructor = Instructor()
            instructor.username = form.username.data.lower()
            instructor.password_hash = pbkdf2_sha256.hash(form.password.data)
            instructor.email = form.email.data.lower()
            instructor.first_name = form.first_name.data.lower()
            instructor.last_name = form.last_name.data.lower()

            db.session.add(instructor)
            print('ADDED INSTRUCTOR', file=sys.stderr)

        db.session.add(user)
        db.session.commit()
        login_user(user)
        if option == 'student':
            return redirect('student/home');
        elif option == 'instructor':
            return redirect('instructor/home');

    return render_template('create.html', form=form)



@app.route('/student/home', methods=['GET'])
@login_required
def student_home():
    return render_template('student_home.html')

@app.route('/instructor/home', methods=['GET'])
@login_required
def instructor_home():
    return render_template('instructor_home.html')


@app.route('/home/courses', methods=['GET'])
# @login_required
def courses():
    course_list = Courses.query.all()

    result = []
    for row in course_list:
        result.append(course_to_obj(row))

    return jsonify({'status': 1, 'courses': result}), 200


@app.route('/home/courses/add', methods=['POST'])
def add_course():
    new_course = Courses(**request.get_json())
    db.session.add(new_course)
    db.session.commit()

    return jsonify({"status": 1, "course": course_to_obj(new_course)}), 200


@app.route('/student/home/preferences/<_username>', methods=['GET'])
# @login_required
def preferences(_username):
    student = Student.query.filter_by(username=_username).first()
    pref_list = student.preferences

    result = []
    for row in pref_list:
        result.append(pref_to_obj(row))

    return jsonify({'status': 1, 'preferences': result}), 200


@app.route('/instructor/home/teaching/<_username>', methods=['GET'])
# @login_required
def teaching(_username):
    instructor = Instructor.query.filter_by(username=_username).first()
    teach_list = instructor.teaching

    result = []
    for row in teach_list:
        result.append(teach_to_obj(row))

    return jsonify({'status': 1, 'teaching': result}), 200


@app.route('/student/home/preferences/<_username>/add', methods=['POST'])
def add_preference(_username):
    new_pref = Preferences(**request.get_json())
    new_pref.student_id = Student.query.filter_by(username=_username).first().id

    db.session.add(new_pref)
    db.session.commit()

    return jsonify({'status': 1, 'preferences': pref_to_obj(new_pref)}), 200


@app.route('/instructor/home/teaching/<_username>/add', methods=['POST'])
def add_teaching(_username):
    new_teach = Teaching(**request.get_json())
    new_teach.instructor_id = Instructor.query.filter_by(username=_username).first().id

    db.session.add(new_teach)
    db.session.commit()

    return jsonify({'status': 1, 'teaching': teach_to_obj(new_teach)}), 200


@app.route('/student/home/preferences/<_username>/delete/<_name>', methods=['DELETE'])
def delete_preference(_username, _name):
    student = Student.query.filter_by(username=_username).first()
    pref_list = student.preferences
    del_pref = next((x for x in pref_list if x.name == _name), None)

    db.session.delete(del_pref)
    db.session.commit()

    return jsonify({'status': 1, 'preferences': pref_to_obj(del_pref)}), 200


@app.route('/instructor/home/teaching/<_username>/delete/<_name>', methods=['DELETE'])
def delete_teaching(_username, _name):
    instructor = Instructor.query.filter_by(username=_username).first()
    teach_list = instructor.teaching
    del_teach = next((x for x in teach_list if x.name == _name), None)

    db.session.delete(del_teach)
    db.session.commit()

    return jsonify({'status': 1, 'teaching': teach_to_obj(del_teach)}), 200

@app.route('/instructor/home/application/<_name>', methods=['GET'])
def applications(_name):
    query = Applications.query.filter_by(course_name=_name).all()
    print(query, file=sys.stderr)
    results = []
    for row in query:
        student_name = Student.query.filter_by(id=row.student_id).first()
        print(student_name, file=sys.stderr)
        row.student_name = student_name.username
        results.append(appl_to_obj_with_student_name(row))


    return jsonify({"status": 1, "applications": results}), 200

@app.route('/student/home/application/<_username>', methods=['GET'])
def my_applications(_username):
    _student_id = Student.query.filter_by(username=_username).first().id
    appl = Applications.query.filter_by(student_id=_student_id).all()

    results = []
    for row in appl:
        results.append(appl_to_obj(row))

    return jsonify({"status": 1, 'applications': results}), 200

@app.route('/student/home/application/<_username>', methods=['POST'])
def add_application(_username):
    appl = Applications(**request.get_json())
    appl.student_id = Student.query.filter_by(username=_username).first().id
    appl.instructor_id = None
    appl.status = "Under Review"

    db.session.add(appl)
    db.session.commit()
    return jsonify({"status": 1, "applications": appl_to_obj(appl)}), 200

@app.route('/home/application/<_username_s>/<_name>', methods=['GET','PUT'])
def update_application(_username_s, _name):
    _student_id = Student.query.filter_by(username=_username_s).first().id
    appl = Applications.query.filter_by(student_id=_student_id).filter_by(course_name=_name).first()
    if request.method == 'PUT':

        data = json.loads(request.data)

        appl.instructor_id = data['instructor_id']
        appl.status = data['status']
        db.session.commit()
        return jsonify({"status": 1, "applications": appl_to_obj(appl)}), 200

    if request.method == 'GET':

        return jsonify({'status': 1, 'applications': appl_to_obj(appl)}), 200

@app.route('/student/home/application/<_username>/<_name>/delete', methods=['DELETE'])
def delete_application(_username, _name):
    _student_id = Student.query.filter_by(username=_username).first().id
    appl = Applications.query.filter_by(student_id=_student_id).filter_by(course_name=_name).first()

    db.session.delete(appl)
    db.session.commit()

    return jsonify({'status': 1, 'applications': appl_to_obj(appl)}), 200


@app.route('/logout')
def logout():
    logout_user()
    return redirect('login')


def course_to_obj(row):
    row = {
        "id": row.id,
        "name": row.name,
        "title": row.title,
        "description": row.description
    }
    return row


def pref_to_obj(row):
    row = {
        "id": row.id,
        "student_id": row.student_id,
        "name": row.name,
        "title": row.title,
        "description": row.description
    }
    return row


def teach_to_obj(row):
    row = {
        "id": row.id,
        "instructor_id": row.instructor_id,
        "name": row.name,
        "title": row.title,
        "description": row.description
    }
    return row

def appl_to_obj(row):
    row = {
        "id": row.id,
        "student_id": row.student_id,
        "instructor_id": row.instructor_id,
        "course_name": row.course_name,
        "grade": row.grade,
        "s_taken": row.s_taken,
        "s_apply": row.s_apply,
        "prior_TA": row.prior_TA,
        "status": row.status
    }
    return row

def appl_to_obj_with_student_name(row):
    row = {
        "id": row.id,
        "student_id": row.student_id,
        "instructor_id": row.instructor_id,
        "course_name": row.course_name,
        "grade": row.grade,
        "s_taken": row.s_taken,
        "s_apply": row.s_apply,
        "prior_TA": row.prior_TA,
        "status": row.status,
        "student_name": row.student_name
    }
    return row

def row_to_sinfo(row):
    row={
        "id": row.wsuid,
        "email": row.email,
        "first": row.first_name,
        "last": row.last_name,
        "phone": row.phone,
        "major": row.major,
        "gpa": row.gpa,
        "graduation": row.graduation_date
        }
    return row


if __name__ == '__main__':
    db.create_all()
    app.run(debug=True)
