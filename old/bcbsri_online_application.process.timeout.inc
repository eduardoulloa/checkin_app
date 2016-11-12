<?php
/* *******************************************************************************************
* Update timeout
******************************************************************************************* */
function bcbsri_online_application_auth_updatetimeout() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
    //if($_REQUEST['process'] == "update-timeout") {
      //$auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      //$auth = bcbsri_online_application_appauth();
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      if(bcbsri_online_application_auth_verify($auth)) {
      //if(!empty($auth)){
        // Update the end time.
        $time = time();
        bcbsri_online_application_set_cookie(array('bcbsri_oa_aid', $auth, '+15 minutes'));
        bcbsri_online_application_db_set('bcbsri_online_application_udata', array('end_time'=>$time, 'auth' => $auth['auth']), 'auth');
        return array('success' => true);
      } else return array('success' => false);
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to update timeout', 'error' => var_dump($e, true));
  }
}

/* *******************************************************************************************
* Timeout check
******************************************************************************************* */
function bcbsri_online_application_auth_timeoutcheck() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
    //}else if($_REQUEST['process'] == "timeout-check") {
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      $expired = true;
      $config = bcbsri_online_application_get_json('config');
      $data_expire = $config['data_expire'];
      $curTimestamp = time();
      $curTime = date("D M j G:i:s T Y", $curTimestamp); // Sat Mar 10 17:16:18 MST 2001
      if($auth != false) {
        $result = bcbsri_online_application_db_get('bcbsri_online_application_udata', array('end_time', 'start_time'), $auth, 'auth');
        foreach($result as $r) {
          $startTime = (int)$r->start_time;
          $modTimestamp = (int)$r->end_time;
          $modTime = date("D M j G:i:s T Y", $modTimestamp);
        }
        
        // Calculate the expire time.
        if(!empty($data_expire)){
          $expireTime = strtotime("-" . $data_expire . " minutes");
          $threshold = $data_expire;
        } else {
          $expireTime = strtotime("-15 minutes");
          $threshold = 15;
        }
        
        // Calculate the elapsed time.
        $timeElapsed = $curTimestamp - $modTimestamp;
        // Validate if the app hasn't expired yet.
        if($modTimestamp >= $expireTime) $expired = false;
        
        $timeRemainng = $threshold - ($timeElapsed / 60);
        
        return array(
          'success' => true,
          'curTimestamp'  => $curTimestamp,
          'curTime'       => $curTime,
          'expireTime'    => $expireTime,
          'modTimestamp'  => $modTimestamp,
          'modTime'       => $modTime,
          'timeElapsed'   => $timeElapsed,
          'expired'       => $expired,
          'timeRemaining' => $timeRemainng
        );
      }
      return array(
        'success' => true,
        'curTimestamp'  => $curTimestamp,
        'curTime'       => $curTime,
        'expired'       => $expired,
        'timeRemaining' => 0
      );
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'failed to check timeout', 'error' => var_dump($e, true));
  }
}

/* *******************************************************************************************
* Get Cookie
******************************************************************************************* */
function bcbsri_online_application_auth_getcookie() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      if(!empty($_REQUEST['form_cookie'])) {
        $form_alias = NULL; $form_year = NULL;
        //$cValue = bcbsri_online_application_get_cookie($_REQUEST['form_cookie'], $form_alias, $form_year);
        $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
        if(bcbsri_online_application_auth_verify($auth)) {
          if(!empty($_REQUEST['form_alias'])) {
            bcbsri_online_application_set_alias($auth, $_REQUEST['form_alias']);
            $form_alias = $_REQUEST['form_alias'];
          }
          if(!empty($_REQUEST['form_year'])) {
            bcbsri_online_application_set_year($auth, $_REQUEST['form_year']);
            $form_year = $_REQUEST['form_year'];
          }
          if($_REQUEST['form_cookie'] == 'planDetails') {
            $result = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
            if(!empty($result)) {
              foreach($result as $r) {
                $cValue = $r->selected;
              }
              $plans = array();
              $plans['success'] = true;
              $plans['data'] = json_decode($cValue, true);
              if($form_alias != 'directpay') {
                $cValue = json_decode($cValue, true);
                $medical = $cValue['medical']; #explode(':',$cValue[0]);
                $dental = $cValue['dental']; #explode(':',$cValue[0]);
                if(!empty($medical)) { #[1])) {
                   $plans['medical'] = true;
                   $medical = explode('|', $medical); #[1]);
                   $plans['medicalPlan'] = $medical[0];
                   $plans['medicalNetwork'] = $medical[1];
                   $mCost = explode('.', $medical[2]);
                   $mCost[0] = str_replace('$', '', $mCost[0]);
                   $mCost[1] = (empty($mCost[1]) ? '00' : (strlen($mCost[1]) < 2 ? $mCost[1] . '0' : $mCost[1]));
                   $plans['medicalCost'] = '$' . $mCost[0] . '<sup>.' . $mCost[1] . '</sup>';
                } else $plans['medical'] = false;
                if(!empty($dental)) { #[1])) {
                   $plans['dental'] = true;
                   $dental = explode('|', $dental); #[1]);
                   $plans['dentalPlan'] = $dental[0];
                   $plans['dentalNetwork'] = $dental[1];
                   $dCost = explode('.', $dental[2]);
                   $dCost[0] = str_replace('$', '', $dCost[0]);
                   $dCost[1] = (empty($dCost[1]) ? '00' : (strlen($dCost[1]) < 2 ? $dCost[1] . '0' : $dCost[1]));
                   $plans['dentalCost'] = '$' . $dCost[0] . '<sup>.' . $dCost[1] . '</sup>';
                } else $plans['dental'] = false;
              }
              return $plans;
            } else return array('success' => false, 'cValue' => 'No value set');
          }else if($_REQUEST['form_cookie'] == 'existing'){
            //check cookie to be set with value
            $existing = bcbsri_online_application_get_cookie('bcbsri_oa_em');
            if(!empty($existing)) return array('success' => true, 'value' => $existing);
            //if cookie not set call the db
            else {
              $result = bcbsri_online_application_db_get('bcbsri_online_application_udata', array('existing_member'), $auth, 'auth');
              if(!empty($result)) {
                foreach($result as $r) {
                  $cValue = $r->existing_member;
                }
                if(isset($cValue)){
                  return array('success' => true, 'value' => $cValue);
                }else{
                  return array('success' => false, 'value' => 'No value set');
                }
                
              } else return array('success' => false, 'value' => 'No value set');
            }
          }
          else return array('success' => false, 'reason' => $_REQUEST['form_cookie'] . ' cookie not assigned', 'cValue' => $cValue);
        } else return array('success' => false, 'reason' => 'Failed to get cookie - auth failed');
      } else return array('success' => false, 'reason' => 'Failed to get cookie info');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to get cookie', 'error' => var_dump($e, true));
  }
}