<?php
/* *******************************************************************************************
* Resume
******************************************************************************************* */
function bcbsri_online_application_resumesaved() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      //if(bcbsri_online_application_check_auth($_REQUEST['resumek'])) {
        //$auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
        
        //if(bcbsri_online_application_auth_verify($auth) == true){
          
          $resumek = $_REQUEST['resumek'];
          
          $query = db_select('bcbsri_online_application_udata', 'boau');
          $query->fields('boau', array('app_confirm', 'app_type', 'app_year', 'auth', 'auths', 'end_time'));
          $query->condition('boau.resumedk', $resumek);
          $result = $query->execute();
          
          if($result->rowCount() > 0){
            foreach($result as $r) {
              $app_confirm = $r->app_confirm;
              $app_type = $r->app_type;
              $app_year = $r->app_year;
              $auth = $r->auth;
              $auths = $r->auths;
              $end_time = (int)$r->end_time;
            }
            $config = bcbsri_online_application_get_json('config');
            $data_expire = $config['data_gc'];
            
            if(!empty($data_expire)) $time_limit = strtotime("-" . $data_expire . " days");
            else $time_limit = strtotime("-30 days");
            
            // Validate if the record is in the table 'ufields'.
            if($end_time >= $time_limit){
              $query2 = db_select('bcbsri_online_application_ufields', 'boaf');
              $query2->fields('boaf', array('flds'));
              $query2->condition('boaf.auth', $auth);
              $result2 = $query2->execute();
              
              if($result2->rowCount() > 0){
                foreach($result2 as $r) {
                  $fields = bcbsri_online_application_decrypt($r->flds, $auth);
                  $fields = json_decode($fields, true);
                }
                // Debugging purposes: see what fields were fetched:
                //return array('fields'=>$fields);
                
                // Validate the fields that were submitted.
                $name = $_REQUEST['name'];
                $dob = $_REQUEST['dob'];
                
                 // Build the regex.
                $regex = '/^';
                $regex .= '(' . trim($fields['first_name']) . '\s)';
                $regex .= (!empty(trim($fields['middle_initial'])))? '(' . trim($fields['middle_initial']) . '\.\s)?(' . trim($fields['middle_initial']) . '\s)?' :
                    '([a-zA-Z]\.\s)?([a-zA-Z]\s)?';
                $regex .= '(' . trim($fields['last_name']) . ')';
                $regex .= (!empty(trim($fields['suffix'])))? '(\s' . trim($fields['suffix']) . ')?(\.)?' : '(\s[a-zA-Z\.]*)?';
                $regex .= '$/';
                
                // For example: Test McTest
                // preg_grep("/^(Test\s)([a-zA-Z]\.\s)?([a-zA-Z]\s)?(McTest)(\s[a-zA-Z\.]*)?$/", explode("\n", $input_lines));
                
                $db_dob = date_create($fields['date_of_birth']);
                
                if(preg_match($regex, $name) && date_format($db_dob, 'm/d/Y') == $dob){
                  
                  if(isset($app_confirm)){
                    return array('success' => false, 'message' => 'Confirmed.');
                  }
                  
                  bcbsri_online_application_clear_cookies();
                  $time = time();
                  bcbsri_online_application_db_set('bcbsri_online_application_udata', array('end_time'=>$time, 'auth' => $auth), 'auth');
                  bcbsri_online_application_set_cookie(array('bcbsri_oa_aid', $auth, '+15 minutes'));
                  
                  // Get the Plan Details.
                  $result3 = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
                  foreach($result3 as $r) {
                    $pDetails = json_decode($r->selected, true);
                  }
                  $pMedical = $pDetails['medical']; #explode(':',$pDetails[0]);
                  $pDental = $pDetails['dental']; #explode(':',$pDetails[0]);
                  if(!empty($pDental[1]) && empty($pMedical[1])) $dental = true;
                  else $dental = false;
                  
                 return array('success' => true, 'auth' => $auth, 'app_confirm' => $app_confirm, 'app_type' => $app_type, 'app_year' => $app_year, 'fields'=>$fields, 'dental'=>$dental, 'medical'=>$pMedical);
                }else{
                  return array('success' => false, 'message' => 'Record exists. However, the information does not match.');
                }
              }else{
                return array('sucess' => false, 'message' => 'The record was not persisted. (It most likely has expired).');
              }
            }else{
              return array('sucess' => false, 'message' => 'The record was not persisted. (It most likely has expired).'); 
            }
          }else{
            return array('sucess' => false, 'message' => 'Record not found.'); 
          }
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to validate data', 'error' => var_dump($e, true));
  }
}

/* *******************************************************************************************
* Resume when there's no resume key available.
******************************************************************************************* */
function bcbsri_online_application_app_searchresumekey() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      //if(bcbsri_online_application_check_auth($_REQUEST['resumek'])) {
        //$auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
        
        //if(bcbsri_online_application_auth_verify($auth) == true){
          
          $query2 = db_select('bcbsri_online_application_ufields', 'boaf');
          $query2->fields('boaf');
          //$query2->condition('boaf.auth', 'itzxfnaKf3wAe1WfrgcoYzjMBlQUYDBU');
          $result2 = $query2->execute();
          
          /****/
          $all_names = array();
          if($result2->rowCount() > 0){
            
            foreach($result2 as $r) {
              
              if(isset($r->flds)){
                $fields = bcbsri_online_application_decrypt($r->flds, $r->auth);
              
                $fields = json_decode($fields, true);
                
                if(isset($fields)){
                  //return array('fields'=>$fields);
                  //return array($fields['first_name'], $fields['last_name'], $fields['date_of_birth']);
                  
                  $name = $_REQUEST['name'];
                  $dob = $_REQUEST['dob'];
                  $email = $_REQUEST['email'];
                  
                   // Build the regex.
                  $regex = '/^';
                  $regex .= '(' . trim($fields['first_name']) . '\s)';
                  $regex .= (!empty(trim($fields['middle_initial'])))? '(' . trim($fields['middle_initial']) . '\.\s)?(' . trim($fields['middle_initial']) . '\s)?' :
                      '([a-zA-Z]\.\s)?([a-zA-Z]\s)?';
                  $regex .= '(' . trim($fields['last_name']) . ')';
                  $regex .= (!empty(trim($fields['suffix'])))? '(\s' . trim($fields['suffix']) . ')?(\.)?' : '(\s[a-zA-Z\.]*)?';
                  $regex .= '$/';
                  
                  // For example: Test McTest
                  // preg_grep("/^(Test\s)([a-zA-Z]\.\s)?([a-zA-Z]\s)?(McTest)(\s[a-zA-Z\.]*)?$/", explode("\n", $input_lines));
                  
                  $db_dob = date_create($fields['date_of_birth']);
                  
                  // If the request data matches the record on the db.
                  if(preg_match($regex, $name) && date_format($db_dob, 'm/d/Y') == $dob){
                    
                    $check = bcbsri_online_application_db_get('bcbsri_online_application_udata', array('resumedk' ,'app_confirm'), $r->auth, 'auth');
                    foreach($check as $c){
                      $resumeKey = $c->resumedk;
                      $app_confirm = $c->app_confirm;
                    }
                    
                    if(empty($app_confirm)){
                      return array('success' => false, 'message' => 'Confirmed.');
                    }
                    
                    if(!empty($resumeKey)) {
                      $to = $email;
                      $headers = "From: website@bcbsri.org\r\n";
                      // $headers .= "Reply-To: \r\n";
                      $headers .= "MIME-Version: 1.0\r\n";
                      $headers .= "Content-Type: text/html; charset=ISO-8859-1\r\n";
                      $subject = 'We saved your form for you';
                      $message = '<p>We have saved your form right where you left off. When you&rsquo;re ready to complete the process, use the link below to return to bcbsri.com. You will need your <strong>form ID</strong> below. We will save your form for 30 days.<br /><br />
              
              If you have any questions, or if you just want a little assistance finishing the form, please let us help. You can call us seven days a week or stop into your nearest Your Blue Store, and we&rsquo;ll walk you through the rest of the process.<br /><br />
              
              Form ID: '.$resumeKey.'</p>
              
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
                    }else{
                      return array('success'=>false, 'message'=>'Not found.');
                    }
                  }
                }
              }
            }
            return array('success'=>false, 'message'=>'Not found.');
          }else{
            return array('sucess' => false, 'message' => 'Not found.');
          }
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to validate data', 'error' => var_dump($e, true));
  }
}