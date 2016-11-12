<?php
if($sfArray['debug']) $sfArray['debug_info']['data']			= $data;

try {
	
	/*$planDetailsArray = explode(',', bcbsri_online_application_get_cookie('planDetails', 'directpay', $objApplicantDetails->PlanYearApplying));
	$planDetails = array();*/
    
  // Plan details for DirectPay
  $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
  $pYear = bcbsri_online_application_get_year($auth);
	$objApplicantDetails->PlanYearApplying		= $pYear;
  $result = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
  foreach($result as $r) {
    $planDetailsArray = json_decode($r->selected, true);
  }
  $planDetails = array();
	foreach($planDetailsArray as $key => $value) {
		#$temp = explode(':', $pD);
		$temp2 = explode('|', $value);
		$planDetails[$key]['planName']		= (!empty($temp2[0]) ? $pYear . ' ' . $temp2[0] : '');
		$planDetails[$key]['planNetwork']	= (!empty($temp2[1]) ? $temp2[1] : '');
		$planDetails[$key]['planCost']		= (!empty($temp2[2]) ? $temp2[2] : '');
		$planDetails[$key]['baseRate']		= (!empty($temp2[3]) ? $temp2[3] : '');
		$planDetails[$key]['planType']		= (!empty($temp2[4]) ? $temp2[4] : '');
		$planDetails[$key]['planYear']		= (!empty($temp2[5]) ? $temp2[5] : '');
		$planDetails[$key]['planMetal']		= (!empty($temp2[6]) ? $temp2[6] : '');
		$planDetails[$key]['planPath']		= (!empty($temp2[7]) ? $temp2[7] : '');
	}
	$quoteCookie = bcbsri_online_application_get_quote($auth);
	$quoteArray = explode('|', $quoteCookie);
	$quote = array();
	foreach($quoteArray as $q) {
		$temp = explode(':', $q);
		$quote[$temp[0]] = $temp[1];
	}
	$coverage = json_decode(bcbsri_online_application_get_covered($auth), true);
	if($sfArray['debug']) {
    $sfArray['debug_info']['coverage']          = $coverage;
    $sfArray['debug_info']['quoteCookie']       = $quoteCookie;
    $sfArray['debug_info']['quoteArray']        = $quoteArray;
    $sfArray['debug_info']['planDetails']       = $planDetails;
    $sfArray['debug_info']['planDetailsCookie'] = $planDetailsArray;
  }
  
	
	/* *******************************************
	* Plan specific effective and requested plans
	******************************************* */
	$objApplicantDetails->SelectedMedicalPlan		= $planDetails['medical']['planName'];
	$objApplicantDetails->SelectedDentalPlan		= $planDetails['dental']['planName'];
	$objApplicantDetails->RequestiveEffectiveDate	= formatDate($quote['eligibilityDate']);
	$objApplicantDetails->Qualified_dental_plan		= (!empty($data['qualified_dental_plan']) ? ucfirst($data['qualified_dental_plan']) : '');
	if($sfArray['debug']) {
    $sfArray['debug_info']['pMedical']  = $planDetails['medical']['planName'];
    $sfArray['debug_info']['pDental']   = $planDetails['dental']['planName'];
  }

	/* *******************************************
	* Applicant Specific questions
	******************************************* */
	if(strtolower($data['mailing_address_different_from_permanent']) == 'no') {
		$mStreet	= $data['permanent_street_address'];
		$mCity		= $data['permanent_address_city'];
		$mState		= $data['permanent_address_state'];
		$mZip		= $data['permanent_address_zip_code'];
	} else {
		$mStreet	= (!empty($data['mailing_street_address']) ? $data['mailing_street_address'] : '');
		$mCity		= (!empty($data['mailing_address_city']) ? $data['mailing_address_city'] : '');
		$mState		= (!empty($data['mailing_address_state']) ? $data['mailing_address_state'] : '');
		$mZip		= (!empty($data['mailing_address_zip']) ? $data['mailing_address_zip'] : '');
	}
	$objApplicantDetails->App_curr_bcbsir_id    = trim($data['bcbsri_id']);
	$objApplicantDetails->App_date_birth				= formatDate(trim($data['date_of_birth']));
	$objApplicantDetails->App_doct_addr					= trim($data['pcp_street_address']);
	$objApplicantDetails->App_doct_city					= trim($data['pcp_city']);
	$objApplicantDetails->App_doct_curr_patient = (strtolower($data['pcp_current_patient']) == 'yes' ? 'Yes' : 'No');
	$objApplicantDetails->App_doct_doct_name    = trim($data['pcp_name']);
	$objApplicantDetails->App_doct_prim_care    = (strtolower($data['applicant_have_pcp']) == 'yes' ? 'Yes' : 'No');
	$objApplicantDetails->App_doct_state        = $data['pcp_state']; #array_search($data['pcp_state'],$stateArr);
	$objApplicantDetails->App_doct_zip          = trim($data['pcp_zip']);
	$objApplicantDetails->App_first_name        = trim($data['first_name']);
	$objApplicantDetails->App_gender            = ucfirst($data['gender']);
	$objApplicantDetails->App_home_addr_abv     = (strtolower($data['mailing_address_different_from_permanent']) == 'no' ? 'Yes' : 'No');
	$objApplicantDetails->App_home_addr1        = trim($data['permanent_street_address']);
	$objApplicantDetails->App_home_addr2        = '';
	$objApplicantDetails->App_home_city         = trim($data['permanent_address_city']);
	$objApplicantDetails->App_home_state        = $data['permanent_address_state']; #array_search(trim($data['permanent_address_state']),$stateArr);
	$objApplicantDetails->App_home_zip          = trim($data['permanent_address_zip_code']);
	$objApplicantDetails->App_last_name         = trim($data['last_name']);
	$objApplicantDetails->App_mail_addr1        = $mStreet;
	$objApplicantDetails->App_mail_addr2        ='';
	$objApplicantDetails->App_mail_city         = $mCity;
	$objApplicantDetails->App_mail_state        = $mState; #array_search($mState, $stateArr);
	$objApplicantDetails->App_mail_zip          = $mZip;
	$objApplicantDetails->App_martial_status    = trim($data['marital_status']);
	$objApplicantDetails->App_middle_initial    = trim($data['middle_initial']);
	$objApplicantDetails->App_phem_primary_ph   = formatPhoneNumber(trim($data['phone_number']));
	$objApplicantDetails->Primary_Phone_Type    = (!empty($data['home_or_cell_phone_number']) ? array_search($data['home_or_cell_phone_number'], $phoneSearch) : '');
	$objApplicantDetails->App_phem_secondary_ph = formatPhoneNumber(trim($data['alternate_phone_number']));
	$objApplicantDetails->Alternate_Phone_Type  = (!empty($data['alt_home_or_cell_phone_number']) ? array_search($data['alt_home_or_cell_phone_number'], $phoneSearch) : '');
	$objApplicantDetails->App_phem_email        = trim($data['email_address']);
	$objApplicantDetails->App_phem_comm_pref    = '';
	$objApplicantDetails->App_phem_besttime     = '';
	$objApplicantDetails->App_primary_lang      = trim($data['primary_language_spoken']);
	$objApplicantDetails->App_race              = (strtolower($data['race']) == 'choose one' ? '' : $data['race']);
	$objApplicantDetails->App_social_sec_no     = formatSSN(trim($data['social_security_number']));
	$objApplicantDetails->App_suffix            = trim($data['suffix']);
	
	/* *******************************************
	* Spouse Specific questions
	******************************************* */
	if(strtolower($data['spouse_mailing_address_different_from_permanent']) == 'no') {
		$smStreet = $data['permanent_street_address'];
		$smCity = $data['permanent_address_city'];
		$smState = $data['permanent_address_state'];
		$smZip = $data['permanent_address_zip_code'];
	} else {
		$smStreet = (!empty($data['spouse_mailing_street_address']) ? $data['spouse_mailing_street_address'] : '');
		$smCity = (!empty($data['spouse_mailing_address_city']) ? $data['spouse_mailing_address_city'] : '');
		$smState = (!empty($data['spouse_mailing_address_state']) ? $data['spouse_mailing_address_state'] : '');
		$smZip = (!empty($data['spouse_mailing_address_zip']) ? $data['spouse_mailing_address_zip'] : '');
	}
	$objApplicantDetails->Civic_partaddr_addr1        = $smStreet;
	$objApplicantDetails->Civic_partaddr_addr2        = '';
	$objApplicantDetails->Civic_partaddr_city         = $smCity;
	$objApplicantDetails->Civic_partaddr_homeaddr     = trim($data['spouse_mailing_address_different_from_permanent']);
	$objApplicantDetails->Civic_partaddr_state        = $smState; #array_search($smState,$stateArr);
	$objApplicantDetails->Civic_partaddr_zip          = $smZip;
	$objApplicantDetails->Civic_partphem_best_time    = '';
	$objApplicantDetails->Civic_partphem_comm_pref    = '';
	$objApplicantDetails->Civic_partphem_email        = trim($data['spouse_email_address']);
	$objApplicantDetails->Civic_partphem_prim_ph      = formatPhoneNumber(trim($data['spouse_phone_number']));
	$objApplicantDetails->Primary_Phone_Type_spouse		= (!empty($data['spouse_home_or_cell_phone_number']) ? array_search($data['spouse_home_or_cell_phone_number'], $phoneSearch) : '');
	$objApplicantDetails->Civic_partphem_second_ph		= formatPhoneNumber(trim($data['spouse_alternate_phone_number']));
	$objApplicantDetails->Alternate_Phone_Type_spouse = (!empty($data['spouse_alt_home_or_cell_phone_number']) ? array_search($data['spouse_alt_home_or_cell_phone_number'], $phoneSearch) : '');
	$objApplicantDetails->Civic_primpart_address      = trim($data['spouse_pcp_street_address']);
	$objApplicantDetails->Civic_primpart_care_doctor  = trim($data['spouse_have_pcp']);
	$objApplicantDetails->Civic_primpart_city         = trim($data['spouse_pcp_city']);
	$objApplicantDetails->Civic_primpart_curr_patient = (!empty($data['spouse_pcp_current_patient']) && strtolower($data['spouse_pcp_current_patient']) == 'yes'? 'Yes' : 'No');
	$objApplicantDetails->Civic_primpart_doctor_name  = trim($data['spouse_pcp_name']);
	$objApplicantDetails->Civic_primpart_state        = $data['spouse_pcp_state']; #array_search($data['spouse_pcp_state'],$stateArr);
	$objApplicantDetails->Civic_primpart_zip          = trim($data['spouse_pcp_zip']);
	$objApplicantDetails->Civic_union_dob             = formatDate(trim($data['spouse_date_of_birth']));
	$objApplicantDetails->Civic_union_first_name      = trim($data['spouse_first_name']);
	$objApplicantDetails->Civic_union_last_name       = trim($data['spouse_last_name']);
	$objApplicantDetails->Civic_union_middle_initial  = trim($data['spouse_middle_initial']);
	$objApplicantDetails->Civic_union_primary_lang    = trim($data['spouse_primary_language_spoken']);
	$objApplicantDetails->Civic_union_suffix          = trim($data['spouse_suffix']);
	$objApplicantDetails->Civic_union_social          = formatSSN(trim($data['spouse_social_security_number']));
	$objApplicantDetails->Spouse_gender               = ucfirst($data['spouse_gender']);
	
	/* *******************************************
	* Dependent Specific questions
	******************************************* */
	$dependentCount = 0;
	foreach($coverage['dependent'] as $dependent){
		if(($dependent['dental'] == 'yes' && !empty($planDetails['dental']['planName'])) || ($dependent['medical'] == 'yes' && !empty($planDetails['medical']['planName']))) {
			$dId = $dependentCount + 1;
			$objDependentDetails[$dependentCount] = new stdclass();
			$objDependentDetails[$dependentCount]->Are_you_a_current_patient__c             = (!empty($data['dependent_pcp_current_patient_' . $dId]) && strtolower($data['dependent_pcp_current_patient_' . $dId]) == 'yes'? 'Yes' : 'No');
			$objDependentDetails[$dependentCount]->Covered_on_Dental_Plan__c                = (strtolower($dependent['dental']) == 'yes' && !empty($planDetails['dental']['planName']) ? 'TRUE'  : 'FALSE');
			$objDependentDetails[$dependentCount]->Covered_on_Medical_Plan__c               = (strtolower($dependent['medical']) == 'yes' && !empty($planDetails['medical']['planName']) ? 'TRUE'  : 'FALSE');
			$objDependentDetails[$dependentCount]->Date_Of_Birth__c                         = formatDate($data['dependent_date_of_birth_' . $dId]);
			$objDependentDetails[$dependentCount]->Do_you_have_a_primary_care_physician__c  = (strtolower($data['dependent_have_pcp_' . $dId]) == 'yes' ? 'Yes' : 'No');
			$objDependentDetails[$dependentCount]->First_Name__c                            = trim($data['dependent_first_name_' . $dId]);
			$objDependentDetails[$dependentCount]->Last_Name__c                             = trim($data['dependent_last_name_' . $dId]);
			$objDependentDetails[$dependentCount]->Middle_Initial__c                        = trim($data['dependent_middle_initial_' . $dId]);
			$objDependentDetails[$dependentCount]->Physician_Address__c                     = trim($data['dependent_pcp_street_address_' . $dId]);
			$objDependentDetails[$dependentCount]->Physician_City_town__c                   = trim($data['dependent_pcp_city_' . $dId]);
			$objDependentDetails[$dependentCount]->Physician_State__c                       = $data['dependent_pcp_state_' . $dId]; #array_search($data['dependent_pcp_state_' . $dId],$stateArr);
			$objDependentDetails[$dependentCount]->Physician_Zip_code__c                    = trim($data['dependent_pcp_zip_' . $dId]);
			$objDependentDetails[$dependentCount]->Primary_care_physician_name__c           = trim($data['dependent_pcp_name_' . $dId]);
			$objDependentDetails[$dependentCount]->Sex__c                                   = (strtolower($data['dependent_relationship_' . $dId]) == 'son' ? 'Male' : 'Female');
			$objDependentDetails[$dependentCount]->Social_Security_Number__c                = trim($data['dependent_social_security_number_' . $dId]);
			$objDependentDetails[$dependentCount]->Suffix__c                                = trim($data['dependent_suffix_' . $dId]);
			$dependentCount++;
		}
    }
	
	/* *******************************************
	* Application questions
	******************************************* */
	$open_enrollment						= '';
	$lost_other_coverage					= '';
	$married_child_adopted					= '';
	$enrolled_not_enrolled_because_of_error	= '';
	$contract_not_followed					= '';
	$moved_to_rhode_island_permanently		= '';
	$lost_eligibility_for_other_coverage	= '';
	$lost_eligibility_for_medicaid_or_chip	= '';
	
	if(is_string($data['select_option_best_fits_your_needs'])) {
		switch($data['select_option_best_fits_your_needs']) {
			case 'open_enrollment':                         $open_enrollment = 'Yes'; break;
			case 'lost_other_coverage':                     $lost_other_coverage = 'Yes'; break;
			case 'married_child_adopted':                   $married_child_adopted = 'Yes'; break;
			case 'enrolled_not_enrolled_because_of_error':  $enrolled_not_enrolled_because_of_error = 'Yes'; break;
			case 'contract_not_followed':                   $contract_not_followed = 'Yes'; break;
			case 'moved_to_rhode_island_permanently':       $moved_to_rhode_island_permanently = 'Yes'; break;
			case 'lost_eligibility_for_other_coverage':     $lost_eligibility_for_other_coverage = 'Yes'; break;
			case 'lost_eligibility_for_medicaid_or_chip':   $lost_eligibility_for_medicaid_or_chip = 'Yes'; break;
		}
	} else {
		for($rEl = 0; $rEl < count($data['select_option_best_fits_your_needs']); $rEl++) {
			if(strtolower($data['select_option_best_fits_your_needs'][$rEl]) == 'open_enrollment')                        $open_enrollment = 'Yes';
			if(strtolower($data['select_option_best_fits_your_needs'][$rEl]) == 'lost_other_coverage')                    $lost_other_coverage = 'Yes';
			if(strtolower($data['select_option_best_fits_your_needs'][$rEl]) == 'married_child_adopted')                  $married_child_adopted = 'Yes';
			if(strtolower($data['select_option_best_fits_your_needs'][$rEl]) == 'enrolled_not_enrolled_because_of_error') $enrolled_not_enrolled_because_of_error = 'Yes';
			if(strtolower($data['select_option_best_fits_your_needs'][$rEl]) == 'contract_not_followed')                  $contract_not_followed = 'Yes';
			if(strtolower($data['select_option_best_fits_your_needs'][$rEl]) == 'moved_to_rhode_island_permanently')      $moved_to_rhode_island_permanently = 'Yes';
			if(strtolower($data['select_option_best_fits_your_needs'][$rEl]) == 'lost_eligibility_for_other_coverage')    $lost_eligibility_for_other_coverage = 'Yes';
			if(strtolower($data['select_option_best_fits_your_needs'][$rEl]) == 'lost_eligibility_for_medicaid_or_chip')  $lost_eligibility_for_medicaid_or_chip = 'Yes';
		}
	}
	
	$objApplicantDetails->AreYouRhodeIsland = (!empty($data['rhode_island_resident']) && strtolower($data['rhode_island_resident']) == 'yes' ? 'Yes' : 'No');
	$objApplicantDetails->Coverage_filling_bankruptcy = $lost_eligibility_for_other_coverage;
	$objApplicantDetails->Contract_another_issuer = $contract_not_followed;
	$objApplicantDetails->Chip_gained_eligiblity = $lost_eligibility_for_medicaid_or_chip;
	$objApplicantDetails->Cobra_app = '';
	$objApplicantDetails->Dental_insurance_Policy_app = '';
	$objApplicantDetails->dental_policy_end = (!empty($data['when_did_dental_coverage_end']) ? formatDate($data['when_did_dental_coverage_end']) : '');
	$objApplicantDetails->Dental_policy_self = (strtolower($data['name_current_dental_insurance_carrier']) == 'choose one' ? '' : $data['name_current_dental_insurance_carrier']);
	$objApplicantDetails->Enrolled_health_benefit_exchange = $enrolled_not_enrolled_because_of_error;
	$objApplicantDetails->Lost_other_coverage = $lost_other_coverage;
	$objApplicantDetails->Medical_Insurance_Policy_app = '';
	$objApplicantDetails->Medicare_app = (!empty($data['eligible_or_enrolled_in_medicare_applicant']) && strtolower($data['eligible_or_enrolled_in_medicare_applicant']) == 'yes' ? 'Yes' : 'No');
	$objApplicantDetails->Medicaid_app = (!empty($data['eligible_or_enrolled_in_medicaid_applicant']) && strtolower($data['eligible_or_enrolled_in_medicaid_applicant']) == 'yes' ? 'Yes' : 'No');
	$objApplicantDetails->Medicare_Spouse_app = (!empty($data['eligible_or_enrolled_in_medicare_spouse']) && strtolower($data['eligible_or_enrolled_in_medicare_spouse']) == 'yes' ? 'Yes' : 'No');
	$objApplicantDetails->Medicaid_Spouse_app = (!empty($data['eligible_or_enrolled_in_medicaid_spouse']) && strtolower($data['eligible_or_enrolled_in_medicaid_spouse']) == 'yes' ? 'Yes' : 'No');
	$objApplicantDetails->Medicare_Dependent_app = (!empty($data['eligible_or_enrolled_in_medicare_dependent']) && strtolower($data['eligible_or_enrolled_in_medicare_dependent']) == 'yes' ? 'Yes' : 'No');
	$objApplicantDetails->Medicaid_Dependent_app = (!empty($data['eligible_or_enrolled_in_medicaid_dependent']) && strtolower($data['eligible_or_enrolled_in_medicaid_dependent']) == 'yes' ? 'Yes' : 'No');
	$objApplicantDetails->Married_child_adopted = $married_child_adopted;
	$objApplicantDetails->Moved_rhode_island = $moved_to_rhode_island_permanently;
	$objApplicantDetails->Medical_comp_self = (strtolower($data['name_current_medical_insurance_carrier']) == 'choose one' ? '' : $data['name_current_medical_insurance_carrier']);
	$objApplicantDetails->Medical_health_insu_end = (!empty($data['when_did_medical_coverage_end']) ? formatDate($data['when_did_medical_coverage_end']) : '');
	$objApplicantDetails->Not_Enrolled_in_Coverage_2014 = (strtolower($data['rhode_island_resident']) == 'yes' ? TRUE : FALSE);
	$objApplicantDetails->Other_indiv_employee = (!empty($data['employer_offer_this_policy']) && strtolower($data['employer_offer_this_policy']) == 'yes' ? 'Yes' : 'No');
	$objApplicantDetails->Other_dental_insur_comp = trim($data['other_dental_insurance_carrier']);
	$objApplicantDetails->Other_med_insur_comp = trim($data['other_medical_insurance_carrier']);
	$objApplicantDetails->Portion_prem_under_policy = (!empty($data['employer_pay_or_reimburse_you']) && strtolower($data['employer_pay_or_reimburse_you']) == 'yes' ? 'Yes' : 'No');
	$objApplicantDetails->Self_employed = '';
	$objApplicantDetails->still_have_medical_insurance_coverage = (!empty($data['medical_coverage_in_effect']) ? (strtolower($data['medical_coverage_in_effect']) == 'yes' ? 'Yes' : 'No') : '');
	$objApplicantDetails->still_have_dental_insurance_coverage = (!empty($data['dental_coverage_in_effect']) ? (strtolower($data['dental_coverage_in_effect']) == 'yes' ? 'Yes' : 'No') : '');
	$objApplicantDetails->Tax_exempt_benefit = (!empty($data['tax_exempt_benefit']) && strtolower($data['tax_exempt_benefit']) == 'yes' ? 'Yes' : 'No');
	$objApplicantDetails->Us_six_months = '';

	/* *******************************************
	* Sign and Submit Page
	******************************************* */
	$objApplicationDetails->Enrolle_Signature			= trim($data['enrollee_signature_name']);
	$objApplicationDetails->Completing_Application		= trim($data['completing_this_application']);
	$objApplicationDetails->Date_app					= formatDate($data['todays_date']);
	/* $objApplicationDetails->Accept_Applicant			= trim($data['']);
	$objApplicationDetails->My_Signature_Name_app		= trim($data['']);
	$objApplicationDetails->My_Signature_Date_app		= trim($data['']);
	
	$objApplicationDetails->Parent_Accept_Applicant		= trim($data['']);
	$objApplicationDetails->Parent_Signature_Name_app	= trim($data['']);
	$objApplicationDetails->Parent_Signature_Date_app	= trim($data['']);
	
	$objApplicationDetails->User_identity				= trim($data['']); */
	$objApplicationDetails->broker_agent_name			= trim($data['broker_agent_name']);
	$objApplicationDetails->broker_agent_Id				= trim($data['broker_agent_id']);
	$objApplicationDetails->broker_channel				= trim($data['broker_agent_channel']);
	$objApplicationDetails->How_old_applicant			= (!empty($data['how_old_applicant']) ? trim($data['how_old_applicant']) : '');

	if($sfArray['debug']) {
    $sfArray['debug_info']['objApplicantDetails']   = $objApplicantDetails;
    $sfArray['debug_info']['objApplicationDetails'] = $objApplicationDetails;
    $sfArray['debug_info']['objDependentDetails']   = $objDependentDetails;
  }
} catch (Exception $e) {
	$sfArray['success'] = false;
	if($sfArray['debug']) {
    $sfArray['debug_info']['errorMsg']              = "failed creating obj for record";
    $sfArray['debug_info']['objApplicantDetails']   = $objApplicantDetails;
    $sfArray['debug_info']['objApplicationDetails'] = $objApplicationDetails;
    $sfArray['debug_info']['objDependentDetails']   = $objDependentDetails;
  }
	return $sfArray;
	//die();
}

try {
	$records = array();
	$records[0] = new stdclass();
	$records[0]->Alternate_Phone_self__c                      = (string)$objApplicantDetails->App_phem_secondary_ph;
	$records[0]->Alternate_Phone_Type_app__c                  = (string)$objApplicantDetails->Alternate_Phone_Type;
	//$records[0]->Apply_for_Access_Blue_app__c               = (string)$objApplicantDetails->Interested_applying_accessblue;
	$records[0]->Are_you_a_current_patient_self__c            = (string)$objApplicantDetails->App_doct_curr_patient;
	$records[0]->Are_you_a_Rhode_Island_resident_app__c       = (string)$objApplicantDetails->AreYouRhodeIsland;
	$records[0]->Are_you_self_employed_app__c                 = (string)$objApplicantDetails->Self_employed;
	$records[0]->Been_in_the_US_for_6_months_or_more_app__c		= (string)$objApplicantDetails->Us_six_months;
	$records[0]->Best_time_to_call_self__c                    = (string)$objApplicantDetails->App_phem_besttime;
	$records[0]->COBRA_app__c                                 = (string)$objApplicantDetails->Cobra_app;
	$records[0]->Communication_Preference_self__c             = (string)$objApplicantDetails->App_phem_comm_pref;
	$records[0]->Contract_with_other_issuer_app__c            = (string)$objApplicantDetails->Contract_another_issuer;
	$records[0]->Current_Blue_Cross_ID_self__c                = (string)$objApplicantDetails->App_curr_bcbsir_id;
	$records[0]->Date_Of_Birth_self__c                        = (string)$objApplicantDetails->App_date_birth;
	$records[0]->Dental_Insurance_Policy_app__c               = (string)$objApplicantDetails->Dental_insurance_Policy_app;
	$records[0]->Did_you_used_Still_Have_Dental_app__c        = (string)$objApplicantDetails->still_have_dental_insurance_coverage;
	$records[0]->Did_you_used_to_have_or_do_you_still__c      = (string)$objApplicantDetails->still_have_medical_insurance_coverage;
	$records[0]->Do_you_have_a_prim_care_physician_self__c    = (string)$objApplicantDetails->App_doct_prim_care;
	$records[0]->Effective_Date_of_Coverage_app__c            = (string)$objApplicantDetails->RequestiveEffectiveDate;
	$records[0]->Employee_Error_app__c                        = (string)$objApplicantDetails->Enrolled_health_benefit_exchange;
	$records[0]->Employer_offer_as_benefit_or_mkt_app__c      = (string)$objApplicantDetails->Other_indiv_employee;
	$records[0]->Employer_reimbursed_twds_premium_app__c      = (string)$objApplicantDetails->Portion_prem_under_policy;
	$records[0]->FirstName_self__c                            = (string)$objApplicantDetails->App_first_name;
	$records[0]->Has_a_qualified_Dental_Plan_app__c           = (string)$objApplicantDetails->Qualified_dental_plan;
	$records[0]->If_other_enter_the_name_of_carrier_D_app__c  = (string)$objApplicantDetails->Other_dental_insur_comp;
	$records[0]->If_other_enter_the_name_of_carrier_M_app__c  = (string)$objApplicantDetails->Other_med_insur_comp;
	$records[0]->Last_Name__c                                 = (string)$objApplicantDetails->App_last_name;
	$records[0]->Loss_due_to_Medicaid_or_state_CHiP_app__c    = (string)$objApplicantDetails->Chip_gained_eligiblity;
	$records[0]->Loss_of_Eligibility_app__c                   = (string)$objApplicantDetails->Coverage_filling_bankruptcy;
	$records[0]->Lost_other_coverage_app__c                   = (string)$objApplicantDetails->Lost_other_coverage;
	$records[0]->Mail_Address_Same_As_Home_Address_self__c    = (string)$objApplicantDetails->App_home_addr_abv;
	$records[0]->Mailing_Address_Line_1_self__c               = (string)$objApplicantDetails->App_mail_addr1;
	$records[0]->Mailing_Address_Line_2_self__c               = (string)$objApplicantDetails->App_mail_addr2;
	$records[0]->Mailing_City_self__c                         = (string)$objApplicantDetails->App_mail_city;
	$records[0]->Mailing_State_self__c                        = (string)$objApplicantDetails->App_mail_state;
	$records[0]->Mailing_Zip_Code_self__c                     = (string)$objApplicantDetails->App_mail_zip;
	$records[0]->Marital_Status_self__c                       = (string)$objApplicantDetails->App_martial_status;
	$records[0]->Married_Had_a_Child_or_Adopted_app__c        = (string)$objApplicantDetails->Married_child_adopted;
	$records[0]->Medicaid_app__c                              = (string)$objApplicantDetails->Medicaid_app;
	$records[0]->Spouse_on_Medicaid_app__c                    = (string)$objApplicantDetails->Medicaid_Spouse_app;
	$records[0]->Spouse_on_Medicare_app__c                    = (string)$objApplicantDetails->Medicare_Spouse_app;
	$records[0]->Dependent_on_Medicaid_app__c                 = (string)$objApplicantDetails->Medicaid_Dependent_app;
	$records[0]->Dependent_on_Medicare_app__c                 = (string)$objApplicantDetails->Medicare_Dependent_app;
	$records[0]->Medical_Insurance_Policy_app__c              = (string)$objApplicantDetails->Medical_Insurance_Policy_app;
	$records[0]->Medicare_app__c                              = (string)$objApplicantDetails->Medicare_app;
	$records[0]->Middle_Initial_self__c                       = (string)$objApplicantDetails->App_middle_initial;
	$records[0]->Moved_to_Rhode_Island_app__c                 = (string)$objApplicantDetails->Moved_rhode_island;
	$records[0]->Name_of_current_prior_dental_carrier_app__c  = (string)$objApplicantDetails->Dental_policy_self;
	$records[0]->Name_of_prior_medical_insurance_carr_app__c  = (string)$objApplicantDetails->Medical_comp_self;
	$records[0]->Not_Enrolled_in_Coverage_2014_app__c         = (string)$objApplicantDetails->Not_Enrolled_in_Coverage_2014;
	$records[0]->PersonEmail_self__c                          = (string)$objApplicantDetails->App_phem_email;
	$records[0]->Phone_self__c                                = (string)$objApplicantDetails->App_phem_primary_ph;
	$records[0]->Physical_Address_Line_1_self__c              = (string)$objApplicantDetails->App_home_addr1;
	$records[0]->Physical_Address_Line_2_self__c              = (string)$objApplicantDetails->App_home_addr2;
	$records[0]->Physical_City_self__c                        = (string)$objApplicantDetails->App_home_city;
	$records[0]->Physical_State_self__c                       = (string)$objApplicantDetails->App_home_state;
	$records[0]->Physical_Zip_Code_self__c                    = (string)$objApplicantDetails->App_home_zip;
	$records[0]->Physician_Address_self__c                    = (string)$objApplicantDetails->App_doct_addr;
	$records[0]->Physician_City_town_self__c                  = (string)$objApplicantDetails->App_doct_city;
	$records[0]->Physician_State_self__c                      = (string)$objApplicantDetails->App_doct_state;
	$records[0]->Physician_Zip_code_self__c                   = (string)$objApplicantDetails->App_doct_zip;
	$records[0]->Plan_Year_Applying_For_app__c                = (string)$objApplicantDetails->PlanYearApplying;
	$records[0]->Primary_care_physician_name_self__c          = (string)$objApplicantDetails->App_doct_doct_name;
	$records[0]->Primary_Language_self__c                     = (string)$objApplicantDetails->App_primary_lang;
	$records[0]->Primary_Phone_Type_app__c                    = (string)$objApplicantDetails->Primary_Phone_Type;
	$records[0]->Race_self__c                                 = (string)$objApplicantDetails->App_race;
	$records[0]->Select_Dental_Plan_app__c                    = (string)$objApplicantDetails->SelectedDentalPlan;
	$records[0]->Select_Medical_Plan_app__c                   = (string)$objApplicantDetails->SelectedMedicalPlan;
	$records[0]->Sex_self__c                                  = (string)$objApplicantDetails->App_gender;
	$records[0]->Social_Security_Number_self__c               = (string)$objApplicantDetails->App_social_sec_no;
	$records[0]->Suffix_self__c                               = (string)$objApplicantDetails->App_suffix;
	$records[0]->tax_exempt_under_IRC_162_125_106_app__c      = (string)$objApplicantDetails->Tax_exempt_benefit;
	$records[0]->Term_date_of_your_dental_coverage_app__c     = (string)$objApplicantDetails->dental_policy_end;
	$records[0]->Term_date_of_your_medical_coverage_app__c    = (string)$objApplicantDetails->Medical_health_insu_end;
	
	if(!empty($coverage['spouse']) && (($coverage['spouse']['medical'] == 'yes' && !empty($planDetails['medical']['planName'])) || ($coverage['spouse']['dental'] == 'yes'  && !empty($planDetails['dental']['planName'])))) {
		$records[0]->Address_Line_1_spouse__c                   = (string)$objApplicantDetails->Civic_partaddr_addr1;
		$records[0]->Address_Line_2_spouse__c                   = (string)$objApplicantDetails->Civic_partaddr_addr2;
		$records[0]->Alternate_Phone_Type_spouse__c             = (string)$objApplicantDetails->Alternate_Phone_Type_spouse;
		$records[0]->Are_you_a_current_patient_spouse__c        = (string)$objApplicantDetails->Civic_primpart_curr_patient;
		$records[0]->Best_time_to_call_spouse__c                = (string)$objApplicantDetails->Civic_partphem_best_time;
		$records[0]->Communication_Preference_spouse__c         = (string)$objApplicantDetails->Civic_partphem_comm_pref;
		$records[0]->Covered_on_Dental_Plan_Spouse__c           = (strtolower($coverage['spouse']['dental']) == 'yes' && !empty($planDetails['dental']['planName']) ? 'TRUE'  : 'FALSE');
		$records[0]->Covered_on_Medical_Plan_Spouse__c          = (strtolower($coverage['spouse']['medical']) == 'yes' && !empty($planDetails['medical']['planName']) ? 'TRUE'  : 'FALSE');
		$records[0]->Date_Of_Birth_spouse__c                    = (string)$objApplicantDetails->Civic_union_dob;
		$records[0]->Do_You_have_a_primary_physician_spouse__c  = (string)$objApplicantDetails->Civic_primpart_care_doctor;
		$records[0]->Email_Address_spouse__c                    = (string)$objApplicantDetails->Civic_partphem_email;
		$records[0]->First_Name_spouse__c                       = (string)$objApplicantDetails->Civic_union_first_name;
		$records[0]->Last_Name_spouse__c                        = (string)$objApplicantDetails->Civic_union_last_name;
		$records[0]->Mailing_City_spouse__c                     = (string)$objApplicantDetails->Civic_partaddr_city;
		$records[0]->Mailing_State_spouse__c                    = (string)$objApplicantDetails->Civic_partaddr_state;
		$records[0]->Mailing_Zip_Code_spouse__c                 = (string)$objApplicantDetails->Civic_partaddr_zip;
		$records[0]->Middle_Initial_spouse__c                   = (string)$objApplicantDetails->Civic_union_middle_initial;
		$records[0]->Physician_Address_spouse__c                = (string)$objApplicantDetails->Civic_primpart_address;
		$records[0]->Physician_City_town_spouse__c              = (string)$objApplicantDetails->Civic_primpart_city;
		$records[0]->Physician_State_spouse__c                  = (string)$objApplicantDetails->Civic_primpart_state;
		$records[0]->Physician_Zip_code_spouse__c               = (string)$objApplicantDetails->Civic_primpart_zip;
		$records[0]->Primary_care_physician_name_spouse__c      = (string)$objApplicantDetails->Civic_primpart_doctor_name;
		$records[0]->Primary_Phone_spouse__c                    = (string)$objApplicantDetails->Civic_partphem_prim_ph;
		$records[0]->Primary_Phone_Type_spouse__c               = (string)$objApplicantDetails->Primary_Phone_Type_spouse;
		$records[0]->Same_as_mailing_address_spouse__c          = (string)$objApplicantDetails->Civic_partaddr_homeaddr;
		$records[0]->Secondary_Phone_spouse__c                  = (string)$objApplicantDetails->Civic_partphem_second_ph;
		$records[0]->Sex_Spouse__c                              = (string)$objApplicantDetails->Spouse_gender;
		$records[0]->Social_Security_Number_spouse__c           = (string)$objApplicantDetails->Civic_union_social;
		$records[0]->Suffix_spouse__c                           = (string)$objApplicantDetails->Civic_union_suffix;
	}
	
	if((string)$objApplicationDetails->Completing_Application == 'broker_agent'){
		$records[0]->ID_of_Individual_Filling_Out_App__c  = 'Broker/Agent';
		$records[0]->Broker_Agent_ID__c                   = (string)$objApplicationDetails->broker_agent_Id;
		$records[0]->Broker_Agent_Name__c                 = (string)$objApplicationDetails->broker_agent_name;
		$records[0]->Channel__c                           = (string)$objApplicationDetails->broker_channel;
		$records[0]->How_Old_is_the_Applicant__c          = (string)$objApplicationDetails->How_old_applicant;
	}
	
	if((string)$objApplicationDetails->Completing_Application == 'applicant17' || (string)$objApplicationDetails->How_old_applicant == '17younger'){
		$records[0]->Guardian_Signature_app__c            = 'yes';
		$records[0]->Guardian_Signature_Name_app__c       = (string)$objApplicationDetails->Enrolle_Signature;
		$records[0]->Guardian_Signature_Date_app__c       = (string)$objApplicationDetails->Date_app;
	}else{
		$records[0]->My_Signature_app__c                  = 'yes';
		$records[0]->My_Signature_Name_app__c             = (string)$objApplicationDetails->Enrolle_Signature;
		$records[0]->My_Signature_Date_app__c             = (string)$objApplicationDetails->Date_app;
	}

	if($sfArray['debug']) $sfArray['debug_info']['record_obj']	= $records[0];
} catch (Exception $e) {
	$sfArray['success'] = false;
	if($sfArray['debug']) {
    $sfArray['errorMsg']    = "failed creating record obj to send to salesforce";
    $sfArray['debug_info']['record_obj']  = $records[0];
    $sfArray['debug_info']['message']     = $e->faultstring;
  }
	return $sfArray;
	//die();
}

try {
  $response = $mySforceConnection->create($records, 'DpDTO__c');
	if($sfArray['debug']) {
    $sfArray['debug_info']['create']['callingCreate']             = $records;
    $sfArray['debug_info']['create']['createResponse']            = $response;
    $sfArray['debug_info']['create']['createLastRequestHeaders']  = $mySforceConnection->getLastRequestHeaders();
    $sfArray['debug_info']['create']['createLastRequest']         = $mySforceConnection->getLastRequest();
    $sfArray['debug_info']['create']['createLastResponseHeaders'] = $mySforceConnection->getLastResponseHeaders();
    $sfArray['debug_info']['create']['createLastResponse']        = $mySforceConnection->getLastResponse();
  }
} catch (Exception $e) {
	$sfArray['success'] = false;
	if($sfArray['debug']) {
    $sfArray['errorMsg']                                = "failed to create record in salesforce";
    $sfArray['debug_info']['create']['createResponse']  = $response[0];
    $sfArray['debug_info']['create']['message']         = $e->faultstring;
  }
	return $sfArray;
	//die();
}

try {
	if(isset($response[0]->id)) $Id = $response[0]->id;
	else $Id = $response->id;
	if(empty($Id)) {
		$sfArray['success'] = false;
		if($sfArray['debug']) $sfArray['errorMsg'] = "failed to get record id from salesforce";
		return $sfArray;
		//die();
	}
} catch(Exception $e) {
	$sfArray['success'] = false;
	if($sfArray['debug']) {
    $sfArray['errorMsg']  = "failed to get record id from salesforce";
    $sfArray['message']   = $e->faultstring;
  }
	return $sfArray;
	//die();
}

try {
	if($dependentCount > 0) {
		for($depts=0; $depts < $dependentCount; $depts++){
			$objDependentDetails[$depts]->ApplicationKey__c = $Id;
		}
	}
	if ($dependentCount > 0) {
		$responseDept = $mySforceConnection->create($objDependentDetails, 'DTODpDependent__c');
		if($sfArray['debug']) {
      $sfArray['dependentRecords']      = $objDependentDetails;
		  $sfArray['createDependentObject'] = $responseDept;
    }
	}
} catch(Exception $e) {
	$sfArray['success'] = false;
	if($sfArray['debug']) {
    $sfArray['errorMsg']  = "failed to set dependent Application Key or create dependent records";
    $sfArray['message']   = $e->faultstring;
  }
	return $sfArray;
	//die();
}

/* ********************************************************
* Update record relationships for dependents and applicant
********************************************************* */
try {
	if(isset($response[0]->id)) $Id = $response[0]->id;
	else $Id = $response->id;
	$records[0]->Id = $Id;
	$response = $mySforceConnection->update($records, 'DpDTO__c');
	if($sfArray['debug']) {
    $sfArray['debug_info']['update']['updateRetriveID']          = $Id;
    $sfArray['debug_info']['update']['updateDependentResponse']  = $response;
  }
} catch (Exception $e) {
	$sfArray['success'] = false;
	if($sfArray['debug']) {
    $sfArray['errorMsg']  = "failed to update dependent and applicant records";
    $sfArray['message']   = $e->faultstring;
  }
	return $sfArray;
	//die();
}

/* ********************************************************
* Retrieve confrimation number from salesforce
********************************************************* */
try {
	$response = $mySforceConnection->retrieve('ConfirmNum_RetCode__c', 'DpDTO__c', $Id);
	if(isset($response[0]->ConfirmNum_RetCode__c) || isset($response->ConfirmNum_RetCode__c)) {
		if(isset($response[0]->ConfirmNum_RetCode__c)) $confirmationNo = $response[0]->ConfirmNum_RetCode__c;
		else $confirmationNo = $response->ConfirmNum_RetCode__c;
	} else {
		if(isset($response[0]->ConfirmationNumber__C)) $confirmationNo = $response[0]->ConfirmationNumber__C;
		else $confirmationNo = $response->ConfirmationNumber__C;
	}
	$sfArray['confirmation'] = $confirmationNo;
	if(!empty($confirmationNo)) $sfArray['success'] = true;
	else {
		$sfArray['success'] = false;
		if($sfArray['debug']) $sfArray['errorMsg'] = "no confirmation number";
	}
	if($sfArray['debug']) {
    $sfArray['debug_info']['record_id']             = $Id;
    $sfArray['debug_info']['confirmation_response'] = $response;
  }
} catch (Exception $e) {
	$sfArray['success'] = false;
	if($sfArray['debug']) {
    $sfArray['errorMsg']  = "failed to retrieve confirmation from salesforce";
    $sfArray['message']   = $e->faultstring;
  }
	return $sfArray;
	//die();
}