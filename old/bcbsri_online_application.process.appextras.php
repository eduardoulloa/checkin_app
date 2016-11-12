<?php
/* *******************************************************************************************
* Check Salesforce Connection
******************************************************************************************* */
function bcbsri_online_application_app_checksf() {
  drupal_add_http_header(
    'Cache-Control', 
    'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
    FALSE
  );
  ini_set("soap.wsdl_cache_enabled", "0");
  $curPath = dirname(__FILE__);
  include('bcbsri_online_application.define.salesforce.inc');
  $config = bcbsri_online_application_get_json('config');
  $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
  $year = bcbsri_online_application_get_year($auth);
  $alias = bcbsri_online_application_get_alias($auth);
  if (isset($_SERVER['AH_SITE_ENVIRONMENT']) && $_SERVER['AH_SITE_ENVIRONMENT'] != 'prod') {
    $sfdcUsername = $sfLogin['sandbox']['sf_username']; #$config['sandbox'][0]['sf_username'];
    $sfdcPassword = $sfLogin['sandbox']['sf_password']; #$config['sandbox'][0]['sf_password'];
    $sfdcToken = $sfLogin['sandbox']['sf_token']; #$config['sandbox'][0]['sf_token'];
    $sfdcWsdl = $sfLogin['sandbox']['sf_wsdl']; #$config['sandbox'][0]['sf_wsdl'];
    $sfdcClientId = $sfLogin['sandbox']['sf_client_id'];
    $sfdcClientSecret = $sfLogin['sandbox']['sf_client_secret'];
  } else {
    $sfdcUsername = $sfLogin['production']['sf_username']; #$config['production'][0]['sf_username'];
    $sfdcPassword = $sfLogin['production']['sf_password']; #$config['production'][0]['sf_password'];
    $sfdcToken = $sfLogin['production']['sf_token']; #$config['production'][0]['sf_token'];
    $sfdcWsdl = $sfLogin['production']['sf_wsdl']; #$config['production'][0]['sf_wsdl'];
    $sfdcClientId = $sfLogin['production']['sf_client_id'];
    $sfdcClientSecret = $sfLogin['production']['sf_client_secret'];
  }
  $sfWsdlPath = $curPath.'/wsdl/';
  try {
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      if(bcbsri_online_application_check_auth($auth)) {
        $mySforceConnection = new SforceEnterpriseClient();
        $mySforceConnection->createConnection($sfWsdlPath . $sfdcWsdl);
        $loginResult = $mySforceConnection->login($sfdcUsername, $sfdcPassword.$sfdcToken);
        return array('success' => true);
      } else return array('success' => false, 'message' => 'Failed auth check');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    include_once('bcbsri_online_application.test_salesforce.php');
    return array('success' => false, 'message' => 'Failed to test salesforce', 'error' => $e->getMessage());
  }
}

/* *******************************************************************************************
* Check Salesforce for existing member
******************************************************************************************* */
function bcbsri_online_application_app_checkmember() {
  drupal_add_http_header(
    'Cache-Control', 
    'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
    FALSE
  );
  ini_set("soap.wsdl_cache_enabled", "0");
  $curPath = dirname(__FILE__);
  include('bcbsri_online_application.define.salesforce.inc');
  $config = bcbsri_online_application_get_json('config');
  $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
  $year = bcbsri_online_application_get_year($auth);
  $alias = bcbsri_online_application_get_alias($auth);
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
  try {
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      if(bcbsri_online_application_check_auth($auth)) {
        $params = "grant_type=password"
          . "&client_id=" . $sfdcClientId
          . "&client_secret=" . $sfdcClientSecret
          . "&username=" . $sfdcUsername
          . "&password=" . $sfdcPassword;
        
        $curl = curl_init($sfdcURL . '/services/oauth2/token');
        curl_setopt($curl, CURLOPT_HEADER, false);
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($curl, CURLOPT_POST, true);
        curl_setopt($curl, CURLOPT_POSTFIELDS, $params);
        
        $json_response = curl_exec($curl);
        $status = curl_getinfo($curl, CURLINFO_HTTP_CODE);
        $response = json_decode($json_response, true);
        if($sfArray['debug']) {
          $sfArray['sf_login']['response'] = $response;
          $sfArray['sf_login']['status'] = $status;
        }
        if ( $status != 200 ) {
          $sfArray['sf_login']['error'] = curl_error($curl);
          $sfArray['success'] = false;
          $sfArray['message'] = 'Failed login to salesforce';
          return $sfArray;
        }
        $access_token = $response['access_token'];
        $instance_url = $response['instance_url'];
        $url = $instance_url . _SF_MA_LOOKUP_URI_;
        $content = json_encode(array('lkup' => array('hicn' => formatMedicareNumber($_REQUEST['mid']), 'memberId' => $_REQUEST['lid'])));
        if($sfArray['debug']) {
          $sfArray['sf_lookup']['access_token'] = $access_token;
          $sfArray['sf_lookup']['instance_url'] = $instance_url;
          $sfArray['sf_lookup']['url'] = $url;
          $sfArray['sf_lookup']['content'] = $content;
        }
        
        $curlLU = curl_init($url);
        curl_setopt($curlLU, CURLOPT_HEADER, false);
        curl_setopt($curlLU, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($curlLU, CURLOPT_HTTPHEADER, array("Authorization: OAuth $access_token", "Content-type: application/json"));
        curl_setopt($curlLU, CURLOPT_POSTFIELDS, $content);
        
        $json_response = curl_exec($curlLU);
        $lkResponse = json_decode($json_response, true);
        $status = curl_getinfo($curlLU, CURLINFO_HTTP_CODE);
        if($sfArray['debug']) {
          $sfArray['sf_lookup']['response'] = $lkResponse;
          $sfArray['sf_lookup']['status'] = $status;
        }
        if($status != 200) {
          include_once('bcbsri_online_application.test_salesforce.php');
          $sfArray['sf_lookup']['error'] = curl_error($curl);
          $sfArray['success'] = false;
          $sfArray['message'] = 'Failed lookup in Salesforce';
          return $sfArray;
        }
        if(strtolower($lkResponse['result']) != 'success') {
          include_once('bcbsri_online_application.test_salesforce.php');
          $sfArray['sf_lookup']['error'] = curl_error($curl);
          $sfArray['success'] = false;
          $sfArray['message'] = 'Member does not exist';
          return $sfArray;
        }
        if(strtolower($lkResponse['result']) != 'error') {
          $member = array();
          foreach($lkResponse as $key => $value) {
            if(!is_array($value)) $member[$key] = $value;
          }
          $sfArray['success'] = true;
          $sfArray['mLookup'] = $member;
          return $sfArray;
        } else {
          $sfArray['success'] = false;
          $sfArray['message'] = 'Account not found';
          return $sfArray;
        }
      } else {
        $sfArray['success'] = false;
        $sfArray['message'] = 'Failed auth check';
        return $sfArray;
      }
    } else {
      $sfArray['success'] = false;
      $sfArray['message'] = 'No data sent';
      return $sfArray;
    }
  } catch (Exception $e) {
    include_once('bcbsri_online_application.test_salesforce.php');
    $sfArray['success'] = false;
    $sfArray['error'] = $e->getMessage();
    $sfArray['message'] = 'Failed to test salesforce';
    return $sfArray;
  }
}

function bcbsri_online_application_app_sendemail() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      if(bcbsri_online_application_check_auth($_REQUEST['auth'])) {
        $auth = $_REQUEST['auth'];
        $data = json_decode(str_replace(array('\"', "\'"), array('"', "'"), $_REQUEST['d']), true);
        // dataToBeSent = 'process=send-email&auth=' + auth + '&form_confirmation=' + sfData.confirmation + '&form_year=' + year + '&d=' + formData + '&form_email=' + $('input[name="email_address"]').val();
        $to = filter_var($_REQUEST['form_email'], FILTER_SANITIZE_EMAIL);
        // $cValue = bcbsri_online_application_get_cookie('planDetails');
        if(bcbsri_online_application_get_alias($auth) == 'medicare') $appName = "BlueCHiP for Medicare";
        else if(bcbsri_online_application_get_alias($auth) == 'direct_pay') $appName = "Direct Pay";
        
        $headers = "From: website@bcbsri.org\r\n";
        // $headers .= "Reply-To: \r\n";
        $headers .= "MIME-Version: 1.0\r\n";
        $headers .= "Content-Type: text/html; charset=ISO-8859-1\r\n";
        $subject = bcbsri_online_application_get_year($auth) . ' ' . $appName . ' Application';
        $message = '<p><strong>Confirmation Number: ' . $_REQUEST['form_confirmation'] . '</strong></p>' . $_REQUEST['dplans'] . ' ' . $_REQUEST['d'];
        return array('success' => mail($to, $subject, $message, $headers));   
      } else return array('success' => false, 'message' => 'Failed auth check');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to send email', 'error' => var_dump($e, true));
  }
}

function bcbsri_online_application_app_sendresumeemail() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      if(bcbsri_online_application_check_auth($auth)) {
        $data = json_decode(str_replace(array('\"', "\'"), array('"', "'"), $_REQUEST['d']), true);
        // dataToBeSent = 'process=send-email&auth=' + auth + '&form_confirmation=' + sfData.confirmation + '&form_year=' + year + '&d=' + formData + '&form_email=' + $('input[name="email_address"]').val();
        $to = filter_var($_REQUEST['saveapp_email'], FILTER_SANITIZE_EMAIL);
        // $cValue = bcbsri_online_application_get_cookie('planDetails');
        bcbsri_online_application_db_set($tableData, array('auth' => $auth, 'resume_email' => $to), 'auth');
        if(bcbsri_online_application_get_alias($auth) == 'medicare') $appName = "BlueCHiP for Medicare";
        else if(bcbsri_online_application_get_alias($auth) == 'direct_pay') $appName = "Direct Pay";
        
        $headers = "From: website@bcbsri.org\r\n";
        // $headers .= "Reply-To: \r\n";
        $headers .= "MIME-Version: 1.0\r\n";
        $headers .= "Content-Type: text/html; charset=ISO-8859-1\r\n";
        $subject = 'We saved your form for you';
        $message = '<p>We have saved your form right where you left off. When you&rsquo;re ready to complete the process, use the link below to return to bcbsri.com. You will need your <strong>form ID</strong> below. We will save your form for 30 days.<br /><br />

If you have any questions, or if you just want a little assistance finishing the form, please let us help. You can call us seven days a week or stop into your nearest Your Blue Store, and we&rsquo;ll walk you through the rest of the process.<br /><br />

Form ID: '.$_REQUEST['rk'].'</p>

<p><a href="' . (isset($_SERVER['AH_SITE_ENVIRONMENT']) && $_SERVER['AH_SITE_ENVIRONMENT'] != 'prod' ? 'http://bcbsrihipaastg.prod.acquia-sites.com' : 'https://www.bcbsri.com') . '/shop/resume">' . (isset($_SERVER['AH_SITE_ENVIRONMENT']) && $_SERVER['AH_SITE_ENVIRONMENT'] != 'prod' ? 'http://bcbsrihipaastg.prod.acquia-sites.com' : 'https://www.bcbsri.com') . '/shop/resume</a></p>

<h3>Call us</h3>

<p>Call the BlueCHiP for Medicare Sales Department at 1-800-505-BLUE (2583) (TTY users should call 711).<br /><br /> 

Monday through Friday, 8:00 a.m. to 8:00 p.m. (Open seven days a week, 8:00 a.m. to 8:00 p.m., from October 1 – February 14.)<br /><br />

You can use our automated answering system outside of these hours.</p>

<h3>Visit Your Blue Store</h3>

<p><strong>Warwick:</strong> Cowesett Corners, 300 Quaker Lane<br /><br />
<strong>Bristol:</strong> Bell Tower Plaza, 576 Metacom Ave., Unit 18<br /><br />
<strong>Lincoln:</strong> Lincoln Mall, 622 George Washington Highway<br /><br />

Monday – Friday, 9:00 a.m. to 5:00 p.m.<br /> 
(Extended hours: October 15, 2015 – January 31, 2016, Monday until 6:00 p.m. and Saturday, 9:00 a.m. to 1:00 p.m.)</p> 

';
        return array('success' => mail($to, $subject, $message, $headers));   
      } else return array('success' => false, 'message' => 'Failed auth check');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to send email', 'error' => var_dump($e, true));
  }
}

/* *******************************************************************************************
 * Get form pages
 ******************************************************************************************* */
function bcbsri_online_application_app_getformpages() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    $curPath = dirname(__FILE__);
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      if(bcbsri_online_application_check_auth($_REQUEST['auth'])) {
        $auth = $_REQUEST['auth'];
        $year = bcbsri_online_application_get_year($auth);
        $data = (!empty ($_REQUEST['d']) ? json_decode(str_replace(array('\"', "\'"), array('"', "'"), $_REQUEST['d']), true) : '');
        $content = new BCBSRIOnlineApplication($curPath . "/content2.json", $_REQUEST['form'], $year, $_REQUEST['dental_only']);
        /* TODO: Possibly remove individual calls and out put all pages at once */
        return json_decode($content->getDisplayPage($_REQUEST['form_page'], $_REQUEST['form_view']), true);
      } else return array('success' => false, 'message' => 'Failed auth check');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to get form page', 'error' => var_dump($e, true));
  }
}

/* *******************************************************************************************
 * Get Form Config
 ******************************************************************************************* */
function bcbsri_online_application_app_getformconfig() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    $curPath = dirname(__FILE__);
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      if(bcbsri_online_application_check_auth($auth)) {
        $year = bcbsri_online_application_get_year($auth);
        $data = (!empty ($_REQUEST['d']) ? json_decode(str_replace(array('\"', "\'"), array('"', "'"), $_REQUEST['d']), true) : '');
        $content = new BCBSRIOnlineApplication($curPath . "/content2.json", $_REQUEST['form'], $year, $_REQUEST['dental_only']);
        $config = json_decode($content->getConfig(), true);
        $quote = bcbsri_online_application_get_quote($auth);
        if(!empty($config)) {
            if(!empty($quote)) {
              $quote = explode('|', $quote);
              foreach($quote as $q) {
                $t = explode(':', $q);
                $config[$t[0]] = $t[1];
              }
            }
          $config['success'] = true;
          return $config;
        } else return array('success' => false, 'message' => 'Failed getting config');
      } else return array('success' => false, 'message' => 'Failed auth check');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to ', 'error' => var_dump($e, true));
  }
}

/* *******************************************************************************************
 * Get number of pages/steps in the application
 ******************************************************************************************* */
function bcbsri_online_application_app_getform_numofpages() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    $curPath = dirname(__FILE__);
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      if(bcbsri_online_application_check_auth($_REQUEST['auth'])) {
        $year = bcbsri_online_application_get_year($auth);
        $data = (!empty ($_REQUEST['d']) ? json_decode(str_replace(array('\"', "\'"), array('"', "'"), $_REQUEST['d']), true) : '');
        $content = new BCBSRIOnlineApplication($curPath . "/content2.json", $_REQUEST['form'], $year, $_REQUEST['dental_only']);
        $numSteps = $content->getNumberofSteps();
        $success = ($numSteps > 0 ? true : false);
        return array('success' => $success, 'numSteps' => $numSteps);
      } else return array('success' => false, 'message' => 'Failed auth check');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to get pages', 'error' => var_dump($e, true));
  }
}

/* *******************************************************************************************
 * Get data
 ******************************************************************************************* */
function bcbsri_online_application_app_getdata() {
  /*try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      if(bcbsri_online_application_check_auth($_REQUEST['auth'])) {
        $year = bcbsri_online_application_get_year($auth);
        $data = (!empty ($_REQUEST['d']) ? json_decode(str_replace(array('\"', "\'"), array('"', "'"), $_REQUEST['d']), true) : '');
      } else return array('success' => false, 'message' => 'Failed auth check');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to get data', 'error' => var_dump($e, true));
  }*/
  
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
    //if($_REQUEST['process'] == "update-timeout") {
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      //$auth = bcbsri_online_application_appauth();
      //if(bcbsri_online_application_auth_verify($auth) == true){
      if($auth != false){
        // Get the app's fields.
        $query = db_select('bcbsri_online_application_ufields', 'boaf');
        $query->fields('boaf', array('flds'));
        $query->condition('boaf.auth', $auth);
        $queryResult = $query->execute();
/*.......*/
        foreach($queryResult as $f) {
            $fields = json_decode(bcbsri_online_application_decrypt($f->flds, $auth), true);
        }
        
        if(!empty($fields)) return array('success' => true, 'fields' => $fields);
        else return array('success' => false, 'reason' => 'The application data is not available.');
      } else return array('success' => false, 'reason' => 'Could not authenticate user.');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to get data.', 'errors' => var_dump($e, true));
  }
  
}