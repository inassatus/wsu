from flask import jsonify
from bs4 import BeautifulSoup
from urllib2 import urlopen
import requests

base = 'http://localhost:5000/home/courses/add'
url = 'https://www.catalog.wsu.edu/Pullman/Courses/BySubject/CPT_S'
page = urlopen(url)
soup = BeautifulSoup(page, 'html.parser')

courses = soup.find_all('p', class_='course')
for c in courses:
    name_title_box = c.find('span', class_='course_header')
    course_name = name_title_box.text[0:3]
    desc_box = c.find('span', class_='course_data')
    course_title = name_title_box.text[4:]
    course_desc = desc_box.text
    r = requests.post(base, json={'name': course_name,
                                    'title': course_title,
                                        'description': course_desc})
