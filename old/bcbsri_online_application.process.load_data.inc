<?php
/* *******************************************************************************************
* Load the application from the database.
******************************************************************************************* */
    }else if($requestProcess == "load-data") {  
        $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
        
        if(bcbsri_online_application_auth_verify($auth) == true){
            $query = db_select('bcbsri_online_application_ufields', 'boaf');
            $query->fields('boaf', array('flds'));
            $query->condition('boad.auth', $auth);
            $queryResult = $query->execute();
            $fields = bcbsri_online_application_decrypt($queryResult->flds, $auth);
            
            if(!empty($fields))
            {
                return array('success' => true, 'fields' => $fields);
            }else{
                return array('success' => false, 'reason' => 'The application data is not available.');
            }
        }else{
             return array('success' => false, 'reason' => 'Could not authenticate user.');
        }
    }