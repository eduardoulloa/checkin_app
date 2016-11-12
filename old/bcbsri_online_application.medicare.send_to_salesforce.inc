<?php
try {
	$objApplicationDetails->Year = bcbsri_online_application_get_year();
  $objApplicantDetails->FirstName_self = trim($data['first_name']);
	$objApplicantDetails->MiddleInitial_self = (!empty($data['middle_initial']) ? trim($data['middle_initial']) : '');
	$objApplicantDetails->LastName_self = trim($data['last_name']);
	$objApplicantDetails->Suffix_self = trim($data['suffix']);
	$objApplicantDetails->DateOfBirth_self = formatDate(trim($data['date_of_birth']), $objApplicationDetails->Year);
	$objApplicantDetails->Sex_self = ucfirst(trim($data['gender']));
	if(!empty($data['phone_number'])) {
		$phone = formatPhoneNumber(trim($data['phone_number']));
	} else $phone = '';
	$objApplicantDetails->PrimaryPhone_self = $phone;
	if(!empty(trim($data['alternate_phone_number']))) {
		$altphone = formatPhoneNumber(trim($data['alternate_phone_number']));
	} else $altphone = '';
	$objApplicantDetails->SecondaryPhone_self = $altphone;
	$email = (!empty($data['email_address']) && filter_var(trim($data['email_address']), FILTER_VALIDATE_EMAIL) != false ? filter_var($data['email_address'], FILTER_VALIDATE_EMAIL) : '');
	$objApplicantDetails->EmailAddress_self = $email;
	$objApplicantDetails->PrimaryLanguage_self = (!empty($data['primary_language']) ? trim($data['primary_language']) :'');
	$objApplicantDetails->Primary_Phone_Type = (!empty($data['home_or_cell_phone_number']) ? array_search($data['home_or_cell_phone_number'], $phoneSearch) :'');
	$objApplicantDetails->Alternate_Phone_Type = (!empty($data['alt_home_or_cell_phone_number']) ? array_search($data['alt_home_or_cell_phone_number'], $phoneSearch) :'');
	
	/* Applicant Details */
	$objApplicantDetails->HomeAddress1_self = trim($data['permanent_street_address']);
	$objApplicantDetails->City_self = trim($data['permanent_address_city']);
	$objApplicantDetails->State_self = trim($data['permanent_address_state']); #array_search($data['permanent_address_state'], $stateArr);
	$objApplicantDetails->ZipCode_self = trim($data['permanent_address_zip_code']);
	$objApplicantDetails->MailingAddressSameAsHome_self = ($data['mailing_address_different_from_permanent'] == 'no' ? 'Yes' : 'No');
	if(!empty($data['mailing_street_address'])) {
		if($data['mailing_address_different_from_permanent'] == 'no') $mStreet = $data['permanent_street_address'];
		else $mStreet = $data['mailing_street_address'];
	} else {
		if($data['mailing_address_different_from_permanent'] == 'no') $mStreet = $data['permanent_street_address'];
		else $mStreet = '';
	}
	$objApplicantDetails->MailingAddress1_self = trim($mStreet);
	if(!empty($data['mailing_address_city'])) {
		if($data['mailing_address_different_from_permanent'] == 'no') $mCity = $data['permanent_address_city'];
		else $mCity = $data['mailing_address_city'];
	} else {
		if($data['mailing_address_different_from_permanent'] == 'no') $mCity = $data['permanent_address_city'];
		else $mCity = '';
	}
	$objApplicantDetails->MailingCity_self = trim($mCity);
	$objApplicantDetails->MailingState_self = $data['mailing_address_state']; #array_search($data['mailing_address_state'], $stateArr);
	if(!empty($data['mailing_address_zip'])) {
		if($data['mailing_address_different_from_permanent'] == 'no') $mZip = $data['permanent_address_zip_code'];
		else $mZip = $data['mailing_address_zip'];
	} else {
		if($data['mailing_address_different_from_permanent'] == 'no') $mZip = $data['permanent_address_zip_code'];
		else $mZip = '';
	}
	$objApplicantDetails->MailingZipCode_self = trim($mZip);
	$objApplicantDetails->BillingAddressSameAsMailing_self = ($data['billing_address_different_from_permanent_or_mailing'] == 'no' ? 'Yes' : 'No');
	if(!empty($data['billing_street_address'])) {
		if($data['billing_address_different_from_permanent_or_mailing'] == 'no') $bStreet = $data['permanent_street_address'];
		else $bStreet = $data['billing_street_address'];
	} else {
		if($data['billing_address_different_from_permanent_or_mailing'] == 'no') $bStreet = $data['permanent_street_address'];
		else $bStreet = '';
	}
	$objApplicantDetails->BillingAddress1_self = trim($bStreet);
	if(!empty($data['billing_address_city'])) {
		if($data['billing_address_different_from_permanent_or_mailing'] == 'no') $bCity = $data['permanent_address_city'];
		else $bCity = $data['billing_address_city'];
	} else {
		if($data['billing_address_different_from_permanent_or_mailing'] == 'no') $bCity = $data['permanent_address_city'];
		else $bCity = '';
	}
	$objApplicantDetails->BillingCity_self = trim($bCity);
	$objApplicantDetails->BillingState_self = $data['billing_address_state']; #array_search($data['billing_address_state'], $stateArr);;;
	if(!empty($data['billing_address_zip'])) {
		if($data['billing_address_different_from_permanent_or_mailing'] == 'no') $bZip = $data['permanent_address_zip_code'];
		else $bZip = $data['billing_address_zip'];
	} else {
		if($data['billing_address_different_from_permanent_or_mailing'] == 'no') $bZip = $data['permanent_address_zip_code'];
		else $bZip = '';
	}
	$objApplicantDetails->BillingZipCode_self = trim($bZip);
	
	/* Emergency and Designee Info */
	$objApplicantDetails->EmergencyContactName_self = (!empty($data['emergency_contact_name']) ? trim($data['emergency_contact_name']) : '');
	if(!empty($data['emergency_contact_phone'])) {
		$emerphone = formatPhoneNumber(trim($data['emergency_contact_phone']));
	} else $emerphone = '';
	$objApplicantDetails->EmergencyContactPhone_self = $emerphone;
	$objApplicantDetails->EmergencyContactRelationship_self = (!empty($data['emergency_contact_relationship']) ? trim($data['emergency_contact_relationship']) : '');
	if(!empty($data['emergency_contact_designee'])) {
		if(strtolower($data['emergency_contact_designee']) == 'yes') {
			$dSame = 'Yes';
			$dcName = (!empty($data['emergency_contact_name']) ? trim($data['emergency_contact_name']) : '');
			$dcPhone = $emerphone;
			$dcRel = (!empty($data['emergency_contact_relationship']) ? trim($data['emergency_contact_relationship']) : '');
		} else {
			$dSame = 'No';
			$dcName = '';
			$dcPhone = '';
			$dcRel = '';
		}
	} else {
		$dSame = '';
		$dcName = '';
		$dcPhone = '';
		$dcRel = '';
	}
	$objApplicantDetails->DesigneeSameasEmergency_self = $dSame;
	$objApplicantDetails->DesigneeContactName_self = $dcName;
	$objApplicantDetails->DesigneeContactPhone_self = $dcPhone;
	$objApplicantDetails->DesigneeContactRelationship_self = $dcRel;
	
	/* Medicare Info */
	$medicareName = trim($data['first_name']) . (!empty($data['middle_initial']) ? ' ' . trim($data['middle_initial']) : '') . ' ' . trim($data['last_name']);
	$objApplicantDetails->Medicare_Health_Insurance_Name_self = $medicareName;
	$objApplicantDetails->Medicare_Health_Insurance_Claim_Number_self = formatMedicareNumber($data['medicare_claim_number']);
	$objApplicantDetails->Medicare_Health_Insurance_Sex_self = ucfirst($data['gender']);
	$objApplicantDetails->Hospital_Part_A_Effective_Date_self = formatDate($data['part_a_effective_date'], $objApplicationDetails->Year);
	$objApplicantDetails->Medical_Part_B_Effective_Date_self = formatDate($data['part_b_effective_date'], $objApplicationDetails->Year);

	$paySwitch = (!empty($data['select_premium_payment_option']) ? $data['select_premium_payment_option'] : 'none');
	switch($paySwitch) {
		case 'quarterly_bill':
			$payOption = 'Get a QUARTERLY bill';
			break;
		case 'eft':
			$payOption = 'Electronic funds transfer (EFT) from your bank account each month.';
			break;
		case 'eft':
			$payOption = 'Electronic funds transfer (EFT) from your bank account each month.';
			break;
		case 'automatic_deduction':
			$payOption = 'Automatic deduction from your monthly Social Security or Railroad Retirement Board (RRB) benefit check.';
			break;
		case 'monthly_bill':
		default:
			$payOption = 'Get a MONTHLY bill';
			break;
	}
	$objApplicantDetails->Premium_Payment_Option_self = $payOption;
	
	$spanish = '';
	$largePrint = '';
	if(!empty($data['prefer_information_in_another_language'])) {
		foreach($data['prefer_information_in_another_language'] as $k => $v) {
			if($v == 'spanish') $spanish = 'Spanish';
			if($v == 'large_print') $largePrint = 'Large Print';
		}
	}
	$objApplicantDetails->Request_other_language_self = $spanish;
	$objApplicationDetails->print_option_app = $largePrint;
	
	$objApplicationDetails->RI_Resident_on_your_start_date_app = (strtolower($data['ri_resident_when_plan_starts']) == 'yes' ? 'Yes' : 'No');
	$objApplicationDetails->Are_you_currently_enrolled_in_BlueCHiP_app = (strtolower($data['currently_enrolled_in_bluechip']) == 'yes' ? 'Yes' : 'No');
	$objApplicationDetails->enrolled_in_med1_A_and_B_app = (strtolower($data['enrolled_in_medicare_part_a_and_b']) == 'yes' ? 'Yes' : 'No');

	$objApplicantDetails->Do_you_self = (!empty($data['you_or_spouse_work']) ? ucfirst($data['you_or_spouse_work']) : '');
	$objApplicationDetails->learning_more_about_these_plans_app = ''; #$xml->$stepId->learning_more_about_these_plans_app;
	$objApplicationDetails->same_rep_app = ''; #$xml->$stepId->same_rep_app;
	
	$planArr = array("BlueCHiP for Medicare Standard with Drugs"=>bcbsri_online_application_get_year($auth) . " BlueCHiP for Medicare Standard with Drugs (HMO)",
					 "BlueCHiP for Medicare Plus"=> bcbsri_online_application_get_year($auth) . " BlueCHiP for Medicare Plus (HMO)",
					 "BlueCHiP for Medicare Extra"=> bcbsri_online_application_get_year($auth) . " BlueCHiP for Medicare Extra (HMO-POS)",
					 "BlueCHiP for Medicare Core"=> bcbsri_online_application_get_year($auth) . " BlueCHiP for Medicare Core (HMO)",
					 "BlueCHiP for Medicare Advance"=> bcbsri_online_application_get_year($auth) . " BlueCHiP for Medicare Advance (HMO)",
					 "BlueCHiP for Medicare Preferred"=> bcbsri_online_application_get_year($auth) . " BlueCHiP for Medicare Preferred (HMO-POS)",
					 "BlueCHiP for Medicare Value"=> bcbsri_online_application_get_year($auth) . " BlueCHiP for Medicare Value (HMO-POS)");

	//$pDetails = bcbsri_online_application_get_cookie('planDetails', 'medicare', bcbsri_online_application_get_year());
  $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
  $result = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
  foreach($result as $r) {
    $pDetails = json_decode($r->selected, true);
  }
	if($sfArray['debug']) {
    #$sfArray['pMedicalOriginal'] = $pMedical[0];
    #$sfArray['pDentalOriginal'] = $pDental[0];
    $sfArray['pMedicalOriginal'] = $pDetails['medical'];
    $sfArray['pDentalOriginal'] = $pDetails['dental'];
  }
	#$pDetails = explode(',', $pDetails);
	#$pMedical = explode(':',$pDetails[0]);
	#$pDental = explode(':',$pDetails[1]);
	if($sfArray['debug']) $sfArray['pMedicalSplit'] = $pMedical; #[0];
	if($sfArray['debug']) $sfArray['pDentalSplit'] = $pDental; #[0];
	#$pMedical = explode('|', $pMedical[1]);
	#$pDental = explode('|', $pDental[1]);
	$pMedical = explode('|', $pDetails['medical']);
	$pDental = explode('|', $pDetails['dental']);
	if($sfArray['debug']) $sfArray['pMedical'] = $pMedical[0];
	if($sfArray['debug']) $sfArray['pDental'] = $pDental[0];
	#$objApplicationDetails->Medicare_Plan_Type_app = array_search($pMedical[0],$planArr);
	$objApplicationDetails->Medicare_Plan_Type_app = (!empty($planArr[$pMedical[0]]) ? $planArr[$pMedical[0]] : $pMedical[0]);
	$dentalRider = 0;
	#if(!empty($pDental[0]) && strpos($pDental[0], 'BlueCHiP for Medicare Dental')) $dentalRider = 1;
	if(!empty($pDental[0]) && strtolower($pDental[0]) == strtolower('BlueCHiP for Medicare Dental')) $dentalRider = 1;
	else $dentalRider = 0;
	$objApplicationDetails->Medicare_Dental_Rider_app = $dentalRider;
	
	$objApplicationDetails->Disorder_of_the_kidneys_or_prostate_app = (!empty($data['end_stage_renal_disease']) ? ucfirst($data['end_stage_renal_disease']) : '');
	/* Successfull Kidney transplant question -- need SF field */
	#$objApplicationDetails-> = (!empty($data['successfull_kidney_transplant']) ? ucfirst($data['successfull_kidney_transplant']) : '');
	
	$objApplicantDetails->PCP_Clinic_Health_Center_name_self = (!empty($data['provide_pcp_name']) ? trim($data['provide_pcp_name']) : '');
	$objApplicationDetails->you_have_presciption_drug_coverage_app = (!empty($data['other_prescription_drug_coverage']) ? ucfirst(trim($data['other_prescription_drug_coverage'])) : '');
	$objApplicationDetails->Name_of_the_other_coverage_app = (!empty($data['other_coverage_name']) ? trim($data['other_coverage_name']) : '');
	$objApplicationDetails->ID_for_Coverage_app = (!empty($data['other_coverage_id']) ? trim($data['other_coverage_id']) : '');
	$objApplicationDetails->Group_for_this_Coverage_app = (!empty($data['other_coverage_group_number']) ? trim($data['other_coverage_group_number']) : '');
	$objApplicationDetails->Resident_of_Long_Term_Care_facility_Nur_app = (!empty($data['resident_in_long_term_care_facility']) ? ucfirst(trim($data['resident_in_long_term_care_facility'])) : '');
	$objApplicationDetails->Name_of_Institution_app = (!empty($data['long_term_care_facility_name']) ? trim($data['long_term_care_facility_name']) : '');
	$institutePhoneAddress = (!empty($data['long_term_care_facility_address']) ? $data['long_term_care_facility_address'] : '') . (!empty($data['long_term_care_facility_city']) ? ', ' . $data['long_term_care_facility_city'] : '') . (!empty($data['long_term_care_facility_phone']) ?  ' ' . $data['long_term_care_facility_phone'] : '');
	$objApplicationDetails->Address_and_Phone_number_of_Institution_app = $institutePhoneAddress;
	$objApplicationDetails->Enrolled_in_State_Medicaid_Progr_app = (!empty($data['enrolled_in_medicaid_program']) ? ucfirst($data['enrolled_in_medicaid_program']) : '');
	$objApplicationDetails->Provide_Medicaid_Number_app = (!empty($data['provide_medicaid_number']) ? trim($data['provide_medicaid_number']) : '');

	/* Setting default values for Salesforce fields */
	$Select_best_option_which_applies_app = '';
	$Attestation_recently_left = '';
	$Attestation_recently_returned = '';
	$Date_disenrolled_in_SNP_app = '';
	$Date_leaving_employer_app = '';
	$Date_left_PACE_program_app = '';
	$Date_lost_credible_drug_coverage_app = '';
	$Date_qualification_ended_for_help_with_p_app = '';
	$Date_returned_from_living_abroad_app = '';
	$Dated_moved_outside_of_service_area_app = '';
	$I_am_new_to_Medicare_self = '';
	$I_get_extra_help_paying_for_drug_coverag_app = '';
	$I_have_both_Medicare_and_Medicaid_or_my_app = '';
	$I_no_longer_qualify_for_help_with_paying_app = '';
	$If_yes_Please_include_name_and_state_of_app = '';
	$Leaving_employer_or_union_coverage_app = '';
	$Moved_in_or_will_move_into_facility_app = '';
	$Moving_into_Long_Term_care_facility_or_c_app = '';
	$Pharmacy_assistance_program_state_app = '';
	$Plan_is_ending_contract_with_Medicare_or_app = '';
	$Recently_lost_credible_drug_coverage_app = '';
	$This_plan_is_a_new_option_for_me_app = '';
	$Was_enrolled_in_SNP_but_not_anymore_app = '';
	
	$selectBestOption = $data['select_option_best_fits_your_needs'];
	$selectSomethingChanged = (!empty($data['something_has_changed']) ? $data['something_has_changed'] : '');
	$selectCoverageChanged = (!empty($data['my_coverage_changed_recently']) ? $data['my_coverage_changed_recently'] : '');
	$sepTxt = 'I may be eligible to apply because of a valid election period such as moving across states, returning to the United States after living out of the country permanently, moving into or out of a Long-Term Care Facility, etc.';
	switch ($selectBestOption) {
		case 'annual_election_period':
			$Select_best_option_which_applies_app = 'I want to make a change during the Annual Open Enrollment period.';
			$I_am_new_to_Medicare_self = '';
			break;
		case 'new_to_medicare':
			$Select_best_option_which_applies_app = 'I am new to Medicare';
			$I_am_new_to_Medicare_self = 'Yes';
			break;
		case 'something_has_changed':
			$Select_best_option_which_applies_app = ($selectSomethingChanged == 'coverage_changed_recently' ? ($selectCoverageChanged == 'lost_will_lose_coverage_from_employer_union' ? 'I am moving off of employer coverage into Medicare.' : $sepTxt) : $sepTxt);
			$I_am_new_to_Medicare_self = '';
			break;
		default:
			break;
	}

	/* switch ($selectSomethingChanged) {
		case 'coverage_changed_recently':
			$I_am_new_to_Medicare_self = '';
			$Attestation_recently_left = '';
			$This_plan_is_a_new_option_for_me_app = '';
			$Date_left_PACE_program_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$Recently_lost_credible_drug_coverage_app = '';
			$Attestation_recently_returned = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Leaving_employer_or_union_coverage_app = 'Yes';
			$I_have_both_Medicare_and_Medicaid_or_my_app = '';
			$Date_leaving_employer_app = $date_lost_will_lose_coverage;
			$I_get_extra_help_paying_for_drug_coverag_app = '';
			$Pharmacy_assistance_program_state_app = '';
			$If_yes_Please_include_name_and_state_of_app = '';
			$I_no_longer_qualify_for_help_with_paying_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = '';
			$Was_enrolled_in_SNP_but_not_anymore_app = '';
			$Moved_in_or_will_move_into_facility_app = '';
			$Date_disenrolled_in_SNP_app = '';
			break;
		case 'receive_state_assistance':
			break;
		case 'recently_moved_or_will_move_soon':
			break;
		case 'none':
			break;
		default:
			break;
	} */
	
	$date_lost_will_lose_coverage = (!empty($data['date_lost_will_lose_coverage']) ? formatDate($data['date_lost_will_lose_coverage'], $objApplicationDetails->Year) : '');
	$date_lost_drug_coverage = (!empty($data['date_lost_coverage']) ? formatDate($data['date_lost_coverage'], $objApplicationDetails->Year) : '');
	switch ($selectCoverageChanged) {
		case 'lost_will_lose_coverage_from_employer_union':
			$I_am_new_to_Medicare_self = '';
			$Attestation_recently_left = '';
			$This_plan_is_a_new_option_for_me_app = '';
			$Date_left_PACE_program_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$Recently_lost_credible_drug_coverage_app = '';
			$Attestation_recently_returned = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Leaving_employer_or_union_coverage_app = 'Yes';
			$I_have_both_Medicare_and_Medicaid_or_my_app = '';
			$Date_leaving_employer_app = $date_lost_will_lose_coverage;
			$I_get_extra_help_paying_for_drug_coverag_app = '';
			$Pharmacy_assistance_program_state_app = '';
			$If_yes_Please_include_name_and_state_of_app = '';
			$I_no_longer_qualify_for_help_with_paying_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = '';
			$Was_enrolled_in_SNP_but_not_anymore_app = '';
			$Moved_in_or_will_move_into_facility_app = '';
			$Date_disenrolled_in_SNP_app = '';
			break;
		case 'lost_creditable_drug_coverage':
			$I_am_new_to_Medicare_self = '';
			$Attestation_recently_left = '';
			$This_plan_is_a_new_option_for_me_app = '';
			$Date_left_PACE_program_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$Recently_lost_credible_drug_coverage_app = 'Yes';
			$Attestation_recently_returned = '';
			$Date_lost_credible_drug_coverage_app = $date_lost_drug_coverage;
			$Date_returned_from_living_abroad_app = '';
			$Leaving_employer_or_union_coverage_app = '';
			$I_have_both_Medicare_and_Medicaid_or_my_app = '';
			$Date_leaving_employer_app = '';
			$I_get_extra_help_paying_for_drug_coverag_app = '';
			$Pharmacy_assistance_program_state_app = '';
			$If_yes_Please_include_name_and_state_of_app = '';
			$I_no_longer_qualify_for_help_with_paying_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = '';
			$Was_enrolled_in_SNP_but_not_anymore_app = '';
			$Moved_in_or_will_move_into_facility_app = '';
			$Date_disenrolled_in_SNP_app = '';
			break;
		case 'medicare_contract_ending':
			$Attestation_recently_left = '';
			$Attestation_recently_returned = '';
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = '';
			$I_get_extra_help_paying_for_drug_coverag_app = '';
			$I_have_both_Medicare_and_Medicaid_or_my_app = '';
			$I_no_longer_qualify_for_help_with_paying_app = '';
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = '';
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = '';
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = 'Yes';
			$Recently_lost_credible_drug_coverage_app = '';
			$This_plan_is_a_new_option_for_me_app = '';
			$Was_enrolled_in_SNP_but_not_anymore_app = '';
			break;
		default:
			break;
	}
	
	$selectReceiveAssistance = (!empty($data['receive_state_assistance']) ? $data['receive_state_assistance'] : '');
	$date_left_pace_program = (!empty($data['date_left_pace_program']) ? formatDate($data['date_left_pace_program'], $objApplicationDetails->Year) : '');
	$belong_to_state_assisted_pharmacy_program = (!empty($data['belong_to_state_assisted_pharmacy_program']) ? $data['belong_to_state_assisted_pharmacy_program'] : '');
	$date_no_longer_eligible_for_extra_help = (!empty($data['date_no_longer_eligible_for_extra_help']) ? formatDate($data['date_no_longer_eligible_for_extra_help'], $objApplicationDetails->Year) : '');
	$date_lost_special_needs_qualifications = (!empty($data['date_lost_special_needs_qualifications']) ? formatDate($data['date_lost_special_needs_qualifications'], $objApplicationDetails->Year) : '');
	switch ($selectReceiveAssistance) {
		case 'recently_left_pace_program':
			$Attestation_recently_left = 'Yes';
			$Attestation_recently_returned = '';
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = $date_left_pace_program;
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = '';
			$I_get_extra_help_paying_for_drug_coverag_app = '';
			$I_have_both_Medicare_and_Medicaid_or_my_app = '';
			$I_no_longer_qualify_for_help_with_paying_app = '';
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = '';
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = '';
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = '';
			$Recently_lost_credible_drug_coverage_app = '';
			$This_plan_is_a_new_option_for_me_app = '';
			$Was_enrolled_in_SNP_but_not_anymore_app = '';
			break;
		case 'belong_to_state_assisted_pharmacy_program':
			$Attestation_recently_left = '';
			$Attestation_recently_returned = '';
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = '';
			$I_get_extra_help_paying_for_drug_coverag_app = '';
			$I_have_both_Medicare_and_Medicaid_or_my_app = '';
			$I_no_longer_qualify_for_help_with_paying_app = '';
			$If_yes_Please_include_name_and_state_of_app = $belong_to_state_assisted_pharmacy_program;
			$Leaving_employer_or_union_coverage_app = '';
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = '';
			$Pharmacy_assistance_program_state_app = 'Yes';
			$Plan_is_ending_contract_with_Medicare_or_app = '';
			$Recently_lost_credible_drug_coverage_app = '';
			$This_plan_is_a_new_option_for_me_app = '';
			$Was_enrolled_in_SNP_but_not_anymore_app = '';
			break;
		case 'have_both_medicare_and_medicaid':
			$Attestation_recently_left = '';
			$Attestation_recently_returned = '';
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = '';
			$I_get_extra_help_paying_for_drug_coverag_app = '';
			$I_have_both_Medicare_and_Medicaid_or_my_app = 'Yes';
			$I_no_longer_qualify_for_help_with_paying_app = '';
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = '';
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = '';
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = '';
			$Recently_lost_credible_drug_coverage_app = '';
			$This_plan_is_a_new_option_for_me_app = '';
			$Was_enrolled_in_SNP_but_not_anymore_app = '';
			break;
		case 'receive_extra_help_paying_for_drug_coverage':
			$Attestation_recently_left = '';
			$Attestation_recently_returned = '';
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = '';
			$I_get_extra_help_paying_for_drug_coverag_app = 'Yes';
			$I_have_both_Medicare_and_Medicaid_or_my_app = '';
			$I_no_longer_qualify_for_help_with_paying_app = '';
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = '';
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = '';
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = '';
			$Recently_lost_credible_drug_coverage_app = '';
			$This_plan_is_a_new_option_for_me_app = '';
			$Was_enrolled_in_SNP_but_not_anymore_app = '';
			break;
		case 'no_longer_eligible_for_extra_help_paying_for_drug_coverage':
			$Attestation_recently_left = '';
			$Attestation_recently_returned = '';
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = $date_no_longer_eligible_for_extra_help;
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = '';
			$I_get_extra_help_paying_for_drug_coverag_app = '';
			$I_have_both_Medicare_and_Medicaid_or_my_app = '';
			$I_no_longer_qualify_for_help_with_paying_app = 'Yes';
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = '';
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = '';
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = '';
			$Recently_lost_credible_drug_coverage_app = '';
			$This_plan_is_a_new_option_for_me_app = '';
			$Was_enrolled_in_SNP_but_not_anymore_app = '';
			break;
		case 'lost_special_needs_qualifications':
			$Attestation_recently_left = '';
			$Attestation_recently_returned = '';
			$Date_disenrolled_in_SNP_app = $date_lost_special_needs_qualifications;
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = '';
			$I_get_extra_help_paying_for_drug_coverag_app = '';
			$I_have_both_Medicare_and_Medicaid_or_my_app = '';
			$I_no_longer_qualify_for_help_with_paying_app = '';
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = '';
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = '';
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = '';
			$Recently_lost_credible_drug_coverage_app = '';
			$This_plan_is_a_new_option_for_me_app = '';
			$Was_enrolled_in_SNP_but_not_anymore_app = 'Yes';
			break;
		default:
			break;
	}
	
	$selectMovedRecently = (!empty($data['recently_moved_or_will_move_soon']) ? $data['recently_moved_or_will_move_soon'] : '');
	$date_moved_or_will_move = (!empty($data['date_moved_or_will_move']) ? formatDate($data['date_moved_or_will_move'], $objApplicationDetails->Year) : '');
	switch ($selectMovedRecently) {
		case 'moved_outside_service_area':
			$Attestation_recently_left = '';
			$Attestation_recently_returned = '';
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = $date_moved_or_will_move;
			$I_am_new_to_Medicare_self = '';
			$I_get_extra_help_paying_for_drug_coverag_app = '';
			$I_have_both_Medicare_and_Medicaid_or_my_app = '';
			$I_no_longer_qualify_for_help_with_paying_app = '';
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = '';
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = '';
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = '';
			$Recently_lost_credible_drug_coverage_app = '';
			$This_plan_is_a_new_option_for_me_app = 'Yes';
			$Was_enrolled_in_SNP_but_not_anymore_app = '';
			break;
		case 'this_plan_is_new_option':
			$Attestation_recently_left = '';
			$Attestation_recently_returned = '';
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = $date_moved_or_will_move;
			$I_am_new_to_Medicare_self = '';
			$I_get_extra_help_paying_for_drug_coverag_app = '';
			$I_have_both_Medicare_and_Medicaid_or_my_app = '';
			$I_no_longer_qualify_for_help_with_paying_app = '';
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = '';
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = '';
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = '';
			$Recently_lost_credible_drug_coverage_app = '';
			$This_plan_is_a_new_option_for_me_app = 'Yes';
			$Was_enrolled_in_SNP_but_not_anymore_app = '';
			break;
		case 'moved_into_or_out_of_long_term_care_facility':
			$Attestation_recently_left = '';
			$Attestation_recently_returned = '';
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = '';
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = '';
			$I_get_extra_help_paying_for_drug_coverag_app = '';
			$I_have_both_Medicare_and_Medicaid_or_my_app = '';
			$I_no_longer_qualify_for_help_with_paying_app = '';
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = '';
			$Moved_in_or_will_move_into_facility_app = $date_moved_or_will_move;
			$Moving_into_Long_Term_care_facility_or_c_app = 'Yes';
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = '';
			$Recently_lost_credible_drug_coverage_app = '';
			$This_plan_is_a_new_option_for_me_app = '';
			$Was_enrolled_in_SNP_but_not_anymore_app = '';
			break;
		case 'returned_to_us':
			$Attestation_recently_left = '';
			$Attestation_recently_returned = 'Yes';
			$Date_disenrolled_in_SNP_app = '';
			$Date_leaving_employer_app = '';
			$Date_left_PACE_program_app = '';
			$Date_lost_credible_drug_coverage_app = '';
			$Date_qualification_ended_for_help_with_p_app = '';
			$Date_returned_from_living_abroad_app = $date_moved_or_will_move;
			$Dated_moved_outside_of_service_area_app = '';
			$I_am_new_to_Medicare_self = '';
			$I_get_extra_help_paying_for_drug_coverag_app = '';
			$I_have_both_Medicare_and_Medicaid_or_my_app = '';
			$I_no_longer_qualify_for_help_with_paying_app = '';
			$If_yes_Please_include_name_and_state_of_app = '';
			$Leaving_employer_or_union_coverage_app = '';
			$Moved_in_or_will_move_into_facility_app = '';
			$Moving_into_Long_Term_care_facility_or_c_app = '';
			$Pharmacy_assistance_program_state_app = '';
			$Plan_is_ending_contract_with_Medicare_or_app = '';
			$Recently_lost_credible_drug_coverage_app = '';
			$This_plan_is_a_new_option_for_me_app = '';
			$Was_enrolled_in_SNP_but_not_anymore_app = '';
			break;
		default:
			break;
	}
	
	$objApplicationDetails->Select_best_option_which_applies_app = $Select_best_option_which_applies_app;
	$objApplicationDetails->I_am_new_to_Medicare_app = $I_am_new_to_Medicare_self;
	$objApplicationDetails->Attestation_recently_left = $Attestation_recently_left;
	$objApplicationDetails->This_plan_is_a_new_option_for_me_app = $This_plan_is_a_new_option_for_me_app;
	$objApplicationDetails->Date_left_PACE_program_app = $Date_left_PACE_program_app;
	$objApplicationDetails->Dated_moved_outside_of_service_area_app = $Dated_moved_outside_of_service_area_app;
	$objApplicationDetails->Recently_lost_credible_drug_coverage_app = $Recently_lost_credible_drug_coverage_app;
	$objApplicationDetails->Attestation_recently_returned = $Attestation_recently_returned;
	$objApplicationDetails->Date_lost_credible_drug_coverage_app = $Date_lost_credible_drug_coverage_app;
	$objApplicationDetails->Date_returned_from_living_abroad_app = $Date_returned_from_living_abroad_app;
	$objApplicationDetails->Leaving_employer_or_union_coverage_app = $Leaving_employer_or_union_coverage_app;
	$objApplicationDetails->I_have_both_Medicare_and_Medicaid_or_my_app = $I_have_both_Medicare_and_Medicaid_or_my_app;
	$objApplicationDetails->Date_leaving_employer_app = $Date_leaving_employer_app;
	$objApplicationDetails->I_get_extra_help_paying_for_drug_coverag_app = $I_get_extra_help_paying_for_drug_coverag_app;
	$objApplicationDetails->Pharmacy_assistance_program_state_app = $Pharmacy_assistance_program_state_app;
	$objApplicationDetails->If_yes_Please_include_name_and_state_of_app = $If_yes_Please_include_name_and_state_of_app;
	$objApplicationDetails->I_no_longer_qualify_for_help_with_paying_app = $I_no_longer_qualify_for_help_with_paying_app;
	$objApplicationDetails->Date_qualification_ended_for_help_with_p_app = $Date_qualification_ended_for_help_with_p_app;
	$objApplicationDetails->Plan_is_ending_contract_with_Medicare_or_app = $Plan_is_ending_contract_with_Medicare_or_app;
	$objApplicationDetails->Moving_into_Long_Term_care_facility_or_c_app = $Moving_into_Long_Term_care_facility_or_c_app;
	$objApplicationDetails->Was_enrolled_in_SNP_but_not_anymore_app = $Was_enrolled_in_SNP_but_not_anymore_app;
	$objApplicationDetails->Moved_in_or_will_move_into_facility_app = $Moved_in_or_will_move_into_facility_app;
	$objApplicationDetails->Date_disenrolled_in_SNP_app = $Date_disenrolled_in_SNP_app;
	/* Test comment */
	
	if($data['completing_this_application'] == 'authorized_representative'){
	#if($xml->$stepId->Identity_Applicant == 'Authorized representative'){
		$objApplicationDetails->Guardian_Signature_app = 'Authorized representative';
		$objApplicationDetails->Guardian_Signature_Name_app = (!empty($data['enrollee_signature_name']) ? trim($data['enrollee_signature_name']) : '');
		$objApplicationDetails->Guardian_Signature_Date_app = (!empty($data['todays_date']) ? $data['todays_date'] : '');
		$objApplicationDetails->Authorized_Representative_Name_app = (!empty($data['authorized_rep_name']) ? trim($data['authorized_rep_name']) : '');
		$objApplicationDetails->Authorized_Representative_Address_app = (!empty($data['authorized_rep_address']) ? trim($data['authorized_rep_address']) : '');
		$objApplicationDetails->Authorized_Representative_City_app = (!empty($data['authorized_rep_city']) ? trim($data['authorized_rep_city']) : '');
		$objApplicationDetails->Authorized_Representative_State_app = (!empty($data['authorized_rep_state']) ? trim($data['authorized_rep_state']) : '');
		$objApplicationDetails->Authorized_Representative_Zip_app = (!empty($data['authorized_rep_zip']) ? trim($data['authorized_rep_zip']) : '');
		$objApplicationDetails->Authorized_Representative_Phone_app = (!empty($data['authorized_rep_phone']) ? formatPhoneNumber($data['authorized_rep_phone']) : '');
		$objApplicationDetails->Authorized_Representative_Relationship_app = (!empty($data['relationship_to_enrollee']) ? trim($data['relationship_to_enrollee']) : '');
	}else{
		$objApplicationDetails->My_Signature_app = 'Applicant';
		$objApplicationDetails->My_Signature_Name_app = (!empty($data['enrollee_signature_name']) ? trim($data['enrollee_signature_name']) : '');
		$objApplicationDetails->My_Signature_Date_app = (!empty($data['todays_date']) ? formatDate($data['todays_date'], $objApplicationDetails->Year) : '');
	}
	
	if($data['completing_this_application'] == 'broker_agent_other'){
	#if($xml->$stepId->Identity_Applicant == 'Broker / Agent' || $xml->$stepId->Identity_Applicant == 'Broker / Agent / Other'){
		$objApplicationDetails->broker_agent_name = (!empty($data['broker_agent_name']) ? trim($data['broker_agent_name']) : '');
		$objApplicationDetails->broker_agent_Id = (!empty($data['broker_agent_id']) ? trim($data['broker_agent_id']) : '');
		$objApplicationDetails->broker_channel = (!empty($data['broker_agent_channel']) ? trim($data['broker_agent_channel']) : '');
		$objApplicationDetails->Election_Type = (!empty($data['broker_agent_options']) ? trim($data['broker_agent_options']) :'');
		$objApplicationDetails->SEP_Eligibility_Date = (!empty($data['sep_eligibility_date']) ? formatDate(trim($data['sep_eligibility_date']), $objApplicationDetails->Year) :'');
	}
	if($sfArray['debug']) $sfArray['obj'] = $objApplicationDetails;
} catch (Exception $e) {
	$sfArray['success'] = false;
	if($sfArray['debug']) $sfArray['errorMsg'] = "failed creating obj for record";
	if($sfArray['debug']) $sfArray['obj'] = $objApplicationDetails;
	return $sfArray;
	//die();
}

try {
	$records = array();
	$records[0] = new stdclass();
	$records[0]->Date__c = (string)$objApplicationDetails->Year;
	// New mapping code for create below
	$records[0]->FirstName_self__c =(string)$objApplicantDetails->FirstName_self;
	$records[0]->MiddleInitial_self__c =(string)$objApplicantDetails->MiddleInitial_self;
	$records[0]->LastName_self__c =(string)$objApplicantDetails->LastName_self;
	$records[0]->Suffix_self__c =(string)$objApplicantDetails->Suffix_self;
	$records[0]->DateOfBirth_self__c =(string)$objApplicantDetails->DateOfBirth_self;
	$records[0]->Sex_self__c =(string)$objApplicantDetails->Sex_self;
	$records[0]->PrimaryPhone_self__c =(string)$objApplicantDetails->PrimaryPhone_self;
	$records[0]->SecondaryPhone_self__c =(string)$objApplicantDetails->SecondaryPhone_self;
	$records[0]->EmailAddress_self__c =(string)$objApplicantDetails->EmailAddress_self;
	$records[0]->HomeAddress1_self__c =(string)$objApplicantDetails->HomeAddress1_self;
	$records[0]->City_self__c =(string)$objApplicantDetails->City_self;
	$records[0]->State_self__c =(string)$objApplicantDetails->State_self;
	$records[0]->PrimaryLanguage_self__c  = (string)$objApplicantDetails->PrimaryLanguage_self;
	$records[0]->Primary_Phone_Type__c  = (string)$objApplicantDetails->Primary_Phone_Type;
	$records[0]->Alternate_Phone_Type__c  = (string)$objApplicantDetails->Alternate_Phone_Type;
	
	$records[0]->ZipCode_self__c =(string)$objApplicantDetails->ZipCode_self;
	$records[0]->MailingAddressSameAsHome_self__c =(string)$objApplicantDetails->MailingAddressSameAsHome_self;
	$records[0]->MailingAddress1_self__c =(string)$objApplicantDetails->MailingAddress1_self;
	$records[0]->MailingCity_self__c =(string)$objApplicantDetails->MailingCity_self;
	$records[0]->MailingState_self__c =(string)$objApplicantDetails->MailingState_self;
	$records[0]->MailingZipCode_self__c =(string)$objApplicantDetails->MailingZipCode_self;
	//$records[0]->BillingAddressSameAsMailing__c =(string)$objApplicantDetails->BillingAddressSameAsMailing_self;
	$records[0]->Billing_Address_only_if_different_from__c =(string)$objApplicantDetails->BillingAddress1_self;
	$records[0]->Billing_Address_City__c =(string)$objApplicantDetails->BillingCity_self;
	$records[0]->Billing_Address_State__c =(string)$objApplicantDetails->BillingState_self;
	$records[0]->Billing_Zip_Code__c =(string)$objApplicantDetails->BillingZipCode_self;
	$records[0]->EmergencyContactName_self__c =(string)$objApplicantDetails->EmergencyContactName_self;
	$records[0]->EmergencyContactPhone_self__c =(string)$objApplicantDetails->EmergencyContactPhone_self;
	$records[0]->EmergencyContactRelationship_self__c =(string)$objApplicantDetails->EmergencyContactRelationship_self;
	$records[0]->Is_your_emergency_contact_also_your_desi__c =(string)$objApplicantDetails->DesigneeSameasEmergency_self;
	$records[0]->Name_of_Designee__c =(string)$objApplicantDetails->DesigneeContactName_self;
	$records[0]->Designee_Phone_Number__c =(string)$objApplicantDetails->DesigneeContactPhone_self;
	$records[0]->Designee_Relationship_to_Customer__c =(string)$objApplicantDetails->DesigneeContactRelationship_self;
	$records[0]->Premium_Payment_Option_self__c =(string)$objApplicantDetails->Premium_Payment_Option_self;
	$records[0]->Request_other_language_self__c =(string)$objApplicantDetails->Request_other_language_self;
	$records[0]->Medicare_Health_Insurance_Name_self__c =(string)$objApplicantDetails->Medicare_Health_Insurance_Name_self;
	$records[0]->Medicare_Hlth_Insur_Claim_Num_self__c=(string)$objApplicantDetails->Medicare_Health_Insurance_Claim_Number_self;
	$records[0]->Medicare_Health_Insurance_Sex_self__c =(string)$objApplicantDetails->Medicare_Health_Insurance_Sex_self;
	$records[0]->Hospital_Part_A_Effective_Date_self__c =(string)$objApplicantDetails->Hospital_Part_A_Effective_Date_self;
	$records[0]->Medical_Part_B_Effective_Date_self__c =(string)$objApplicantDetails->Medical_Part_B_Effective_Date_self;
	$records[0]->Do_you_self__c =(string)$objApplicantDetails->Do_you_self;
	$records[0]->PCP_Clinic_Health_Center_name_self__c =(string)$objApplicantDetails->PCP_Clinic_Health_Center_name_self;
	$records[0]->RI_Resident_on_your_start_date_app__c =(string)$objApplicationDetails->RI_Resident_on_your_start_date_app;
	$records[0]->Are_you_crt_nrlled_in_BlueCHiP_app__c=(string)$objApplicationDetails->Are_you_currently_enrolled_in_BlueCHiP_app;
	$records[0]->enrolled_in_med1_A_and_B_app__c =(string)$objApplicationDetails->enrolled_in_med1_A_and_B_app;
	$records[0]->Select_best_option_which_applies_app__c=(string)$objApplicationDetails->Select_best_option_which_applies_app;
	$records[0]->learning_more_about_these_plans_app__c =(string)$objApplicationDetails->learning_more_about_these_plans_app;
	$records[0]->same_rep_app__c =(string)$objApplicationDetails->same_rep_app;
	$records[0]->Medicare_Plan_Type_app__c =(string)$objApplicationDetails->Medicare_Plan_Type_app;
	$records[0]->Essential_Dental_Rider__c =(string)$objApplicationDetails->Medicare_Dental_Rider_app;
	$records[0]->print_option_app__c =(string)$objApplicationDetails->print_option_app;
	$records[0]->I_am_new_to_Medicare_app__c =(string)$objApplicationDetails->I_am_new_to_Medicare_app;
	$records[0]->Attestation_recently_left__c =(string)$objApplicationDetails->Attestation_recently_left;
	$records[0]->This_plan_is_a_new_option_for_me_app__c =(string)$objApplicationDetails->This_plan_is_a_new_option_for_me_app;
	$records[0]->Date_left_PACE_program_app__c =(string)$objApplicationDetails->Date_left_PACE_program_app;
	$records[0]->Dated_moved_outside_of_service_area_app__c =(string)$objApplicationDetails->Dated_moved_outside_of_service_area_app;
	$records[0]->Recently_lost_credible_drug_coverage_app__c =(string)$objApplicationDetails->Recently_lost_credible_drug_coverage_app;
	$records[0]->Attestation_recently_returned__c =(string)$objApplicationDetails->Attestation_recently_returned;
	$records[0]->Date_lost_credible_drug_coverage_app__c =(string)$objApplicationDetails->Date_lost_credible_drug_coverage_app;
	$records[0]->Date_returned_from_living_abroad_app__c =(string)$objApplicationDetails->Date_returned_from_living_abroad_app;
	$records[0]->Leaving_employer_or_union_coverage_app__c =(string)$objApplicationDetails->Leaving_employer_or_union_coverage_app;
	$records[0]->I_hve_bth_Mdcre_and_Mdcd_or_my_app__c=(string)$objApplicationDetails->I_have_both_Medicare_and_Medicaid_or_my_app;
	$records[0]->Date_leaving_employer_app__c =(string)$objApplicationDetails->Date_leaving_employer_app;
	$records[0]->I_get_xtr_help_pay_for_drug_cvrg_app__c=(string)$objApplicationDetails->I_get_extra_help_paying_for_drug_coverag_app;
	$records[0]->Pharmacy_assistance_program_state_app__c =(string)$objApplicationDetails->Pharmacy_assistance_program_state_app;
	$records[0]->If_yes_Pls_ncld_nm_and_st_of_app__c=(string)$objApplicationDetails->If_yes_Please_include_name_and_state_of_app;
	$records[0]->I_no_lngr_qual_for_help_with_pay_app__c=(string)$objApplicationDetails->I_no_longer_qualify_for_help_with_paying_app;
	$records[0]->Date_qual_end_for_hlp_wth_p_app__c =(string)$objApplicationDetails->Date_qualification_ended_for_help_with_p_app;
	$records[0]->Plan_end_cntct_with_Mdicre_or_app__c=(string)$objApplicationDetails->Plan_is_ending_contract_with_Medicare_or_app;
	$records[0]->Moving_into_L_T_C_fac_or_c_app__c=(string)$objApplicationDetails->Moving_into_Long_Term_care_facility_or_c_app;
	$records[0]->Was_nrld_in_SNP_but_not_nymre_app__c=(string)$objApplicationDetails->Was_enrolled_in_SNP_but_not_anymore_app;
	$records[0]->Moved_in_or_will_move_into_facility_app__c =(string)$objApplicationDetails->Moved_in_or_will_move_into_facility_app;
	$records[0]->Date_disenrolled_in_SNP_app__c =(string)$objApplicationDetails->Date_disenrolled_in_SNP_app;
	$records[0]->Disorder_of_the_kidneys_or_prostate_app__c =(string)$objApplicationDetails->Disorder_of_the_kidneys_or_prostate_app;
	$records[0]->you_have_presciption_drug_coverage_app__c =(string)$objApplicationDetails->you_have_presciption_drug_coverage_app;
	$records[0]->Name_of_the_other_coverage_app__c =(string)$objApplicationDetails->Name_of_the_other_coverage_app;
	$records[0]->ID_for_Coverage_app__c =(string)$objApplicationDetails->ID_for_Coverage_app;
	$records[0]->Group_for_this_Coverage_app__c =(string)$objApplicationDetails->Group_for_this_Coverage_app;
	$records[0]->Rsdnt_of_Lng_Trm_Care_fac_Nur_app__c=(string)$objApplicationDetails->Resident_of_Long_Term_Care_facility_Nur_app;
	$records[0]->Name_of_Institution_app__c =(string)$objApplicationDetails->Name_of_Institution_app;
	$records[0]->Address_and_Phone_of_Institution_app__c=(string)$objApplicationDetails->Address_and_Phone_number_of_Institution_app;
	$records[0]->Enrolled_in_State_Medicaid_Progr_app__c =(string)$objApplicationDetails->Enrolled_in_State_Medicaid_Progr_app;
	$records[0]->Provide_Medicaid_Number_app__c =(string)$objApplicationDetails->Provide_Medicaid_Number_app;

	if($data['completing_this_application'] == 'authorized_representative'){
		#$records[0]->Guardian_Signature_app__c = true;
		#$records[0]->Guardian_Signature_Name_app__c = (string)$objApplicationDetails->Guardian_Signature_Name_app;
		#$records[0]->Guardian_Signature_Date_app__c = (string)$objApplicationDetails->Guardian_Signature_Date_app;
		$records[0]->Authorized_Rep_Name__c = (string)$objApplicationDetails->Authorized_Representative_Name_app;
		$records[0]->Authorized_Rep_Address__c = (string)$objApplicationDetails->Authorized_Representative_Address_app;
		$records[0]->Authorized_Rep_City__c = (string)$objApplicationDetails->Authorized_Representative_City_app;
		$records[0]->Authorized_Rep_State__c = (string)$objApplicationDetails->Authorized_Representative_State_app;
		$records[0]->Authorized_Rep_Zip__c = (string)$objApplicationDetails->Authorized_Representative_Zip_app;
		$records[0]->Authorized_Rep_Phone__c = (string)$objApplicationDetails->Authorized_Representative_Phone_app;
		$records[0]->Authorized_Rep_Relationship__c = (string)$objApplicationDetails->Authorized_Representative_Relationship_app;
	}else{
		$records[0]->My_Signature_app__c = true;
		$records[0]->My_Signature_Name_app__c = (string)$objApplicationDetails->My_Signature_Name_app;
		$records[0]->My_Signature_Date_app__c = (string)$objApplicationDetails->My_Signature_Date_app;
	}
	
	if($data['completing_this_application'] == 'broker_agent_other'){
		$records[0]->Broker_Agent_ID__c =(string)$objApplicationDetails->broker_agent_Id;
		$records[0]->Broker_Agent_Name__c =(string)$objApplicationDetails->broker_agent_name;
		$records[0]->Channel__c =(string)$objApplicationDetails->broker_channel;
		$records[0]->Election_Type__c =(string)$objApplicationDetails->Election_Type;
		$records[0]->SEP_Eligibility_Date__c =(string)$objApplicationDetails->SEP_Eligibility_Date;
	}
	if($sfArray['debug']) $sfArray['record_obj'] = $records[0];
} catch (Exception $e) {
	$sfArray['success'] = false;
	if($sfArray['debug']) $sfArray['errorMsg'] = "failed creating record obj to send to salesforce";
	if($sfArray['debug']) $sfArray['record_obj'] = $records[0];
	if($sfArray['debug']) $sfArray['message'] = $e->faultstring;
	return $sfArray;
	//die();
}

try {
	$response = $mySforceConnection->create($records, 'MdDTO__c');
	if($sfArray['debug']) $sfArray['callingCreate'] = $records;
	if($sfArray['debug']) $sfArray['createResponse'] = $response;
	if($sfArray['debug']) $sfArray['createLastRequestHeaders'] = $mySforceConnection->getLastRequestHeaders();
	if($sfArray['debug']) $sfArray['createLastRequest'] = $mySforceConnection->getLastRequest();
	if($sfArray['debug']) $sfArray['createLastResponseHeaders'] = $mySforceConnection->getLastResponseHeaders();
	if($sfArray['debug']) $sfArray['createLastResponse'] = $mySforceConnection->getLastResponse();
} catch (Exception $e) {
	$sfArray['success'] = false;
	if($sfArray['debug']) $sfArray['errorMsg'] = "failed to create record in salesforce";
	if($sfArray['debug']) $sfArray['createResponse'] = $response[0];
	if($sfArray['debug']) $sfArray['message'] = $e->faultstring;
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
	$response = $mySforceConnection->retrieve('ConfirmNum_RetCode__c, ConfirmationNumber__C', 'MdDTO__c', $Id);
	if(isset($response[0]->ConfirmNum_RetCode__c) || isset($response->ConfirmNum_RetCode__c)) {
		if(isset($response[0]->ConfirmNum_RetCode__c)) $confirmationNo = $response[0]->ConfirmNum_RetCode__c;
		else $confirmationNo = $response->ConfirmNum_RetCode__c;
	} else {
		if(isset($response[0]->ConfirmationNumber__C)) $confirmationNo = $response[0]->ConfirmationNumber__C;
		else $confirmationNo = $response->ConfirmationNumber__C;
	}
	$sfArray['confirmation'] = $confirmationNo;
  bcbsri_online_application_db_set('bcbsri_online_application_udata', array('auth' => $auth, 'app_confirm' => $confirmationNo), 'auth');
	#$sfArray['success'] = true;
	if(!empty($confirmationNo)) $sfArray['success'] = true;
	else {
		$sfArray['success'] = false;
		if($sfArray['debug']) $sfArray['errorMsg'] = "no confirmation number";
	}
	if($sfArray['debug']) $sfArray['record_id'] = $Id;
	if($sfArray['debug']) $sfArray['confirmation_response'] = $response;
} catch (Exception $e) {
	$sfArray['success'] = false;
	if($sfArray['debug']) $sfArray['errorMsg'] = "failed to retrieve confirmation from salesforce";
	if($sfArray['debug']) $sfArray['message'] = $e->faultstring;
	return $sfArray;
	//die();
}