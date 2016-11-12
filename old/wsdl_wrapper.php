<?php
 /*
class DebuggingInfo{
var $debugLog;//string
}
class LogInfo{
var $category;//LogCategory
var $level;//LogCategoryLevel
}
class DebuggingHeader{
var $categories;//LogInfo
var $debugLevel;//LogType
}
class CallOptions1{
var $client;//string
}
class SessionHeader{
var $sessionId;//string
}
class AllowFieldTruncationHeader{
var $allowFieldTruncation;//boolean
} */
class ApplicantDetails{
var $AreYouCurrentPatient_self;//string
var $BestTimeToCall_self;//string
var $City_self;//string
var $CommunicationPreference_self;//string
var $CurrentBlueCrossId_self;//string
var $DateOfBirth_self;//string
var $EmailAddress_self;//string
var $FirstName_self;//string
var $Height_self;//string
var $HomeAddress1_self;//string
var $HomeAddress2_self;//string
var $LastName_self;//string
var $MailingAddress1_self;//string
var $MailingAddress2_self;//string
var $MailingAddressSameAsHome_self;//string
var $MailingCity_self;//string
var $MailingState_self;//string
var $MailingZipCode_self;//string
var $MaritalStatus_self;//string
var $MiddleInitial_self;//string
var $PhysicianAddress_self;//string
var $PhysicianCity_self;//string
var $PhysicianState_self;//string
var $PhysicianZipCode_self;//string
var $PrimaryCarePhysician_self;//string
var $PrimaryCarePhysicianName_self;//string
var $PrimaryLanguage_self;//string
var $PrimaryPhone_self;//string
var $Race_self;//string
var $SecondaryPhone_self;//string
var $Sex_self;//string
var $SocialSecurityNumber_self;//string
var $State_self;//string
var $Suffix_self;//string
var $Weight_self;//string
var $ZipCode_self;//string
}
class ApplicationDetails{
//var $Address_app;//string
var $Advised_Future_hospitalization_surgery_app;//string
var $AIDS_ARC_or_AIDS_related_conditions_app;//string
var $Alcohol_drug_substance_abuse_addiction_app;//string
var $Any_disease_or_disorder_of_the_skin_app;//string
var $Any_disorder_not_mentioned_above_app;//string
var $Any_type_of_cancer_or_other_tumor_app;//string
var $Any_type_of_hernia_app;//string
var $Applicant_Name_app;//string
var $Apply_for_Access_Blue_app;//string
var $Apply_for_Preferred_Rate_app;//string
var $Are_applicants_still_smoking_app;//string
var $Are_you_a_Rhode_Island_resident_app;//string
var $Are_you_self_employed_app;//string
var $BCBSRI_Confirmation_Number_app;//string
var $Been_in_the_US_for_6_months_or_more_app;//string
//var $City_town_app;//string
var $COBRA_app;//string
var $Currently_taking_any_medications_app;//string
var $Dental_Insurance_Policy_app;//string
var $Dental_Plan_Type_app;//string
var $Diabetes_app;//string
var $Disorder_of_back_neck_spine_or_muscles_app;//string
var $Disorder_of_blood_or_lymph_system_app;//string
var $Disorder_of_heart_or_circulatory_system_app;//string
var $Disorder_of_lungs_or_respiratory_system_app;//string
var $Disorder_of_stomach_intestines_rectum_app;//string
var $Disorder_of_the_blood_vessels_app;//string
var $Disorder_of_the_brain_or_nervous_system_app;//string
var $Disorder_of_the_ENT_mouth_or_sinuses_app;//string
var $Disorder_of_the_kidneys_or_prostate_app;//string
var $Disorder_of_the_reproductive_organs_app;//string
var $Employer_offer_as_benefit_or_mkt_Policy_app;//string
var $Employer_reimbursed_twds_policy_premium_app;//string
var $Ever_been_rejected_ridered_or_rated_app;//string
var $Exam_or_treatment_not_mentioned_above_app;//string
//var $From_app;//string
var $Guardian_Signature_app;//boolean
var $Guardian_Signature_Date_app;//string
var $Guardian_Signature_Name_app;//string
var $Have_applicants_ever_smoked_app;//string
var $If_no_what_was_the_date_of_termination_app;//string
var $If_other_enter_the_name_of_dental_carrier_app;//string
var $If_other_enter_the_name_of_medical_carrier_app;//string
var $If_yes_at_what_age_did_it_onset_app;//string
var $If_yes_what_is_the_insulin_dosage_app;//string
var $If_yes_what_kind_of_hernia_app;//string
var $If_yes_what_type_app;//string
var $If_yes_when_is_the_due_date_app;//string
//var $Illness_or_nature_of_complaint_treatment_app;//string
var $Is_your_dental_coverage_still_in_effect_app;//string
var $Medicaid_app;//string
var $Medical_Insurance_Policy_app;//string
var $Medical_Plan_Type_app;//string
var $Medicare_app;//string
var $Medications_app;//string
var $Mental_nervous_or_emotional_disorders_app;//string
var $My_Signature_app;//boolean
var $My_Signature_Date_app;//string
var $My_Signature_Name_app;//string
var $Name_of_current_prior_dental_carrier_app;//string
//var $Name_of_individual_app;//string
var $Name_of_prior_medical_insurance_carrier_app;//string
var $Physician_consult_for_conditions_app;//string
//var $Physician_name_app;//string
//var $Please_indicate_if_still_receiving_care_app;//string
var $Positive_blood_test_for_HIV_app;//string
var $Requested_dental_coverage_date_app;//string
var $Requested_medical_coverage_date_app;//string
var $Selected_Dental_Plan_app;//string
var $Selected_Medical_Plan_app;//string
var $Spouse_Signature_app;//boolean
var $Spouse_Signature_Date_app;//string
var $Spouse_Signature_Name_app;//string
//var $State_app;//string
var $Sudden_weight_loss_night_sweats_malaise_app;//string
var $tax_exempt_under_IRC_sectn_162_125_106_app;//string
var $Term_date_of_your_medical_coverage_app;//string
//var $To_app;//string
var $What_age_did_applicants_start_smoking_app;//string
var $When_did_applicants_quit_smoking_app;//string
var $You_or_any_dependents_currently_pregnant_app;//string
//var $Zip_code_app;//string
var $broker_agent_name;
var $broker_agent_Id;
var $broker_channel;
}
class DependentDetails{
var $AreYouCurrentPatient_dependent;//string
var $DateOfBirth_dependent;//string
var $FirstName_dependent;//string
var $Height_dependent;//string
var $LastName_dependent;//string
var $MiddleInitial_dependent;//string
var $PhysicianAddress_dependent;//string
var $PhysicianCity_dependent;//string
var $PhysicianState_dependent;//string
var $PhysicianZipCode_dependent;//string
var $PrimaryCarePhysician_dependent;//string
var $PrimaryCarePhysicianName_dependent;//string
var $Sex_dependent;//string
var $SocialSecurityNumber_dependent;//string
var $Suffix_dependent;//string
var $Weight_dependent;//string
}
class SpouseDetails{
var $Address1_spouse;//string
var $Address2_spouse;//string
var $AreYouCurrentPatient_spouse;//string
var $BestTimeToCall_spouse;//string
var $City_spouse;//string
var $CommunicationPreference_spouse;//string
var $DateOfBirth_spouse;//string
var $DoesSpouseUseYourHomeAddress_spouse; //string
var $EmailAddress_spouse;//string
var $FirstName_spouse;//string
var $Height_spouse;//string
var $LastName_spouse;//string
var $MiddleInitial_spouse;//string
var $PhysicianAddress_spouse;//string
var $PhysicianCity_spouse;//string
var $PhysicianState_spouse;//string
var $PhysicianZipCode_spouse;//string
var $PrimaryCarePhysician_spouse;//string
var $PrimaryCarePhysicianName_spouse;//string
var $PrimaryPhone_spouse;//string
var $SecondaryPhone_spouse;//string
var $Sex_spouse;//string
var $SocialSecurityNumber_spouse;//string
var $State_spouse;//string
var $Suffix_spouse;//string
var $Weight_spouse;//string
var $ZipCode_spouse;//string
}
class UnderwritingConsultation{
    var $Address_consult; //string
    var $City_town_consult; //string
    var $From_consult; //date
    var $Illness_or_nature_of_complaint_treatment_consult; //string
    var $Name_of_individual_consult; //string
    var $Physician_name_consult; //string
    var $Please_indicate_if_still_receiving_care_consult; //string
    var $QuestionCode_consult; //string
    var $State_consult; //string
    var $To_consult; //date
    var $Zip_code_consult; //string
}
class SubmitApplication{
var $applicant_details;//ApplicantDetails
var $spouse_details;//SpouseDetails
var $lstdependant_details;//DependentDetails
var $appl_details;//ApplicationDetails
var $lstconsultation_details; // Underwriting consultation
}
class SubmitApplicationResponse{
var $result;//string
}
class BCBSRIApplication 
 {
 var $soapClient;
 
private static $classmap = array('DebuggingInfo'=>'DebuggingInfo'
,'LogInfo'=>'LogInfo'
,'DebuggingHeader'=>'DebuggingHeader'
,'CallOptions'=>'CallOptions'
,'SessionHeader'=>'SessionHeader'
,'AllowFieldTruncationHeader'=>'AllowFieldTruncationHeader'
,'ApplicantDetails'=>'ApplicantDetails'
,'ApplicationDetails'=>'ApplicationDetails'
,'DependentDetails'=>'DependentDetails'
,'SpouseDetails'=>'SpouseDetails'
,'SubmitApplication'=>'SubmitApplication'
,'SubmitApplicationResponse'=>'SubmitApplicationResponse'

);

 function __construct($url='https://na12-api.salesforce.com/services/Soap/class/BCBSRIApplication')
 {
  $this->soapClient = new SoapClient($url,array("classmap"=>self::$classmap,"trace" => true,"exceptions" => true));
 }
 
function SubmitApplication($SubmitApplication)
{

$SubmitApplicationResponse = $this->soapClient->SubmitApplication($SubmitApplication);
return $SubmitApplicationResponse;

}}
?>
