<?php

/* global configuration for nodeaccess */
function nodeaccess_admin_settings() {
  $settings = nodeaccess_get_settings();
  
  $form['nodeaccess']['settings'] = array('#tree' => true, '#type' => 'fieldset', '#title' => t('Settings'));
  
  $form['nodeaccess']['settings']['defaults_type'] = array(
    '#type'         => 'radios',
    '#title'        => 'Default Permissions Type',
    '#options'      => array(t('Global Defaults'), t('Content Type'), t('Taxonomy') ),
    '#description'  => t('This option determines how the default permissions for nodes are configured. The options are: <ul><li><b>Global Defaults:</b> Choosing this option allows you to set default permissions on this page. This is the best option if you want to have one set of default permissions that can be overridden on a per node basis</li><li><b>Content Type:</b> This option allows you to specify default permissions per content (node) type. Choose this is you want to have different permissions for each node type on your site.</li><li><b>Taxonomy:</b> This Option allows you to set permissions per taxonomy vocabulary/term. Choose this if you want to be able to have different permissions per taxonomy term.</li></ul>'),
    '#default_value'  => $settings['defaults_type'],
    '#attached' => array(
      'js'  => array( drupal_get_path('module', 'nodeaccess') . '/nodeaccess-admin.js' ),
      'css' => array( drupal_get_path('module', 'nodeaccess') . '/nodeaccess.css' ),
      'library' => array( array( 'system', 'ui.dialog' ) )
    )
  );
  
  $form['nodeaccess']['settings']['pernode'] = array('#type' => 'checkbox',
                            '#title' => t('Allow per node access control'),
                            '#default_value' => 1,
                            '#description' => '<small>' . t('If you check this box, users with appropriate permision will be able to configure per node permissions on the node add/edit form.') . '</small>');
                           

  $form['nodeaccess']['settings']['help'] = array('#markup' => t('Select which permissions you want to be available on the node add/edit form. For example, if you want users to be able to assign view permissions but not edit or delete, only select view here.'));
  $form['nodeaccess']['settings']['allow_view'] = array('#default_value' => $settings['allow_view'], '#type' => 'checkbox', '#title' => t('View'));
  $form['nodeaccess']['settings']['allow_edit'] = array('#default_value' => $settings['allow_edit'], '#type' => 'checkbox', '#title' => t('Edit'));
  $form['nodeaccess']['settings']['allow_delete'] = array('#default_value' => $settings['allow_delete'], '#type' => 'checkbox', '#title' => t('Delete'));

  if ( $settings['defaults_type'] === "0" ) {
    $form['nodeaccess']['default_permissions'] = array( '#tree' => TRUE, '#type' => 'fieldset', '#title' => 'Default Permissions');
    $form['nodeaccess']['default_permissions']['settings'] = nodeaccess_permissions_form( );
  }
  return system_settings_form($form);
}

function nodeaccess_admin_form($form, &$form_state, $type) {
  // drupal_add_js('misc/ui/jquery.ui.dialog.min.js');
  // drupal_add_js(drupal_get_path('module', 'nodeaccess') . '/nodeaccess-admin.js');
  // drupal_add_css(drupal_get_path('module', 'nodeaccess') . '/nodeaccess.css');
  // drupal_add_css('/misc/ui/jquery.ui.dialog.css');
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
    '#theme'  => 'nodeaccess_admin_users',
    '#attached' => array(
      'js'  => array( drupal_get_path('module', 'nodeaccess') . '/nodeaccess-admin.js' ),
      'css' => array( drupal_get_path('module', 'nodeaccess') . '/nodeaccess.css' ),
      'library' => array( array( 'system', 'ui.dialog' ) )
    )
  );
  
  $users = array();
  foreach( $users as $id => $user ) {
    $form['nodeaccess']['content-types'][$type]['users']['grants'][$id]['name'] = array('#type' => 'markup', '#value' => $user);
    $form['nodeaccess']['content-types'][$type]['users']['grants'][$id]['grant_view'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
    $form['nodeaccess']['content-types'][$type]['users']['grants'][$id]['grant_update'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
    $form['nodeaccess']['content-types'][$type]['users']['grants'][$id]['grant_delete'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
  }
  
  // user search...
  $header = array(
    'username' => array('data' => t('Username'), 'field' => 'u.name'),
    'mail' => array('data' => t('Email'), 'field' => 'u.mail')
  );

  $query = db_select('users', 'u');
  $query->condition('u.uid', 0, '<>');
  user_build_filter_query($query);

  $count_query = clone $query;
  $count_query->addExpression('COUNT(u.uid)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('u', array('uid', 'name', 'mail'))
    ->limit(1)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();

  $accounts = array();
  foreach ($result as $account) {
    $options[$account->uid] = array(
      'username' => theme('username', array('account' => $account)),
      'mail' => $account->mail,
    );
  }

  $form['nodeaccess']['content-types'][$type]['users']['grants']['filter'] = array(
    '#type' => 'textfield',
    '#title'  => t('Filter Users'),
    '#size'   => 20
  );
  
  $form['nodeaccess']['content-types'][$type]['users']['grants']['search']['accounts'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No people available.'),
  );
  $form['nodeaccess']['content-types'][$type]['users']['grants']['search']['pager'] = array('#markup' => theme('pager'));
  
  // last row to add a new user
  // $form['nodeaccess']['content-types'][$type]['users']['grants']['new']['name'] = array('#type' => 'textfield', '#size' => 20, '#autocomplete_path' => 'user/autocomplete', '#suffix' => " <input type='button' id='add-user' value='Add' />");
  // $form['nodeaccess']['content-types'][$type]['users']['grants']['new']['grant_view'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
  // $form['nodeaccess']['content-types'][$type]['users']['grants']['new']['grant_update'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
  // $form['nodeaccess']['content-types'][$type]['users']['grants']['new']['grant_delete'] = array('#type' => 'checkbox', '#default_value' => $author_prefs[$type]['grant_view']);
  //   
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

function nodeaccess_filter_users($form, $form_state) {
  return 'foo';
}
