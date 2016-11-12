<?php
//print json_encode(array('success' => true));
function formatDate($tempDate, $year) {
  if($year < 2017) {
    if(preg_match("/^[0-9]{4}-(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])$/", $tempDate)) {
      $d = explode('-', $tempDate);
      $d = $d[1] . '/' . $d[2] . '/' . $d[0];
    } else if(preg_match("/^[0-9]{4}\/(0[1-9]|1[0-2])\/(0[1-9]|[1-2][0-9]|3[0-1])$/", $tempDate)) {
      $d = explode('/', $tempDate);
      $d = $d[1] . '/' . $d[2] . '/' . $d[0];
    } else if(preg_match("/^(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])-[0-9]{4}$/", $tempDate)) {
      $d = explode('-', $tempDate);
      $d = $d[0] . '/' . $d[1] . '/' . $d[2];
    } else if(preg_match("/^(0[1-9]|1[0-2])\/(0[1-9]|[1-2][0-9]|3[0-1])\/[0-9]{4}$/", $tempDate)) $d = $tempDate;
  } else {
    if(preg_match("/^(0[1-9]|1[0-2])\/(0[1-9]|[1-2][0-9]|3[0-1])\/[0-9]{4}$/", $tempDate)) {
      $d = explode('-', $tempDate);
      $d = $d[2] . '-' . $d[0] . '-' . $d[1];
    } else if(preg_match("/^[0-9]{4}\/(0[1-9]|1[0-2])\/(0[1-9]|[1-2][0-9]|3[0-1])$/", $tempDate)) {
      $d = explode('/', $tempDate);
      $d = $d[0] . '-' . $d[1] . '-' . $d[2];
    } else if(preg_match("/^(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])-[0-9]{4}$/", $tempDate)) {
      $d = explode('-', $tempDate);
      $d = $d[4] . '-' . $d[2] . '-' . $d[1];
    } else if(preg_match("/^[0-9]{4}-(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])$/", $tempDate)) $d = $tempDate;
  }
	return $d;
}

function formatPhoneNumber($phoneNumber) {
    $phoneNumber = preg_replace('/[^0-9]/','',$phoneNumber);
    if(strlen($phoneNumber) > 10) {
        $countryCode = substr($phoneNumber, 0, strlen($phoneNumber)-10);
        $areaCode = substr($phoneNumber, -10, 3);
        $nextThree = substr($phoneNumber, -7, 3);
        $lastFour = substr($phoneNumber, -4, 4);
        $phoneNumber = '+'.$countryCode.'-'.$areaCode.'-'.$nextThree.'-'.$lastFour;
    }
    else if(strlen($phoneNumber) == 10) {
        $areaCode = substr($phoneNumber, 0, 3);
        $nextThree = substr($phoneNumber, 3, 3);
        $lastFour = substr($phoneNumber, 6, 4);
        $phoneNumber = $areaCode.'-'.$nextThree.'-'.$lastFour;
    }
    else if(strlen($phoneNumber) == 7) {
        $nextThree = substr($phoneNumber, 0, 3);
        $lastFour = substr($phoneNumber, 3, 4);
        $phoneNumber = $nextThree.'-'.$lastFour;
    }
    return $phoneNumber;
}

function formatMedicareNumber($medicareNumber) {
	$medicareNumber = preg_replace('/[^0-9a-zA-Z]/','',$medicareNumber);
	#$firstThree = substr($medicareNumber, 0, 3);
	#$nextTwo = substr($medicareNumber, -7, 2);
	#$lastFour = substr($medicareNumber, -5, 4);
	#$lastLetter = substr($medicareNumber, -1, 1);
	#$medicareNumber = $firstThree . '-' . $nextTwo . '-' . $lastFour . $lastLetter;
	return $medicareNumber;
}

function formatMemberId($id) {
	if(count($id) > 9) $str = substr($id, 0, -9);
  else $str = $id;
  return $str;
}

function formatSSN($ssn) {
	$ssn = preg_replace('/[^0-9]/','',$ssn);
	$firstThree = substr($ssn, 0, 3);
	$nextTwo = substr($ssn, -6, 2);
	$lastFour = substr($ssn, -4, 4);
	$ssn = $firstThree . '-' . $nextTwo . '-' . $lastFour;
	return $ssn;
}

function bcbsri_online_application_sendtosalesforce() {
  drupal_add_http_header(
    'Cache-Control', 
    'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
    FALSE
  );
  ini_set("soap.wsdl_cache_enabled", "0");
  $curPath = dirname(__FILE__);
  
  $sfArray = array();
  include('bcbsri_online_application.define.salesforce.inc');
  if(isset($_SERVER['AH_SITE_ENVIRONMENT']) && $_SERVER['AH_SITE_ENVIRONMENT'] != 'prod') $sfArray['debug'] = true;
  else $sfArray['debug'] = false;

  if($_SERVER['REQUEST_METHOD'] == 'POST') {
    $config = bcbsri_online_application_get_json('config');
    $data = json_decode(str_replace(array('\"', "\'"), array('"', "'"), $_REQUEST['d']), true);
    if (isset($_SERVER['AH_SITE_ENVIRONMENT']) && $_SERVER['AH_SITE_ENVIRONMENT'] != 'prod') {
      $sfdcUsername = $sfLogin['sandbox']['sf_username']; #$config['sandbox'][0]['sf_username'];
      $sfdcPassword = $sfLogin['sandbox']['sf_password']; #$config['sandbox'][0]['sf_password'];
      $sfdcToken = $sfLogin['sandbox']['sf_token']; #$config['sandbox'][0]['sf_token'];
      $sfdcWsdl = $sfLogin['sandbox']['sf_wsdl']; #$config['sandbox'][0]['sf_wsdl'];
      $sfdcClientId = $sfLogin['sandbox']['sf_client_id'];
      $sfdcClientSecret = $sfLogin['sandbox']['sf_client_secret'];
      $sfdcURL = $sfLogin['sandbox']['sf_url'];
    } else {
      $sfdcUsername = $sfLogin['production']['sf_username']; #$config['production'][0]['sf_username'];
      $sfdcPassword = $sfLogin['production']['sf_password']; #$config['production'][0]['sf_password'];
      $sfdcToken = $sfLogin['production']['sf_token']; #$config['production'][0]['sf_token'];
      $sfdcWsdl = $sfLogin['production']['sf_wsdl']; #$config['production'][0]['sf_wsdl'];
      $sfdcClientId = $sfLogin['production']['sf_client_id'];
      $sfdcClientSecret = $sfLogin['production']['sf_client_secret'];
      $sfdcURL = $sfLogin['production']['sf_url'];
    }
    $sfWsdlPath = $curPath.'/wsdl/';
    $sfArray['confirmation'] = '';
    //$sfArray['debug'] = true;
    $stateArr = array("RI"=>"Rhode Island","AL"=>"Alabama","AK"=>"Alaska","AZ"=>"Arizona",
                      "AR"=>"Arkansas","CA"=>"California","CO"=>"Colorado","CI"=>"Connecticut",
                      "DE"=>"Delaware","DC"=>"District Of Columbia","FL"=>"Florida","GA"=>"Georgia",
                      "HI"=>"Hawaii","ID"=>"Idaho","IL"=>"Illinois","IN"=>"Indiana","IA"=>"Iowa",
                      "KS"=>"Kansas","KY"=>"Kentucky","LA"=>"Louisiana","ME"=>"Maine","MD"=>"Maryland",
                      "MA"=>"Massachusetts","MI"=>"Michigan","MN"=>"Minnesota","MS"=>"Mississippi",
                      "MO"=>"Missouri","MT"=>"Montana","NE"=>"Nebraska","NV"=>"Nevada","NH"=>"New Hampshire",
                      "NJ"=>"New Jersey","NM"=>"New Mexico","NY"=>"New York","NC"=>"North Carolina",
                      "ND"=>"North Dakota","OH"=>"Ohio","OK"=>"Oklahoma","OR"=>"Oregon","PA"=>"Pennsylvania",
                      "SC"=>"South Carolina","SD"=>"South Dakota","TN"=>"Tennessee","TX"=>"Texas","UT"=>"Utah",
                      "VT"=>"Vermont","VA"=>"Virginia","WA"=>"Washington","WV"=>"West Virginia","WI"=>"Wisconsin","WY"=>"Wyoming");
    $phoneSearch = array("Home" => "home_phone", "Cell" => "cell_phone");

    $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
    $sfAlias = bcbsri_online_application_get_alias($auth);
    $sfYear = (int)bcbsri_online_application_get_year($auth);
    if($sfArray['debug']) {
      $sfArray['sts']['auth'] = $auth;
      $sfArray['sts']['alias'] = $sfAlias;
      $sfArray['sts']['year'] = $sfYear;
    }
    
    try {
      $mySforceConnection = new SforceEnterpriseClient();
      $mySforceConnection->createConnection($sfWsdlPath.basename($sfdcWsdl));
      #$mySforceConnection->setCallOptions(new CallOptions($sfdcClientId, null));
      $loginResult = $mySforceConnection->login($sfdcUsername, $sfdcPassword.$sfdcToken);
    } catch (Exception $e) {
      $sfArray['success'] = false;
      $sfArray['message'] = "failed login";
      if($sfArray['debug']) {
        $sfArray['errorMsg'] = $e->faultstring;
      }
      include('bcbsri_online_application.test_salesforce.php');
      return $sfArray;
      //die();
    }
    
    $objApplicantDetails = new stdClass();
    $objApplicationDetails = new stdClass();
    $objSpouseDetails = new stdClass();
    $objDependentDetails = array();
    
    if($sfAlias == 'medicare' && $sfYear == 2016) include('bcbsri_online_application.medicare.send_to_salesforce.inc');
    else if($sfAlias == 'medicare' && $sfYear > 2016) include('bcbsri_online_application.medicare.send_to_salesforce.json.inc');
    else if($sfAlias == 'medicare-plan-change' && $sfYear > 2016) include('bcbsri_online_application.medicare.plan_change.send_to_salesforce.json.inc');
    else if($sfAlias == 'directpay' && $sfYear == 2016) include('bcbsri_online_application.directpay.send_to_salesforce.inc');
    #else if($sfAlias == 'directpay' && $sfYear > 2016) include('bcbsri_online_application.directpay.send_to_salesforce.json.inc');
    else {
      $sfArray['success'] = false;
      $sfArray['message'] = "failed to send to salesforce - no app mapped to " . $sfAlias . ", and year " . $sfYear;
    }
    
    return $sfArray;
  } else return array('success' => false, 'message' => 'No data sent');
}