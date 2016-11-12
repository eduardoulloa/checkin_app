<?php
header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

/*$sessionCookie = session_get_cookie_params();
setcookie('bcbsri_ws[oa_aid]', 'abcdefghijklmnopqrstuvwxyz', 0, $sessionCookie['path'], $sessionCookie['domain'], $sessionCookie['secure'], TRUE);
setcookie('bcbsri_ws[oa_year]', '2016', 0, $sessionCookie['path'], $sessionCookie['domain'], $sessionCookie['secure'], TRUE);
setcookie('bcbsri_ws[oa_prod]', 'medicare', 0, $sessionCookie['path'], $sessionCookie['domain'], $sessionCookie['secure'], TRUE);
setcookie('bcbsri_ws[oa_plan]', 'Select', 0, $sessionCookie['path'], $sessionCookie['domain'], $sessionCookie['secure'], TRUE);
setcookie('bcbsri_ws[oa_price]', '83', 0, $sessionCookie['path'], $sessionCookie['domain'], $sessionCookie['secure'], TRUE);
setcookie('bcbsri_ws[oa_quote]', '', 0, $sessionCookie['path'], $sessionCookie['domain'], $sessionCookie['secure'], TRUE);

echo '<pre>';
var_dump($_COOKIE['bcbsri_ws']);
echo '<br />**************************************<br />';
var_dump($_COOKIE['bcbsri_ws']['oa_aid']);
echo '</pre>';*/

$curPath = dirname(__FILE__);
if (! defined('DRUPAL_ROOT')) define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

$query = db_select('bcbsri_online_application_udata', 'boad');
$query->fields('boad', array('auth', 'resumedk', 'resumed_email', 'reminder_sent'));
$query->condition('boad.end_time', strtotime('-1 days'), '<');
$query->condition('boad.reminder_sent', 1, '<');
$query->isNotNull('boad.resumedk');
$query->isNotNull('boad.resumed_email');
$result = $query->execute();
echo '<h3>Query with resumed_email set to not null</h3>';
if($result->rowCount() > 0) {
  foreach($result as $r) {
    echo '<pre>';
    var_dump($r);
    echo '<br />**************************************<br />';
    echo '</pre>';
  }
}

$query = db_select('bcbsri_online_application_udata', 'boad');
$query->fields('boad', array('auth', 'resumedk', 'resumed_email', 'reminder_sent'));
$query->condition('boad.end_time', strtotime('-1 days'), '<');
$query->condition('boad.reminder_sent', 1, '<');
$query->isNotNull('boad.resumedk');
$result = $query->execute();
echo '<h3>Query without resumed_email filter</h3>';
if($result->rowCount() > 0) {
  foreach($result as $r) {
    echo '<pre>';
    var_dump($r);
    echo '<br />**************************************<br />';
    echo '</pre>';
  }
}