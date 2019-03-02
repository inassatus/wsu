var Login = (function(){
  var base = 'http://localhost:5000';
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

    var log_in_handler = function(){
      var submit = document.getElementsByName('submit')[0];

      $(submit).on('click', function(){
        var onSuccess = function(){

        };
        var onFailure = function(){

        };
        get_request('/login', onSuccess, onFailure);
      });


    };



  var start = function(){
    log_in_handler();
  };


  return {
        start: start
    };
})();
