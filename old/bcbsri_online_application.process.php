<?php
function bcbsri_online_application_appprocess() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    
    $curPath = dirname(__FILE__);
    if (! defined('DRUPAL_ROOT')) define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
    require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    
    #include_once('bcbsri_online_application.helper.inc');
    /* ........ */
    
    #include_once('BCBSRIOnlineApplication.inc');
    module_load_include('inc', 'bcbsri_online_application', 'BCBSRIOnlineApplication');
    
    $config = bcbsri_online_application_get_json('config');
    if (isset($_SERVER['AH_SITE_ENVIRONMENT']) && $_SERVER['AH_SITE_ENVIRONMENT'] != 'prod') {
      $sfdcUsername = $config['sandbox'][0]['sf_username'];
      $sfdcPassword = $config['sandbox'][0]['sf_password'];
      $sfdcToken = $config['sandbox'][0]['sf_token'];
      $sfdcWsdl = $config['sandbox'][0]['sf_wsdl'];
    } else {
      $sfdcUsername = $config['production'][0]['sf_username'];
      $sfdcPassword = $config['production'][0]['sf_password'];
      $sfdcToken = $config['production'][0]['sf_token'];
      $sfdcWsdl = $config['production'][0]['sf_wsdl'];
    }
    $sfWsdlPath = $curPath.'/wsdl/';

    $requestProcess = (!empty($_REQUEST['process']) ? $_REQUEST['process'] : 'not-entered');
    if($_SERVER['REQUEST_METHOD'] == 'POST' || $requestProcess == "send-to-salesforce"){
      /* *******************************************************************************************
       * Any functions that are shopping cart or plan list specific
       ******************************************************************************************* */
      try {
        #require_once('bcbsri_online_application.process.cart.php');
        #module_load_include('php', 'bcbsri_online_application', 'bcbsri_online_application.process.cart');
        require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'bcbsri_online_application') . "/bcbsri_online_application.process.cart.php";
      } catch (Exception $e) {
        return array('success' => false, 'reason' => 'Failed Include', 'process' => $requestProcess, 'error' => var_dump($e, true));
      }

      /* *******************************************************************************************
       * Application start functionality
       ******************************************************************************************* */
      if($requestProcess == "app-start") {
        //$auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
        $auth = json_decode(bcbsri_online_application_appauth(), true);
        //$skey = (bcbsri_online_application_get_cookie('ks') ? bcbsri_online_application_get_cookie('ks') : bcbsri_online_application_authgen(16));
        //bcbsri_online_application_set_cookie(array('ks', $skey));
        //$pDetails = bcbsri_online_application_get_cookie('planDetails');
        $result = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
        foreach($result as $r) {
          $pDetails = $r->selected;
        }
        $pDetails = explode(',', $pDetails);
        $pMedical = explode(':',$pDetails[0]);
        $pDental = explode(':',$pDetails[1]);
        if(!empty($pDental[1]) && empty($pMedical[1])) $dental = true;
        else $dental = false;        
        if($auth['success'] == true) {
          $browser = (!empty($_REQUEST['uastring']) ? $_REQUEST['uastring'] : '');
          $table = 'bcbsri_online_application_udata';
          $record = array('ua_info' => $browser, 'end_time' => $auth['mtime']);
          /* $record = new stdClass();
          $record->auth = $auth['auth'];
          $record->skey = $skey;
          $record->app_type = bcbsri_online_application_get_year() . ' ' . bcbsri_online_application_get_alias(); // year and product
          $record->start_time = $auth['stime'];
          $record->end_time = $auth['mtime'];
          $record->ua_info = $browser;
          drupal_write_record($table, $record); */
          bcbsri_online_application_db_set($table, $record, $auth);
          return array('success' => true, 'auth' => $auth, 'alias' => bcbsri_online_application_get_alias(), 'year' => bcbsri_online_application_get_year(), 'dental' => $dental); /* , 'app' => bcbsri_online_application_get_cookie('prod'))); */
        } else return array('success' => false, 'reason' => 'Auth Failed', 'process' => $requestProcess);
        
      /* *******************************************************************************************
       * Application clear cookies
       ******************************************************************************************* */
      } else if($requestProcess == "clear-data") {
        // clear the following cookies
        try {
          bcbsri_online_application_clear_cookies();
          return array('success' => true);
        } catch (Exception $e) {
          return array('success' => false, 'reason' => 'Failed to clear cookies', 'process' => $requestProcess, 'error' => var_dump($e, true));
        }
      } else if($requestProcess == "edit-quote") {
        if(!empty($_REQUEST['quoteInfo'])) {
          bcbsri_online_application_set_quote($_REQUEST['quoteInfo']);
          $covered = bcbsri_online_application_get_covered();
          if($covered != false) {
            $covered = json_decode($covered, true);
            $coverage = explode('|', $_REQUEST['quoteInfo']);
            $whosCovered = '';
            foreach($coverage as $c) {
              $t = explode(':', $c);
              switch($t[0]) {
                case 'memberAge':
                  $pricing['applicant']['age'] = (int)$t[1];
                  $pricing['applicant']['medical'] = (!empty($covered['applicant']['medical']) ? $covered['applicant']['medical'] : 'yes');
                  $pricing['applicant']['dental'] = (!empty($covered['applicant']['dental']) ? $covered['applicant']['dental'] : 'yes');
                  $pricing['applicant']['price'] = (!empty($covered['applicant']['price']) ? $covered['applicant']['price'] : '');
                  break;
                case 'spouseAge':
                  if(!empty(trim($t[1])) && (int)$t[1] > 0 && $t[1] != 'none') {
                    $pricing['spouse']['age'] = (int)$t[1];
                    $pricing['spouse']['medical'] = (!empty($covered['spouse']['medical']) ? $covered['spouse']['medical'] : 'yes');
                    $pricing['spouse']['dental'] = (!empty($covered['spouse']['dental']) ? $covered['spouse']['dental'] : 'yes');
                    $pricing['spouse']['price'] = (!empty($covered['spouse']['price']) ? $covered['spouse']['price'] : '');
                  }
                  break;
                case 'covered':
                  $pricing['covered'] = $t[1];
                  $whosCovered = $t[1];
                  break;
                case 'kidCount':
                  $pricing['dependentcount'] = (int)$t[1];
                  break;
                case 'kidAges':
                  if(strpos($t[1],',') !== false) $ka = explode(',', $t[1]);
                  else if(!empty($t[1])) $ka[0] = $t[1];
                  else $ka = '';
                  if($ka[0] != 'none') { //if(!empty($ka) || $ka[0] != 'none') {
                    for($ki = 0; $ki < count($ka); $ki++) {
                      $pricing['dependent'][$ki]['age'] = (int)$ka[$ki];
                      $pricing['dependent'][$ki]['number'] = $ki + 1;
                      if((int)$ka[$ki] < 26) $pricing['dependent'][$ki]['medical'] = (!empty($covered['dependent'][$ki]['medical']) ? $covered['dependent'][$ki]['medical'] : 'yes');
                      else $pricing['dependent'][$ki]['medical'] = 'no';
                      if((int)$ka[$ki] < 26) $pricing['dependent'][$ki]['dental'] = (!empty($covered['dependent'][$ki]['dental']) ? $covered['dependent'][$ki]['dental'] : 'yes');
                      else $pricing['dependent'][$ki]['dental'] = 'no';
                      $pricing['dependent'][$ki]['price'] = (!empty($covered['dependent'][$ki]['price']) ? $covered['dependent'][$ki]['price'] : '');
                    }
                  }
                  break;
              }
            }
            bcbsri_online_application_set_covered(json_encode($pricing));
          }
          //$cPlanDetails = bcbsri_online_application_get_cookie(array($name, 'planDetails', 2016, 'directpay'));
          $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
          $result = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
          foreach($result as $r) {
            $cPlanDetails = $r->selected;
          }
          return array('success' => true, 'data' => $cPlanDetails);
        } else return array('success' => false, 'reason' => 'No data for quote', 'process' => $requestProcess);
      } else if($requestProcess == "get-quote") {
        $retData = bcbsri_online_application_get_quote();
        if($retData != false) return array('success' => true, 'quote' => $retData);
        else return array('success' => false, 'reason' => 'No data for quote', 'process' => $requestProcess);
      } else if($requestProcess == "get-price") {
        #include_once('bcbsri_online_application.planpricing.php');
        module_load_include('php', 'bcbsri_online_application', 'bcbsri_online_application.planpricing');
      } else if($requestProcess == "change-coverage") {
        $covered = bcbsri_online_application_get_covered();
        if($covered != false && !empty($_REQUEST['person']) && !empty($_REQUEST['planType']) && !empty($_REQUEST['covered'])) {
          $covered = json_decode($covered, true);
          if(strpos($_REQUEST['person'],'dependent') !== false) {
            $d = explode('-', $_REQUEST['person']);
            $id = (int)$d[1]; # - 1;
            if($covered['dependent'][$id]['age'] < 27) $covered['dependent'][$id][$_REQUEST['planType']] = $_REQUEST['covered'];
          }else $covered[$_REQUEST['person']][$_REQUEST['planType']] = $_REQUEST['covered'];
          bcbsri_online_application_set_covered(json_encode($covered));
          return array('success' => true, 'data' => $covered);
        } else return array('success' => false, 'reason' => 'incorrect data for coverage', 'process' => $requestProcess);
      } else if($requestProcess == "get-covered") {
        $covered = json_decode(bcbsri_online_application_get_covered(), true);
        if($covered != false) return array('success' => true, 'covered' => $covered);
        else return array('success' => false, 'reason' => 'failed to get covered', 'process' => $requestProcess);
      } else if($requestProcess == "set-alias-year") { 
        if(!empty($_REQUEST['plan_type']) && !empty($_REQUEST['plan_year'])) {
          $sessionCookie = session_get_cookie_params();
          bcbsri_online_application_set_alias($_REQUEST['plan_type']);
          bcbsri_online_application_set_year($_REQUEST['plan_year']);
          return array('success' => true);
        } else return array('success' => false, 'reason' => 'not enough data', 'process' => $requestProcess);
        
      /* *******************************************************************************************
      * Get Cookie
      ******************************************************************************************* */
      }else if($requestProcess == "get-cookie") {
        if(!empty($_REQUEST['form_cookie'])) {
          $form_alias = NULL; $form_year = NULL;
          if(!empty($_REQUEST['form_alias'])) {
            bcbsri_online_application_set_alias($_REQUEST['form_alias']);
            $form_alias = $_REQUEST['form_alias'];
          }
          if(!empty($_REQUEST['form_year'])) {
            bcbsri_online_application_set_year($_REQUEST['form_year']);
            $form_year = $_REQUEST['form_year'];
          }
          //$cValue = bcbsri_online_application_get_cookie($_REQUEST['form_cookie'], $form_alias, $form_year);
          $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
          $result = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
          foreach($result as $r) {
            $cValue = $r->selected;
          }
          if(!empty($cValue)) {
            if($_REQUEST['form_cookie'] == 'planDetails') {
              $plans = array();
              $plans['success'] = true;
              if($form_alias == 'directpay') {
                $plans['data'] = $cValue;
              } else {
                $cValue = explode(',', $cValue);
                $plans['success'] = true;
                $medical = explode(':',$cValue[0]);
                $dental = explode(':',$cValue[1]);
                if(!empty($medical[1])) {
                   $plans['medical'] = true;
                   $medical = explode('|', $medical[1]);
                   $plans['medicalPlan'] = $medical[0];
                   $plans['medicalNetwork'] = $medical[1];
                   $mCost = explode('.', $medical[2]);
                   $mCost[0] = str_replace('$', '', $mCost[0]);
                   //$mCost[1] = (empty($mCost[1]) ? '00' : $mCost[1]);
                   $mCost[1] = (empty($mCost[1]) ? '00' : (strlen($mCost[1]) < 2 ? $mCost[1] . '0' : $mCost[1]));
                   $plans['medicalCost'] = '$' . $mCost[0] . '<sup>.' . $mCost[1] . '</sup>';
                } else $plans['medical'] = false;
                if(!empty($dental[1])) {
                   $plans['dental'] = true;
                   $dental = explode('|', $dental[1]);
                   $plans['dentalPlan'] = $dental[0];
                   $plans['dentalNetwork'] = $dental[1];
                   $dCost = explode('.', $dental[2]);
                   $dCost[0] = str_replace('$', '', $dCost[0]);
                   $dCost[1] = (empty($dCost[1]) ? '00' : (strlen($dCost[1]) < 2 ? $dCost[1] . '0' : $dCost[1]));
                   $plans['dentalCost'] = '$' . $dCost[0] . '<sup>.' . $dCost[1] . '</sup>';
                } else $plans['dental'] = false;
              }
              return $plans;
            } else return array('success' => true, 'cValue' => $cValue);
          } else return array('success' => false, 'reason' => $_REQUEST['form_cookie'] . ' cookie not assigned', 'cValue' => $cValue, 'process' => $requestProcess);
        } else return array('success' => false, 'reason' => 'Failed to get cookie info', 'process' => $requestProcess);
        
      } else {
        
         /* *******************************************************************************************
        * Verify Auth code and year/review
        ******************************************************************************************* */
        $year = (is_numeric($_REQUEST['form_year']) ? intval($_REQUEST['form_year']): 0);
        if(bcbsri_online_application_check_auth($_REQUEST['auth']) > 0 && $year != 0) {
          $data = json_decode(str_replace(array('\"', "\'"), array('"', "'"), $_REQUEST['d']), true);
          #include_once('bcbsri_online_application_send_to_salesforce.inc');
          #module_load_include('inc', 'bcbsri_online_application', 'bcbsri_online_application_send_to_salesforce');
          #include_once('bcbsri_online_application.process.timeout.inc');
          module_load_include('inc', 'bcbsri_online_application', 'bcbsri_online_application.process.timeout');
          #include_once('bcbsri_online_application.process.validate_app.inc');
          module_load_include('inc', 'bcbsri_online_application', 'bcbsri_online_application.process.validate_app');
          #include_once('bcbsri_online_application.process.verify_plan_change.inc');
          module_load_include('inc', 'bcbsri_online_application', 'bcbsri_online_application.process.verify_plan_change');
          module_load_include('inc', 'bcbsri_online_application', 'bcbsri_online_application.process.getdata');
          
          /* *******************************************************************************************
          * Get data
          ******************************************************************************************* */
          if($requestProcess == "get-data") {
            /*include_once('bcbsri_online_application.process.getdata.inc');*/
            //module_load_include('inc', 'bcbsri_online_application', 'bcbsri_online_application.process.getdata');
          
          /* *******************************************************************************************
          * Check Salesforce Connection
          ******************************************************************************************* */
          }else if($requestProcess == "check-sf") {
            try {
              $mySforceConnection = new SforceEnterpriseClient();
              $mySforceConnection->createConnection($sfWsdlPath.basename($sfdcWsdl));
              $loginResult = $mySforceConnection->login($sfdcUsername, $sfdcPassword.$sfdcToken);
              return array('success' => true);
            } catch (Exception $e) {
              #include_once('bcbsri_online_application.test_salesforce.php');
              module_load_include('inc', 'bcbsri_online_application', 'bcbsri_online_application.test_salesforce');
              return array('success' => false, 'process' => $requestProcess, 'error' => var_dump($e, true));
            }
          /* *******************************************************************************************
          * Send to Salesforce
          ******************************************************************************************* */
          }else if($requestProcess == "send-to-salesforce") {
            if($data != '') {
              //include_once('bcbsri_online_application_send_to_salesforce.inc');
              module_load_include('inc', 'bcbsri_online_application', 'bcbsri_online_application_send_to_salesforce');
            } else return array('success' => false, 'reason' => 'Failed to send to salesforce', 'process' => $requestProcess);
            
          /* *******************************************************************************************
          * Get Form Config
          ******************************************************************************************* */
          } else if($requestProcess == "get-form-config") {
            $content = new BCBSRIOnlineApplication($curPath . "/content2.json", $_REQUEST['form'], $year, $_REQUEST['dental_only']);
            $config = json_decode($content->getConfig(), true);
            $quote = bcbsri_online_application_get_quote();
            if($quote !== true) {
              $quote = explode('|', bcbsri_online_application_get_quote());
              foreach($quote as $q) {
                $t = explode(':', $q);
                $config[$t[0]] = $t[1];
              }
            }
            print $config;
            
          /* *******************************************************************************************
          * Get form pages
          ******************************************************************************************* */
          } else if($requestProcess == "get-form") {
            $content = new BCBSRIOnlineApplication($curPath . "/content2.json", $_REQUEST['form'], $year, $_REQUEST['dental_only']);
            /* TODO: Possibly remove individual calls and out put all pages at once */
            print $content->getDisplayPage($_REQUEST['form_page'], $_REQUEST['form_view']);
            
          /* *******************************************************************************************
          * Get number of pages/steps in the application
          ******************************************************************************************* */
          } else if($requestProcess == "get-form-pages") {
            try {
              $content = new BCBSRIOnlineApplication($curPath . "/content2.json", $_REQUEST['form'], $year, $_REQUEST['dental_only']);
              $numSteps = $content->getNumberofSteps();
              $success = ($numSteps > 0 ? true : false);
              return array('success' => $success, 'numSteps' => $numSteps);
            } catch (Exception $e) {
              return array('success' => false, 'reason' => 'Failed to get pages', 'process' => $requestProcess, 'error' => var_dump($e, true));
            }
            
          /* *******************************************************************************************
          * Process data as they are entered or next is clicked
          ******************************************************************************************* */
          } else if($requestProcess == "process-data") {
            try {
              if($data != '') {
                    $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
                
                    $query = db_select('bcbsri_online_application_ufields', 'boad');
                    $query->fields('boad', array('flds'));
                    $query->condition('boad.auth', $auth);
                    $result = $query->execute();
                    
                    $existing = json_decode($result->flds, true);
                    foreach($data as $key => $value) {
                        if(isset($value) && $value != '') $existing[$key] = 1;
                        else $existing[$key] = 0;
                    }
                    $table = "bcbsri_online_application_ufields";
                    $record = array();
                    
                    $record['flds'] = json_encode($existing);
                    
                    if($data['flds'] != '') {
                        //$record->flds = bcbsri_online_application_encrypt($data['flds'], $auth);
                        $record['flds'] = bcbsri_online_application_encrypt($data['flds'], $auth);
                    }
                    
                    if($data['pc_eligib'] != '') {
                        $record['pc_eligib'] = $data['pc_eligib'];
                    }
                    
                    try {
                        bcbsri_online_application_db_set($table, $record, 'auth');
                        //drupal_write_record($table, $record, 'auth');
                    } catch (Exception $e) {
                        /*print "failed write record<br />";
                        print "<pre>";
                        var_dump($e);
                        print "</pre>";
                        die(); */
                        return array('success' => false, 'reason' => 'Failed to write record', 'error' => var_dump($e, true));
                    }
                    //bcbsri_online_application_set_cookie(array('mtime', $time, $year, $alias));
                    // Update the end time.
                    $time = time();
                    bcbsri_online_application_db_set('bcbsri_online_application_udata', array('end_time'=>$time, 'auth' => $auth), 'auth');
                    return array('success' => true);
              } else return array('success' => false, 'data' => $_REQUEST['data'], 'decodedata' => $data, 'process' => $requestProcess);
            } catch (Exception $e) {
              return array('success' => false, 'reason' => 'Failed to process data', 'process' => $requestProcess, 'error' => var_dump($e, true));
            }
            
          } else if ($requestProcess == 'send-email') {
            // dataToBeSent = 'process=send-email&auth=' + auth + '&form_confirmation=' + sfData.confirmation + '&form_year=' + year + '&d=' + formData + '&form_email=' + $('input[name="email_address"]').val();
            $to = filter_var($_REQUEST['form_email'], FILTER_SANITIZE_EMAIL);
            // $cValue = bcbsri_online_application_get_cookie('planDetails');
            if(bcbsri_online_application_get_alias() == 'medicare') $appName = "BlueCHiP for Medicare";
            else if(bcbsri_online_application_get_alias() == 'direct_pay') $appName = "Direct Pay";
            
            $headers = "From: website@bcbsri.org\r\n";
            // $headers .= "Reply-To: \r\n";
            $headers .= "MIME-Version: 1.0\r\n";
            $headers .= "Content-Type: text/html; charset=ISO-8859-1\r\n";
            $subject = bcbsri_online_application_get_year() . ' ' . $appName . ' Application';
            $message = '<p><strong>Confirmation Number: ' . $_REQUEST['form_confirmation'] . '</strong></p>' . $_REQUEST['dplans'] . ' ' . $_REQUEST['d'];
            return array('success' => mail($to, $subject, $message, $headers));  
            
          /* *******************************************************************************************
          * Error output if no above process is choosen
          ******************************************************************************************* */
          } else return array('success' => false, 'reason' => 'No process assigned to: ' . $requestProcess, 'process' => $requestProcess);
            
         /* *******************************************************************************************
         * Return failed if auth did not validate as being assigned
         ******************************************************************************************* */
        } else return array('success' => false, 'reason' => 'Failed returned auth check', 'process' => $requestProcess);
      }
    } else {
       /* *******************************************************************************************
      * Generic fail message if request is not posted or submit to salesforce
      ******************************************************************************************* */
      return array('success' => false, 'message' => 'Failed to process anything', 'process' => $requestProcess);
    }
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to include processing file', 'process' => $requestProcess, 'error' => var_dump($e, true));
  }
}
