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