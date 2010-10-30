<?php
$roles = element_children($form);
$header = array(t('Role'), t('View'), t('Edit'), t('Delete'));
foreach ($roles as $role) {
  $row = array();
  $row[] = $form[$role]['name']['#value'];
  $row[] = drupal_render($form[$role]['grant_view']);
  $row[] = drupal_render($form[$role]['grant_update']);
  $row[] = drupal_render($form[$role]['grant_delete']);
  $rows[] = $row;
}
echo theme('table', array('header' => $header, 'rows' => $rows));
?>