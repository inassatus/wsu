var Home = (function(){
  var base = 'http://localhost:5000';
  var username = $('head').find('meta[name=_name]').attr('content');
  var user_id = $('head').find('meta[name=_id]').attr('content');

  var desktop;
  var bottom_buffer;
  var course_list_desktop;
  var application_desktop;
  var preference_list_desktop;
  var course_desktop;

  var mobile;
  var accordion_btn;
  var course_list_mobile;
  var application_mobile;
  var application_close;
  var submit_desktop;
  var preference_list_mobile;
  var course_mobile;
  var clicked = false;

  var student_name;

  var get_request = function(url, onSuccess, onFailure) {
       $.ajax({
           type: 'GET',
           url: base + url,
           dataType: "json",
           success: onSuccess,
           error: onFailure
       });
   };

   var post_request = function(url, data, onSuccess, onFailure) {
        $.ajax({
            type: 'POST',
            url: base + url,
            data: JSON.stringify(data),
            contentType: "application/json",
            dataType: "json",
            success: onSuccess,
            error: onFailure
        });
   };

   var put_request = function(url, data, onSuccess, onFailure){
     $.ajax({
         type: 'PUT',
         url: base + url,
         data: JSON.stringify(data),
         contentType: "application/json",
         dataType: "json",
         success: onSuccess,
         error: onFailure
     });
   };

   var delete_request = function(url, data, onSuccess, onFailure){
     $.ajax({
       type: 'DELETE',
       url: base + url,
       success: onSuccess,
       error: onFailure
     });

   };

  var teach_post = function(pref){

    var onSuccess = function(data){
      console.log(data);
    };

    var onFailure = function(){
    };

    post_request('/instructor/home/teaching/' + username + '/add', pref, onSuccess, onFailure);
  };

  var get_teach = function(){
    var course_id;
    var course_name;
    var course_desc;
    var onSuccess = function(data){
      var loaded_data = preference_list_desktop.find('.preference_data-d');
      var loaded_data_mobile = preference_list_mobile.find('.preference_data-m');
      for(i = 0; i < data.teaching.length; i++){
        course_id = data.teaching[i].id;
        course_name = data.teaching[i].name;
        course_desc = data.teaching[i].title + "<br>" + data.teaching[i].description;

        var course_txt_d = "<div class='card color'>\n\t<div id='CPT_S-" + course_id + "-d' class='card-header'>\n\t<h3 class='mb-0 text-center'>\n\t<button class='btn btn-outline-secondary border-light btn-block my-btn collapsed' data-toggle='collapse' data-target='#CPT_S-" + course_name + "-d-p'>\n\t" + "CPT_S " + course_name + "\n</button>\n</h3>\n</div>\n\t<div id='CPT_S-"+ course_name + "-d-p' class='collapse' data-parent='#accordion_pref-d'>\n\t<div class='card-body'>" + course_desc +
        "<br>\n\t<button class='btn btn-outline-secondary border-light float-right my-btn m-2 delete_desktop'>\n\tDelete\n</button>\n</div>\n</div>\n</div>\n";

        var course_d = $(course_txt_d);

        var apply_btn_desktop = course_desktop.find('#CPT_S-' + course_name + '-d').find('.card-body').find('button');
        apply_btn_desktop.removeClass('btn-outline-secondary my-btn');
        apply_btn_desktop.addClass('btn-success');
        apply_btn_desktop.html('Applied');
        apply_btn_desktop.attr('disabled', true);

        var course_txt_m = "<div class='card color'>\n\t<div id='CPT_S-" + course_id + "-m' class='card-header'>\n\t<h3 class='mb-0 text-center'>\n\t<button class='btn btn-outline-secondary border-light btn-block my-btn collapsed' data-toggle='collapse' data-target='#CPT_S-" + course_name + "-m-p'>\n\t" + "CPT_S " + course_name + "\n</button>\n</h3>\n</div>\n\t<div id='CPT_S-"+ course_name + "-m-p' class='collapse' data-parent='#accordion_pref-m'>\n\t<div class='card-body'>" + course_desc + "<br>\n\t<button class='btn btn-outline-secondary border-light float-right my-btn m-2 delete_mobile'>\n\tDelete\n</button>\n</div>\n</div>\n</div>\n";

        var course_m = $(course_txt_m);

        get_TA(course_name, course_d, course_m);


        var apply_btn_mobile = course_mobile.find('#CPT_S-' + course_name + '-m').find('.card-body').find('button');
        apply_btn_mobile.removeClass('btn-outline-secondary my-btn');
        apply_btn_mobile.addClass('btn-success');
        apply_btn_mobile.html('Applied');
        apply_btn_mobile.attr('disabled', true);

      }
    };
    var onFailure = function(){
      alert('Failed To Load Classes Teaching');
    };

    get_request('/instructor/home/teaching/' + username, onSuccess, onFailure);
  };

  var get_TA = function(_name, course_d, course_m){
    var onSuccess = function(data){
      var btn_d = course_d.find('.card-body').html();
      var string_d = "";
      btn_d.split('<button').splice(1,1);
      btn_d = '<button' + btn_d.split('<button').splice(1,1);
      course_d.find('.card-body').html('');
      for(var i in data.applications){
        string_d +='<button class="btn btn-outline-secondary border-light m-2 my-btn TA">' + data.applications[i]['student_name'] + '</button>';
      }
      course_d.find('.card-body').html(string_d);
      course_d.find('.card-body').append(btn_d);

      var btn_m = course_m.find('.card-body').html();
      var string_m = "";
      btn_m.split('<button').splice(1,1);
      btn_m = '<button' + btn_m.split('<button').splice(1,1);
      course_m.find('.card-body').html('');
      for(var i in data.applications){
        string_m +='<button class="btn btn-outline-secondary border-light m-2 my-btn TA">' + data.applications[i]['student_name'] + '</button>';
      }
      course_m.find('.card-body').html(string_m);
      course_m.find('.card-body').append(btn_m);

      preference_list_desktop.find('.preference_data-d').append(course_d);
      preference_list_mobile.find('.preference_data-m').append(course_m);


    };
    var onFailure = function(){
      alert('Failed to load TAs');
    }
    get_request('/instructor/home/application/' + _name, onSuccess, onFailure);
  };

  var delete_teach = function(teach){
    var onSuccess = function(data){
      console.log(data);

    };

    var onFailure = function(){
    };

    delete_request('/instructor/home/teaching/' + username + '/delete/' + teach.name, onSuccess, onFailure);
  };


  var course_load = function() {
    var course_id;
    var course_name;
    var course_desc;
       var onSuccess = function(data) {
         var loaded_data = desktop.find('.course_data')
         var loaded_data_mobile = mobile.find('.course_data');

         for(i = 0; i < data.courses.length; i++){
           course_id = data.courses[i].id;
           course_name = data.courses[i].name;
           course_desc = data.courses[i].title + "<br>" + data.courses[i].description;
           loaded_data.append("<div class='card color'>\n\t<div id='CPT_S-" + course_id + "' class='card-header'>\n\t<h3 class='mb-0 text-center'>\n\t<button class='btn btn-outline-secondary border-light btn-block my-btn collapsed' data-toggle='collapse' data-target='#CPT_S-" + course_name +"-d'>\n\t" + "CPT_S " + course_name + "\n</button>\n</h3>\n</div>\n\t<div id='CPT_S-"+ course_name + "-d' class='collapse' data-parent='#accordion_inner_desktop'>\n\t<div class='card-body'>" + course_desc + "<br>\n\t<button class='btn btn-outline-secondary border-light float-right my-btn m-2 apply_desktop'>\n\tApply\n</button>\n</div>\n</div>\n</div>\n");

           loaded_data_mobile.append("<div class='card color'>\n\t<div id='CPT_S-" + course_id + "' class='card-header'>\n\t<h3 class='mb-0 text-center'>\n\t<button class='btn btn-outline-secondary border-light btn-block my-btn collapsed' data-toggle='collapse' data-target='#CPT_S-" + course_name +"-m'>\n\t" + "CPT_S " + course_name + "\n</button>\n</h3>\n</div>\n\t<div id='CPT_S-"+ course_name + "-m' class='collapse' data-parent='#accordion_inner_mobile'>\n\t<div class='card-body'>" + course_desc + "<br>\n\t<button class='btn btn-outline-secondary border-light float-right my-btn m-2 apply_mobile'>\n\tApply\n</button>\n</div>\n</div>\n</div>\n");
         }

       };
       var onFailure = function () { console.log("Failure...") };

       get_request("/home/courses", onSuccess, onFailure);
    };

   var teach_load = function(){
     get_teach();
   };


   var get_application = function(student_username, course_name){
     var onSuccess = function(data){
       console.log(data);
       application_desktop.find('#course_number').val(data.applications.course_name);
       application_desktop.find('#grade').val(data.applications.grade);
       application_desktop.find('#st').val(data.applications.s_taken);
       application_desktop.find('#sa').val(data.applications.s_apply);
       application_desktop.find('#check').prop('checked', data.applications.prior_TA);

       application_mobile.find('#course_number').val(data.applications.course_name);
       application_mobile.find('#grade').val(data.applications.grade);
       application_mobile.find('#st').val(data.applications.s_taken);
       application_mobile.find('#sa').val(data.applications.s_apply);
       application_mobile.find('#check').prop('checked', data.applications.prior_TA);
     };
     var onFailure = function(){
     };
     get_request('/home/application/' + student_username + '/' + course_name, onSuccess, onFailure);
   };

   var put_appl = function(appl, username_s, course_name){
     var onSuccess = function(data) {
       console.log(data);
     };
     var onFailure = function(){
     };

     put_request('/home/application/' + username_s + '/' + course_name, appl, onSuccess, onFailure);
   }

   var resize_handler = function(){
   //Starting size of the screen effects the placement of divs.

     if($(window).width() < 992){
       mobile.removeClass('d-none');
       desktop.addClass('d-none');
     }
     else{
       mobile.addClass('d-none');
       desktop.removeClass('d-none');
     }


     //Dynamicly change div placement based on screen size
     $(window).on('resize', function(){
         if($(window).width() < 992){
           mobile.removeClass('d-none');
           desktop.addClass('d-none');
           application_desktop.find('#course_number').val(null);
         }
         else{
           mobile.addClass('d-none');
           desktop.removeClass('d-none');
           application_mobile.find('#course_number').val(null);
         }
     });
   };

    //show hidden application form when a course in course_list is selected and inject selected course number to form (Desktop)
    // var course_desktop_handler = function(){
    //   course_list_desktop.on('click', '.apply_desktop', function(){
    //
    //     // application_desktop.removeClass('invisible');
    //     //
    //     // course_list_desktop.removeClass('col-lg-5');
    //     // course_list_desktop.addClass('col-lg-4');
    //     //
    //     // application_desktop.removeClass('col-lg-2');
    //     // application_desktop.addClass('col-lg-4');
    //     //
    //     // preference_list_desktop.removeClass('col-lg-5');
    //     // preference_list_desktop.addClass('col-lg-4');
    //
    //     var course_number = $(this).parents('.collapse').attr('id');
    //     // application_desktop.find('#course_number').val(course_number.substring(6, 9));
    //
    //     var teach = {};
    //     teach.name = course_number.substring(6, 9);
    //     var card_body = $(this).parents('.card-body').html();
    //     teach.title = card_body.substring(0, card_body.indexOf('<br>'));
    //     var desc_start = card_body.indexOf('<br>');
    //     teach.description = card_body.split('<br>')[1];
    //     pref_post(teach);
    //
    //
    //   });
    // };

    var application_close_handler = function(){
      application_close.on('click', function(){

        application_desktop.addClass('invisible');

        course_list_desktop.addClass('col-lg-5');
        course_list_desktop.removeClass('col-lg-4');

        application_desktop.addClass('col-lg-2');
        application_desktop.removeClass('col-lg-4');

        preference_list_desktop.addClass('col-lg-5');
        preference_list_desktop.removeClass('col-lg-4');
      });
    };
    var add_teaching_desktop_helper = function(_course_number){
      var status = 'Under Review';

      var course_number = _course_number;

      var course = course_desktop.find('#CPT_S-' + course_number + '-d').parent().clone();

      //change button from "Apply" to "Applied" with disabled green success button
      var apply_btn = course_desktop.find('#CPT_S-' + course_number + '-d').find('.card-body').find('button');
      apply_btn.removeClass('btn-outline-secondary my-btn');
      apply_btn.addClass('btn-success');
      apply_btn.html('Applied');
      apply_btn.attr('disabled', true);

      //create unique card div that is collapsed at first
      course.find('.show').removeClass('show');

      course_id_for_card_header = course.find('.card-header').attr('id');
      course.find('.card-header').attr('id', course_id_for_card_header + '-p');
      course.find('div[data-parent="#accordion_inner_desktop"]').attr('data-parent', '#accordion_pref-d');

      course_id_to_change = course.find('div[data-parent="#accordion_pref-d"]').attr('id');
      course.find('div[data-parent="#accordion_pref-d"]').attr('id', course_id_to_change + '-p');

      course_target_to_change = course.find('.card-header').find('button').attr('data-target');
      course.find('.card-header').find('button').attr('data-target', course_target_to_change + '-p');


      course.find('.card-body').find('button').removeClass('apply_desktop');
      course.find('.card-body').find('button').addClass('delete_desktop');
      //make old apply button into a delete button
      course.find('.card-body').find('button').html('Delete');

      course.find('.card-body')


      return course;
    };

    var add_teaching_mobile_helper = function(_course_number){
      var status = 'Under Review';
      var course_number = _course_number

      var course = course_mobile.find('#CPT_S-' + course_number + '-m').parent().clone();
      //change button from "Apply" to "Applied" with disabled green success button
      var apply_btn = course_mobile.find('#CPT_S-' + course_number + '-m').find('.card-body').find('button');
      apply_btn.removeClass('btn-outline-secondary my-btn');
      apply_btn.addClass('btn-success');
      apply_btn.html('Applied');
      apply_btn.attr('disabled', true);

      //create unique card div that is collapsed at first
      course.find('.show').removeClass('show');

      course_id_for_card_header = course.find('.card-header').attr('id');
      course.find('.card-header').attr('id', course_id_for_card_header + '-p');
      course.find('div[data-parent="#accordion_inner_mobile"]').attr('data-parent', '#accordion_pref-m');

      course_id_to_change = course.find('div[data-parent="#accordion_pref-m"]').attr('id');
      course.find('div[data-parent="#accordion_pref-m"]').attr('id', course_id_to_change + '-p');

      course_target_to_change = course.find('.card-header').find('button').attr('data-target');
      course.find('.card-header').find('button').attr('data-target', course_target_to_change + '-p');


      course.find('.card-body').find('button').removeClass('apply_mobile');
      course.find('.card-body').find('button').addClass('delete_mobile');
      //make old apply button into a delete button
      course.find('.card-body').find('button').html('Delete');


      return course;
    };

    var add_teaching_desktop_handler = function(){
      course_desktop.on('click', '.apply_desktop', function(e){
        //e.preventDefault();
        var course_for_desktop = add_teaching_desktop_helper($(this).parents('.collapse').attr('id').substring(6,9));
        var course_for_mobile = add_teaching_mobile_helper($(this).parents('.collapse').attr('id').substring(6,9));

        var teach = {};
        teach.name = course_for_desktop.find('.collapse').attr('id').substring(6,9);
        var card_body = course_for_desktop.find('.card-body').html()
        teach.title = card_body.substring(0, card_body.indexOf('<br>'));
        var desc_start = card_body.indexOf('<br>');
        teach.description = card_body.split('<br>')[1];

        teach_post(teach);
        get_TA(course_for_desktop.find('.collapse').attr('id').substring(6,9), course_for_desktop, course_for_mobile);


        // preference_list_desktop.find('.preference_data-d').append(course_for_desktop);
        // preference_list_mobile.find('.preference_data-m').append(course_for_mobile);

        //hide application field
        // course_desktop.removeClass('clicked');
        // application_desktop.addClass('invisible');
        //
        // course_list_desktop.addClass('col-lg-5');
        // course_list_desktop.removeClass('col-lg-4');
        //
        // application_desktop.addClass('col-lg-2');
        // application_desktop.removeClass('col-lg-4');
        //
        // preference_list_desktop.addClass('col-lg-5');
        // preference_list_desktop.removeClass('col-lg-4');
      });
    };

    var add_teaching_mobile_handler = function(){
      course_mobile.on('click', '.apply_mobile', function(){
        course_list_mobile.removeClass('clicked');

        preference_list_mobile.removeClass('d-none');

        course_list_mobile.find('#course_list_mobile_collapse').removeClass('show');
        course_list_mobile.addClass('d-none');

        preference_list_mobile.find('#preference_list_mobile_collapse').addClass('show');
        preference_list_mobile.addClass('clicked');


        var course_for_desktop = add_teaching_desktop_helper($(this).parents('.collapse').attr('id').substring(6,9));
        var course_for_mobile = add_teaching_mobile_helper($(this).parents('.collapse').attr('id').substring(6,9));

        // preference_list_desktop.find('.preference_data-d').append(course_for_desktop);
        // preference_list_mobile.find('.preference_data-m').append(course_for_mobile);

        var teach = {};
        teach.name = course_for_mobile.find('.collapse').attr('id').substring(6,9);
        var card_body = course_for_mobile.find('.card-body').html()
        teach.title = card_body.substring(0, card_body.indexOf('<br>'));
        var desc_start = card_body.indexOf('<br>');
        teach.description = card_body.split('<br>')[1];

        teach_post(teach);
        get_TA(course_for_desktop.find('.collapse').attr('id').substring(6,9), course_for_desktop, course_for_mobile);

        mobile.find('#top_buffer_mobile').removeClass('top_buffer');

        // var course_number = $(this).parents('.collapse').attr('id');
        // application_mobile.find('#course_number').val(course_number.substring(6, 9));

      });
    };

    var submit_desktop_handler = function(){
      submit_desktop.on('submit', function(e){
        e.preventDefault();
        var appl = {};
        appl.status = application_desktop.find('#status_select option:selected').text();
        appl.instructor_id = user_id;
        put_appl(appl, student_name, application_desktop.find('#course_number').val());

        //hide application field
        course_desktop.removeClass('clicked');
        application_desktop.addClass('invisible');

        course_list_desktop.addClass('col-lg-5');
        course_list_desktop.removeClass('col-lg-4');

        application_desktop.addClass('col-lg-2');
        application_desktop.removeClass('col-lg-4');

        preference_list_desktop.addClass('col-lg-5');
        preference_list_desktop.removeClass('col-lg-4');

      });
    };

    var submit_mobile_handler = function(){
      submit_mobile.on('submit', function(e){
        e.preventDefault();
        //
        // var course_for_desktop = submit_desktop_helper($(this).parents('.collapse').attr('id').substring(6,9));
        // var course_for_mobile = submit_mobile_helper($(this).parents('.collapse').attr('id').substring(6,9));
        //
        // preference_list_desktop.find('.preference_data-d').append(course_for_desktop);
        // preference_list_mobile.find('.preference_data-m').append(course_for_mobile);
        //
        // clicked = false;
        // mobile.find('#top_buffer_mobile').addClass('top_buffer');
        //
        // application_mobile.removeClass('clicked');
        // application_mobile.find('#application_mobile_collapse').removeClass('show');
        // course_list_mobile.removeClass('d-none');
        // preference_list_mobile.removeClass('d-none');
        //

        var appl = {};
        appl.status = application_mobile.find('#status_select option:selected').text();
        appl.instructor_id = user_id;
        put_appl(appl, student_name, application_mobile.find('#course_number').val());

        clicked = false;
        mobile.find('#top_buffer_mobile').addClass('top_buffer');

        application_mobile.removeClass('clicked');
        application_mobile.find('#application_mobile_collapse').removeClass('show');
        course_list_mobile.removeClass('d-none');
        preference_list_mobile.removeClass('d-none');

      });
    };

    var delete_teach_desktop = function(){
      preference_list_desktop.on('click', '.delete_desktop', function(){
        //revert apply button back to original state
        var course_number = '#' + $(this).parents('.collapse').attr('id').substring(0, 9);

        var apply_btn_desktop = course_desktop.find(course_number + '-d').find('.card-body').find('button');
        apply_btn_desktop.addClass('btn-outline-secondary my-btn');
        apply_btn_desktop.removeClass('btn-success');
        apply_btn_desktop.html('Apply');
        apply_btn_desktop.attr('disabled', false);

        var apply_btn_mobile = course_mobile.find(course_number + '-m').find('.card-body').find('button');
        apply_btn_mobile.addClass('btn-outline-secondary my-btn');
        apply_btn_mobile.removeClass('btn-success');
        apply_btn_mobile.html('Apply');
        apply_btn_mobile.attr('disabled', false);

        //delete preference from preference lists
        $(this).parents('.card').remove();
        preference_list_mobile.find(course_number + '-m-p').parent().remove();


        //add code to delete from backend as well
        var teach = {};
        teach.name = $(this).parents('.collapse').attr('id').substring(6, 9);
        var card_body = $(this).parent().html()
        teach.title = card_body.substring(0, card_body.indexOf('<br>'));
        var desc_start = card_body.indexOf('<br>');
        teach.description = card_body.split('<br>')[1];

        delete_teach(teach);

      });
    };

    var delete_teach_mobile = function(){
      preference_list_mobile.on('click', '.delete_mobile', function(){
        //revert apply button back to original state
        var course_number = '#' + $(this).parents('.collapse').attr('id').substring(0, 9);

        var apply_btn_mobile = course_mobile.find(course_number + '-m').find('.card-body').find('button');
        apply_btn_mobile.addClass('btn-outline-secondary my-btn');
        apply_btn_mobile.removeClass('btn-success');
        apply_btn_mobile.html('Apply');
        apply_btn_mobile.attr('disabled', false);

        var apply_btn_desktop = course_desktop.find(course_number + '-d').find('.card-body').find('button');
        apply_btn_desktop.addClass('btn-outline-secondary my-btn');
        apply_btn_desktop.removeClass('btn-success');
        apply_btn_desktop.html('Apply');
        apply_btn_desktop.attr('disabled', false);

        //delete preference from preference list
        $(this).parents('.card')[0].remove();
        preference_list_desktop.find('CPT_S-' + course_number + '-d-p').parent().remove();

        //add code to delete from backend as well
        var teach = {};
        teach.name = $(this).parents('.collapse').attr('id').substring(6, 9);
        var card_body = $(this).parent().html()
        teach.title = card_body.substring(0, card_body.indexOf('<br>'));
        var desc_start = card_body.indexOf('<br>');
        teach.description = card_body.split('<br>')[1];

        delete_teach(teach);
      });
    };

    var accordion_btn_handler = function(){
      accordion_btn.on('click', function(){
        // Determine which button was clicked
        if($(this).data('target') ==='#course_list_mobile_collapse'){
          if(clicked){
            $(this).parents('.card').removeClass('clicked');
            clicked = false;
            application_mobile.removeClass('d-none');
            preference_list_mobile.removeClass('d-none');

            mobile.find('#top_buffer_mobile').addClass('top_buffer');
          }
          else{
            $(this).parents('.card').addClass('clicked');
            clicked = true;
            application_mobile.addClass('d-none');
            preference_list_mobile.addClass('d-none');

            mobile.find('#top_buffer_mobile').removeClass('top_buffer');
          }
        }
        else if($(this).data('target') ==='#application_mobile_collapse'){
          if(clicked){
            $(this).parents('.card').removeClass('clicked');
            clicked = false;
            course_list_mobile.removeClass('d-none');
            preference_list_mobile.removeClass('d-none');

            mobile.find('#top_buffer_mobile').addClass('top_buffer');
          }
          else{
            $(this).parents('.card').addClass('clicked');
            clicked = true;
            course_list_mobile.addClass('d-none');
            preference_list_mobile.addClass('d-none');

            mobile.find('#top_buffer_mobile').removeClass('top_buffer');
          }
        }
        else if($(this).data('target') ==='#preference_list_mobile_collapse'){
          if(clicked){
            $(this).parents('.card').removeClass('clicked');
            clicked = false;
            application_mobile.removeClass('d-none');
            course_list_mobile.removeClass('d-none');

            mobile.find('#top_buffer_mobile').addClass('top_buffer');
          }
          else{
            $(this).parents('.card').addClass('clicked');
            clicked = true;
            application_mobile.addClass('d-none');
            course_list_mobile.addClass('d-none');

            mobile.find('#top_buffer_mobile').removeClass('top_buffer');
          }
        }
      });
    };

  var view_application_desktop = function(){
    preference_list_desktop.on('click', '.TA', function(){
      get_application($(this).html(), $(this).parents('.collapse').attr('id').substring(6,9));
      application_desktop.removeClass('invisible');

      course_list_desktop.removeClass('col-lg-5');
      course_list_desktop.addClass('col-lg-4');

      application_desktop.removeClass('col-lg-2');
      application_desktop.addClass('col-lg-4');

      preference_list_desktop.removeClass('col-lg-5');
      preference_list_desktop.addClass('col-lg-4');

      student_name = $(this).html();
    });
  };

  var view_application_mobile = function(){
    preference_list_mobile.on('click', '.TA', function(){
      get_application($(this).html(), $(this).parents('.collapse').attr('id').substring(6,9));

      preference_list_mobile.removeClass('clicked');

      application_mobile.removeClass('d-none');

      preference_list_mobile.find('#preference_list_mobile_collapse').removeClass('show');
      preference_list_mobile.addClass('d-none');

      application_mobile.find('#application_mobile_collapse').addClass('show');
      application_mobile.addClass('clicked');

      student_name = $(this).html();

    });
  };

  var start = function(){

    desktop = $('#desktop');
    bottom_buffer = desktop.find('#buffer2');
    course_list_desktop = desktop.find('.row').find('#course_list_desktop');
    application_desktop = desktop.find('.row').find('#application_desktop');
    preference_list_desktop = desktop.find('.row').find('#preference_list_desktop');
    course_desktop = course_list_desktop.find('#accordion_inner_desktop');
    submit_desktop = application_desktop.find('form');

    mobile = $('#accordion');
    accordion_btn = mobile.find('.my-btn');
    course_list_mobile = mobile.find('#course_list_card');
    application_mobile = mobile.find('#application_card');
    application_close = application_desktop.find('#close');
    submit_mobile = application_mobile.find('button[type="submit"]');
    preference_list_mobile = mobile.find('#preference_list_card');
    course_mobile = course_list_mobile.find('#accordion_inner_mobile');
    submit_mobile = application_mobile.find('form');

    resize_handler();
    course_load();
    teach_load();

    submit_desktop_handler();
    //course_desktop_handler();
    application_close_handler();
    add_teaching_desktop_handler();
    delete_teach_desktop();

    add_teaching_mobile_handler();
    submit_mobile_handler();
    accordion_btn_handler();
    delete_teach_mobile();

    view_application_desktop();
    view_application_mobile();

  };


  return {
        start: start
    };
})();
