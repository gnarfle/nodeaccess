var timeout;

(function ($) {
  Drupal.behaviors.nodeaccessAdmin = {
    attach: function (context) {
      $('#dialog').dialog({
        autoOpen: false,
        show: "blind",
        hide: "explode",
        buttons: {
          "Add users": function() {
            
          }
        } 
      });

      
      // submit handler
      $('a#add-user', context).once().click( function() {
        $( "#dialog" ).dialog( "open" );
        return false;
      });
      
      $('#edit-grants-filter', context).once().keypress(function() {
        // set a timeout
        clearTimeout(timeout);
        timeout = setTimeout(function() {
          $.post(Drupal.settings.basePath + 'nodeaccess/usersearch', {filter: $('#edit-grants-filter').val()}, function(response) {
            $('#user-list').html(response);
          });
        }, 3000);
        
      });
      
      // hijack the pager to make it work ajaxily
      $('#user-list ul.pager a', context).live('click', function() {
        var link = $(this).attr('href');
        var parts = link.split('?');
        var params = '?' + parts[1]
        $.post(Drupal.settings.basePath + 'nodeaccess/usersearch' + params, {filter: $('#edit-grants-filter').val()}, function(response) {
          $('#user-list').html(response);
        });
        return false;
      });
    }
  }

})(jQuery);