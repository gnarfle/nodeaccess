<?php

function nodeaccess_admin_form($form, &$form_state, $type) {
  drupal_add_js(drupal_get_path('module', 'nodeaccess') . '/nodeaccess-admin.js');
  
  $type = $type->type;
    
  // Set defaults from variable_get.
  $show = variable_get('nodeaccess-types', array());
  $allowed_roles = variable_get('nodeaccess-roles', array());
  $allowed_grants = variable_get('nodeaccess-grants-' . $type, array());

  $settings = variable_get('nodeaccess');
  $roles = user_roles();
  
  $form['nodeaccess']['content-types'][$type]['pernode'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Per Node Settings'),
    '#tree' => TRUE
  );
  
  $form['nodeaccess']['content-types'][$type]['pernode']['enable'] = array('#type' => 'checkbox',
                            '#title' => t('Allow per node access control'),
                            '#default_value' => 1,
                            '#description' => '<small>' . t('If you check this box, users with appropriate permision will be able to configure per node permissions on the node add/edit form.') . '</small>');
                           

  $form['nodeaccess']['content-types'][$type]['pernode']['help'] = array('#markup' => t('Select which permissions you want to be available on the node add/edit form. For example, if you want users to be able to assign view permissions but not edit or delete, only select view here.'));
  $form['nodeaccess']['content-types'][$type]['pernode']['view'] = array('#type' => 'checkbox', '#title' => t('View'), '#default_value' => $allowed_grants['view']);
  $form['nodeaccess']['content-types'][$type]['pernode']['edit'] = array('#type' => 'checkbox', '#title' => t('Edit'), '#default_value' => $allowed_grants['edit']);
  $form['nodeaccess']['content-types'][$type]['pernode']['delete'] = array('#type' => 'checkbox', '#title' => t('Delete'), '#default_value' => $allowed_grants['delete']);

  // Select roles the permissions of which you want to allow users to
  // view and edit, and the aliases and weights of those roles.
  $form['nodeaccess']['content-types'][$type]['role'] = array('#type' => 'fieldset',
                        '#collapsible' => TRUE,
                        '#collapsed' => TRUE,
                        '#title' => t('Allowed Roles'),
                        '#tree' => TRUE,
                        '#theme' => 'nodeaccess_admin_form_roles',
                        '#description' => '<small>' . t('The selected roles will be listed on individual node add/edit forms. If you wish for certain roles to be hidden from users, make sure they are not selected here.') . '</small>');
  foreach ($roles as $id => $role) {
    $form['nodeaccess']['content-types'][$type]['role'][$id]['name'] = array('#type' => 'hidden', '#value' => $role);
    $form['nodeaccess']['content-types'][$type]['role'][$id]['allow'] = array('#type' => 'checkbox', '#title' => $role, '#default_value' => $allowed_roles[$id]);
  }

  $form['nodeaccess']['content-types'][$type]['roles']['grants'] = array('#type' => 'fieldset',
                                                              '#collapsible' => TRUE,
                                                              '#collapsed' => TRUE,
                                                              '#title' => 'Role Permissions',
                                                              '#tree' => TRUE,
                                                              '#theme' => 'nodeaccess_admin_roles');
                                                              
  foreach( $roles as $id => $role ) {
    $form['nodeaccess']['content-types'][$type]['roles']['grants'][$id]['name'] = array('#type' => 'hidden', '#value' => $role);
    $form['nodeaccess']['content-types'][$type]['roles']['grants'][$id]['grant_view'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
    $form['nodeaccess']['content-types'][$type]['roles']['grants'][$id]['grant_update'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
    $form['nodeaccess']['content-types'][$type]['roles']['grants'][$id]['grant_delete'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
  }
  
  // user based permissions
  $form['nodeaccess']['content-types'][$type]['users']['grants'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title'  => t('User Permissions'),
    '#tree'   => TRUE,
    '#theme'  => 'nodeaccess_admin_users'
  );
  
  $users = array();
  foreach( $users as $id => $user ) {
    $form['nodeaccess']['content-types'][$type]['users']['grants'][$id]['name'] = array('#type' => 'markup', '#value' => $user);
    $form['nodeaccess']['content-types'][$type]['users']['grants'][$id]['grant_view'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
    $form['nodeaccess']['content-types'][$type]['users']['grants'][$id]['grant_update'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
    $form['nodeaccess']['content-types'][$type]['users']['grants'][$id]['grant_delete'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
  }
  
  // last row to add a new user
  $form['nodeaccess']['content-types'][$type]['users']['grants']['new']['name'] = array('#type' => 'textfield', '#size' => 20, '#autocomplete_path' => 'user/autocomplete', '#suffix' => " <input type='button' id='add-user' value='Add' />");
  $form['nodeaccess']['content-types'][$type]['users']['grants']['new']['grant_view'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
  $form['nodeaccess']['content-types'][$type]['users']['grants']['new']['grant_update'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
  $form['nodeaccess']['content-types'][$type]['users']['grants']['new']['grant_delete'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
  
  //TODO  user reference permissions
  
  
  
  // // Generate fieldsets for each node type.
  // foreach (node_get_types() as $type => $name) {
  //   $form['nodeaccess'][$type] = array('#type' => 'fieldset',
  //                                      '#collapsible' => TRUE,
  //                                      '#collapsed' => TRUE,
  //                                      '#title' => $name->name,
  //                                      '#tree' => TRUE,
  //                                      '#theme' => 'nodeaccess_admin_form_types');
  // 
  //   $form['nodeaccess'][$type]['show'] = array('#type' => 'checkbox',
  //                                              '#title' => t('Show grant tab for this node type'),
  //                                              '#default_value' => $show[$type]);
  // 
  //   // Set default author permissions for node type.
  //   $author_prefs = variable_get('nodeaccess_authors', array());
  //   $form['nodeaccess'][$type]['author']['grant_view'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
  //   $form['nodeaccess'][$type]['author']['grant_update'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_update']);
  //   $form['nodeaccess'][$type]['author']['grant_delete'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_delete']);
  // 
  //   $perms = variable_get('nodeaccess_' . $type, array());
  //   foreach ($perms as $perm) {
  //     $opts[$perm['gid']] = $perm;
  //   }
  //   // Set default role permissions for node type.
  //   foreach (user_roles() as $id => $role) {
  //     $form['nodeaccess'][$type]['roles'][$id]['name'] = array('#value' => $role);
  //     $form['nodeaccess'][$type]['roles'][$id]['grant_view'] = array('#type' => 'checkbox', '#default_value' => $opts[$id]['grant_view']);
  //     $form['nodeaccess'][$type]['roles'][$id]['grant_update'] = array('#type' => 'checkbox', '#default_value' => $opts[$id]['grant_update']);
  //     $form['nodeaccess'][$type]['roles'][$id]['grant_delete'] = array('#type' => 'checkbox', '#default_value' => $opts[$id]['grant_delete']);
  //   }
  // 
  //   // Set the default permissions if userreference exists and is enabled on
  //   // the content type.
  //   if (module_exists('userreference')) {
  //     $type_info = content_types($type);
  //     $fields = $type_info['fields'];
  // 
  //     $userreference_perms = variable_get('nodeaccess_' . $type . '_userreference', array());
  //     foreach ($fields as $field) {
  //       if ($field['type'] == 'userreference') {
  //         $form['nodeaccess'][$type]['userreference'][$field['field_name']]['name'] = array('#value' => $field['widget']['label']);
  //         $form['nodeaccess'][$type]['userreference'][$field['field_name']]['enabled'] = array('#type' => 'checkbox', '#default_value' => $userreference_perms[$field['field_name']]['enabled']);
  //         $form['nodeaccess'][$type]['userreference'][$field['field_name']]['grant_view'] = array('#type' => 'checkbox', '#default_value' => $userreference_perms[$field['field_name']]['grant_view']);
  //         $form['nodeaccess'][$type]['userreference'][$field['field_name']]['grant_update'] = array('#type' => 'checkbox', '#default_value' => $userreference_perms[$field['field_name']]['grant_update']);
  //         $form['nodeaccess'][$type]['userreference'][$field['field_name']]['grant_delete'] = array('#type' => 'checkbox', '#default_value' => $userreference_perms[$field['field_name']]['grant_delete']);
  //       }
  //     }
  //   }
  // }
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save Grants'));
  return $form;
}
