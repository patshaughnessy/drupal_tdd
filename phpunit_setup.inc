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
  while ($table = db_result($result)) {
    $table_callback($table);
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

function create_test_drupal_schema()
{
  include_once './includes/install.inc';
  drupal_install_system();
  drupal_install_modules(drupal_verify_profile('default', 'en'));
  $task = 'profile';
  default_profile_tasks($task, '');
  menu_rebuild();
  actions_synchronize();
  _drupal_flush_css_js();

  variable_set('user_mail_status_activated_notify', FALSE);
  $account = user_load(1);
  $merge_data = array('name' => 'admin', 'pass' => 'adminpassword', 'roles' => array(), 'status' => 1);
  user_save($account, $merge_data);  
}


?>