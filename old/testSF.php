<?php
header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
ini_set("soap.wsdl_cache_enabled", "0");
$curPath = dirname(__FILE__);
if (! defined('DRUPAL_ROOT')) define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

include_once('bcbsri_online_application.helper.inc');

$auth = 'dvmetbQfjK8n4bxLDgMldFefzGGacF5o';
print 'Alias: ' . bcbsri_online_application_get_alias($auth);
print '<br />Year: ' . (int)bcbsri_online_application_get_year($auth);
/*
// Login test
include('bcbsri_online_application.define.salesforce.inc');
#$config = bcbsri_online_application_get_json('config');
#$year = bcbsri_online_application_get_year($auth);
#$alias = bcbsri_online_application_get_alias($auth);
#if (isset($_SERVER['AH_SITE_ENVIRONMENT']) && $_SERVER['AH_SITE_ENVIRONMENT'] != 'prod') {
  $sfdcUsername = $sfLogin['sandbox']['sf_username']; #$config['sandbox'][0]['sf_username'];
  $sfdcPassword = $sfLogin['sandbox']['sf_password']; #$config['sandbox'][0]['sf_password'];
  $sfdcToken = $sfLogin['sandbox']['sf_token']; #$config['sandbox'][0]['sf_token'];
  $sfdcWsdl = $sfLogin['sandbox']['sf_wsdl']; #$config['sandbox'][0]['sf_wsdl'];
  $sfdcClientId = $sfLogin['sandbox']['sf_client_id'];
  $sfdcClientSecret = $sfLogin['sandbox']['sf_client_secret'];
  $sfdcURL = $sfLogin['sandbox']['sf_url'];
#} else {
#  $sfdcUsername = $sfLogin['production']['sf_username']; #$config['production'][0]['sf_username'];
#  $sfdcPassword = $sfLogin['production']['sf_password']; #$config['production'][0]['sf_password'];
#  $sfdcToken = $sfLogin['production']['sf_token']; #$config['production'][0]['sf_token'];
#  $sfdcWsdl = $sfLogin['production']['sf_wsdl']; #$config['production'][0]['sf_wsdl'];
#  $sfdcClientId = $sfLogin['production']['sf_client_id'];
#  $sfdcClientSecret = $sfLogin['production']['sf_client_secret'];
#  $sfdcURL = $sfLogin['production']['sf_url'];
#}
$sfWsdlPath = $curPath.'/wsdl/';

$params = "grant_type=password"
  . "&client_id=" . $sfdcClientId
  . "&client_secret=" . $sfdcClientSecret
  . "&username=" . $sfdcUsername
  . "&password=" . $sfdcPassword;

$curl = curl_init($sfdcURL . '/services/oauth2/token'); #authorize'); #?' . $params);
#$curl = curl_init(_SF_TOKEN_URI_);
curl_setopt($curl, CURLOPT_HEADER, false);
curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
curl_setopt($curl, CURLOPT_POST, true);
curl_setopt($curl, CURLOPT_POSTFIELDS, $params);

$json_response = curl_exec($curl);

$status = curl_getinfo($curl, CURLINFO_HTTP_CODE);

if ( $status != 200 ) {
  die("Error: call to URL failed with status $status, response $json_response, curl_error " . curl_error($curl) . ", curl_errno " . curl_errno($curl));
}

curl_close($curl);
$response = json_decode($json_response, true);

echo "Login Response<br />";
foreach ($response as $key => $value) {
  if(!is_array($value)) echo "$key:$value<br/>";
  else {
    echo "$key:<br/>";
    foreach ((array) $response as $key => $value) {
      echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$key:$value<br/>";
    }
  } 
}

/*try {
#  if($_SERVER['REQUEST_METHOD'] == 'POST') {
#    if(bcbsri_online_application_check_auth($_REQUEST['auth'])) {
      $mySforceConnection = new SforceEnterpriseClient();
      $mySforceConnection->createConnection($sfWsdlPath . $sfdcWsdl);
#      $mySforceConnection->setClientId($sfdcClientId);
#      $mySforceConnection->setCallOptions(new CallOptions($sfdcClientId, null));
      $loginResult = $mySforceConnection->login($sfdcUsername, $sfdcPassword.$sfdcToken);
      var_dump(array('success' => true));
#    } else var_dump(array('success' => false, 'message' => 'Failed auth check'));
#  } else var_dump(array('success' => false, 'message' => 'No data sent'));
} catch (Exception $e) {
#  include_once('bcbsri_online_application.test_salesforce.php');
  var_dump(array('success' => false, 'message' => 'Failed to test salesforce', 'error' => $e->getMessage(), 'error_file' => $e->getFile(), 'error_line' => $e->getLine(), 'error_trace' => $e->getTraceAsString()));
}

try {
#  if($_SERVER['REQUEST_METHOD'] == 'POST') {
#    if(bcbsri_online_application_check_auth($_REQUEST['auth'])) {
      $mySforceConnection = new SforceEnterpriseClient();
      $mySforceConnection->createConnection($sfWsdlPath . $sfdcWsdl);
#      $mySforceConnection->setClientId($sfdcClientId);
      $mySforceConnection->setCallOptions(new CallOptions($sfdcClientId, null));
      $loginResult = $mySforceConnection->login($sfdcUsername, $sfdcPassword.$sfdcToken);
      var_dump(array('success' => true));
#    } else var_dump(array('success' => false, 'message' => 'Failed auth check'));
#  } else var_dump(array('success' => false, 'message' => 'No data sent'));
} catch (Exception $e) {
#  include_once('bcbsri_online_application.test_salesforce.php');
  var_dump(array('success' => false, 'message' => 'Failed to test salesforce', 'error' => $e->getMessage(), 'error_file' => $e->getFile(), 'error_line' => $e->getLine(), 'error_trace' => $e->getTraceAsString()));
} */

/*
//Test Member Lookup
$access_token = $response['access_token'];
$instance_url = $response['instance_url'];
$url = $instance_url . _SF_MA_LOOKUP_URI_;
#$content = json_encode(array('lkup' => array('hicn' => '036225366a', 'memberId' => '801051512')));
$content = json_encode(array('lkup' => array('hicn' => '256252562a', 'memberId' => '256256256')));

$curlLU = curl_init($url);
curl_setopt($curlLU, CURLOPT_HEADER, false);
curl_setopt($curlLU, CURLOPT_RETURNTRANSFER, true);
curl_setopt($curlLU, CURLOPT_HTTPHEADER, array("Authorization: OAuth $access_token", "Content-type: application/json"));
curl_setopt($curlLU, CURLOPT_POSTFIELDS, $content);
//curl_setopt($curl, CURLOPT_CUSTOMREQUEST, "POST"); // may not need

$json_response = curl_exec($curlLU);
$status = curl_getinfo($curlLU, CURLINFO_HTTP_CODE);

echo "Status: $status<br />";

echo "Response: $json_response<br />";
$jsonArray = json_decode($json_response, true);
foreach ($jsonArray as $key => $value) {
  if(!is_array($value)) echo "$key:$value<br/>";
  else {
    echo "$key:<br/>";
    foreach ($value as $key => $value) {
      echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$key:$value<br/>";
    }
  } 
}


// Get authorization code, access token, instance url
/*
$code = $response;
$params = "code=" . $code
  . "&grant_type=authorization_code"
  . "&client_id=" . CLIENT_ID
  . "&client_secret=" . CLIENT_SECRET
  . "&redirect_uri=" . urlencode(REDIRECT_URI);

$curl = curl_init($token_url);
curl_setopt($curl, CURLOPT_HEADER, false);
curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
curl_setopt($curl, CURLOPT_POST, true);
curl_setopt($curl, CURLOPT_POSTFIELDS, $params);
$json_response = curl_exec($curl);
$status = curl_getinfo($curl, CURLINFO_HTTP_CODE);

if ( $status != 200 ) {
  die("Error: call to token URL $token_url failed with status $status, response $json_response, curl_error " . curl_error($curl) . ", curl_errno " . curl_errno($curl));
}

curl_close($curl);
$response = json_decode($json_response, true);
echo "Get authorization code, access token, instance url and such response";
foreach ((array) $response as $key => $value) {
  if(!is_array($value)) echo "$key:$value<br/>";
  else {
    echo "$key:<br/>";
    foreach ((array) $response as $key => $value) {
      echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$key:$value<br/>";
    }
  } 
}

$access_token = $response['access_token'];
$instance_url = $response['instance_url'];

if (!isset($access_token) || $access_token == "") {
  die("Error - access token missing from response!");
}
 
if (!isset($instance_url) || $instance_url == "") {
  die("Error - instance URL missing from response!");
}
*/

// Send data test
/*
$content = json_encode(array("Name" => $new_name, "BillingCity" => $city));

$url = "$instance_url/services/data/v20.0/sobjects/Account/$id";
$curl = curl_init($url);
curl_setopt($curl, CURLOPT_HEADER, false);
curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
curl_setopt($curl, CURLOPT_HTTPHEADER, array("Authorization: OAuth $access_token"));
curl_setopt($curl, CURLOPT_POSTFIELDS, $content);
curl_setopt($curl, CURLOPT_CUSTOMREQUEST, "PATCH"); // may not need

$json_response = curl_exec($curl);
$status = curl_getinfo($curl, CURLINFO_HTTP_CODE);

if ( $status != 200 ) {
  die("Error: call to URL $url failed with status $status, response $json_response, curl_error " . curl_error($curl) . ", curl_errno " . curl_errno($curl));
}

curl_close($curl);
$response = json_decode($json_response, true);

echo "Send data response";
foreach ((array) $response as $key => $value) {
  if(!is_array($value)) echo "$key:$value<br/>";
  else {
    echo "$key:<br/>";
    foreach ((array) $response as $key => $value) {
      echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$key:$value<br/>";
    }
  } 
}
*/
