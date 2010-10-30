(function ($) {
  Drupal.behaviors.nodeaccessAdmin = {
    attach: function (context) {
      console.log('yep');
      $('input#edit-grants-new-name', context).val('Add user...').focus(function() {
        if ( $(this).val() == 'Add user...') { 
          $(this).val(''); 
        }
      });
      
      // submit handler
      $('input#add-user', context).click(function() {
        if ($('input#edit-grants-new-name').length > 0 && $('input#edit-grants-new-name').val() != 'Add user...') {
          $.post(Drupal.settings.basePath + 'nodeaccess/usersearch', { user: $('input#edit-grants-new-name').val()}, function(response) {
            console.log(response);
          });
          $(this).parents('.fieldset-wrapper').html('foobar');
        }
      });
    }
  }
})(jQuery);