<?php 
function bcbsri_online_application_appstart() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      //$auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      //$rAuth = bcbsri_online_application_appauth(TRUE);
      //if(!empty($rAuth['auth'])) {
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      if(bcbsri_online_application_auth_verify($auth)) {
        //$skey = (bcbsri_online_application_get_cookie('ks') ? bcbsri_online_application_get_cookie('ks') : bcbsri_online_application_authgen(16));
        //bcbsri_online_application_set_cookie(array('ks', $skey));
        //$pDetails = bcbsri_online_application_get_cookie('planDetails');
        $result = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
        foreach($result as $r) {
          $pDetails = json_decode($r->selected, true);
        }
        //$pDetails = explode(',', $pDetails);
        $pMedical = $pDetails['medical']; #explode(':',$pDetails[0]);
        $pDental = $pDetails['dental']; #explode(':',$pDetails[0]);
        if(!empty($pDental[1]) && empty($pMedical[1])) $dental = true;
        else $dental = false;        
        //if($auth['success'] == true) {
        $etime = time();
        $browser = (!empty($_REQUEST['uastring']) ? $_REQUEST['uastring'] : '');
        $table = 'bcbsri_online_application_udata';
        bcbsri_online_application_set_cookie(array('bcbsri_oa_aid', $auth, '+15 minutes'));
        $record = array('ua_info' => $browser, 'end_time' => $etime, 'auth' => $auth);
        /* $record = new stdClass();
        $record->auth = $auth['auth'];
        $record->skey = $skey;
        $record->app_type = bcbsri_online_application_get_year() . ' ' . bcbsri_online_application_get_alias(); // year and product
        $record->start_time = $auth['stime'];
        $record->end_time = $auth['mtime'];
        $record->ua_info = $browser;
        drupal_write_record($table, $record); */
        bcbsri_online_application_db_set($table, $record, 'auth');
        return array('success' => true, 'auth' => $auth, 'alias' => bcbsri_online_application_get_alias($auth), 'year' => bcbsri_online_application_get_year($auth), 'dental' => $dental); /* , 'app' => bcbsri_online_application_get_cookie('prod'))); */
        //} else return array('success' => false, 'reason' => 'Auth Failed'); 
      } else return array('success' => false, 'reason' => 'Auth Failed - not current'); 
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to start application', 'error' => var_dump($e, true));
  }
}

function bcbsri_online_application_cleardata() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      // clear the following cookies
      bcbsri_online_application_clear_cookies();
      return array('success' => true);
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to clear cookies', 'error' => var_dump($e, true));
  }
}

/* *******************************************************************************************
 * Get resume key to be used on return
 ******************************************************************************************* */
function bcbsri_online_application_app_getresumekey() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      if(bcbsri_online_application_check_auth($auth)) {
        $check = bcbsri_online_application_db_get('bcbsri_online_application_udata', array('resumedk'), $auth, 'auth');
        foreach($check as $c) $resumeKey = $c->resumedk;
        if(empty($resumeKey)) {
          do {
            $token = bcbsri_online_application_resumekey();
            $num_rows = bcbsri_online_application_check_resumekey($token);
          } while ($num_rows > 0);
          bcbsri_online_application_db_set('bcbsri_online_application_udata', array('auth' => $auth, 'resumedk' => $token), 'auth');
        } else $token = $resumeKey;
        return array('success' => true, 'resume_key' => $token);
      } else return array('success' => false, 'message' => 'Failed auth check');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to get resume key', 'error' => var_dump($e, true));
  }
}

/* *******************************************************************************************
* Process data as they are entered or next is clicked
******************************************************************************************* */
function bcbsri_online_application_app_processdata() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      if(bcbsri_online_application_check_auth($_REQUEST['auth'])) {
        $data = json_decode(str_replace(array('\"', "\'"), array('"', "'"), $_REQUEST['d']), true);
        try {
          if($data != '') {
            $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
        
            /* $query = db_select('bcbsri_online_application_ufields', 'boad');
            $query->fields('boad', array('flds'));
            $query->condition('boad.auth', $auth);
            $result = $query->execute();
            
            $existing = json_decode($result->flds, true);
            foreach($data as $key => $value) {
              if(isset($value) && $value != '') $existing[$key] = 1;
              else $existing[$key] = 0;
            }*/
            $table = "bcbsri_online_application_ufields";
            $record = array();
            
            //$record['flds'] = json_encode($existing);
            
            /*if($data['pc_eligib'] != '') {
                $record['pc_eligib'] = $data['pc_eligib'];
            }*/
            
            try {
              if(!empty($data)) {
                //$record->flds = bcbsri_online_application_encrypt($data['flds'], $auth);
                //return array('data_encoded' => json_encode($data));
                $record['flds'] = bcbsri_online_application_encrypt(json_encode($data), $auth);
                $record['auth'] = $auth;
                bcbsri_online_application_db_set($table, $record, 'auth');
                //drupal_write_record($table, $record, 'auth');
                $time = time();
                bcbsri_online_application_set_cookie(array('bcbsri_oa_aid', $auth, '+15 minutes'));
                bcbsri_online_application_db_set('bcbsri_online_application_udata', array('end_time'=>$time, 'auth' => $auth), 'auth');
                return array('success' => true, 'data' => $_REQUEST['d'], 'decodedata' => $data, 'encryptdata' => $record['flds']);
              } else return array('success' => false, 'message' => 'data was empty');
            } catch (Exception $e) {
              /* print "failed write record<br />";
              print "<pre>";
              var_dump($e);
              print "</pre>";
              die(); */
              return array('success' => false, 'message' => 'failed write record', 'error' => var_dump($e, true));
            }
            //bcbsri_online_application_set_cookie(array('mtime', $time, $year, $alias));
            // Update the end time.
          } else return array('success' => false, 'data' => $_REQUEST['d'], 'decodedata' => $data);
        } catch (Exception $e) {
          return array('success' => false, 'reason' => 'Failed to process data', 'error' => var_dump($e, true));
        }
      } else return array('success' => false, 'message' => 'Failed auth check');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to ', 'error' => var_dump($e, true));
  }
}