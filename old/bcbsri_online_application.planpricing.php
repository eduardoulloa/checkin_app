<?php
function sort_by_age ($a, $b) {
	if((int)$a['age'] > (int)$b['age']) return -1;
	else if((int)$a['age'] < (int)$b['age']) return 1;
	else if((int)$a['age'] == (int)$b['age']) {
		if(!empty($a['number'])) {
			if((int)$a['number'] > (int)$b['number']) return 1;
			else return -1;
		} else return 0;
	} else return 0;
}

function bcbsri_online_application_getprice() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      if(!empty($_REQUEST['plans'])) {
        $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
        if(!bcbsri_online_application_auth_verify($auth)) return array('success' => false, 'message' => 'Failed auth check');
        include('bcbsri_online_application.pricing.modal.php');
        #if(empty($pricingIndexMedical[2016])) return array('success' => false, 'reason' => 'failed to import pricing');
        $covered = bcbsri_online_application_get_covered($auth);
        if($covered != false) {
          $pricing = json_decode($covered, true);
        } else {
          $pricing = array();
          $coverage = bcbsri_online_application_get_quote($auth);
          $coverage = explode('|', $coverage);
          foreach($coverage as $c) {
            $t = explode(':', $c);
            switch($t[0]) {
              case 'memberAge':
                $pricing['applicant']['age'] = (int)$t[1];
                $pricing['applicant']['medical'] = 'yes';
                $pricing['applicant']['dental'] = 'yes';
                break;
              case 'spouseAge':
                $pricing['spouse']['age'] = (int)$t[1];
                $pricing['spouse']['medical'] = 'yes';
                $pricing['spouse']['dental'] = 'yes';
                break;
              case 'covered':
                $pricing['covered'] = $t[1];
                break;
              case 'kidCount':
                $pricing['dependentcount'] = (int)$t[1];
                break;
              case 'kidAges':
                if($t[1] != 'none') { //if(!empty($t[1]) || $t[1] != 'none') {
                  $ka = explode(',', $t[1]);
                  for($ki = 0; $ki < count($ka); $ki++) {
                    $pricing['dependent'][$ki]['age'] = (int)$ka[$ki];
                    $pricing['dependent'][$ki]['number'] = $ki + 1;
                    $pricing['dependent'][$ki]['medical'] = 'yes';
                    $pricing['dependent'][$ki]['dental'] = 'yes';
                  }
                }
                break;
            }
          }
        }
        
        
      
        $plans = json_decode($_REQUEST['plans'], true);
        $planPricing = array();
        $pId = 0;
        $year = $year = (is_numeric($_REQUEST['form_year']) ? intval($_REQUEST['form_year']): bcbsri_online_application_get_year());
        //$planDetails = bcbsri_online_application_get_cookie('planDetails', 'directpay', $year);
          $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
          $result = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
          
          
          foreach($result as $r) {
            $planDetails = json_decode($r->selected, true);
          }
          
           
        if(!empty($planDetails)) {
          #$planDetails = explode(',', $planDetails);
          #$medicalPlan = explode(':', $planDetails[0]);
          $medicalPlan = explode('|', $planDetails['medical']); #medicalPlan[1]);
          #$dentalPlan = explode(':', $planDetails[1]);
          $dentalPlan = explode('|', $planDetails['dental']); #$dentalPlan[1]);
        }
        //return array('medical'=>$medicalPlan, 'dental'=>$dentalPlan, 'planDetails'=>$planDetails);
        foreach($plans as $p) { 
          $planYear = $p['planYear'];
          if(strtolower($p['planName']) == "dental direct standard" || strtolower($p['planName']) == "dental direct plus") $pricingIndexDental[$planYear]['50-59'] = 1.099916;
          //if($p['element'] == 'premium-dental-direct-elite') $pricingIndexDental[$planYear]['50-59'] = 1.099916;
          else $pricingIndexDental[$planYear]['50-59'] = 1.1;
          $planPricing[$pId]['element'] = $p['element'];
          $planPricing[$pId]['total'] = 0;
          $planType = $p['planType'];
          $planPrice = $p['baseRate'];
      
          foreach($pricing as $key => $value) {
            $pAge = '';
            if(!empty($value['age'])) {
              if($planType != 'dental') {
                $pIndex = $pricingIndexMedical[$planYear];
                if($value['age'] < 21) $pAge = 'under21';
                else if($value['age'] >= 65) $pAge = '65+';
                else $pAge = $value['age'];
              } else {
                $pIndex = $pricingIndexDental[$planYear];
                if($value['age'] < 19) $pAge = 'under19';
                else if($value['age'] >= 19 && $value['age'] < 30) $pAge = '19-29';
                else if($value['age'] >= 30 && $value['age'] < 40) $pAge = '30-39';
                else if($value['age'] >= 40 && $value['age'] < 50) $pAge = '40-49';
                else if($value['age'] >= 50 && $value['age'] < 60) $pAge = '50-59';
                else if($value['age'] >= 60) $pAge = '60+';
              }
              if($planType == 'dental' && $pAge == 'under19') $price = 29.87;
              else $price = $pIndex[$pAge] * $planPrice;
              if($value[$planType] == 'yes') {
                $planPricing[$pId]['total'] += round($price, 2);
                $planPricing[$pId][$key]['price'] = round($price, 2);
              }
              $pricing[$key]['price'] = round($price, 2);
              $pricing[$key][$planType . 'Price'] = round($price, 2);
              $planPricing[$pId][$key]['age'] = $pAge;
            }
          }
          
          //return array('pricing'=>$pricing, 'planPricing'=>$planPricing);
      
          $dependentCount = 0;
          if(!empty($pricing['dependent'])) {
            usort($pricing['dependent'], 'sort_by_age');
            foreach($pricing['dependent'] as $key => $value) {
              $pAge = '';
              if(!empty($value['age']) || $value['age'] == 0) {
                $addPremium = true;
                if($value['age'] >= 21 && $value[$planType] == 'yes') $addPremium = true;
                else {
                  if($dependentCount < 3 && $value[$planType] == 'yes') {
                    $addPremium = true;
                    $dependentCount++;
                  } else $addPremium = false;
                }
                if($planType != 'dental') {
                  $pIndex = $pricingIndexMedical[$planYear];
                  if($value['age'] < 21) $pAge = 'under21';
                  else if($value['age'] >= 65) $pAge = '65+';
                  else $pAge = $value['age'];
                } else {
                  $pIndex = $pricingIndexDental[$planYear];
                  if($value['age'] < 19) $pAge = 'under19';
                  else if($value['age'] >= 19 && $value['age'] < 30) $pAge = '19-29';
                  else if($value['age'] >= 30 && $value['age'] < 40) $pAge = '30-39';
                  else if($value['age'] >= 40 && $value['age'] < 50) $pAge = '40-49';
                  else if($value['age'] >= 50 && $value['age'] < 60) $pAge = '50-59';
                  else if($value['age'] >= 60) $pAge = '60+';
                }
                if($planType == 'dental' && $pAge == 'under19') $price = 29.87;
                else $price = $pIndex[$pAge] * $planPrice;
                if($value[$planType] == 'yes' && $addPremium) {
                  $planPricing[$pId]['total'] += round($price, 2);
                  $planPricing[$pId]['dependent'][$key]['price'] = round($price, 2);
                  $pricing['dependent'][$key]['price'] = round($price, 2);
                  $pricing['dependent'][$key][$planType . 'Price'] = round($price, 2);
                } else {
                  if($dependentCount < 3) {
                    $pricing['dependent'][$key]['price'] = round($price, 2);
                    $pricing['dependent'][$key][$planType . 'Price'] = round($price, 2);
                  } else $pricing['dependent'][$key]['price'] = 0;
                  $planPricing[$pId]['dependent'][$key]['price'] = 0;
                }
                $planPricing[$pId]['dependent'][$key]['age'] = $pAge;
              }
            }
          }
          if($planType == 'medical' && !empty($medicalPlan)) {
            if(strtolower($p['planName']) == strtolower($medicalPlan[0])) $medicalPlan[2] = '$' . $planPricing[$pId]['total'];
          }
          if($planType == 'dental' && !empty($dentalPlan)) {
            if(strtolower($p['planName']) == strtolower($dentalPlan[0])) $dentalPlan[2] = '$' . $planPricing[$pId]['total'];
          }
          /*if(strtolower($p['planName']) == strtolower($medicalPlan[0]) || strtolower($p['planName']) == strtolower($dentalPlan[0])) {
            if($planType == 'medical' && !empty($medicalPlan)) $medicalPlan[2] = '$' . $planPricing[$pId]['total'];
            else if(!empty($dentalPlan)) $dentalPlan[2] = '$' . $planPricing[$pId]['total'];
          }*/
          $pId++;
        }
        
        if(!empty($planDetails)) {
          $medicalString = implode('|', $medicalPlan);
          $dentalString = implode('|', $dentalPlan);
          $pdCookie['medical'] = (!empty($medicalPlan) ? $medicalString : '');
          $pdCookie['dental'] = (!empty($dentalPlan) ? $dentalString : '');
          //$pdCookie = 'medical:' . $medicalString . ',dental:' . $dentalString;
          //bcbsri_online_application_set_cookie(array('planDetails', $pdCookie, $year, 'directpay'));
              $record = array();
              $record['selected'] = json_encode($pdCookie);
              $record['auth'] = $auth;
              bcbsri_online_application_db_set('bcbsri_online_application_plans', $record, 'auth');
        }
        bcbsri_online_application_set_covered($auth, json_encode($pricing));
        return array('success' => true, 'pricing' => $planPricing, 'data' => $pricing, 'planDetails' => $planDetails, 'year' => $year);
      } else return array('success' => false, 'reason' => 'No data for price quote');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to get pricing', 'error' => var_dump($e, true));
  }
}

function bcbsri_online_application_editquote() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      if(!empty($_REQUEST['quoteInfo'])) {
        $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
        if(!bcbsri_online_application_auth_verify($auth)) return array('success' => false, 'message' => 'Failed auth check');
        bcbsri_online_application_set_quote($auth, $_REQUEST['quoteInfo']);
        $covered = bcbsri_online_application_get_covered($auth);
        if($covered != false) $covered = json_decode($covered, true);
        $coverage = explode('|', $_REQUEST['quoteInfo']);
        $whosCovered = '';
        foreach($coverage as $c) {
          $t = explode(':', $c);
          switch($t[0]) {
            case 'memberAge':
              $pricing['applicant']['age'] = (int)$t[1];
              $pricing['applicant']['medical'] = (!empty($covered['applicant']['medical']) ? $covered['applicant']['medical'] : 'yes');
              $pricing['applicant']['dental'] = (!empty($covered['applicant']['dental']) ? $covered['applicant']['dental'] : 'yes');
              $pricing['applicant']['price'] = (!empty($covered['applicant']['price']) ? $covered['applicant']['price'] : '');
              break;
            case 'spouseAge':
              if(!empty(trim($t[1])) && (int)$t[1] > 0 && $t[1] != 'none') {
                $pricing['spouse']['age'] = (int)$t[1];
                $pricing['spouse']['medical'] = (!empty($covered['spouse']['medical']) ? $covered['spouse']['medical'] : 'yes');
                $pricing['spouse']['dental'] = (!empty($covered['spouse']['dental']) ? $covered['spouse']['dental'] : 'yes');
                $pricing['spouse']['price'] = (!empty($covered['spouse']['price']) ? $covered['spouse']['price'] : '');
              }
              break;
            case 'covered':
              $pricing['covered'] = $t[1];
              $whosCovered = $t[1];
              break;
            case 'kidCount':
              $pricing['dependentcount'] = (int)$t[1];
              break;
            case 'kidAges':
              if(strpos($t[1],',') !== false) $ka = explode(',', $t[1]);
              else if(!empty($t[1])) $ka[0] = $t[1];
              else $ka = '';
              if($ka[0] != 'none') { //if(!empty($ka) || $ka[0] != 'none') {
                for($ki = 0; $ki < count($ka); $ki++) {
                  $pricing['dependent'][$ki]['age'] = (int)$ka[$ki];
                  $pricing['dependent'][$ki]['number'] = $ki + 1;
                  if((int)$ka[$ki] < 26) $pricing['dependent'][$ki]['medical'] = (!empty($covered['dependent'][$ki]['medical']) ? $covered['dependent'][$ki]['medical'] : 'yes');
                  else $pricing['dependent'][$ki]['medical'] = 'no';
                  if((int)$ka[$ki] < 26) $pricing['dependent'][$ki]['dental'] = (!empty($covered['dependent'][$ki]['dental']) ? $covered['dependent'][$ki]['dental'] : 'yes');
                  else $pricing['dependent'][$ki]['dental'] = 'no';
                  $pricing['dependent'][$ki]['price'] = (!empty($covered['dependent'][$ki]['price']) ? $covered['dependent'][$ki]['price'] : '');
                }
              }
              break;
          }
          bcbsri_online_application_set_covered($auth, json_encode($pricing));
        }
        //$cPlanDetails = bcbsri_online_application_get_cookie(array($name, 'planDetails', 2016, 'directpay'));
        $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
        $result = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
        foreach($result as $r) {
          $cPlanDetails = json_decode($r->selected, true);
        }
        return array('success' => true, 'data' => $cPlanDetails);
      } else return array('success' => false, 'reason' => 'No data for quote');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to edit quote', 'error' => var_dump($e, true));
  }
}

function bcbsri_online_application_getquote() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      if(!bcbsri_online_application_auth_verify($auth)) return array('success' => false, 'message' => 'Failed auth check');
      $retData = bcbsri_online_application_get_quote($auth);
      if($retData != false) return array('success' => true, 'quote' => $retData);
      else return array('success' => false, 'reason' => 'No data for quote');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to get quote', 'error' => var_dump($e, true));
  }
}

function bcbsri_online_application_changecoverage() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      if(!bcbsri_online_application_auth_verify($auth)) return array('success' => false, 'message' => 'Failed auth check');
      $covered = bcbsri_online_application_get_covered($auth);
      if($covered != false && !empty($_REQUEST['person']) && !empty($_REQUEST['planType']) && !empty($_REQUEST['covered'])) {
        $covered = json_decode($covered, true);
        if(strpos($_REQUEST['person'],'dependent') !== false) {
          $d = explode('-', $_REQUEST['person']);
          $id = (int)$d[1]; # - 1;
          if($covered['dependent'][$id]['age'] < 27) $covered['dependent'][$id][$_REQUEST['planType']] = $_REQUEST['covered'];
        }else $covered[$_REQUEST['person']][$_REQUEST['planType']] = $_REQUEST['covered'];
        bcbsri_online_application_set_covered($auth, json_encode($covered));
        return array('success' => true, 'data' => $covered);
      } else return array('success' => false, 'reason' => 'incorrect data for coverage');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to change coverage', 'error' => var_dump($e, true));
  }
}

function bcbsri_online_application_getcoverage() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');  
      $covered = json_decode(bcbsri_online_application_get_covered($auth), true);
      if($covered != false) return array('success' => true, 'covered' => $covered);
      else return array('success' => false, 'reason' => 'failed to get covered');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to get coverage', 'error' => var_dump($e, true));
  }
}

function bcbsri_online_application_setaliasyear() {
  try {
    drupal_add_http_header(
      'Cache-Control', 
      'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
      FALSE
    );
    if($_SERVER['REQUEST_METHOD'] == 'POST') {
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      if(!bcbsri_online_application_auth_verify($auth)) return array('success' => false, 'message' => 'Failed auth check');
      if(!empty($_REQUEST['plan_type']) && !empty($_REQUEST['plan_year'])) {
        $sessionCookie = session_get_cookie_params();
        bcbsri_online_application_set_alias($auth, $_REQUEST['plan_type']);
        bcbsri_online_application_set_year($auth, $_REQUEST['plan_year']);
        return array('success' => true);
      } else return array('success' => false, 'reason' => 'not enough data');
    } else return array('success' => false, 'message' => 'No data sent');
  } catch (Exception $e) {
    return array('success' => false, 'message' => 'Failed to set plan alias and year', 'error' => var_dump($e, true));
  }
}