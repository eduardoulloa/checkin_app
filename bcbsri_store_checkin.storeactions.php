<?php
function bcbsri_store_checkin_admin() {
  return '';
}

function bcbsri_store_checkin_queue($storeName) {
  $storeName = str_replace("-", ' ', $storeName);
  $result = bcbsri_store_checkin_db_get('bcbsri_checkin_stores', array('location', 'id'), strtolower($storeName), 'location');
  if($result->rowCount() < 1){
    die('The store you are trying to find does not exist');
  }else{
    foreach($result as $r){
      $id = $r->id;
      $location = str_replace(" ", '-', $r->location);
    }
  }
  $output= '<div id="store_id" class="hide">'.$id.'</div>';
  $output .= '<div id="sname" class="hide">'.$location.'</div>';
  return $output;
}

function bcbsri_store_checkin_store($storeName) {
  $storeName = str_replace("-", ' ', $storeName);
  $result = bcbsri_store_checkin_db_get('bcbsri_checkin_stores', array('location', 'id'), strtolower($storeName), 'location');
  if($result->rowCount() < 1){
    die('The store you are trying to find does not exist');
  }else{
    foreach($result as $r){
      $id = $r->id;
      $location = str_replace(" ", '-', $r->location);
    }
  }
  $output= '<div id="store_id" class="hide">'.$id.'</div>';
  $output .= '<div id="sname" class="hide">'.$location.'</div>';
  return $output;
}

function bcbsri_store_checkin_addstore(){
  try {
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      if(isset($_REQUEST['sn'])){
        $fields = array('id');
        $query = db_select('bcbsri_checkin_stores', 's');
        $query->fields('s', $fields);
        $query->condition('location', '%' . db_like($_REQUEST['sn']) . '%', 'LIKE');
        $results = $query->execute();
        
        if($results->rowCount() > 0){
          return array('success'=>false, 'reason'=>'Exists');
        }
        
        bcbsri_online_application_db_set('bcbsri_checkin_stores', 
          array('location' => $_REQUEST['sn'], 'wait_time' => '0 - 15 minutes', 'url' => '/'.str_replace(" ", '-', strtolower($_REQUEST['sn'])))
        );
        return array('success' => true);
      }
      return array('success'=>false, 'reason'=>'Error sending the POST request.');
    }else return array('success'=>false, 'reason'=>'No data sent.');
  } catch (Exception $e) {
    return array('success'=>false, 'reason'=>'Errored adding store', 'exception' => $e->getMessage());
  }
}

function bcbsri_store_checkin_removestore(){
  try{
    if(isset($_REQUEST['id'])){
      bcbsri_online_application_db_set('bcbsri_checkin_stores', 
        array(
          'id' => $_REQUEST['id'], 
          'show_s' => 0
        ), 'id'
       );
      return array ('success' => true);
    }
    return array('success'=>false, 'reason'=>'Error sending the POST request.');
  } catch (Exception $e) {
    return array('success'=>false, 'reason'=>'Errored adding store', 'exception' => $e->getMessage());
  }
}

function bcbsri_store_checkin_outputreport($page_callback_result){
  
  //$result = bcbsri_store_checkin_db_get('bcbsri_checkin_stores', , strtolower($storeName), 'location');
  
  $fields = array('location', 'id');
  $query = db_select('bcbsri_checkin_stores', 's');
	$query->fields('s', $fields);
  $result = $query->execute();
  
  $stores = array();
  
  if($result->rowCount() > 0){
    foreach($result as $r){
      $stores[$r->id] = $r->location;
    }
  }
  
  $file = 'history_' . date('mdy_His') . '.csv';
  header('Content-Type: text/csv; utf-8');
  header('Content-Disposition: attachment;filename=' . $file);
  header("Cache-Control: no-cache, no-store, must-revalidate"); // HTTP 1.1.
  header("Pragma: no-cache"); // HTTP 1.0.
  header("Expires: 0"); // Proxies.
  
  // create a file pointer connected to the output stream
  $output = fopen('php://output', 'w');
  
  $query = db_select('bcbsri_checkin_visitors', 'v');
	$query->fields('v');
  $results = $query->execute();
  
  $headers = array();
  $headers['fname'] = 'First name';
  $headers['lname'] = 'Last name';
  $headers['email'] = 'E-mail';
  $headers['phone'] = 'Phone';
  $headers['store'] = 'Store';
  $headers['checkin_date']= 'Check-in Date';
  $headers['seen'] = 'Seen';
  $headers['time_seen'] = 'Time seen';
  $headers['proposed_wait_time'] = 'Displayed wait time';
  $headers['reason_for_visit']='Reason for Visit';
  $headers['product'] = 'Product';
  $headers['segment'] = 'Segment';
  $headers['appointment'] = 'Appointment';
  $headers['appointment_with'] = 'Appointment with';
  $headers['appointment_time'] = 'Appointment time';
  
  fputcsv($output, $headers);
  
  foreach($results as $r){
    $arr = (array)$r;
    $csv_row['fname'] = $arr['first_name'];
    $csv_row['lname'] = $arr['last_name'];
    $csv_row['email'] = $arr['email'];
    $csv_row['phone'] = $arr['phone'];
    $csv_row['store'] = $stores[$arr['store_id']];
    $csv_row['checkin_date']= date('c', $arr['check_in']);
    $csv_row['seen'] = ($arr['seen'] == 1? 'Yes' : 'No');
    $csv_row['time_seen'] = (isset($arr['wait_time']))? date('h:i:sa', $arr['wait_time']) : 'N/A';
    $csv_row['proposed_wait_time'] = $arr['promised_wait_time'];
    switch($arr['reason_for_visit']){
      case 'cs': $csv_row['reason_for_visit']='Customer Service'; break;
      case 's': $csv_row['reason_for_visit']='Sales'; break;
      default: $csv_row['reason_for_visit']='Undefined';
    }
    $csv_row['product'] = $arr['product'];
    $csv_row['segment'] = $arr['segment'];
    $csv_row['appointment'] = ($arr['appointment'] == 1? 'Yes' : 'No');
    $csv_row['appointment_with'] = ($arr['appointment'] == 1? $arr['appointment_with'] : 'N/A');
    $csv_row['appointment_time'] = (isset($arr['appt_time'])? $arr['appt_time'] : 'N/A');
    fputcsv($output, $csv_row);
  }

}


function bcbsri_store_checkin_generatetest(){
    //if($_REQUEST['report'] == 'yesplease') {
      $curPath = dirname(__FILE__); /*getcwd();*/
      //include_once('bcbsri_online_application.helper.inc');
      
      /*if (!defined('DRUPAL_ROOT')) define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
      require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
      drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);*/
    
      $file = 'history_' . date('mdy_His') . '.csv';
      header('Content-Type: text/csv; utf-8');
      header('Content-Disposition: attachment;filename=' . $file);
      header("Cache-Control: no-cache, no-store, must-revalidate"); // HTTP 1.1.
      header("Pragma: no-cache"); // HTTP 1.0.
      header("Expires: 0"); // Proxies.
      
      $output = 'testfile';
      
      /*$keys = array('id', 'App Type', 'Name', 'Phone', 'Email', 'Application Start', 'Application Modified', 'Application Confirmation', 'Entered By', 'Fields Entered', 'UA String');
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
      }*/
      
      /*ob_clean();
      print $output;
      exit;*/
    /*} else {
      print 'Unauthorized';
      die();
    }*/
    
    $authUrl ='https://622-xeh-027.mktorest.com/identity/oauth/token?grant_type=client_credentials&client_id=d0e96119-faf5-4d4a-b09c-437efc8725a0&client_secret=fN3sgrWc0DHkiZkYkceCCdAvM0LzEIdH';
    
    $token = drupal_http_request($authUrl);
    
    $response = json_decode($token->data);
    
    $at = $response->access_token;
    
    $input = array(
      'input' =>
      array( 
        [
          'firstName' => 'Eduardo',
          'lastName' => 'Ulloa',
          'email' => 'consultanteduardo.Ulloa@bcbsri.org'
        ],
         [
          'firstName' => 'Dig',
          'lastName' => 'Ulloa',
          'email' => 'dig.Ulloa@bcbsri.org'
        ],
        )
    );
    
    $arr = json_encode($input);
    
    /*$input = json_encode('{"input":[  
        {  
           "email":"kjashaedd-1@klooblept.com",
           "firstName":"Kataldar-1"
        }
      ]
    }');*/
    
    $headers = array(
      'content type' => 'application/json',
      'User-Agent' => 'Drupal (+http://drupal.org/)'
    );
    
    $body = '
          {  
         "action":"createOnly",
         "lookupField":"email",
         "input":[  
            {  
               "email":"ed@test.com",
               "firstName":"Eduardo",
               "lastName":"Ulloa",
               "cSRServiced": true,
               "storeLocation": "Warwick"
            }
         ]
      }';
    
    /*$url = 'https://622-xeh-027.mktorest.com/rest/v1/push.json?access_token=b3d082d7-d0c2-4896-bd8a-61ae02582a5a:sj&action=createOnly&input='.$input.'&filterType=id';*/
    $url = 'https://622-xeh-027.mktorest.com/rest/v1/leads.json?access_token='.$at;
    
    $options = array();
    $options['method'] = 'POST';
    $options['headers']['content-type'] = 'application/json';
    $options['data'] = $body;

    
    $response = drupal_http_request($url, $options);
    return $response;
    //print $response;
}

function bcbsri_store_checkin_setstorewaittime(){
  try{
    if(isset($_REQUEST['waittime'])){
      bcbsri_online_application_db_set('bcbsri_checkin_stores', 
        array('id'=>$_REQUEST['store'], 'wait_time'=>($_REQUEST['waittime'])), 'id'
      );
      return array ('success' => true);
    }
    return array('success'=>false, 'reason'=>'Error sending the POST request.');
  } catch (Exception $e) {
    return array('success'=>false, 'reason'=>'Errored adding store', 'exception' => $e->getMessage());
  }
}

function bcbsri_store_checkin_getstorewaittime(){
  if($_SERVER['REQUEST_METHOD'] == 'POST') {
    if(isset($_REQUEST['store'])){
      $store_id = $_REQUEST['store'];
      $result = bcbsri_store_checkin_db_get('bcbsri_checkin_stores', array('wait_time'), $store_id, 'id');
      if($result->rowCount() > 0){
        foreach($result as $r){
          $wait_time = $r->wait_time;
        }
        return array('success'=>true, 'wait_time'=>$wait_time);
      }    
    }
    return array('success'=>false, 'reason'=>'Error sending the POST request.');
  }
}

function bcbsri_store_checkin_addvisitor(){
  try {
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      if(isset($_REQUEST['fn'])){
        
        $formatted_time;
        
        if(!empty($_REQUEST['appt_time'])){
          $time = explode(":", $_REQUEST['appt_time']);
          $am_or_pm = explode(" ", $time[1]);
          if(sizeof($am_or_pm) < 2){
            if($time[0][0] != "0"){
              $hours = (int)$time[0];
              if($hours > 12){
                $hours = $hours - 12;
                $formatted_time = $hours.":".$time[1]." PM";
              }else if($hours == 12){
                $formatted_time = "12:".$time[1]." PM";
              }
            }else{
                $formatted_time = $_REQUEST['appt_time']." AM";
            }
          }
        }
        
        $record = bcbsri_online_application_db_set('bcbsri_checkin_visitors', 
          array(
            'first_name' => $_REQUEST['fn'],
            'last_name' => $_REQUEST['ln'], 
            'email' => $_REQUEST['email'], 
            'phone' => $_REQUEST['phone'],
            'reason_for_visit'=>$_REQUEST['rv'],
            'product'=>$_REQUEST['product'],
            'segment'=>$_REQUEST['segment'],
            'signature_received'=>$_REQUEST['signature'],
            'appointment'=>$_REQUEST['appt'],
            'appointment_with'=>$_REQUEST['awith'],
            'promised_wait_time'=>$_REQUEST['w'],
            'check_in'=>time(),
            'store_id'=>(int)$_REQUEST['store'],
            'appt_time'=>(!empty($formatted_time)? $formatted_time : $_REQUEST['appt_time']),
          )
        );
        return array('success' => true);
      }
      return array('success'=>false, 'reason'=>'Error sending the POST request.');
    }else return array('success'=>false, 'reason'=>'No data sent.');
  } catch (Exception $e) {
    return array('success'=>false, 'reason'=>'Errored adding visitor', 'exception' => $e->getMessage());
  }
}

function bcbsri_store_checkin_updatevisitor(){
  try {
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
        
        if(isset($_REQUEST['edit'])){
          
          $formatted_time;
        
          if(!empty($_REQUEST['appt_time'])){
            $time = explode(":", $_REQUEST['appt_time']);
            $am_or_pm = explode(" ", $time[1]);
            if(sizeof($am_or_pm) < 2){
              if($time[0][0] != "0"){
                $hours = (int)$time[0];
                if($hours > 12){
                  $hours = $hours - 12;
                  $formatted_time = $hours.":".$time[1]." PM";
                }else if($hours == 12){
                  $formatted_time = "12:".$time[1]." PM";
                }
              }else{
                  $formatted_time = $_REQUEST['appt_time']." AM";
              }
            }
          }
          
        $record = bcbsri_online_application_db_set('bcbsri_checkin_visitors', 
          array(
            'id' => $_REQUEST['vid'],
            'first_name' => $_REQUEST['fn'],
            'last_name' => $_REQUEST['ln'], 
            'email' => $_REQUEST['email'], 
            'phone' => $_REQUEST['phone'],
            'reason_for_visit'=>$_REQUEST['rv'],
            'product'=>$_REQUEST['product'],
            'segment'=>$_REQUEST['segment'],
            'signature_received'=>$_REQUEST['signature'],
            'appointment'=>$_REQUEST['appt'],
            'appointment_with'=>$_REQUEST['awith'],
            'promised_wait_time'=>$_REQUEST['w'],
            'appt_time'=>(!empty($formatted_time)? $formatted_time : $_REQUEST['appt_time']),
            'store_id'=>(int)$_REQUEST['store']
            ),
            'id'
          );
          return array('success' => true);
        }else{
          if(isset($_REQUEST['seen'])){
            $record = bcbsri_online_application_db_set('bcbsri_checkin_visitors', 
              array(
                'id' => $_REQUEST['id'],
                'seen' => 1,
                'wait_time' => time()
              ),
              'id'
            );
            return array('success' => true);
          }else if(isset($_REQUEST['notseen'])){
            $record = bcbsri_online_application_db_set('bcbsri_checkin_visitors', 
              array(
                'id' => $_REQUEST['id'],
                'not_seen' => 1,
                'wait_time' => time()
              ),
              'id'
            );
            return array('success' => true);
          }  
        }
        
      return array('success'=>false, 'reason'=>'Error sending the POST request.');
    }else return array('success'=>false, 'reason'=>'No data sent.');
  } catch (Exception $e) {
    return array('success'=>false, 'reason'=>'Errored adding or updating visitor', 'exception' => $e->getMessage());
  }
}

function bcbsri_sort($a, $b){
  if($a['appointment'] == 1 && $b['appointment'] == 0){
    return -1;
  }else if($b['appointment'] == 1 && $a['appointment'] == 0){
    return 1;
  }else if($a['appointment'] == 1 && $b['appointment'] == 1){
      $prev_appt_time = $a['appt_time'];
      $current_appt_time = $b['appt_time'];
      
      $prev_am_or_pm = explode(" ", $prev_appt_time);
      $prev_h = explode(":", $prev_am_or_pm[0]);
      
      $current_am_or_pm = explode(" ", $current_appt_time);
      $current_h = explode(":", $current_am_or_pm[0]);
      
      $prev_hours = ((int)$prev_h[0] == 12 ? 0 : (int)$prev_h[0]);
      $current_hours = ((int)$current_h[0] == 12 ? 0 : (int)$current_h[0]);
      
      $prev_minutes = (int)$prev_h[1];
      $current_minutes = (int)$current_h[1];
      
      // Having set all the variables, do the comparison.
      if($prev_am_or_pm[1] == 'PM' && $current_am_or_pm[1] == 'AM'){
        return 1;
      } else if($prev_am_or_pm[1] == 'AM' && $current_am_or_pm[1] == 'PM') {
        return -1;
      }
      
      if($prev_am_or_pm[1] == $current_am_or_pm[1]){
        if($current_hours == $prev_hours){
          // Two appointments at the same hour. Check the minutes to see who has the earliest appointment.
          if($current_minutes < $prev_minutes){
            // The current element's minutes are smaller than the previous element's minutes. Swipe the elements.
            return 1;
          }else if($current_minutes > $prev_minutes){
            // The current element's minutes are smaller than the previous element's minutes. Swipe the elements.
            return -1;
          }
          
        }else{
         if($current_hours < $prev_hours){
             // If the current element's hours are smaller than the previous one's, swipe the elements.
             return 1;
          }
         else if($current_hours > $prev_hours){
             // If the current element's hours are smaller than the previous one's, swipe the elements.
             return -1;
          }
        }
      }
  }
  // if checkin time is diff
  if($a['check_in'] < $b['check_in']){
    return -1;
  }else if($a['check_in'] > $b['check_in']){
    return 1;
  }
  
  return 0;
}

function bcbsri_store_checkin_sendtomarketo(){
  try {
    
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      
      $visitors = bcbsri_store_checkin_db_get('bcbsri_checkin_visitors', array('first_name', 'last_name', 'email'), $_REQUEST['id'], 'id');
      foreach($visitors as $v){
        $firstName = $v->first_name;
        $lastName = $v->last_name;
        $email = $v->email;
      }
      
      $stores = bcbsri_store_checkin_db_get('bcbsri_checkin_stores', array('location', 'url'), $_REQUEST['store'], 'id');
      
      foreach($stores as $s){
        $location = substr($s->url, 1);
      }
      
      if(isset($_SERVER['AH_SITE_ENVIRONMENT']) && $_SERVER['AH_SITE_ENVIRONMENT'] == 'prod'){
        // Set the Marketo variables to generate the access token.
        // PRODUCTION ENVIRONMENT
        $endpointUrl = 'https://177-WFD-895.mktorest.com';
        $client_id = '38856d39-e439-4dfd-9e46-0f89217b8e80';
        $client_secret = 'Ax618cyYKCrjCDf3LXjeGl8zRcIHM4lW';
      }else{
        // SANDBOX ENVIRONMENT
        $endpointUrl = 'https://622-xeh-027.mktorest.com';
        $client_id = 'd0e96119-faf5-4d4a-b09c-437efc8725a0';
        $client_secret = 'fN3sgrWc0DHkiZkYkceCCdAvM0LzEIdH';
      }
      
      $authUrl = $endpointUrl.'/identity/oauth/token?grant_type=client_credentials&client_id='.$client_id.'&client_secret='.$client_secret;
      
      // Given an access token, make an HTTP request to the URL.
      $token = drupal_http_request($authUrl);
      
      $response = json_decode($token->data);
      
      $at = $response->access_token;
      
      // Set the URL of the API that we're calling. In this case, we're using leads.json to create a new lead.
      $url = $endpointUrl.'/rest/v1/leads.json?access_token='.$at;
      
      // An array with options to be sent along with the HTTP request.
      $options = array();
      $options['method'] = 'POST';
      $options['headers']['content-type'] = 'application/json';
      
      if($_REQUEST['seen']){
        //Create the body of the request. This is the format that is expected on Marketo's end.
        $body = '
              {  
             "action":"createOrUpdate",
             "lookupField":"email",
             "input":[  
                {  
                   "email":"'.$email.'",
                   "firstName":"'.$firstName.'",
                   "lastName":"'.$lastName.'",
                   "cSRServiced": true,
                   "storeLocation": "'.$location.'"
                }
             ]
          }';
         
         // Send  the body of the request as an option.
         $options['data'] = $body;
         
         // Pass the options as an argument.
         $response = drupal_http_request($url, $options);
      
        return array ('success' => true, 'response' => $response);
    
      }else if($_REQUEST['notseen']){
        $body = '
              {  
             "action":"createOrUpdate",
             "lookupField":"email",
             "input":[  
                {  
                   "email":"'.$email.'",
                   "firstName":"'.$firstName.'",
                   "lastName":"'.$lastName.'",
                   "cSRServiced": false,
                   "storeLocation": "'.$location.'"
                }
             ]
          }';
          
          $options['data'] = $body;
          
          $response = drupal_http_request($url, $options);
          return array ('success' => true, 'response' => $response);
      }
      return array('success'=>false, 'reason'=>'Error sending the data to marketo.');
    }else return array('success'=>false, 'reason'=>'No data sent.');
  } catch (Exception $e) {
    return array('success'=>false, 'reason'=>'Errored editing store', 'exception' => $e->getMessage());
  }
}

function bcbsri_store_checkin_getstorelists(){
  try {
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      
      $store_id = $_REQUEST['store'];
      
      $fields = array('first_name', 'last_name', 'id', 'product', 'appointment', 'appointment_with', 'check_in', 'appt_time');
      
      $query_cs_appointment = db_select('bcbsri_checkin_visitors', 'v');
      $query_cs_appointment->fields('v', $fields);
      $query_cs_appointment->condition('seen', 0);
      $query_cs_appointment->condition('not_seen', 0);
      $query_cs_appointment->condition('reason_for_visit', 'cs');
      $query_cs_appointment->condition('store_id', $store_id);
      $query_cs_appointment->orderBy('appointment', 'DESC');
      $query_cs_appointment->orderBy('check_in', 'ASC');
      $query_cs_appointment->range(0, 12);
      
      $query_s_appointment = db_select('bcbsri_checkin_visitors', 'v');
      $query_s_appointment->fields('v', $fields);
      $query_s_appointment->condition('seen', 0);
      $query_s_appointment->condition('not_seen', 0);
      $query_s_appointment->condition('reason_for_visit', 's');
      $query_s_appointment->condition('store_id', $store_id);
      $query_s_appointment->orderBy('appointment', 'DESC');
      $query_s_appointment->orderBy('check_in', 'ASC');
      $query_s_appointment->range(0, 12);
      
      $fields_store = array('wait_time');
      $query_v = db_select('bcbsri_checkin_stores', 's');
      $query_v->fields('s', $fields_store);
      $query_v->condition('id', $store_id);
      
      $result_cs_appointment = $query_cs_appointment->execute();
      $result_s_appointment = $query_s_appointment->execute();
      $result_store = $query_v->execute();
      
      $result_cs = array();
      $result_s = array();
      
      // Get the CS customers with appts.
      if($result_cs_appointment->rowCount() > 0){
        $resultsList = $result_cs_appointment->fetchAll(); $rCount = 0;
        foreach($resultsList as $r){
          $result_cs[$rCount]['name'] = $r->first_name.' '.substr($r->last_name,0,1);
          $result_cs[$rCount]['id'] = $r->id;
          $result_cs[$rCount]['product'] = $r->product;
          $result_cs[$rCount]['appointment'] = $r->appointment;
          $result_cs[$rCount]['appointment_with'] = $r->appointment_with;
          $result_cs[$rCount]['check_in'] = $r->check_in;
          $result_cs[$rCount]['appt_time'] = $r->appt_time;
          $rCount++;
        }
      }
      
      usort($result_cs, "bcbsri_sort");
      
      //sort them according to the appt. time
      
      /*$times = array();
      for($i = 0; $i<sizeof($result_cs); $i++){
        for($j = $i+1; $j<sizeof($result_cs); $j++){
          if(!empty($result_cs[$i]['appt_time']) && !empty($result_cs[$j]['appt_time'])){
            
            $prev_appt_time = $result_cs[$i]['appt_time'];
            $current_appt_time = $result_cs[$j]['appt_time'];
            
            $prev_h = explode(":", $prev_appt_time);
            $prev_am_or_pm = explode(" ", $prev_h[1]);
            
            $current_h = explode(":", $current_appt_time);
            $current_am_or_pm = explode(" ", $current_h[1]);
            
            // Get the previous element's hours.
            if($prev_h[0][0] == "0"){
              $prev_hours = (int)$prev_h[0][1];
            }else{
              $prev_hours = (int)$prev_h[0];
            }
            
            // Get the current element's hours.
            if($current_h[0][0] == "0"){
              $current_hours = (int)$current_h[0][1];
            }else{
              $current_hours = (int)$current_h[0];
            }
            
            if($prev_am_or_pm[0][0] == "0"){
              $prev_minutes = (int)$prev_am_or_pm[0][1];
            }else{
              $prev_minutes = (int)$prev_am_or_pm[0];
            }
              
            if($current_am_or_pm[0][0] == "0"){
              $current_minutes = (int)$current_am_or_pm[0][1];
            }else{
              $current_minutes = (int)$current_am_or_pm[0];
            }
            
            if($prev_am_or_pm[1] == 'PM' && $current_am_or_pm[1] == 'AM'){
              $temp = $result_cs[$i];
              $result_cs[$i] = $result_cs[$j];
              $result_cs[$j] = $temp;
            } else if($prev_am_or_pm[1] == "PM" && $current_am_or_pm[1] == "PM"){
                
                //$times[] = array('current'=>$current_hours, 'prev'=>$prev_hours );
                
                if($prev_hours < 12 && $current_hours == 12){
                  $temp = $result_cs[$i];
                  $result_cs[$i] = $result_cs[$j];
                  $result_cs[$j] = $temp;
                }else if($prev_hours == 12 && $current_hours == 12){
                  if($current_minutes < $prev_minutes){
                    // The current element's minutes are smaller than the previous element's minutes. Swipe the elements.
                    $temp = $result_cs[$i];
                    $result_cs[$i] = $result_cs[$j];
                    $result_cs[$j] = $temp;
                  }
                }else{
                  if($current_hours == $prev_hours){
                    // Two appointments at the same hour. Check the minutes to see who has the earliest appointment.
                    if($current_minutes < $prev_minutes){
                      // The current element's minutes are smaller than the previous element's minutes. Swipe the elements.
                      $temp = $result_cs[$i];
                      $result_cs[$i] = $result_cs[$j];
                      $result_cs[$j] = $temp;
                    }
                    
                  }else{
                   if($current_hours < $prev_hours){
                       // If the current element's hours are smaller than the previous one's, swipe the elements.
                       $temp = $result_cs[$i];
                       $result_cs[$i] = $result_cs[$j];
                       $result_cs[$j] = $temp;
                    }
                  }
                }
                
            }else if($prev_am_or_pm[1] == "AM" && $current_am_or_pm[1] == "AM"){
                 if($prev_hours < 12 && $current_hours == 12){
                  $temp = $result_cs[$i];
                  $result_cs[$i] = $result_cs[$j];
                  $result_cs[$j] = $temp;
                }else if($prev_hours == 12 && $current_hours == 12){
                  if($current_minutes < $prev_minutes){
                    // The current element's minutes are smaller than the previous element's minutes. Swipe the elements.
                    $temp = $result_cs[$i];
                    $result_cs[$i] = $result_cs[$j];
                    $result_cs[$j] = $temp;
                  }
                }else{
                  if($current_hours == $prev_hours){
                    // Two appointments at the same hour. Check the minutes to see who has the earliest appointment.
                    if($current_minutes < $prev_minutes){
                      // The current element's minutes are smaller than the previous element's minutes. Swipe the elements.
                      $temp = $result_cs[$i];
                      $result_cs[$i] = $result_cs[$j];
                      $result_cs[$j] = $temp;
                    }
                    
                  }else{
                   if($current_hours < $prev_hours){
                       // If the current element's hours are smaller than the previous one's, swipe the elements.
                       $temp = $result_cs[$i];
                       $result_cs[$i] = $result_cs[$j];
                       $result_cs[$j] = $temp;
                    }
                  }
                }
            }else{
              //return array('entered else');
              // Compare the hours.
              if($current_hours == $prev_hours){
                // Two appointments at the same hour. Check the minutes to see who has the earliest appointment.
                if($current_minutes < $prev_minutes){
                  // The current element's minutes are smaller than the previous element's minutes. Swipe the elements.
                  $temp = $result_cs[$i];
                  $result_cs[$i] = $result_cs[$j];
                  $result_cs[$j] = $temp;
                }
                
              }else{
               if($current_hours < $prev_hours){
                   // If the current element's hours are smaller than the previous one's, swipe the elements.
                   $temp = $result_cs[$i];
                   $result_cs[$i] = $result_cs[$j];
                   $result_cs[$j] = $temp;
                }
              }
            }
          }
        }
      } // end for*/
      
      // Get the Sales customers with appts.
      if($result_s_appointment->rowCount() > 0){
        $resultsList = $result_s_appointment->fetchAll(); $rCount = 0;
        foreach($resultsList as $r){
          $result_s[$rCount]['name'] = $r->first_name.' '.substr($r->last_name,0,1);
          $result_s[$rCount]['id'] = $r->id;
          $result_s[$rCount]['product'] = $r->product;
          $result_s[$rCount]['appointment'] = $r->appointment;
          $result_s[$rCount]['appointment_with'] = $r->appointment_with;
          $result_s[$rCount]['check_in'] = $r->check_in;
          $result_s[$rCount]['appt_time'] = $r->appt_time;
          $rCount++;
        }
      }
      
      /*for($i = 0; $i<sizeof($result_s); $i++){
        for($j = $i+1; $j<sizeof($result_s); $j++){
          if(!empty($result_s[$i]['appt_time']) && !empty($result_s[$j]['appt_time'])){
            
            $prev_appt_time = $result_s[$i]['appt_time'];
            $current_appt_time = $result_s[$j]['appt_time'];
            
            $prev_h = explode(":", $prev_appt_time);
            $prev_am_or_pm = explode(" ", $prev_h[1]);
            
            $current_h = explode(":", $current_appt_time);
            $current_am_or_pm = explode(" ", $current_h[1]);
            
            // Get the previous element's hours.
              if($prev_h[0][0] == "0"){
                $prev_hours = (int)$prev_h[0][1];
              }else{
                $prev_hours = (int)$prev_h[0];
              }
              
              // Get the current element's hours.
              if($current_h[0][0] == "0"){
                $current_hours = (int)$current_h[0][1];
              }else{
                $current_hours = (int)$current_h[0];
              }
              
              if($prev_am_or_pm[0][0] == "0"){
                $prev_minutes = (int)$prev_am_or_pm[0][1];
              }else{
                $prev_minutes = (int)$prev_am_or_pm[0];
              }
                
              if($current_am_or_pm[0][0] == "0"){
                $current_minutes = (int)$current_am_or_pm[0][1];
              }else{
                $current_minutes = (int)$current_am_or_pm[0];
              }
            
            if($prev_am_or_pm[1] == 'PM' && $current_am_or_pm[1] == 'AM'){
              $temp = $result_s[$i];
              $result_s[$i] = $result_s[$j];
              $result_s[$j] = $temp;
            } else if($prev_am_or_pm[1] == "PM" && $current_am_or_pm[1] == "PM"){
                //$times[] = array('current'=>$current_hours, 'prev'=>$prev_hours );
                
                  //return array('prev'=>$prev_hours , 'curr'=>$current_hours);
                if($prev_hours < 12 && $current_hours == 12){
                  $temp = $result_s[$i];
                  $result_s[$i] = $result_s[$j];
                  $result_s[$j] = $temp;
                }else if($prev_hours == 12 && $current_hours == 12){
                  if($current_minutes < $prev_minutes){
                    // The current element's minutes are smaller than the previous element's minutes. Swipe the elements.
                    $temp = $result_s[$i];
                    $result_s[$i] = $result_s[$j];
                    $result_s[$j] = $temp;
                  }
                }else{
                  
                  if($current_hours == $prev_hours){
                    // Two appointments at the same hour. Check the minutes to see who has the earliest appointment.
                    if($current_minutes < $prev_minutes){
                      // The current element's minutes are smaller than the previous element's minutes. Swipe the elements.
                      $temp = $result_s[$i];
                      $result_s[$i] = $result_s[$j];
                      $result_s[$j] = $temp;
                    }
                    
                  }else{
                   if($current_hours < $prev_hours){
                       // If the current element's hours are smaller than the previous one's, swipe the elements.
                       $temp = $result_s[$i];
                       $result_s[$i] = $result_s[$j];
                       $result_s[$j] = $temp;
                    }
                  }
                  
                }
                
            }else if($prev_am_or_pm[1] == "AM" && $current_am_or_pm[1] == "AM"){
                 if($prev_hours < 12 && $current_hours == 12){
                  $temp = $result_s[$i];
                  $result_s[$i] = $result_s[$j];
                  $result_s[$j] = $temp;
                }else if($prev_hours == 12 && $current_hours == 12){
                  if($current_minutes < $prev_minutes){
                    // The current element's minutes are smaller than the previous element's minutes. Swipe the elements.
                    $temp = $result_s[$i];
                    $result_s[$i] = $result_s[$j];
                    $result_s[$j] = $temp;
                  }
                }else{
                  
                  if($current_hours == $prev_hours){
                    // Two appointments at the same hour. Check the minutes to see who has the earliest appointment.
                    if($current_minutes < $prev_minutes){
                      // The current element's minutes are smaller than the previous element's minutes. Swipe the elements.
                      $temp = $result_s[$i];
                      $result_s[$i] = $result_s[$j];
                      $result_s[$j] = $temp;
                    }
                    
                  }else{
                   if($current_hours < $prev_hours){
                       // If the current element's hours are smaller than the previous one's, swipe the elements.
                       $temp = $result_s[$i];
                       $result_s[$i] = $result_s[$j];
                       $result_s[$j] = $temp;
                    }
                  }
                  
                }
            }else{
              
              
              
              // Compare the hours.
              if($current_hours == $prev_hours){
                // Two appointments at the same hour. Check the minutes to see who has the earliest appointment.
                if($current_minutes < $prev_minutes){
                  // The current element's minutes are smaller than the previous element's minutes. Swipe the elements.
                  $temp = $result_s[$i];
                  $result_s[$i] = $result_s[$j];
                  $result_s[$j] = $temp;
                }
                
              }else{
               if($current_hours < $prev_hours){
                   // If the current element's hours are smaller than the previous one's, swipe the elements.
                   $temp = $result_s[$i];
                   $result_s[$i] = $result_s[$j];
                   $result_s[$j] = $temp;
                }
              }
            }
          }
        }
      } // end for*/
      
      usort($result_s, "bcbsri_sort");
      
      //$result_store = bcbsri_store_checkin_db_get('bcbsri_checkin_stores', array('wait_time'), $store_id, 'id');
      
      if($result_store->rowCount() > 0){
        //$results = $result_store->fetchAll();
        foreach($result_store as $r){
          $wait_time = $r->wait_time;
        }
      }
      
      return array('success'=>true, 'results_cs'=>$result_cs, 'results_s'=>$result_s, 'wait_time'=>$wait_time);
      //return array('success'=>false, 'reason'=>'Error sending the POST request.');
    }else return array('success'=>false, 'reason'=>'No data sent.');
  } catch (Exception $e) {
    return array('success'=>false, 'reason'=>'Errored adding visitor', 'exception' => $e->getMessage());
  }
}

function bcbsri_store_checkin_editstore(){
  try {
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      if($_REQUEST['id']){
        
        $fields = array('id');
        $query = db_select('bcbsri_checkin_stores', 's');
        $query->fields('s', $fields);
        $query->condition('location', db_like($_REQUEST['l']), 'LIKE');
        $results = $query->execute();
        
        if($results->rowCount() > 0){
          foreach($results as $r){
            $id = $r->id;
          }
          if($id == $_REQUEST['id']){
            bcbsri_online_application_db_set('bcbsri_checkin_stores', 
              array(
                'id' => $_REQUEST['id'],
                'location' => $_REQUEST['l'],
                'green_threshold' => $_REQUEST['g'],
                'orange_threshold' => $_REQUEST['o']
              ),
              'id'
            );
            return array('success'=>true);
          }else return array('success'=>false, 'reason'=>'Exists');
        }
        // No results were found with that name.
        bcbsri_online_application_db_set('bcbsri_checkin_stores', 
          array(
            'id' => $_REQUEST['id'],
            'location' => $_REQUEST['l'],
            'green_threshold' => $_REQUEST['g'],
            'orange_threshold' => $_REQUEST['o']
          ),
          'id'
        );
        return array ('success' => true);
      }else return array('success'=>false, 'reason'=>'Error in the update.');
    }else return array('success'=>false, 'reason'=>'No data sent.');
  } catch (Exception $e) {
    return array('success'=>false, 'reason'=>'Errored editing store', 'exception' => $e->getMessage());
  }
}

function bcbsri_store_checkin_recoverstore(){
  try {
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      if($_REQUEST['id']){
        bcbsri_online_application_db_set('bcbsri_checkin_stores', 
          array(
            'id' => $_REQUEST['id'],
            'show_s' => 1
          ),
          'id'
        );
        return array('success'=>true);
      }else return array('success'=>false, 'reason'=>'Error in the update.');
    }else return array('success'=>false, 'reason'=>'No data sent.');
  } catch (Exception $e) {
    return array('success'=>false, 'reason'=>'Errored editing store', 'exception' => $e->getMessage());
  }
}

function bcbsri_store_checkin_updatevisitorlist(){
  try {
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      $fields = array('first_name', 'last_name', 'id', 'product', 'segment', 'check_in', 'reason_for_visit', 'phone', 'email', 'appointment','appointment_with', 'appt_time', 'signature_received');
      $query = db_select('bcbsri_checkin_visitors', 'v');
      $query->fields('v', $fields);
      $query->condition('seen', 0);
      $query->condition('not_seen', 0);
      $query->condition('store_id', $_REQUEST['store']);
      $result = $query->execute();
      
      $fields_store = array('wait_time');
      $query_v = db_select('bcbsri_checkin_stores', 's');
      $query_v->fields('s', $fields_store);
      $query_v->condition('id', $_REQUEST['store']);
      $result_store = $query_v->execute();
      
      $currentTime = time();
      if($result->rowCount() > 0){
        //$result = $result->fetchAll();
        $visitors = array(); $vCount = 0;
        foreach($result as $r){
          $visitors[$vCount]['check_in'] = $r->check_in;
          $visitors[$vCount]['elapsed'] = (int)$currentTime - (int)$r->check_in;
          $visitors[$vCount]['name'] = $r->first_name.' '.$r->last_name;
          $visitors[$vCount]['id'] = $r->id;
          $visitors[$vCount]['product'] = $r->product;
          $visitors[$vCount]['segment'] = $r->segment;
          $visitors[$vCount]['reason_for_visit'] = $r->reason_for_visit;
          $visitors[$vCount]['phone'] = $r->phone;
          $visitors[$vCount]['email'] = $r->email;
          $visitors[$vCount]['appointment'] = $r->appointment;
          $visitors[$vCount]['appointment_with'] = $r->appointment_with;
          $visitors[$vCount]['appointment_time'] = $r->appt_time;
          $visitors[$vCount]['first_name'] = $r->first_name;
          $visitors[$vCount]['last_name'] = $r->last_name;
          $visitors[$vCount]['signature_received'] = $r->signature_received;
          $vCount++;
        }
        
        if($result_store->rowCount() > 0){
          foreach($result_store as $r){
            $wait_time = $r->wait_time;
          }
        }
        return array('success'=>true, 'results'=>$visitors, 'wait_time'=>$wait_time);
      }else{
        return array('sucess'=> false, 'message'=>'No data found.');
      }
      return array('success'=>false, 'reason'=>'Error sending the POST request.');
    }else return array('success'=>false, 'reason'=>'No data sent.');
  } catch (Exception $e) {
    return array('success'=>false, 'reason'=>'Errored adding visitor', 'exception' => $e->getMessage());
  }
}

function bcbsri_store_checkin_getadmindata(){
  if($_SERVER['REQUEST_METHOD'] == 'POST') {
      
      $fields = array('id', 'location', 'green_threshold', 'orange_threshold');
      $query = db_select('bcbsri_checkin_stores', 's');
      $query->fields('s', $fields);
      $query->condition('show_s', 1);
      $stores = $query->execute();
      
      $visitorData = array();
      $longestWait = (int)time();
      foreach($stores as $s){
        $queue_fields = array('id', 'check_in');
        $query_v = db_select('bcbsri_checkin_visitors', 'v');
        $query_v->fields('v', $queue_fields);
        $query_v->condition('store_id', $s->id);
        $query_v->condition('seen', 0);
        $query_v->condition('not_seen', 0);
        $visitors = $query_v->execute();
        //Find the longest wait time.
        if($visitors->rowCount() > 0){
          foreach($visitors as $v){
            if($v->check_in < $longestWait){
              $longestWait = (int)$v->check_in;
            }
          }
          $visitorData[$s->location]['elapsed'] = (int)time() - $longestWait;
          $longestWait = (int)time();
        }
        $visitors = $query_v->execute();
        $visitorData[$s->location]['inQueue'] = $visitors->rowCount();
        $visitorData[$s->location]['greenThreshold'] = $s->green_threshold;
        $visitorData[$s->location]['orangeThreshold'] = $s->orange_threshold;
        $visitorData[$s->location]['id'] = $s->id;
      }
      
      $storeCount = 0; // reset the store count for the next query.
      //$visitorsSeenToday = array();
      $stores = $query->execute();
      foreach($stores as $s){
        $queue_fields = array('id');
        $query_v = db_select('bcbsri_checkin_visitors', 'v');
        $query_v->fields('v', $queue_fields);
        $query_v->condition('store_id', $s->id);
        $query_v->condition('seen', 1);
        $beginOfDay = strtotime('today midnight');
        $endOfDay = strtotime("tomorrow", $beginOfDay) - 1;
        $query_v->condition('wait_time', $beginOfDay, '>=');
        $query_v->condition('wait_time', $endOfDay, '<=');
        $visitors = $query_v->execute();
        $visitorData[$s->location]['seenToday'] = $visitors->rowCount();
      }
      
      
      
     return array('success'=>true, 'visitorsInQueue' => $visitorData);
      
      
  }else return array('success'=>false, 'reason'=>'No data sent.');
  
}

function bcbsri_store_checkin_getstoreurls(){
  if($_SERVER['REQUEST_METHOD'] == 'POST') {
      
      $fields = array('id', 'url', 'location');
      $query = db_select('bcbsri_checkin_stores', 's');
      $query->fields('s', $fields);
      $query->condition('show_s', 1);
      $stores = $query->execute();
      
     return array('success'=>true, 'storedata' => $stores->fetchAll());
      
      
  }else return array('success'=>false, 'reason'=>'No data sent.');
  
}

function bcbsri_store_checkin_getremovedstores(){
  if($_SERVER['REQUEST_METHOD'] == 'POST') {
      
      $fields = array('id','location');
      $query = db_select('bcbsri_checkin_stores', 's');
      $query->fields('s', $fields);
      $query->condition('show_s', 0);
      $stores = $query->execute();
      
     return array('success'=>true, 'storedata' => $stores->fetchAll());
      
      
  }else return array('success'=>false, 'reason'=>'No data sent.');
  
}


/******************************************************************
 * $table - database table name
 * $record - array that represents the record to update
 * $updateby - array that contains the primary key or keys
 ******************************************************************/
function bcbsri_store_checkin_db_set($table, $record, $updateby = NULL) {
  $record = (object)$record;
  if(!empty($updateby)) drupal_write_record($table, $record, $updateby);
  else drupal_write_record($table, $record);
  return $record->id;
}

/******************************************************************
 * $table - database table name
 * $fields - array of table columns
 * $data - value to filter by
 * $condition - column to filter by
 ******************************************************************/
function bcbsri_store_checkin_db_get($table, $fields, $data, $condition) {
	$query = db_select($table, 't');
	$query->fields('t', $fields);
  if(!empty($condition) && !empty($data))	{
    $c = 't.' . $condition;
    $query->condition($c, $data);
  }
	$result = $query->execute();
	return $result; #->fetchAll();
}
