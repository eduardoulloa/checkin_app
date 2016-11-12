<?php
try {
  $record = new stdClass();
  $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
	$planYear = bcbsri_online_application_get_year($auth);
  $sfResult = bcbsri_online_application_db_get('bcbsri_online_application_udata', array('sid'), $auth, 'auth');
  foreach($sfResult as $sf) {
    $sfIDS = (!empty($sf->sid) ? json_decode($sf->sid, true) : '');
  }
  
  $record->assumeComplete = (empty($_REQUEST['completed']) ? false : (bool)$_REQUEST['completed']);
  // empty when passed in the first time
  $record->trackingId = (!empty($sfIDs) && !empty($sfIDs['trackingID']) && $sfIDs['trackingID'] != 'null' ? $sfIDs['trackingID'] : (empty($_REQUEST['sfTracking']) && $_REQUEST['sfTracking'] != 'null' ? '' : $_REQUEST['sfTracking']));

/*	$planArr = array(
        "BlueCHiP for Medicare Standard with Drugs"=> $planYear . " BlueCHiP for Medicare Standard with Drugs (HMO)",
        "BlueCHiP for Medicare Plus"=> $planYear . " BlueCHiP for Medicare Plus (HMO)",
        "BlueCHiP for Medicare Extra"=> $planYear . " BlueCHiP for Medicare Extra (HMO-POS)",
        "BlueCHiP for Medicare Core"=> $planYear . " BlueCHiP for Medicare Core (HMO)",
        "BlueCHiP for Medicare Advance"=> $planYear . " BlueCHiP for Medicare Advance (HMO)",
        "BlueCHiP for Medicare Preferred"=> $planYear . " BlueCHiP for Medicare Preferred (HMO-POS)",
        "BlueCHiP for Medicare Value"=> $planYear . " BlueCHiP for Medicare Value (HMO-POS)"); */
	$planArr = array(
        "BlueCHiP for Medicare Standard with Drugs"=> "STANDARD",
        "BlueCHiP for Medicare Plus"=> "PLUS",
        "BlueCHiP for Medicare Extra"=> "EXTRA",
        "BlueCHiP for Medicare Core"=> "CORE",
        "BlueCHiP for Medicare Advance"=> "ADVANCE",
        "BlueCHiP for Medicare Preferred"=> "PREFERRED",
        "BlueCHiP for Medicare Value"=> "VALUE");
	
  $result = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
  foreach($result as $r) {
    $pDetails = json_decode($r->selected, true);
  }
	if($sfArray['debug']) {
    $sfArray['debug_info']['pMedicalOriginal'] = $pDetails['medical'];
    $sfArray['debug_info']['pDentalOriginal'] = $pDetails['dental'];
  }
	$pMedical = explode('|', $pDetails['medical']);
	$pDental = explode('|', $pDetails['dental']);
	if($sfArray['debug']) {
    $sfArray['debug_info']['pMedicalSplit'] = $pMedical; #[0];
	  $sfArray['debug_info']['pDentalSplit'] = $pDental; #[0];
	  $sfArray['debug_info']['pMedical'] = $pMedical[0];
    $sfArray['debug_info']['pDental'] = $pDental[0];
  }
	$dentalRider = 0;
	if(!empty($pDental[0]) && strtolower($pDental[0]) == strtolower('BlueCHiP for Medicare Dental')) $dentalRider = 1;
	else $dentalRider = 0;

  $record->product      = (!empty($planArr[$pMedical[0]]) ? $planArr[$pMedical[0]] : $pMedical[0]);
  $record->addDental    = (bool)$dentalRider;
  $record->memNumber    = formatMedicareNumber(trim($data['first_name']));
  $record->hicn         = $record->memNumber;
  
  $record->fName        = trim($data['first_name']);
  $record->mName        = (!empty($data['middle_initial']) ? trim($data['middle_initial']) : '');
  $record->lName        = trim($data['last_name']);
  #$record->salutation   = trim($data['suffix']);
  #$record->dob          = formatDate(trim($data['date_of_birth']), $planYear); // passed as CCYY-MM-DD
  #$record->sex          = ucfirst(trim($data['gender']));
  $hPhone = ''; $cPhone = '';
	$pPhoneType = (!empty($data['home_or_cell_phone_number']) ? array_search($data['home_or_cell_phone_number'], $phoneSearch) :'');
	$aPhoneType = (!empty($data['alt_home_or_cell_phone_number']) ? array_search($data['alt_home_or_cell_phone_number'], $phoneSearch) :'');
  if(!empty(trim($data['phone_number']))) {
    if($pPhoneType == 'Home' || empty($pPhoneType)) $hPhone = formatPhoneNumber(trim($data['phone_number']));
    else $cPhone == formatPhoneNumber(trim($data['phone_number']));
	}
	if(!empty(trim($data['alternate_phone_number']))) {
    if($aPhoneType == 'Home') {
      if(!empty($hPhone)) {
        if(!empty($pPhoneType)) $cPhone = formatPhoneNumber(trim($data['alternate_phone_number']));
        else {
          $cPhone = $hPhone;
          $hPhone = formatPhoneNumber(trim($data['alternate_phone_number']));
        }
      } else $hPhone = formatPhoneNumber(trim($data['alternate_phone_number']));
    } else {
      if(!empty($cPhone)) {
        if(!empty($pPhoneType)) $hPhone = formatPhoneNumber(trim($data['alternate_phone_number']));
        else {
          $hPhone = $cPhone;
          $cPhone = formatPhoneNumber(trim($data['alternate_phone_number']));
        }
      } else $cPhone == formatPhoneNumber(trim($data['alternate_phone_number']));
    }
	}
  $record->hPhone       = $hPhone;
  $record->cPhone       = $cPhone;
  $record->aPhone       = '';
  $record->primLang     = (!empty($data['primary_language']) ? trim($data['primary_language']) :'');
  $email = (!empty($data['email_address']) && filter_var(trim($data['email_address']), FILTER_VALIDATE_EMAIL) != false ? filter_var($data['email_address'], FILTER_VALIDATE_EMAIL) : '');
	$record->email        = $email;
  
  // address information
	$record->phAddress1   = trim($data['permanent_street_address']);
  $record->phAddress2   = '';
  $record->phAddress3   = '';
  $record->phCity       = trim($data['permanent_address_city']);
  $record->phState      = trim($data['permanent_address_state']); #array_search($data['permanent_address_state'], $stateArr);
  $record->phZip        = trim($data['permanent_address_zip_code']);
  
  if($data['mailing_address_different_from_permanent'] == 'yes') {
    $record->mAddress1  = trim($data['mailing_street_address']);
    $record->mAddress2  = '';
    $record->mAddress3  = '';
    $record->mCity      = trim($data['mailing_address_city']);
    $record->mState     = trim($data['mailing_address_state']); #array_search($data['mailing_address_state'], $stateArr);
    $record->mZip       = trim($data['mailing_address_zip']);
  } else {
    $record->mAddress1  = $record->phAddress1;
    $record->mAddress2  = $record->phAddress2;
    $record->mAddress3  = $record->phAddress3;
    $record->mCity      = $record->phCity;
    $record->mState     = $record->phState;
    $record->mZip       = $record->phZip;
  }
  
  if($data['billing_address_different_from_permanent_or_mailing'] == 'yes') {
    $record->bAddress1  = trim($data['billing_street_address']);
    $record->bAddress2  = '';
    $record->bAddress3  = '';
    $record->bCity      = trim($data['billing_address_city']);
    $record->bState     = trim($data['billing_address_state']); #array_search($data['billing_address_state'], $stateArr);
    $record->bZip       = trim($data['billing_address_zip']);
  } else {
    $record->bAddress1  = $record->phAddress1;
    $record->bAddress2  = $record->phAddress2;
    $record->bAddress3  = $record->phAddress3;
    $record->bCity      = $record->phCity;
    $record->bState     = $record->phState;
    $record->bZip       = $record->phZip;
  }
  
  // PCP  
  $pcpFName = ''; $pcpLName = '';
  if(!empty($data['provide_pcp_name']) ? trim($data['provide_pcp_name']) : '') {
    if(strpos($data['provide_pcp_name'], ',') !== false) {
      $splitName = split(',', $data['provide_pcp_name']);
      $pcpLName = $splitName[0];
      for($i = 1; $i < count($splitName); $i++) {
        $pcpFName .= $splitName[$i] . ' ';
      }
    } else {
      $splitName = split(' ', $data['provide_pcp_name']);
      $pcpFName = $splitName[0];
      for($i = 1; $i < count($splitName); $i++) {
        $pcpLName .= $splitName[$i] . ' ';
      }
    }
  }
  $record->pcpFName = '';
  $record->pcpLName = '';
  #$record->pcpName = (!empty($data['provide_pcp_name']) ? trim($data['provide_pcp_name']) : ''); // TODO: get pcp combined to one name feild
  $record->pcpPhone = '';
  $record->pcpAddress1 = (!empty($data['pcp_street_address']) ? trim($data['pcp_street_address']) : '');
  $record->pcpAddress2 = '';
  $record->pcpCity = (!empty($data['pcp_city']) ? trim($data['pcp_city']) : '');
  $record->pcpState = (!empty($data['pcp_state']) ? trim($data['pcp_state']) : '');
  $record->pcpZip = (!empty($data['pcp_zip']) ? trim($data['pcp_zip']) : '');
  $record->pcpRecSeen = (!empty($data['pcp_current_patient']) ? (strtolower($data['pcp_current_patient']) == 'yes' ? TRUE : FALSE) : '');
  
  //Payment Info
	$paySwitch = (!empty($data['select_premium_payment_option']) ? $data['select_premium_payment_option'] : 'none');
	switch($paySwitch) {
		case 'quarterly_bill':
			$payOption = 'Quarterly';
      $paymentType = 'Direct Bill';
			break;
		case 'eft':
			$payOption = '';
	    $paymentType = 'EFT';
		break;
		case 'automatic_deduction':
			$payOption = '';
      $paymentType = 'Automatic Deduction';
			break;
		case 'monthly_bill':
		default:
			$payOption = 'Monthly';
      $paymentType = 'Direct Bill';
		break;
	}
  $record->paymentType = $paymentType;
  $record->eftAccountName = '';
  $record->eftAccountNum = '';
  $record->eftRoutingNum = '';
  $record->eftAccountType = '';
  $record->directBillOptions = $payOption;
  
  //Member Signature Date
  $record->memSignDate = (!empty($data['todays_date']) ? formatDate($data['todays_date'], $planYear) : ''); //Format CCYY-MM-DD
		
	if($data['completing_this_application'] == 'authorized_representative'){
    $authFName = ''; $authLName = '';
    if(!empty($data['authorized_rep_name']) ? trim($data['authorized_rep_name']) : '') {
      if(strpos($data['authorized_rep_name'], ',') !== false) {
        $splitName = split(',', $data['authorized_rep_name']);
        $authLName = $splitName[0];
        for($i = 1; $i < count($splitName); $i++) {
          $authFName .= $splitName[$i] . ' ';
        }
      } else {
        $splitName = split(' ', $data['authorized_rep_name']);
        $authFName = $splitName[0];
        $authLName = '';
        for($i = 1; $i < count($splitName); $i++) {
          $authLName .= $splitName[$i] . ' ';
        }
      }
    }
    #$record->authName = (!empty($data['authorized_rep_name']) ? trim($data['authorized_rep_name']) : '');
    $record->authFName = trim($authFName);
    $record->authLName = trim($authLName);
    $record->authAddress1 = (!empty($data['authorized_rep_address']) ? trim($data['authorized_rep_address']) : '');
    $record->authAddress2 = '';
    $record->authCity = (!empty($data['authorized_rep_city']) ? trim($data['authorized_rep_city']) : '');
    $record->authState = (!empty($data['authorized_rep_state']) ? trim($data['authorized_rep_state']) : '');
    $record->authZip = (!empty($data['authorized_rep_zip']) ? trim($data['authorized_rep_zip']) : '');
    $record->authRel = (!empty($data['relationship_to_enrollee']) ? trim($data['relationship_to_enrollee']) : '');
    $record->authPhone = (!empty($data['authorized_rep_phone']) ? formatPhoneNumber($data['authorized_rep_phone']) : '');
		# $objApplicationDetails->Guardian_Signature_app = 'Authorized representative';
		# $objApplicationDetails->Guardian_Signature_Name_app = (!empty($data['enrollee_signature_name']) ? trim($data['enrollee_signature_name']) : '');
		# $objApplicationDetails->Guardian_Signature_Date_app = (!empty($data['todays_date']) ? $data['todays_date'] : '');
	}else{
		/* ***************************************************
     * The below fields do not exist
     *************************************************** */
    # $objApplicationDetails->My_Signature_app = 'Applicant';
		# $objApplicationDetails->My_Signature_Name_app = (!empty($data['enrollee_signature_name']) ? trim($data['enrollee_signature_name']) : '');
		# $objApplicationDetails->My_Signature_Date_app = (!empty($data['todays_date']) ? formatDate($data['todays_date']) : '');
	}
	
	if($data['completing_this_application'] == 'broker_agent_other'){
    // TODO: Get broker name combined. Only one field on app
    $bFName = ''; $bLName = '';
    if(!empty($data['broker_agent_name']) ? trim($data['broker_agent_name']) : '') {
      if(strpos($data['broker_agent_name'], ',') !== false) {
        $splitName = split(',', $data['broker_agent_name']);
        $bLName = $splitName[0];
        for($i = 1; $i < count($splitName); $i++) {
          $bFName .= $splitName[$i] . ' ';
        }
      } else {
        $splitName = split(' ', $data['broker_agent_name']);
        $bFName = $splitName[0];
        for($i = 1; $i < count($splitName); $i++) {
          $bLName .= $splitName[$i] . ' ';
        }
      }
    }
    #$record->bName = (!empty($data['broker_agent_name']) ? trim($data['broker_agent_name']) : '');
    $record->bFName = $bFName;
    $record->bLName = $bLName;
    $record->bIDNum = (!empty($data['broker_agent_id']) ? trim($data['broker_agent_id']) : '');
	}
  
  //effective date
  // TODO: Even though we do not send, do we want to add it to the broker/agent section?
  $record->effDate = ''; //Format CCYY-MM-DD
	if($sfArray['debug']) $sfArray['debug_info']['obj'] = $record;
} catch (Exception $e) {
	$sfArray['success'] = false;
	if($sfArray['debug']) {
    $sfArray['errorMsg'] = "failed creating obj for record";
    $sfArray['debug_info']['obj'] = $record;
  }
	return $sfArray;
	//die();
}

try {
  /* *******************************************
   * Login to Salesforce
   ******************************************* */
  $params = "grant_type=password"
    . "&client_id=" . $sfdcClientId
    . "&client_secret=" . $sfdcClientSecret
    . "&username=" . $sfdcUsername
    . "&password=" . $sfdcPassword . $sfdcToken;
  $curl = curl_init($sfdcURL . '/services/oauth2/token');
  #$curl = curl_init(_SF_TOKEN_URI_);
  curl_setopt($curl, CURLOPT_HEADER, false);
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($curl, CURLOPT_POST, true);
  curl_setopt($curl, CURLOPT_POSTFIELDS, $params);
  
  $json_response = curl_exec($curl);
  $response = json_decode($json_response, true);
  $status = curl_getinfo($curl, CURLINFO_HTTP_CODE);
  
  if ( $status != 200 ) {
    $sfArray['success'] = false;
    $sfArray['message'] = "failed to connect to Salesforce";
    if($sfArray['debug']) {
      $sfArray['debug_info']['loginResponse'] = $response;
      $sfArray['debug_info']['status'] = $status;
    }
    include('bcbsri_online_application.test_salesforce.php');
    return $sfArray;
  }
  if(empty($response['access_token']) || empty($response['instance_url'])) {
    $sfArray['success'] = false;
    $sfArray['message'] = "Failed to get access token or instance url";
    if($sfArray['debug']) {
      $sfArray['debug_info']['login']['response'] = $response;
      $sfArray['debug_info']['login']['status'] = $status;
    }
    include('bcbsri_online_application.test_salesforce.php');
    return $sfArray;
  }

  /* *******************************************
   * Send data to Salesforce
   ******************************************* */
  $access_token = $response['access_token'];
  $instance_url = $response['instance_url'];
  $url = $instance_url . _SF_MA_PC_URI_;
  $content = json_encode(array('plnChg' => $record));

  $curlLU = curl_init($url);
  curl_setopt($curlLU, CURLOPT_HEADER, false);
  curl_setopt($curlLU, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($curlLU, CURLOPT_HTTPHEADER, array("Authorization: OAuth $access_token", "Content-type: application/json"));
  curl_setopt($curlLU, CURLOPT_POSTFIELDS, $content);
  //curl_setopt($curl, CURLOPT_CUSTOMREQUEST, "POST"); // may not need

  $json_response = curl_exec($curlLU);
  $response = json_decode($json_response, true);
  $status = curl_getinfo($curlLU, CURLINFO_HTTP_CODE);

  if ( $status != 200 ) {
    $sfArray['success'] = false;
    $sfArray['message'] = "Failed to access Salesforce to create record";
    if($sfArray['debug']) {
      $sfArray['debug_info']['send']['response'] = $response;
      $sfArray['debug_info']['send']['status'] = $status;
    }
    include('bcbsri_online_application.test_salesforce.php');
    return $sfArray;
  }
  if($response['result'] == 'SUCCESS') {
    $sfArray['success'] = false;
    $sfArray['message'] = "Failed to access Salesforce to create record";
    if($sfArray['debug']) {
      $sfArray['debug_info']['send']['response'] = $response;
      $sfArray['debug_info']['send']['status'] = $status;
    }
    bcbsri_online_application_db_set('bcbsri_online_application_udata', array('auth' => $auth, 'sid'=> json_encode(array('trackingId' => $response->trackingId, 'acctId' => $response->acctId, 'oppId' => $response->oppId)), 'app_confirm' => $response->trackingId), 'auth');
  	if($record->assumeComplete) $sfArray['confirmation'] = $response->trackingId;
    return $sfArray;
  } else {
    $sfArray['success'] = false;
    $sfArray['message'] = "Failed to create record in Salesforce";
    if($sfArray['debug']) {
      $sfArray['debug_info']['send']['response'] = $response;
      $sfArray['debug_info']['send']['status'] = $status;
    }
    return $sfArray;
  }

} catch (Exception $e) {
	$sfArray['success'] = false;
  $sfArray['message'] = "failed to create record in salesforce";
	if($sfArray['debug']) {
    $sfArray['debug_info']['sending']['createResponse'] = $response[0];
    $sfArray['debug_info']['sending']['message'] = $e->faultstring;
  }
  include('bcbsri_online_application.test_salesforce.php');
	return $sfArray;
	//die();
}
