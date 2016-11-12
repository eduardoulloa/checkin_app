<?php
/* *******************************************************************************************
* Validate the application.
******************************************************************************************* */
function bcbsri_online_application_app_validate() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      if(bcbsri_online_application_check_auth($_REQUEST['auth'])) {
        $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
        
        if(bcbsri_online_application_auth_verify($auth) == true){
          $query = db_select('bcbsri_online_application_udata', 'boad');
          $query->join('bcbsri_online_application_ufields', 'boaf', 'boad.auth = boaf.auth');
          $query->fields('boad', array('resumed', 'resumed_email'));
          $query->fields('boaf', array('flds'));
          $query->condition('boad.auth', $auth);
          $queryResult = $query->execute();
          $fields = bcbsri_online_application_decrypt($queryResult->flds, $auth);
          $fields = json_decode($fields, true);
          
          // Build the regex.
          $regex = '/^';
          $regex .= '(' . trim($data['first_name']) . '\s)';
          $regex .= (!empty(trim($data['middle_initial'])))? '(' . trim($data['middle_initial']) . '\.\s)?(' . trim($data['middle_initial']) . '\s)?' :
              '([a-zA-Z]\.\s)?([a-zA-Z]\s)?';
          $regex .= '(' . trim($data['last_name']) . ')';
          $regex .= (!empty(trim($data['suffix'])))? '(\s' . trim($data['suffix']) . ')?(\.)?' : '(\s[a-zA-Z\.]*)?';
          $regex .= '$/';
          
          // For example: Test McTest
          // preg_grep("/^(Test\s)([a-zA-Z]\.\s)?([a-zA-Z]\s)?(McTest)(\s[a-zA-Z\.]*)?$/", explode("\n", $input_lines));
          
          $fullName = $fields['name'];
          
          if(preg_match($regex, $fullName) && $fields['date_of_birth'] == $data['date_of_birth']) return array('success' => 'true', 'app_match' => true);
          else return array('success' => 'true', 'app_match' => false);
        } else return array('success' => false, 'reason' => 'Could not authenticate user');
      } else return array('success' => false, 'message' => 'Failed auth check');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to validate data', 'error' => var_dump($e, true));
  }
}