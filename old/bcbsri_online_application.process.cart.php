<?php
function bcbsri_online_application_appprocess_setplan() {
  drupal_add_http_header(
    'Cache-Control', 
    'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
    FALSE
  );
  try {
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      /*$sessionCookie = session_get_cookie_params();
      $name = $_REQUEST['cookie_name'];
      $cookie = 'medical:';
      $cookie .= ($_REQUEST['plan_medical'] != '' ? $_REQUEST['plan_medical'] : '');
      $cookie .= ',dental:';
      $cookie .= ($_REQUEST['plan_dental'] != '' ? $_REQUEST['plan_dental'] : '');
      bcbsri_online_application_set_cookie(array($name, $cookie, $_REQUEST['plan_year'], $_REQUEST['plan_type']));*/
      $plans['medical'] = (!empty($_REQUEST['plan_medical']) ? $_REQUEST['plan_medical'] : '');
      $plans['dental'] = (!empty($_REQUEST['plan_dental']) ? $_REQUEST['plan_dental'] : '');
      //$appauth = bcbsri_online_application_appauth();
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      $record = array();
      if(bcbsri_online_application_auth_verify($auth)) {
      //if(!empty($appauth['auth'])){
        bcbsri_online_application_set_alias($auth, $_REQUEST['plan_type']);
        bcbsri_online_application_set_year($auth, $_REQUEST['plan_year']);
        #if($_REQUEST['recommended'] != '')  $record['recommended'] = $_REQUEST['recommended'];
        if(!empty($plans)) $record['selected'] = json_encode($plans);
        #if($_REQUEST['purchased'] != '') $record['purchased'] = $_REQUEST['purchased'];
        #if($_REQUEST['recommended_quest'] != '') $record['recommended_quest']= $_REQUEST['recommended_quest'];
        $record['auth'] = $auth;
        $urecord = array('auth' => $auth, 'app_type' => $_REQUEST['plan_type'], 'app_year' => $_REQUEST['plan_year']);
        bcbsri_online_application_db_set('bcbsri_online_application_plans', $record, 'auth');
        bcbsri_online_application_db_set('bcbsri_online_application_udata', $urecord, 'auth');
        return array('success' => true, 'message' => 'Set plans');
      } else return array('success' => false, 'message' => 'Failed to set plans, invalid auth');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to set plans', 'error' => var_dump($e, true));
  }
}

function bcbsri_online_application_appprocess_adddental() {
  drupal_add_http_header(
    'Cache-Control', 
    'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
    FALSE
  );
//else if($requestProcess == "addDental") {
  try {
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      $form_alias = NULL; $form_year = NULL;
      $name = $_REQUEST['cookie_name'];
      
      //$pDetails = bcbsri_online_application_get_cookie('planDetails', $form_alias, $form_year);
      
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      $result = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
      if(!empty($_REQUEST['form_alias'])) {
        bcbsri_online_application_set_alias($auth, $_REQUEST['form_alias']);
        $form_alias = $_REQUEST['form_alias'];
      }
      if(!empty($_REQUEST['form_year'])) {
        bcbsri_online_application_set_year($auth, $_REQUEST['form_year']);
        $form_year = $_REQUEST['form_year'];
      }
      foreach($result as $r) {
        $pDetails = json_decode($r->selected, true);
      }
      
      /*if($pDetails) {
        $pDetails = explode(',', $pDetails);
        $pMedical = explode(':',$pDetails[0]);
        $pDental = explode(':',$pDetails[1]);
      }*/
      /* $cookie = 'medical:';
      $cookie .= (!empty($pMedical[1]) ? $pMedical[1] : '');
      $cookie .= ',dental:';
      $cookie .= (!empty($_REQUEST['cookie_value']) ? $_REQUEST['cookie_value'] : ''); */
      $cookie['medical'] = (!empty($pDetails['medical']) ? $pDetails['medical'] : '');
      $cookie['dental'] = (!empty($_REQUEST['cookie_value']) ? $_REQUEST['cookie_value'] : '');
      
      $record = array();
      $record['selected'] = json_encode($cookie);
      $record['auth'] = $auth;
      bcbsri_online_application_db_set('bcbsri_online_application_plans', $record, 'auth');
      //bcbsri_online_application_set_cookie(array($name, $cookie, $form_year, $form_alias));
      return array('success' => true, 'data' => $cookie);
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'error' => var_dump($e, true));
  }
  //die();
}

function bcbsri_online_application_appprocess_addmedical() {
  drupal_add_http_header(
    'Cache-Control', 
    'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
    FALSE
  );
//else if($requestProcess == "addMedical") {
  try {
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      $form_alias = NULL; $form_year = NULL;
      $name = $_REQUEST['cookie_name'];
      //$pDetails = bcbsri_online_application_get_cookie('planDetails', $form_alias, $form_year);
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      $result = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
      if(!empty($_REQUEST['form_alias'])) {
        bcbsri_online_application_set_alias($auth, $_REQUEST['form_alias']);
        $form_alias = $_REQUEST['form_alias'];
      }
      if(!empty($_REQUEST['form_year'])) {
        bcbsri_online_application_set_year($auth, $_REQUEST['form_year']);
        $form_year = $_REQUEST['form_year'];
      }
      foreach($result as $r) {
        $pDetails = json_decode($r->selected, true);
      }
      /*if($pDetails) {
        $pDetails = explode(',', $pDetails);
        $pMedical = explode(':',$pDetails[0]);
        $pDental = explode(':',$pDetails[1]);
      }*/
      /*$cookie = 'medical:';
      $cookie .= (!empty($_REQUEST['cookie_value']) ? urldecode($_REQUEST['cookie_value']) : '');
      $cookie .= ',dental:';
      $cookie .= (!empty($pDental[1]) ? $pDental[1] : '');*/
      $cookie['medical'] = (!empty($_REQUEST['cookie_value']) ? $_REQUEST['cookie_value'] : '');
      $cookie['dental'] = (!empty($pDetails['dental']) ? $pDetails['dental'] : '');

      //bcbsri_online_application_set_cookie(array($name, $cookie, $form_year, $form_alias));
      $record = array();
      $record['selected'] = json_encode($cookie);
      $record['auth'] = $auth;
      bcbsri_online_application_db_set('bcbsri_online_application_plans', $record, 'auth');
      return array('success' => true, 'data' => $cookie);
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'error' => var_dump($e, true));
  }
  //die();
}

function bcbsri_online_application_appprocess_editplanlist() {
  drupal_add_http_header(
    'Cache-Control', 
    'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
    FALSE
  );
//else if($requestProcess == "editPlanList") {
  if($_SERVER['REQUEST_METHOD'] == 'POST') {
    if(!empty($_REQUEST['planList'])) {
      $sessionCookie = session_get_cookie_params();
      setcookie('bcbsri_plan_list', $_REQUEST['planList'], 0, '/', $_SERVER['HTTP_HOST'], $sessionCookie['secure'], TRUE);
      return array('success' => true);
    } else return array('success' => false, 'reason' => 'No data to set');
  } else return array('success' => false, 'message' => 'No data sent');
  //die();
}

function bcbsri_online_application_appprocess_getplanlist() {
  drupal_add_http_header(
    'Cache-Control', 
    'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
    FALSE
  );
//else if($requestProcess == "getPlanList") {
  if($_SERVER['REQUEST_METHOD'] == 'POST') {
    $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
    if ($auth != false){
      $result = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
      foreach($result as $r) {
        $planList = json_decode($r->selected, true);
      }
      return array('success' => true, 'planList' => $planList);
    } else {
      //if(!empty($_COOKIE['bcbsri_plan_list'])) return array('success' => true, 'planList' => urldecode($_COOKIE['bcbsri_plan_list']));
      return array('success' => false, 'reason' => 'Auth not valid.'); 
    }
  } else return array('success' => false, 'message' => 'No data sent');
  //die();
}

/* **********************************
 * Can send planList or getOptions as process request
 ********************************** */
function bcbsri_online_application_appprocess_planlist() {
//else if($requestProcess == "planList" || $requestProcess == "getOptions") {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      $requestProcess = $_REQUEST['process'];
      //include_once('BCBSRIPlanList.inc');
      if (isset($_SERVER['AH_NON_PRODUCTION']) && $_SERVER['AH_SITE_ENVIRONMENT'] != 'prod') $auth = "bcbsri:testing@";
      else $auth = '';
      if (isset($_SERVER['AH_NON_PRODUCTION']) && $_SERVER['AH_SITE_ENVIRONMENT'] != 'prod') {
        $protocol = (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off' || $_SERVER['SERVER_PORT'] == 443) ? "https://" : "http://";
        $domainName = $_SERVER['HTTP_HOST'];
      } else {
        $protocol = 'https://';
        $domainName = 'www.bcbsri.com';
      }
      #$protocol = (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off' || $_SERVER['SERVER_PORT'] == 443) ? "https://" : "http://";
      #$domainName = $_SERVER['HTTP_HOST'];
      $pList = $protocol.$auth.$domainName;
      if(empty($_REQUEST['plan_list']) || $_REQUEST['plan_list'] == 'NULL') $pList .= '/shop-for-plan/' . $_REQUEST['form_year'] . '/json';
      else $pList .= $_REQUEST['plan_list'];
      $plans = new BCBSRIPlanList($pList, $_REQUEST['form_alias'], $_REQUEST['form_year'], $_REQUEST['form_sort']);
      //return array('plans'=>$plans);
      if($requestProcess == "planList") {
        if($plans->getJson() == false) return array('success' => false, 'message' => 'failed to read json file', 'file' => $plans->getJsonfile());
        else return array('success' => true, 'medical' => $plans->getDisplay('medical'), 'dental' => $plans->getDisplay('dental'));
      } else {
        if($plans->getJson() == false) return array('success' => false, 'message' => 'failed to read json file', 'file' => $plans->getJsonfile());
        else return array('success' => true, 'opts' => $plans->getOptions());
      }
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'failed to get plans', 'error' => var_dump($e, true));
  }
  //die();
}