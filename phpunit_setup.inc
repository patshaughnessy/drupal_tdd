<?php

require_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

validate_test_database();
db_set_active("test");
drop_test_tables();
create_test_drupal_schema();
enable_mysql_transactions();

function validate_test_database()
{
  global $db_url;
  
  if (!is_array($db_url) || !$db_url['test']) {
    echo "Error: please specify your test database URL in settings.php.\n";
    echo "You can set your original database as the default DB URL.\n";
    echo "Example:\n";
    echo '$db_url[\'default\'] = \'mysql://user:password@localhost/dbname\'' . "\n";
    echo '$db_url[\'test\'] = \'mysql://user:password@localhost/dbname_test\'' . "\n";
    exit;
  }

  $url = parse_url($db_url['test']);
  if (!stristr($db_url['test'], 'test')) {
    echo "Error: the word 'test' does not appear in your test database URL in settings.php.\n";
    echo "Please include 'test' in your test database name.\n";
    exit;
  }
  
  $test_connection = mysql_connect($url['host'], $url['user'], $url['pass'], TRUE, 2);
  if (!$test_connection || !mysql_select_db(substr($url['path'], 1))) {
    echo "Error: please create a test database.\n";
    echo "MySQL is unable to connection to: " . $db_url['test'] . "\n";
    exit;
  }
  
  mysql_close($test_connection);
}

function drop_test_tables()
{
  each_table('drop');
}

function enable_mysql_transactions()
{
  convert_test_tables_to_innodb();
  db_query("SET AUTOCOMMIT = 0");  
}

function convert_test_tables_to_innodb()
{
  each_table('convert_to_innodb');  
}

function each_table($table_callback)
{
  global $db_url;
  
  $url = parse_url($db_url['test']);
  $database = substr($url['path'], 1);
  
  $result = db_query("SELECT table_name FROM information_schema.tables WHERE table_schema = '$database'");
  while ($table = db_fetch_array($result)) {
    $table_callback($table['table_name']);
  }
}

function convert_to_innodb($table)
{
  db_query("ALTER TABLE $table ENGINE = INNODB");
}

function drop($table)
{
  db_query("DROP TABLE $table");
}

/**
 * Install modules associated with the default profile.
 */
function install_modules() {

  require_once './includes/install.inc';

  // Is the drupal_install_system() function available?
  // If it is we'll use that first
  if (function_exists("drupal_install_system")) {
    // Do the Drupal 6 style install
    install_modules__with_drupal_install_system();
  }
  else if (function_exists("drupal_install_profile")) {
    // Do the Drupal 5 style install
    install_modules__with_drupal_install_profile();
  }
  else {
    // We don't support the configuration you're using!
    die("Sorry phpunit_setup doesn't recognize your Drupal installation.");
  }
}

/**
 * Use the drupal_install_system() function to install the test database.
 *
 * drupal_install_system() has been available since Drupal 6.
 */
function install_modules__with_drupal_install_system() {
  include_once './includes/install.inc';
  drupal_install_system();
  drupal_install_modules(drupal_verify_profile('default', 'en'));
  $task = 'profile';
  default_profile_tasks($task, '');
  menu_rebuild();
  actions_synchronize();
  _drupal_flush_css_js();
}

/**
 * Use the drupal_install_profile() function to install the test database.
 *
 * drupal_install_profile() is available in Drupal 5.
 */
function install_modules__with_drupal_install_profile() {

  drupal_bootstrap(DRUPAL_BOOTSTRAP_CONFIGURATION);
  // This must go after drupal_bootstrap(), which unsets globals!
  global $profile, $install_locale;
  require_once './modules/system/system.install';
  require_once './includes/file.inc';

  // Load module basics (needed for hook invokes).
  include_once './includes/module.inc';
  $module_list['system']['filename'] = 'modules/system/system.module';
  $module_list['filter']['filename'] = 'modules/filter/filter.module';
  module_list(TRUE, FALSE, FALSE, $module_list);
  drupal_load('module', 'system');
  drupal_load('module', 'filter');

  // Decide which profile to use.
  $profile = 'default';

  // Load the profile.
  require_once "./profiles/$profile/$profile.profile";

  // Verify existence of all required modules.
  $modules = drupal_verify_profile($profile, $install_locale);
  if (!$modules) {
    // Missing required modules
    die("Missing required modules!");
  }

  // Perform actual installation defined in the profile.
  drupal_install_profile($profile, $modules);
}

function create_test_drupal_schema()
{
  install_modules();

  variable_set('user_mail_status_activated_notify', FALSE);
  $account = user_load(1);
  $merge_data = array('name' => 'admin', 'pass' => 'adminpassword', 'roles' => array(), 'status' => 1);
  user_save($account, $merge_data);  
}


?>
