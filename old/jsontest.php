<?php
ini_set('display_errors',1);
ini_set('display_startup_errors',1);
error_reporting(-1);

header("Cache-Control: no-cache, no-store, must-revalidate"); // HTTP 1.1.
header("Pragma: no-cache"); // HTTP 1.0.
header("Expires: 0"); // Proxies.

$curPath = dirname(__FILE__); /*getcwd();*/

include_once('BCBSRIOnlineApplication.inc');
include_once('bcbsri_online_application.helper.inc');

if(empty($_REQUEST['data'])) {
	$data = json_decode('{"currently_enrolled_in_bluechip":"no","enrolled_in_medicare_part_a_and_b":"yes","ri_resident_when_plan_starts":"yes","completing_this_application":"enrollee","broker_agent_name":"","broker_agent_id":"","broker_agent_channel":"Choose One","authorized_rep_name":"","authorized_rep_address":"","authorized_rep_city":"","authorized_rep_state":"RI","authorized_rep_zip":"","authorized_rep_phone":"","relationship_to_enrollee":"","select_option_best_fits_your_needs":"annual_election_period","date_lost_will_lose_coverage":"","date_lost_coverage":"","date_left_pace_program":"","belong_to_state_assisted_pharmacy_program":"RI","date_no_longer_eligible_for_extra_help":"","date_lost_special_needs_qualifications":"","date_moved_or_will_move":"","prefix":"mr","first_name":"James","middle_initial":"A","last_name":"Williams","suffix":"","date_of_birth":"1983-10-02","primary_language":"English","gender":"male","phone_number":"4014592255","home_or_cell_phone_number":"home_phone","alternate_phone_number":"4014445555","alt_home_or_cell_phone_number":"cell_phone","email_address":"","confirm_email_address":"","permanent_street_address":"500 Exchange St","permanent_address_city":"Providence","permanent_address_state":"RI","permanent_address_zip_code":"02888","mailing_address_different_from_permanent":"no","mailing_street_address":"","mailing_address_city":"","mailing_address_state":"RI","mailing_address_zip":"","billing_address_different_from_permanent_or_mailing":"no","billing_street_address":"","billing_address_city":"","billing_address_state":"RI","billing_address_zip":"","emergency_contact_name":"James Contact","emergency_contact_phone":"4011231234","emergency_contact_relationship":"Neighbor","emergency_contact_designee":"no","medicare_claim_number":"123-12-1234a","part_a_effective_date":"2015-08-01","part_b_effective_date":"2015-08-01","end_stage_renal_disease":"no","other_coverage_name":"","other_coverage_id":"","other_coverage_group_number":"","long_term_care_facility_name":"","long_term_care_facility_address":"","long_term_care_facility_city":"","long_term_care_facility_phone":"","provide_medicaid_number":"","provide_pcp_name":"","select_premium_payment_option":"monthly_bill","enrollee_signature_checkbox":"enrollee_signature_checkbox","enrollee_signature_name":"James Williams","todays_date":"2015-09-08","sep_eligibility_date":""}', true);
} else {
	$data = $_REQUEST['data'];
}

print '<pre>';
//foreach($data as $k => $v) {
//	print $k . ' => ' . $v . '<br />';
//}
var_dump($data);
print '</pre>';


//$content = display($med['content'], 'content');
$content = new BCBSRIOnlineApplication($curPath . "/" . $_REQUEST['file'], 'medicare', 2016, false);

	/*$to = filter_var($_REQUEST['form_email'], FILTER_SANITIZE_EMAIL);
	$cValue = bcbsri_online_application_get_cookie('planDetails');
	if(bcbsri_online_application_get_alias() == 'medicare') $appName = "BlueCHiP for Medicare";
	else if(bcbsri_online_application_get_alias() == 'direct_pay') $appName = "Direct Pay";
	
	$headers = "From: website@bcbsri.org\r\n";
	// $headers .= "Reply-To: \r\n";
	$headers .= "MIME-Version: 1.0\r\n";
	$headers .= "Content-Type: text/html; charset=ISO-8859-1\r\n";
	$subject = bcbsri_online_application_get_year() . ' ' . $appName . ' Application';
	$message = '<p><strong>Confirmation Number: ' . $_REQUEST['form_confirmation'] . '</strong></p>' . $data;
	print json_encode(array('success' => mail($to, $subject, $message, $headers)));*/
				
//print $content->getEmail($data);
 $c = $content->getArray('content');
// print '<pre>';
// var_dump($content->getDisplay());
// print '</pre>';
// print '<hr />';
print '<pre>';
$c = json_decode($content->getDisplayPage(2, 'full', false), true);
print $c['content'];
print '</pre>';
	print '<hr />';
//print '<pre>';
//var_dump($c[2]);
//print '</pre>';
//print '<hr />';
//print '<pre>';
//var_dump(json_decode(file_get_contents($curPath . "/" . $_REQUEST['file']), true));
//print '</pre>';
/*print '<pre>';
var_dump($c[3]);
print '</pre>';
	print '<hr />';
	$view = 'full';
	$review = false;
print '<pre>';
var_dump(json_decode($content->getDisplayPage(2, $view, $review), true));
print '</pre>';

for($i = 1; $i <= $content->getNumberofSteps(); $i++) {
	print '<pre>';
	$view = 'full';
	$review = ($i == 10 || $i == $content->getNumberofSteps() ? true : false);
	$c = json_decode($content->getDisplayPage($i, $view, $review), true);
	print $c['content'];
	print $c['sidebar'];
	print $c['content_bottom'];
	print $c['footer'];
	print '</pre>';
	print '<hr />';
}