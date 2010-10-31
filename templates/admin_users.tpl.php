<?php
$users = element_children($form);
$header = array(t('User'), t('View'), t('Edit'), t('Delete'));
foreach ($users as $user) {
  $row = array();
  $row[] = drupal_render($form[$user]['name']);
  $row[] = drupal_render($form[$user]['grant_view']);
  $row[] = drupal_render($form[$user]['grant_update']);
  $row[] = drupal_render($form[$user]['grant_delete']);
  $rows[] = $row;
}

echo theme('table', array('header' => $header, 'rows' => $rows));
?>
<a id="add-user">Add User...</a>

<div id="dialog" title="Add User">
  <?php echo drupal_render($form['filter']); ?>
  <div id="user-list">
    <?php echo drupal_render($form['search']); ?>
  </div>
</div>
