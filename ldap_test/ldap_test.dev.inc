<?php

/**
 * @file
 * just a file for developer and test functions
 *
 */

/**
 * function to test provision functions in ldap_user
 * basically a one off of simpletests to make testing and development faster
 * without waiting for simpletest build process.
 */

function ldap_test_dev_provision() {
  ldap_server_module_load_include('php', 'ldap_test', 'LdapTestFunctionsv2.class');
  variable_set('ldap_simpletest', 1);
  variable_set('ldap_help_watchdog_detail', 1);
  //return 'set to return and clear simpletest flag';

  $test_functions = new LdapTestFunctionsv2();
  $sids = array('activedirectory');
  ldap_test_dev_prepTestData($sids, $test_functions, 'default', 'default');

  //dpm('test_functions'); dpm($test_functions);

  $ldap_user_conf = ldap_user_conf('admin', TRUE);
  dpm('ldap_user_conf'); dpm($ldap_user_conf);

  // test basic ldap user functions
  $synch_context = LDAP_USER_SYNCH_CONTEXT_AUTHENTICATE_DRUPAL_USER;

  if ($user = user_load_by_name('jkeats')) {
    user_delete($user->uid);
  }

  $account = NULL;
  $user_edit = array('name' => 'jkeats');
  $jkeats = $ldap_user_conf->provisionDrupalAccount($account, $user_edit, LDAP_USER_SYNCH_CONTEXT_INSERT_DRUPAL_USER, NULL, TRUE);
  dpm('jkeats user afterprovisionDrupalAccount'); dpm($jkeats);

  $jkeats = user_load_by_name('jkeats');

  $properties_set = (
    $jkeats->name == 'jkeats' &&
    $jkeats->mail == 'boygen@hotmail.com' &&
    $jkeats->init == 'boygen@hotmail.com' &&
    $jkeats->status == 1
  );


  $fields_set = (
    isset($jkeats->ldap_user_puid['und'][0]['value']) &&
    $jkeats->ldap_user_puid['und'][0]['value'] == 'e2eb4d760f81e89df1524cff71f720a0' &&
    isset($jkeats->ldap_user_puid_sid['und'][0]['value']) &&
    $jkeats->ldap_user_puid_sid['und'][0]['value'] == 'activedirectory' &&
    isset($jkeats->ldap_user_current_dn['und'][0]['value']) &&
    $jkeats->ldap_user_current_dn['und'][0]['value'] == 'CN=jkeats,CN=Users,DC=activedirectory,DC=ldap,DC=pixotech,DC=com' &&
    isset($jkeats->ldap_user_puid_property['und'][0]['value']) &&
    $jkeats->ldap_user_puid_property['und'][0]['value'] == 'pixotechcomregistryuniqueid'
  );

  $data_diff = array_diff(
    $jkeats->data['ldap_authentication'],
    array (
    'init' =>
      array (
        'sid' => 'activedirectory',
        'dn' => NULL,
        'mail' => 'boygen@hotmail.com',
      ),
    )
   );

  dpm("properties set=$properties_set, fields set=$fields_set, data diff=". count($data_diff));

  variable_del('ldap_help_watchdog_detail');
  variable_del('ldap_simpletest');

  return 'end of ldap_test_dev_provision()';

}


function ldap_test_dev_prepTestData($sids,
      $test_functions,
      $ldap_user_conf_id = NULL,
      $ldap_authentication_conf_id = NULL,
      $ldap_authorization_conf_id = NULL,
      $ldap_feeds_conf_id = NULL,
      $ldap_query_conf_id = NULL
    ) {

    $test_functions->configureLdapServers($sids);
    if ($ldap_user_conf_id) {
      $test_functions->configureLdapUser($ldap_user_conf_id, $sids);
    }
    if ($ldap_authentication_conf_id) {
      $test_functions->configureLdapAuthentication($ldap_authentication_conf_id, $sids);
    }
  }