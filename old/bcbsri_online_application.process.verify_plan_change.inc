<?php
/* *******************************************************************************************
* If the spouse or dependents have changed, verify the data of the application.
******************************************************************************************* */
function bcbsri_online_application_verifyplanchange() {
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
          $query = db_select('bcbsri_online_application_ufields', 'boaf');
          $query->fields('boaf', array('flds'));
          $query->condition('boad.auth', $auth);
          $queryResult = $query->execute();
          $fields = bcbsri_online_application_decrypt($queryResult->flds, $auth);
          $fields = json_decode($fields, true);
          
          // TODO: This might require a regex to validate spouse's full name (like we do for the applicant's full name).
          if($fields['spouse_first_name'] != $data['spouse_first_name'] ||
              $fields['spouse_middle_initial'] != $data['spouse_middle_initial'] ||
              $fields['spouse_last_name'] != $data['spouse_last_name'] ||
              $fields['spouse_date_of_birth'] != $data['spouse_date_of_birth'] ||
              
              $fields['dependent_first_name_1'] != $data['dependent_first_name_1'] ||
              $fields['dependent_middle_initial_1'] != $data['dependent_middle_initial_1'] ||
              $fields['dependent_last_name_1'] != $data['dependent_last_name_1'] ||
              $fields['dependent_date_of_birth_1'] != $data['dependent_date_of_birth_1'] ||
              
              $fields['dependent_first_name_2'] != $data['dependent_first_name_2'] ||
              $fields['dependent_middle_initial_2'] != $data['dependent_middle_initial_2'] ||
              $fields['dependent_last_name_2'] != $data['dependent_last_name_2'] ||
              $fields['dependent_date_of_birth_2'] != $data['dependent_date_of_birth_2'] ||
              
              $fields['dependent_first_name_3'] != $data['dependent_first_name_3'] ||
              $fields['dependent_middle_initial_3'] != $data['dependent_middle_initial_3'] ||
              $fields['dependent_last_name_3'] != $data['dependent_last_name_3'] ||
              $fields['dependent_date_of_birth_3'] != $data['dependent_date_of_birth_3'] ||
              
              $fields['dependent_first_name_4'] != $data['dependent_first_name_4'] ||
              $fields['dependent_middle_initial_4'] != $data['dependent_middle_initial_4'] ||
              $fields['dependent_last_name_4'] != $data['dependent_last_name_4'] ||
              $fields['dependent_date_of_birth_4'] != $data['dependent_date_of_birth_4'] ||
              
              $fields['dependent_first_name_5'] != $data['dependent_first_name_5'] ||
              $fields['dependent_middle_initial_5'] != $data['dependent_middle_initial_5'] ||
              $fields['dependent_last_name_5'] != $data['dependent_last_name_5'] ||
              $fields['dependent_date_of_birth_5'] != $data['dependent_date_of_birth_5'] ||
              
              $fields['dependent_first_name_6'] != $data['dependent_first_name_6'] ||
              $fields['dependent_middle_initial_6'] != $data['dependent_middle_initial_6'] ||
              $fields['dependent_last_name_6'] != $data['dependent_last_name_6'] ||
              $fields['dependent_date_of_birth_6'] != $data['dependent_date_of_birth_6'] ||
              
              $fields['dependent_first_name_7'] != $data['dependent_first_name_7'] ||
              $fields['dependent_middle_initial_7'] != $data['dependent_middle_initial_7'] ||
              $fields['dependent_last_name_7'] != $data['dependent_last_name_7'] ||
              $fields['dependent_date_of_birth_7'] != $data['dependent_date_of_birth_7'] ||
              
              $fields['dependent_first_name_8'] != $data['dependent_first_name_8'] ||
              $fields['dependent_middle_initial_8'] != $data['dependent_middle_initial_8'] ||
              $fields['dependent_last_name_8'] != $data['dependent_last_name_8'] ||
              $fields['dependent_date_of_birth_8'] != $data['dependent_date_of_birth_8'] ||
              
              $fields['dependent_first_name_9'] != $data['dependent_first_name_9'] ||
              $fields['dependent_middle_initial_9'] != $data['dependent_middle_initial_9'] ||
              $fields['dependent_last_name_9'] != $data['dependent_last_name_9'] ||
              $fields['dependent_date_of_birth_9'] != $data['dependent_date_of_birth_9'])
          {
            // Either the spouse or the dependents have changed. Reverify application info and if the data
            // is correct, indicate so.
            return array('success' => 'true', 'info_changed'=> true, 'info_correct' => true);
          // Nothing has changed. No need to reverify application info.
          } else return array('success' => 'true', 'info_changed'=> false, 'info_correct' => true);
        }
      } else return array('success' => false, 'message' => 'Failed auth check');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Error retrieving the application.', 'error' => var_dump($e, true));
  }
}