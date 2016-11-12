<?php
if($_REQUEST['report'] == 'yesplease') {
	$curPath = dirname(__FILE__); /*getcwd();*/
	include_once('bcbsri_online_application.helper.inc');
	
	if (!defined('DRUPAL_ROOT')) define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
	require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

	$file = 'bcbsri_online_application_' . date('mdy_His') . '.csv';
	header('Content-Type: text/csv; utf-8');
	header('Content-Disposition: attachment;filename=' . $file);
	header("Cache-Control: no-cache, no-store, must-revalidate"); // HTTP 1.1.
	header("Pragma: no-cache"); // HTTP 1.0.
	header("Expires: 0"); // Proxies.
	
	$output = '';
	
	$keys = array('id', 'App Type', 'Name', 'Phone', 'Email', 'Application Start', 'Application Modified', 'Application Confirmation', 'Entered By', 'Fields Entered', 'UA String');
	if ($keys) $output .= implode(",", $keys) . "\n";
	
	$query = db_select('bcbsri_online_application_data', 'boad');
	$query->fields('boad', array('id', 'app_type', 'app_confirm', 'auth', 'skey', 'fn', 'ln', 'email', 'phone', 'enter_by', 'start_time', 'end_time', 'fields', 'ua_info'));
	$query->orderBy('boad.start_time', 'DESC');
	$result = $query->execute();
	
	foreach($result as $r) {
		$row = array();
		$row[0] = $r->id;
		$row[1] = $r->app_type;
		$row[2] = (!empty($r->fn) ? bcbsri_online_application_decrypt($r->fn, $r->auth, $r->skey) : '') . ' ' . (!empty($r->ln) ? bcbsri_online_application_decrypt($r->ln, $r->auth, $r->skey) : '');
		$row[3] = (!empty($r->phone) ? bcbsri_online_application_decrypt($r->phone, $r->auth, $r->skey) : '');
		$row[4] = (!empty($r->email) ? bcbsri_online_application_decrypt($r->email, $r->auth, $r->skey) : '');
		$row[5] = ($r->start_time != 0 ? date("F j Y g:i:s a", $r->start_time) : '');
		$row[6] = ($r->end_time != 0 ? date("F j Y g:i:s a", $r->end_time) : '');
		$row[7] = (!empty($r->app_confirm) ? $r->app_confirm : '');
		$row[8] = '';
		if(!empty($r->enter_by)) {
			$eBy = explode('|', bcbsri_online_application_decrypt($r->enter_by, $r->auth, $r->skey));
			foreach($eBy as $e) $row[8] .= $e . ' | ';
		}
		$row[9] = '';
		if(!empty($r->fields)) {
			$eFields = json_decode($r->fields, true);
			foreach($eFields as $key => $value) {
				if(!empty($value)) $row[9] .= $key . ' | ';
			}
		}
		$row[10] = $r->ua_info;
		$output .= implode(",", $row) . "\n";
	}
	
	ob_clean();
	print $output;
	exit;
} else {
	print 'Unauthorized';
	die();
}