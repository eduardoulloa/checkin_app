<?php
try {
  $record = new stdClass();
  $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
	$planYear = bcbsri_online_application_get_year($auth);
  $sfResult = bcbsri_online_application_db_get('bcbsri_online_application_udata', array('sid'), $auth, 'auth');
  foreach($sfResult as $sf) {
    $sfIDS = (!empty($sf->sid) ? json_decode($sf->sid, true) : '');
  }
  $setSFID = true;
  
  $record->assumeComplete = (empty($_REQUEST['completed']) ? FALSE : (bool)$_REQUEST['completed']);
  // empty when passed in the first time
  if(!empty($sfIDs) && !empty($sfIDs['trackingID']) && $sfIDs['trackingID'] != 'null') {
    $record->trackingId = $sfIDs['trackingID'];
    $setSFID = false;
  } else {
    $record->trackingId = (empty($_REQUEST['sfTracking']) && $_REQUEST['sfTracking'] != 'null' ? '' : $_REQUEST['sfTracking']);
    $setSFID = true;
  }
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
	/* Setting default values for Salesforce fields */
	$Select_best_option_which_applies_app = '';
	$Attestation_recently_left = FALSE;
	$Attestation_recently_returned = FALSE;
	$Date_disenrolled_in_SNP_app = '';
	$Date_leaving_employer_app = '';
	$Date_left_PACE_program_app = '';
	$Date_lost_credible_drug_coverage_app = '';
	$Date_qualification_ended_for_help_with_p_app = '';
	$Date_returned_from_living_abroad_app = '';
	$Dated_moved_outside_of_service_area_app = '';
	$I_am_new_to_Medicare_self = FALSE;
	$I_get_extra_help_paying_for_drug_coverag_app = FALSE;
	$I_have_both_Medicare_and_Medicaid_or_my_app = FALSE;
	$I_no_longer_qualify_for_help_with_paying_app = FALSE;
	$If_yes_Please_include_name_and_state_of_app = '';
	$Leaving_employer_or_union_coverage_app = FALSE;
	$Moved_in_or_will_move_into_facility_app = '';
	$Moving_into_Long_Term_care_facility_or_c_app = FALSE;
	$Pharmacy_assistance_program_state_app = '';
	$Plan_is_ending_contract_with_Medicare_or_app = FALSE;
	$Recently_lost_credible_drug_coverage_app = FALSE;
	$This_plan_is_a_new_option_for_me_app = FALSE;
	$Was_enrolled_in_SNP_but_not_anymore_app = FALSE;
	
  $result = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
  foreach($result as $r) {
    $pDetails = json_decode($r->selected, true);
  }
	if($sfArray['debug']) {
    $sfArray['data'] = $data;
    $sfArray['pMedicalOriginal'] = $pDetails['medical'];
    $sfArray['pDentalOriginal'] = $pDetails['dental'];
  }
	if($sfArray['debug']) $sfArray['pMedicalSplit'] = $pMedical; #[0];
	if($sfArray['debug']) $sfArray['pDentalSplit'] = $pDental; #[0];
	$pMedical = explode('|', $pDetails['medical']);
	$pDental = explode('|', $pDetails['dental']);
	if($sfArray['debug']) $sfArray['pMedical'] = $pMedical[0];
	if($sfArray['debug']) $sfArray['pDental'] = $pDental[0];
	$dentalRider = 0;
	if(!empty($pDental[0]) && strtolower($pDental[0]) == strtolower('BlueCHiP for Medicare Dental')) $dentalRider = 1;
	else $dentalRider = 0;

  $record->product = (!empty($planArr[$pMedical[0]]) ? $planArr[$pMedical[0]] : $pMedical[0]);
  $record->addDental = (bool)$dentalRider;
  
  $record->fName        = trim($data['first_name']);
  $record->mName        = (!empty($data['middle_initial']) ? trim($data['middle_initial']) : '');
  $record->lName        = trim($data['last_name']);
  $record->salutation   = (!empty($data['prefix']) ? ucfirst(trim($data['prefix'])) : '');
  #$record->suffix       = trim($data['suffix']);
  $record->dob          = formatDate(trim($data['date_of_birth']), $planYear); // passed as CCYY-MM-DD
  $record->sex          = ucfirst(trim($data['gender']));
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
  $record->primLang     = (!empty($data['primary_language']) ? trim($data['primary_language']) :'');
  $email = (!empty($data['email_address']) && filter_var(trim($data['email_address']), FILTER_VALIDATE_EMAIL) != false ? filter_var($data['email_address'], FILTER_VALIDATE_EMAIL) : '');
	$record->email        = $email;
  
  // address information
	$record->phAddress1   = trim($data['permanent_street_address']);
  $record->phAddress2   = '';
  $record->phCity       = trim($data['permanent_address_city']);
  $record->phState      = trim($data['permanent_address_state']); #array_search($data['permanent_address_state'], $stateArr);
  $record->phZip        = trim($data['permanent_address_zip_code']);
  
  if($data['mailing_address_different_from_permanent'] == 'yes') {
    $record->mAddress1  = trim($data['mailing_street_address']);
    $record->mAddress2  = '';
    $record->mCity      = trim($data['mailing_address_city']);
    $record->mState     = trim($data['mailing_address_state']); #array_search($data['mailing_address_state'], $stateArr);
    $record->mZip       = trim($data['mailing_address_zip']);
  } else {
    $record->mAddress1  = $record->phAddress1;
    $record->mAddress2  = $record->phAddress2;
    $record->mCity      = $record->phCity;
    $record->mState     = $record->phState;
    $record->mZip       = $record->phZip;
  }
  
  if($data['billing_address_different_from_permanent_or_mailing'] == 'yes') {
    $record->bAddress1  = trim($data['billing_street_address']);
    $record->bAddress2  = '';
    $record->bCity      = trim($data['billing_address_city']);
    $record->bState     = trim($data['billing_address_state']); #array_search($data['billing_address_state'], $stateArr);
    $record->bZip       = trim($data['billing_address_zip']);
  } else {
    $record->bAddress1  = $record->phAddress1;
    $record->bAddress2  = $record->phAddress2;
    $record->bCity      = $record->phCity;
    $record->bState     = $record->phState;
    $record->bZip       = $record->phZip;
  }
  
  // emergency contact
  /*$ecFName = ''; $ecLName = '';
  if(!empty($data['emergency_contact_name']) ? trim($data['emergency_contact_name']) : '') {
    if(strpos($data['emergency_contact_name'], ',') !== false) {
      $splitName = split(',', $data['emergency_contact_name']);
      $ecLName = $splitName[0];
      for($i = 1; $i < count($splitName); $i++) {
        $ecFName .= $splitName[$i] . ' ';
      }
    } else {
      $splitName = split(' ', $data['emergency_contact_name']);
      $ecFName = $splitName[0];
      for($i = 1; $i < count($splitName); $i++) {
        $ecLName .= $splitName[$i] . ' ';
      }
    }
  }*/
  // TODO: If FName and LName are reduced to just Name, then we can use below
  $record->ecFullName       = (!empty($data['emergency_contact_name']) ? trim($data['emergency_contact_name']) : '');
  $record->ecFName      = $ecFName;
  $record->ecLName      = $ecLName;
  $record->ecPhone      = (!empty($data['emergency_contact_phone']) ? formatPhoneNumber(trim($data['emergency_contact_phone'])) : '');
  $record->ecRelShip    = (!empty($data['emergency_contact_relationship']) ? trim($data['emergency_contact_relationship']) : '');
  $record->ecIsDesignee = (!empty($data['emergency_contact_designee']) && strtolower($data['emergency_contact_designee']) == 'yes' ? TRUE : FALSE);

  // PCP  
  /*$pcpFName = ''; $pcpLName = '';
  if(!empty($data['pcp_name']) ? trim($data['pcp_name']) : '') {
    if(strpos($data['pcp_name'], ',') !== false) {
      $splitName = split(',', $data['pcp_name']);
      $pcpLName = $splitName[0];
      for($i = 1; $i < count($splitName); $i++) {
        $pcpFName .= $splitName[$i] . ' ';
      }
    } else {
      $splitName = split(' ', $data['pcp_name']);
      $pcpFName = $splitName[0];
      for($i = 1; $i < count($splitName); $i++) {
        $pcpLName .= $splitName[$i] . ' ';
      }
    }
  }*/
  $record->pcpFName = '';
  $record->pcpLName = '';
  $record->pcpFullName = (!empty($data['pcp_name']) ? trim($data['pcp_name']) : ''); // TODO: get pcp combined to one name feild
  $record->pcpPhone = '';
  $record->pcpAddress1 = (!empty($data['pcp_street_address']) ? trim($data['pcp_street_address']) : '');
  $record->pcpAddress2 = '';
  $record->pcpCity = (!empty($data['pcp_city']) ? trim($data['pcp_city']) : '');
  $record->pcpState = (!empty($data['pcp_state']) ? trim($data['pcp_state']) : '');
  $record->pcpZip = (!empty($data['pcp_zip']) ? trim($data['pcp_zip']) : '');
  $record->pcpRecSeen = (!empty($data['pcp_current_patient']) ? (strtolower($data['pcp_current_patient']) == 'yes' ? TRUE : FALSE) : '');
  
  // Medicare
	/* Medicare Info */
	$medicareName = trim($data['first_name']) . (!empty($data['middle_initial']) ? ' ' . trim($data['middle_initial']) : '') . ' ' . trim($data['last_name']);
  $record->hicn = formatMedicareNumber($data['medicare_claim_number']);
  #$record->medName = $medicareName; // TODO: see if we can get added to SF
  #$record->medSex = ucfirst($data['gender']); // TODO: see if we can get added to SF
  $record->effDtPartA = formatDate($data['part_a_effective_date'], $planYear); // Format CCYY-MM-DD
  $record->effDtPartB = formatDate($data['part_b_effective_date'], $planYear); // Format CCYY-MM-DD
  $record->medClNbr = formatMedicareNumber($data['medicare_claim_number']);
  
  // 1.  Do you have End-Stage Renal Disease (ESRD)
  $record->hasRenalDis = (!empty($data['end_stage_renal_disease']) ? (strtolower($data['end_stage_renal_disease']) == 'yes' ? TRUE : FALSE) : FALSE);
	/* Successfull Kidney transplant question -- need SF field */
	# $record->hadKidneyTransplant = (!empty($data['successfull_kidney_transplant']) ? (strtolower($data['successfull_kidney_transplant']) == 'yes' ? TRUE : FALSE) : FALSE);
  
  // 2 - other drug coverage
  $record->hasOthPD = (!empty($data['other_prescription_drug_coverage']) ? (strtolower($data['other_prescription_drug_coverage']) == 'yes' ? TRUE : FALSE) : FALSE);
  $record->othPDName = (!empty($data['other_coverage_name']) ? trim($data['other_coverage_name']) : '');
  $record->othPDId = (!empty($data['other_coverage_id']) ? trim($data['other_coverage_id']) : '');
  $record->othPDGrpNo = (!empty($data['other_coverage_group_number']) ? trim($data['other_coverage_group_number']) : '');
  
  // 3. Care facility
  // TODO: See if we can pass state and zip
  $record->isInCF = (!empty($data['resident_in_long_term_care_facility']) ? (strtolower($data['resident_in_long_term_care_facility']) == 'yes' ? TRUE : FALSE) : FALSE);
  $record->cFName = (!empty($data['long_term_care_facility_name']) ? trim($data['long_term_care_facility_name']) : '');
  $record->cFAddress1 = (!empty($data['long_term_care_facility_address']) ? trim($data['long_term_care_facility_address']) : '');
  $record->cFAddress2 = '';
  $record->cFCity = (!empty($data['long_term_care_facility_city']) ? trim($data['long_term_care_facility_city']) : '');
  $record->cFState = '';
  $record->cFZip = '';
  $record->cFPhone = (!empty($data['long_term_care_facility_phone']) ?  $data['long_term_care_facility_phone'] : '');
  
  // 4. Medicaid
  $record->isInMA = (!empty($data['enrolled_in_medicaid_program']) ? (strtolower($data['enrolled_in_medicaid_program']) == 'yes' ? TRUE : FALSE) : FALSE);
  $record->mANbr = (!empty($data['provide_medicaid_number']) ? trim($data['provide_medicaid_number']) : '');
  
  // 5. Spouse work?
  $record->isSW = (!empty($data['you_or_spouse_work']) ? (strtolower($data['you_or_spouse_work']) == 'yes' ? TRUE : FALSE) : FALSE);
  
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
  $record->directBillOptions = $payOption; // TODO: Verify that this is the right one to send to and not paymentType
  
	$selectBestOption = $data['select_option_best_fits_your_needs'];
	$selectSomethingChanged = (!empty($data['something_has_changed']) ? $data['something_has_changed'] : '');
	$selectCoverageChanged = (!empty($data['my_coverage_changed_recently']) ? $data['my_coverage_changed_recently'] : '');
	$sepTxt = 'I may be eligible to apply because of a valid election period such as moving across states, returning to the United States after living out of the country permanently, moving into or out of a Long-Term Care Facility, etc.';
	switch ($selectBestOption) {
		case 'annual_election_period':
			$Select_best_option_which_applies_app = 'I want to make a change during the Annual Open Enrollment period.';
			$I_am_new_to_Medicare_self = FALSE;
      $isAEP = TRUE;
			break;
		case 'new_to_medicare':
			$Select_best_option_which_applies_app = 'I am new to Medicare';
			$I_am_new_to_Medicare_self = TRUE;
      $isAEP = FALSE;
			break;
		case 'something_has_changed':
			$Select_best_option_which_applies_app = ($selectSomethingChanged == 'coverage_changed_recently' ? ($selectCoverageChanged == 'lost_will_lose_coverage_from_employer_union' ? 'I am moving off of employer coverage into Medicare.' : $sepTxt) : $sepTxt);
			$I_am_new_to_Medicare_self = FALSE;
      $isAEP = FALSE;
			break;
		default:
			break;
	}
	
	$date_lost_will_lose_coverage = (!empty($data['date_lost_will_lose_coverage']) ? formatDate($data['date_lost_will_lose_coverage'], $planYear) : '');
	$date_lost_drug_coverage = (!empty($data['date_lost_coverage']) ? formatDate($data['date_lost_coverage'], $planYear) : '');
	switch ($selectCoverageChanged) {
		case 'lost_will_lose_coverage_from_employer_union':
			$I_am_new_to_Medicare_self = FALSE;
			$Attestation_recently_left = FALSE;
			$This_plan_is_a_new_option_for_me_app = FALSE;
			$Date_left_PACE_program_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$Recently_lost_credible_drug_coverage_app = FALSE;
			$Attestation_recently_returned = FALSE;
			$Date_lost_credible_drug_coverage_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Leaving_employer_or_union_coverage_app = TRUE;
			$I_have_both_Medicare_and_Medicaid_or_my_app = FALSE;
			$Date_leaving_employer_app = $date_lost_will_lose_coverage;
			$I_get_extra_help_paying_for_drug_coverag_app = FALSE;
			$Pharmacy_assistance_program_state_app = '';
			$If_yes_Please_include_name_and_state_of_app = '';
			$I_no_longer_qualify_for_help_with_paying_app = FALSE;
			$Date_qualification_ended_for_help_with_p_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = FALSE;
			$Moving_into_Long_Term_care_facility_or_c_app = FALSE;
			$Was_enrolled_in_SNP_but_not_anymore_app = FALSE;
			$Moved_in_or_will_move_into_facility_app = '';
			$Date_disenrolled_in_SNP_app = '';
			break;
		case 'lost_creditable_drug_coverage':
			$I_am_new_to_Medicare_self = FALSE;
			$Attestation_recently_left = FALSE;
			$This_plan_is_a_new_option_for_me_app = FALSE;
			$Date_left_PACE_program_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$Recently_lost_credible_drug_coverage_app = TRUE;
			$Attestation_recently_returned = FALSE;
			$Date_lost_credible_drug_coverage_app = $date_lost_drug_coverage;
			$Date_returned_from_living_abroad_app = '';
			$Leaving_employer_or_union_coverage_app = FALSE;
			$I_have_both_Medicare_and_Medicaid_or_my_app = FALSE;
			$Date_leaving_employer_app = '';
			$I_get_extra_help_paying_for_drug_coverag_app = FALSE;
			$Pharmacy_assistance_program_state_app = '';
			$If_yes_Please_include_name_and_state_of_app = '';
			$I_no_longer_qualify_for_help_with_paying_app = FALSE;
			$Date_qualification_ended_for_help_with_p_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = FALSE;
			$Moving_into_Long_Term_care_facility_or_c_app = FALSE;
			$Was_enrolled_in_SNP_but_not_anymore_app = FALSE;
			$Moved_in_or_will_move_into_facility_app = '';
			$Date_disenrolled_in_SNP_app = '';
			break;
		case 'medicare_contract_ending':
			$Attestation_recently_left = FALSE;
			$Attestation_recently_returned = FALSE;
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = FALSE;
			$I_get_extra_help_paying_for_drug_coverag_app = FALSE;
			$I_have_both_Medicare_and_Medicaid_or_my_app = FALSE;
			$I_no_longer_qualify_for_help_with_paying_app = FALSE;
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = FALSE;
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = FALSE;
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = TRUE;
			$Recently_lost_credible_drug_coverage_app = FALSE;
			$This_plan_is_a_new_option_for_me_app = FALSE;
			$Was_enrolled_in_SNP_but_not_anymore_app = FALSE;
			break;
		default:
			break;
	}
	
	$selectReceiveAssistance = (!empty($data['receive_state_assistance']) ? $data['receive_state_assistance'] : '');
	$date_left_pace_program = (!empty($data['date_left_pace_program']) ? formatDate($data['date_left_pace_program'], $planYear) : '');
	$belong_to_state_assisted_pharmacy_program = (!empty($data['belong_to_state_assisted_pharmacy_program']) ? $data['belong_to_state_assisted_pharmacy_program'] : '');
	$date_no_longer_eligible_for_extra_help = (!empty($data['date_no_longer_eligible_for_extra_help']) ? formatDate($data['date_no_longer_eligible_for_extra_help'], $planYear) : '');
	$date_lost_special_needs_qualifications = (!empty($data['date_lost_special_needs_qualifications']) ? formatDate($data['date_lost_special_needs_qualifications'], $planYear) : '');
	switch ($selectReceiveAssistance) {
		case 'recently_left_pace_program':
			$Attestation_recently_left = TRUE;
			$Attestation_recently_returned = FALSE;
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = $date_left_pace_program;
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = FALSE;
			$I_get_extra_help_paying_for_drug_coverag_app = FALSE;
			$I_have_both_Medicare_and_Medicaid_or_my_app = FALSE;
			$I_no_longer_qualify_for_help_with_paying_app = FALSE;
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = FALSE;
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = FALSE;
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = FALSE;
			$Recently_lost_credible_drug_coverage_app = FALSE;
			$This_plan_is_a_new_option_for_me_app = FALSE;
			$Was_enrolled_in_SNP_but_not_anymore_app = FALSE;
			break;
		case 'belong_to_state_assisted_pharmacy_program':
			$Attestation_recently_left = FALSE;
			$Attestation_recently_returned = FALSE;
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = FALSE;
			$I_get_extra_help_paying_for_drug_coverag_app = FALSE;
			$I_have_both_Medicare_and_Medicaid_or_my_app = FALSE;
			$I_no_longer_qualify_for_help_with_paying_app = FALSE;
			$If_yes_Please_include_name_and_state_of_app = $belong_to_state_assisted_pharmacy_program;
			$Leaving_employer_or_union_coverage_app = FALSE;
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = FALSE;
			$Pharmacy_assistance_program_state_app = TRUE;
			$Plan_is_ending_contract_with_Medicare_or_app = FALSE;
			$Recently_lost_credible_drug_coverage_app = FALSE;
			$This_plan_is_a_new_option_for_me_app = FALSE;
			$Was_enrolled_in_SNP_but_not_anymore_app = FALSE;
			break;
		case 'have_both_medicare_and_medicaid':
			$Attestation_recently_left = FALSE;
			$Attestation_recently_returned = FALSE;
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = FALSE;
			$I_get_extra_help_paying_for_drug_coverag_app = FALSE;
			$I_have_both_Medicare_and_Medicaid_or_my_app = TRUE;
			$I_no_longer_qualify_for_help_with_paying_app = FALSE;
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = FALSE;
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = FALSE;
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = FALSE;
			$Recently_lost_credible_drug_coverage_app = FALSE;
			$This_plan_is_a_new_option_for_me_app = FALSE;
			$Was_enrolled_in_SNP_but_not_anymore_app = FALSE;
			break;
		case 'receive_extra_help_paying_for_drug_coverage':
			$Attestation_recently_left = FALSE;
			$Attestation_recently_returned = FALSE;
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = FALSE;
			$I_get_extra_help_paying_for_drug_coverag_app = TRUE;
			$I_have_both_Medicare_and_Medicaid_or_my_app = FALSE;
			$I_no_longer_qualify_for_help_with_paying_app = FALSE;
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = FALSE;
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = FALSE;
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = FALSE;
			$Recently_lost_credible_drug_coverage_app = FALSE;
			$This_plan_is_a_new_option_for_me_app = FALSE;
			$Was_enrolled_in_SNP_but_not_anymore_app = FALSE;
			break;
		case 'no_longer_eligible_for_extra_help_paying_for_drug_coverage':
			$Attestation_recently_left = FALSE;
			$Attestation_recently_returned = FALSE;
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = $date_no_longer_eligible_for_extra_help;
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = FALSE;
			$I_get_extra_help_paying_for_drug_coverag_app = FALSE;
			$I_have_both_Medicare_and_Medicaid_or_my_app = FALSE;
			$I_no_longer_qualify_for_help_with_paying_app = TRUE;
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = FALSE;
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = FALSE;
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = FALSE;
			$Recently_lost_credible_drug_coverage_app = FALSE;
			$This_plan_is_a_new_option_for_me_app = FALSE;
			$Was_enrolled_in_SNP_but_not_anymore_app = FALSE;
			break;
		case 'lost_special_needs_qualifications':
			$Attestation_recently_left = FALSE;
			$Attestation_recently_returned = FALSE;
			$Date_disenrolled_in_SNP_app = $date_lost_special_needs_qualifications;
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = FALSE;
			$I_get_extra_help_paying_for_drug_coverag_app = FALSE;
			$I_have_both_Medicare_and_Medicaid_or_my_app = FALSE;
			$I_no_longer_qualify_for_help_with_paying_app = FALSE;
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = FALSE;
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = FALSE;
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = FALSE;
			$Recently_lost_credible_drug_coverage_app = FALSE;
			$This_plan_is_a_new_option_for_me_app = FALSE;
			$Was_enrolled_in_SNP_but_not_anymore_app = TRUE;
			break;
		default:
			break;
	}
	
	$selectMovedRecently = (!empty($data['recently_moved_or_will_move_soon']) ? $data['recently_moved_or_will_move_soon'] : '');
	$date_moved_or_will_move = (!empty($data['date_moved_or_will_move']) ? formatDate($data['date_moved_or_will_move'], $planYear) : '');
	switch ($selectMovedRecently) {
		case 'moved_outside_service_area':
			$Attestation_recently_left = FALSE;
			$Attestation_recently_returned = FALSE;
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = $date_moved_or_will_move;
			$I_am_new_to_Medicare_self = FALSE;
			$I_get_extra_help_paying_for_drug_coverag_app = FALSE;
			$I_have_both_Medicare_and_Medicaid_or_my_app = FALSE;
			$I_no_longer_qualify_for_help_with_paying_app = FALSE;
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = FALSE;
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = FALSE;
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = FALSE;
			$Recently_lost_credible_drug_coverage_app = FALSE;
			$This_plan_is_a_new_option_for_me_app = TRUE;
			$Was_enrolled_in_SNP_but_not_anymore_app = FALSE;
			break;
		case 'this_plan_is_new_option':
			$Attestation_recently_left = FALSE;
			$Attestation_recently_returned = FALSE;
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = $date_moved_or_will_move;
			$I_am_new_to_Medicare_self = FALSE;
			$I_get_extra_help_paying_for_drug_coverag_app = FALSE;
			$I_have_both_Medicare_and_Medicaid_or_my_app = FALSE;
			$I_no_longer_qualify_for_help_with_paying_app = FALSE;
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = FALSE;
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = FALSE;
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = FALSE;
			$Recently_lost_credible_drug_coverage_app = FALSE;
			$This_plan_is_a_new_option_for_me_app = TRUE;
			$Was_enrolled_in_SNP_but_not_anymore_app = FALSE;
			break;
		case 'moved_into_or_out_of_long_term_care_facility':
			$Attestation_recently_left = FALSE;
			$Attestation_recently_returned = FALSE;
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = FALSE;
			$I_get_extra_help_paying_for_drug_coverag_app = FALSE;
			$I_have_both_Medicare_and_Medicaid_or_my_app = FALSE;
			$I_no_longer_qualify_for_help_with_paying_app = FALSE;
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = FALSE;
			$Moved_in_or_will_move_into_facility_app = $date_moved_or_will_move;
			$Moving_into_Long_Term_care_facility_or_c_app = TRUE;
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = FALSE;
			$Recently_lost_credible_drug_coverage_app = FALSE;
			$This_plan_is_a_new_option_for_me_app = FALSE;
			$Was_enrolled_in_SNP_but_not_anymore_app = FALSE;
			break;
		case 'returned_to_us':
			$Attestation_recently_left = FALSE;
			$Attestation_recently_returned = TRUE;
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = $date_moved_or_will_move;
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = FALSE;
			$I_get_extra_help_paying_for_drug_coverag_app = FALSE;
			$I_have_both_Medicare_and_Medicaid_or_my_app = FALSE;
			$I_no_longer_qualify_for_help_with_paying_app = FALSE;
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = FALSE;
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = FALSE;
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = FALSE;
			$Recently_lost_credible_drug_coverage_app = FALSE;
			$This_plan_is_a_new_option_for_me_app = FALSE;
			$Was_enrolled_in_SNP_but_not_anymore_app = FALSE;
			break;
		default:
			break;
	}
	
  /* ******************************************************
   * Could not match the below
   ****************************************************** */
	# $objApplicationDetails->Select_best_option_which_applies_app = $Select_best_option_which_applies_app;
	# $objApplicationDetails->This_plan_is_a_new_option_for_me_app = $This_plan_is_a_new_option_for_me_app;

  //enrollment eligibility - 22
  //New to Medicare?
  $record->isNewToMedicare = $I_am_new_to_Medicare_self;
  
  //Moved out of Service Area
  $record->isMovedOutOfCurrPlan = $This_plan_is_a_new_option_for_me_app;
  $record->movedOutOfCurrPlanOn = $Dated_moved_outside_of_service_area_app; //Format CCYY-MM-DD
  
  //Returned to the U.S
  $record->isReturnedToUS = $Attestation_recently_returned;
  $record->returnedToUSOn = $Date_returned_from_living_abroad_app; //Format CCYY-MM-DD
  
  //Both Medicare and Medicaid
  $record->isBothMCAndMA = $I_have_both_Medicare_and_Medicaid_or_my_app;
  
  //Extra Help paying Medicare Prescription Drugs
  $record->isExtraHlpForMC = $I_get_extra_help_paying_for_drug_coverag_app;
  
  //No longer qualify for Extra Help
  $record->isNoLongerHlpForDrugs = $I_no_longer_qualify_for_help_with_paying_app;
  $record->noLongerHlpForDrugsOn = $Date_qualification_ended_for_help_with_p_app; //Format CCYY-MM-DD
  
  //Resides in Long-Term Care Facility
  $record->isMoveToLTCF = $Moving_into_Long_Term_care_facility_or_c_app;
  $record->moveToLTCFOn = $Moved_in_or_will_move_into_facility_app; //Format CCYY-MM-DD
  
  //Left a PACE Program
  $record->isLeftPACE = $Attestation_recently_left;
  $record->leftPACEOn = $Date_left_PACE_program_app; //Format CCYY-MM-DD
  
  //Lost Drug Coverage
  $record->isLostDrugCoverage = $Recently_lost_credible_drug_coverage_app;
  $record->lostDrugCoverageOn = $Date_lost_credible_drug_coverage_app; //Format CCYY-MM-DD
  
  //Leaving Employer or Union Coverage
  $record->isLeavingEmplCov = $Leaving_employer_or_union_coverage_app;
  $record->leavingEmplCovOn = $Date_leaving_employer_app; //Format CCYY-MM-DD
  
  //Belong to Pharmacy Assistance Program
  $record->isPharmaAssist = $Pharmacy_assistance_program_state_app;
  // TODO: Get state added to pharmacy assistance
  #$record->pharmaAssistState = $If_yes_Please_include_name_and_state_of_app;
  
  //Plan Contract ends
  $record->isCurrPlanEndingMC = $Plan_is_ending_contract_with_Medicare_or_app;
  
  //Disenrolled in SNP
  $record->isLostSNP = $Was_enrolled_in_SNP_but_not_anymore_app;
  $record->lostSNPOn = $Date_disenrolled_in_SNP_app; //Format CCYY-MM-DD
  
  //AEP Enrollment
  $record->isAEP = $isAEP;
  
  //Other language format SPANISH, LARGEPRINT
	$spanish = FALSE;
	$largePrint = FALSE;
	if(!empty($data['prefer_information_in_another_language'])) {
		foreach($data['prefer_information_in_another_language'] as $k => $v) {
			if($v == 'spanish') $spanish = TRUE;
			if($v == 'large_print') $largePrint = TRUE;
		}
	}
  $record->isSpanish = $spanish;
  $record->isLargePrint = $largePrint;
  
  $record->isPermToCall = (!empty($data['saveappcontact']) ? TRUE : (!empty($data['saveappcontact_mobile']) ? TRUE : ''));
    
  //Plan ID
  $record->planID = '';
  	
	/* ************************************************************************************
   * Below was not added and will be left commented out until I can find where they fit
   **************************************************************************************
	$objApplicationDetails->RI_Resident_on_your_start_date_app = (strtolower($data['ri_resident_when_plan_starts']) == 'yes' ? 'Yes' : 'No');
	$objApplicationDetails->Are_you_currently_enrolled_in_BlueCHiP_app = (strtolower($data['currently_enrolled_in_bluechip']) == 'yes' ? 'Yes' : 'No');
	$objApplicationDetails->enrolled_in_med1_A_and_B_app = (strtolower($data['enrolled_in_medicare_part_a_and_b']) == 'yes' ? 'Yes' : 'No');
	$objApplicationDetails->learning_more_about_these_plans_app = ''; #$xml->$stepId->learning_more_about_these_plans_app;
	$objApplicationDetails->same_rep_app = ''; #$xml->$stepId->same_rep_app;	
  
  */
  
  //Member Signature Date
  $record->memSignDate = (!empty($data['todays_date']) ? formatDate($data['todays_date'], $planYear) : ''); //Format CCYY-MM-DD
		
	if($data['completing_this_application'] == 'authorized_representative'){
    $record->authFullName = (!empty($data['authorized_rep_name']) ? trim($data['authorized_rep_name']) : '');
    /*$authFName = ''; $authLName = '';
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
    }*/
    $record->authFName = ''; #trim($authFName);
    $record->authLName = ''; #trim($authLName);
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
    /*$bFName = ''; $bLName = '';
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
    }*/
    $record->bFullName = (!empty($data['broker_agent_name']) ? trim($data['broker_agent_name']) : '');
    $record->bFName = ''; #$bFName;
    $record->bLName = ''; #$bLName;
    $record->bIDNum = (!empty($data['broker_agent_id']) ? trim($data['broker_agent_id']) : '');
    // TODO: get broker channel added
    #$record->bChannel = (!empty($data['broker_agent_channel']) ? trim($data['broker_agent_channel']) : '');
    $record->aep = (!empty($data['broker_agent_options']) && trim($data['broker_agent_options']) == 'aep' ? TRUE : FASLE);
    $record->icepMA = (!empty($data['broker_agent_options']) && trim($data['broker_agent_options']) == 'icep_ma_enrollees' ? TRUE : FASLE);
    $record->iepMAPD = (!empty($data['broker_agent_options']) && trim($data['broker_agent_options']) == 'iep_ma_pd_enrollees' ? TRUE : FASLE);
    $record->iepMAPD2 = (!empty($data['broker_agent_options']) && trim($data['broker_agent_options']) == 'iep_ma_pd_enrollees_eligible_second_iep' ? TRUE : FASLE);
    $record->oepi = (!empty($data['broker_agent_options']) && trim($data['broker_agent_options']) == 'oepi' ? TRUE : FASLE);
    $record->sepChronic = (!empty($data['broker_agent_options']) && trim($data['broker_agent_options']) == 'sep_chronic' ? TRUE : FASLE);
    $record->sepFull = (!empty($data['broker_agent_options']) && trim($data['broker_agent_options']) == 'sep_full_duel_eligible' ? TRUE : FASLE);
    $record->sepPartial = (!empty($data['broker_agent_options']) && trim($data['broker_agent_options']) == 'sep_full_partial_eligible' ? TRUE : FASLE);
    $record->othSep = (!empty($data['broker_agent_options']) && trim($data['broker_agent_options']) == 'other_sep_reason' ? TRUE : FASLE);
    // TODO: See if we want to add Other SEP Reason to app and if checking SEP Elgibility box is fine if they enter the date
    $record->othSepReason = '';
    $record->sepEligible = (!empty($data['sep_eligibility_date']) ? TRUE : FASLE);
    $record->sepEligibleDate = (!empty($data['sep_eligibility_date']) ? formatDate(trim($data['sep_eligibility_date']), $planYear) :''); //Format CCYY-MM-DD
    $record->notEligible = (!empty($data['broker_agent_options']) && trim($data['broker_agent_options']) == 'not_eligible' ? TRUE : FASLE);

		$objApplicationDetails->Election_Type = (!empty($data['broker_agent_options']) ? trim($data['broker_agent_options']) :'');
		$objApplicationDetails->SEP_Eligibility_Date = (!empty($data['sep_eligibility_date']) ? formatDate(trim($data['sep_eligibility_date']), $planYear) :'');
	}
	if($sfArray['debug']) $sfArray['obj'] = $record;
  
  //effective date
  // TODO: Even though we do not send, do we want to add it to the broker/agent section?
  $record->effDate = ''; //Format CCYY-MM-DD
} catch (Exception $e) {
	$sfArray['success'] = false;
	if($sfArray['debug']) $sfArray['errorMsg'] = "failed creating obj for record";
	if($sfArray['debug']) $sfArray['obj'] = $record;
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
    if($sfArray['debug']) $sfArray['errorMsg'] = "failed to connect to Salesforce";
    if($sfArray['debug']) $sfArray['loginResponse'] = $response;
    if($sfArray['debug']) $sfArray['status'] = $status;
    return $sfArray;
  }
  if(empty($response['access_token']) || empty($response['instance_url'])) {
    $sfArray['success'] = false;
    if($sfArray['debug']) $sfArray['errorMsg'] = "Failed to get access token or instance url";
    if($sfArray['debug']) $sfArray['loginResponse'] = $response;
    if($sfArray['debug']) $sfArray['status'] = $status;
    return $sfArray;
  }

  /* *******************************************
   * Send data to Salesforce
   ******************************************* */
  $access_token = $response['access_token'];
  $instance_url = $response['instance_url'];
  $url = $instance_url . _SF_MA_APP_URI_;
  $content = json_encode(array('napp' => $record));

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
    if($sfArray['debug']) $sfArray['errorMsg'] = "Failed to access Salesforce to create record";
    if($sfArray['debug']) $sfArray['response'] = $response;
    if($sfArray['debug']) $sfArray['status'] = $status;
    return $sfArray;
  }
  if(strtolower($response['result']) == 'success') {
    $sfArray['success'] = TRUE;
    $sfArray['sfTracking'] = (!empty($response['trackingId']) ? $response['trackingId'] : $record->trackingId);
    //if($sfArray['debug']) $sfArray['errorMsg'] = "Failed to access Salesforce to create record";
    if($sfArray['debug']) $sfArray['response'] = $response;
    if($sfArray['debug']) $sfArray['status'] = $status;
    if(!empty($response['trackingId']) && $response['trackingId'] != $record->trackingId) {
      $setSFID = true;
    }
    if($setSFID) bcbsri_online_application_db_set('bcbsri_online_application_udata', array('auth' => $auth, 'sid'=> json_encode(array('trackingId' => $response['trackingId'], 'acctId' => $response['acctId'], 'oppId' => $response['oppId']))), 'auth');
  	if($record->assumeComplete) {
      $sfArray['confirmation'] = (!empty($response['trackingId']) ? $response['trackingId'] : $record->trackingId);
      bcbsri_online_application_db_set('bcbsri_online_application_udata', array('auth' => $auth, 'app_confirm' => $sfArray['confirmation']), 'auth');
    }
    return $sfArray;
  } else {
    $sfArray['success'] = false;
    if($sfArray['debug']) $sfArray['errorMsg'] = "Failed to create record in Salesforce";
    if($sfArray['debug']) $sfArray['response'] = $response;
    if($sfArray['debug']) $sfArray['status'] = $status;
    return $sfArray;
  }

} catch (Exception $e) {
	$sfArray['success'] = false;
	if($sfArray['debug']) $sfArray['errorMsg'] = "failed to create record in salesforce";
	if($sfArray['debug']) $sfArray['createResponse'] = $response;
	if($sfArray['debug']) $sfArray['message'] = $e->faultstring;
	return $sfArray;
	//die();
}