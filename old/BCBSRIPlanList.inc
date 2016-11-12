<?php 
class BCBSRIPlanList {
	protected $covered = array();
	protected $medical = array();
	protected $dental = array();
	protected $plansSelected = array();
	protected $personas = array();
	protected $metal = array();
	protected $plans = array();
	protected $pulledCookie;
	protected $json;
	protected $jsonFile;
	protected $sort;
	protected $sortBy;
	protected $year;
	
	public function __construct($data = "/shop-for-plan/2016/json", $app = 'directpay', $year = 2016, $sort = 'default') {
		$this->jsonFile = $data;
		$ch = curl_init($this->jsonFile);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		if(($this->json = curl_exec($ch)) !== false ) {
			$this->json = json_decode($this->json, true);
			if($sort != 'default' && $sort != 'all' && strpos($sort,'persona') === false) {
				$this->sort = explode('|', $sort);
				$fSort = array();
				foreach($this->sort as $s) {
					$t = explode(':', $s);
					if(!empty($t[1])) array_push($fSort, $s);
				}
				$this->sort = $fSort;
			} else {
				if(strpos($sort,'persona') !== false) $this->sort = explode('-', $sort);
				else $this->sort = $sort;
			}
			//$this->sort = $sort;
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
			if(isset($_COOKIE['bcbsri_' . $year . '_' . $app]['planDetails'])) {
        //$cookie = urldecode($_COOKIE['bcbsri_' . $year . '_' . $app]['planDetails']);
				$result = bcbsri_online_application_db_get('bcbsri_online_application_plans', array('selected'), $auth, 'auth');
        foreach($result as $r) {
          $cookie = $r->selected;
        }
        $this->pulledCookie = $cookie;
				$cookie = explode(',', $cookie);
				$mCookie = explode(':', $cookie[0]);
				$mCookie = (!empty($mCookie[1]) ? explode('|', $mCookie[1]) : 'none');
				$dCookie = explode(':', $cookie[1]);
				$dCookie = (!empty($dCookie[1]) ? explode('|', $dCookie[1]) : 'none');
				$this->planSelected['medical'] = (!empty($mCookie[0]) ? $mCookie[0] : $mCookie);
				$this->planSelected['dental'] = (!empty($dCookie[0]) ? $dCookie[0] : $dCookie);
			}
			$this->createArrays();
			
      $result = bcbsri_online_application_db_get('bcbsri_online_application_udata', array('covered', 'quote'), $auth, 'auth');
      foreach($result as $r) {
        $covered = $r->covered;
        $quote = $r->quote;
      }
			#if(isset($_COOKIE['bcbsri_covered'])) {
      if(!empty($covered)) {
				$this->covered = json_decode($covered, true); #$_COOKIE['bcbsri_covered'], true);
			} else {
				if(empty($quote))  $this->covered = false; #(!isset($_COOKIE['bcbsri_quote'])) $this->covered = false;
				else {
					$coverage = $quote;
					$coverage = explode('|', $coverage);
					foreach($coverage as $c) {
						$t = explode(':', $c);
						switch($t[0]) {
							case 'memberAge':
								$this->covered['applicant']['age'] = (int)$t[1];
								$this->covered['applicant']['medical'] = 'yes';
								$this->covered['applicant']['dental'] = 'yes';
								break;
							case 'spouseAge':
								$this->covered['spouse']['age'] = (int)$t[1];
								$this->covered['spouse']['medical'] = 'yes';
								$this->covered['spouse']['dental'] = 'yes';
								break;
							case 'covered':
								$this->covered['covered'] = $t[1];
								break;
							case 'kidCount':
								$this->covered['dependentcount'] = (int)$t[1];
								break;
							case 'kidAges':
								if($t[1] != 'none') { //if(!empty($t[1]) || $t[1] != 'none') {
									$ka = explode(',', $t[1]);
									for($ki = 0; $ki < count($ka); $ki++) {
										$this->covered['dependent'][$ki]['age'] = (int)$ka[$ki];
										$this->covered['dependent'][$ki]['number'] = $ki + 1;
										$this->covered['dependent'][$ki]['medical'] = 'yes';
										$this->covered['dependent'][$ki]['dental'] = 'yes';
									}
								}
								break;
						}
					}
				}
			}
		}
		curl_close($ch);
	}
	
	protected function createArrays() {
		$mCount = 0; $dCount = 0;
		foreach($this->json['plans'] as $p) {
			$temp = array();
			if(!empty($p['medicalDental'])) {
				$temp['adultCalendarYearMax']	= $p['adultCalendarYearMax'];
				$temp['adultDentalDeductible']	= $p['adultDentalDeductible'];
				$temp['adultDentalOOPMax']		= $p['adultDentalOOPMax'];
				$temp['baseRate']				= $p['baseRate'];
				$temp['childCalanderYearMax']	= $p['childCalanderYearMax'];
				$temp['childDentalDeductible']	= $p['childDentalDeductible'];
				$temp['childDentalOOPMax']		= $p['childDentalOOPMax'];
				$temp['deductibleAmount']		= $p['deductibleAmount'];
				$temp['familyDeductible']		= $p['familyDeductible'];
				$temp['familyOOPMax']			= $p['familyOOPMax'];
				$temp['individualOOPMax']		= $p['individualOOPMax'];
				$temp['medicalDental']			= $p['medicalDental'];
				$temp['path']					= $p['path'];
				$temp['persona']				= $p['persona'];
				$temp['personaName']			= $p['personaName'];
				$temp['planFamily']				= $p['planFamily'];
				$temp['planFamilyName']			= $p['planFamilyName'];
				$temp['planYear']				= $p['planYear'];
				switch($p['planRating']) {
					case 3:
						$temp['planRatingHtml']	= '<span class="gold bentonsansbold">Gold</span>';
						$temp['planRating']		= 'gold';
						break;
					case 2:
						$temp['planRatingHtml']	= '<span class="silver bentonsansbold">Silver</span>';
						$temp['planRating']		= 'silver';
						break;
					case 1:
						$temp['planRatingHtml']	= '<span class="bronze bentonsansbold">Bronze</span>';
						$temp['planRating']		= 'bronze';
						break;
					default:
						$temp['planRatingHtml']	= '<span class="noRating bentonsansbold"></span>';
						$temp['planRating']		= 'noRating';
						break;
				}
				$temp['planRatingName']			= $p['planRatingName'];
				$temp['promoted']				= $p['promoted'];
				$temp['selected']				= (!empty($this->planSelected[$p['medicalDental']]) && strtolower($this->planSelected[$p['medicalDental']]) == strtolower($p['title']) ? 1 : 0);
				$temp['shortDescription']		= $p['shortDescription'];
				$temp['title']					= $p['title'];
				$temp['weight']					= $p['weight'];
				if($p['medicalDental'] == 'medical') {
					if(!empty($p['persona']) && !in_array($p['persona'], $this->personas)) $this->personas[$p['persona']] = $p['personaName'];
					if(!empty($p['planFamily']) && !in_array($p['planFamily'], $this->plans)) $this->plans[$p['planFamily']] = $p['planFamilyName'];
					if(!empty($p['planRating']) && !in_array($p['planRating'], $this->metal)) $this->metal[$temp['planRating']] = $p['planRatingName'];
					$this->medical[$mCount] = $temp;
					$mCount++;
				} else if($p['medicalDental'] == 'dental') {
					$this->dental[$dCount] = $temp;
					$dCount++;
				}
			}
		}
		if($this->sort != 'default' && $this->sort != 'all') {
			/* TODO: add more then one sort option for compounding */
			if(count($this->sort) == 1) uasort($this->medical, array('BCBSRIPlanList','sort_by_one'));
			if(count($this->sort) > 1) uasort($this->medical, array('BCBSRIPlanList','sort_by_two'));
		} else {
			if($this->sort == 'default') uasort($this->medical, array('BCBSRIPlanList','sort_by_all')); #,'sort_by_one'));
			if($this->sort == 'all') uasort($this->medical, array('BCBSRIPlanList','sort_by_all'));
		}
		uasort($this->dental, array('BCBSRIPlanList','sort_dental'));
	}
	
	public function getDisplay($array) {
		$output = array();
		$oCount = 0;
		$t = $this->{$array};
		//$output['dataFile'] = $this->jsonFile;
		foreach($t as $temp) {
			$output[$oCount] = array();
			$output[$oCount]['medicalDental'] = $temp['medicalDental'];
			$output[$oCount]['persona']			= $temp['persona'];
			$output[$oCount]['personaName']		= $temp['personaName'];
			$output[$oCount]['planFamily']		= $temp['planFamily'];
			$output[$oCount]['planFamilyName']	= $temp['planFamilyName'];
			$output[$oCount]['planRating']		= $temp['planRating'];
			$output[$oCount]['planRatingName']	= $temp['planRatingName'];
			$output[$oCount]['planYear']		= $temp['planYear'];
			$output[$oCount]['promoted']		= $temp['promoted'];
			$output[$oCount]['weight']			= $temp['weight'];
			#$output[$oCount]['price']			= false;
			$output[$oCount]['price']			= $this->getPricing(array('planYear' => $temp['planYear'], 'planName' => $temp['title'], 'planType' => $temp['medicalDental'], 'baseRate' => $temp['baseRate']));
			$output[$oCount]['html']		= '<div class="medium-4 columns end"><div class="panel';
			$output[$oCount]['html']		.= ($temp['selected'] == 1 ? ' selected' : '');
			$output[$oCount]['html']		.= '" data-equalizer-watch>';
			$output[$oCount]['html']		.= '<div class="row">';
			$output[$oCount]['html']		.= '<div class="medium-10 columns">';
			$output[$oCount]['html']		.= '<p class="network">' . $temp['planRatingHtml'] . '<!-- | HMO Network--></p>';
			$output[$oCount]['html']		.= '<p class="planselect">' . $temp['title'] . '</p>';
			$output[$oCount]['html']		.= (!empty($temp['shortDescription']) ? '<p class="plandescription">' . $temp['shortDescription'] . '</p>' : '');
			$output[$oCount]['html']		.= '</div></div><div class="row"><div class="medium-12 columns">';
			$output[$oCount]['html']		.= '<p class="sm-title">Monthly Premium <span data-tooltip aria-haspopup="true" class="has-tip tip-top" title="The fixed amount that you pay each month for your insurance. The monthly premiums shown are estimates only based on the information you provided."><i class="icon icon-info-circle"></i></span></p>';
			if($output[$oCount]['price'] != false) {
				 $pCost = explode('.', $output[$oCount]['price']);
				 $pCost[0] = str_replace('$', '', $pCost[0]);
				 $pCost[1] = (empty($pCost[1]) ? '00' : (strlen($pCost[1]) < 2 ? $pCost[1] . '0' : $pCost[1]));
				 $price = '$' . $pCost[0] . '<sup>.' . $pCost[1] . '</sup>';
				$output[$oCount]['html']	.= '<p class="premium premiumPlaceholder">' . $price . '</p>';
			} else $output[$oCount]['html']	.= '<p class="premiumPlaceholder"><a href="#" class="bentonsansregular" data-reveal-id="quote">Get Quote</a></p>';
			$output[$oCount]['html']		.= '<div class="data hide">';
			$output[$oCount]['html']		.= '<div class="base-rate">' . $temp['baseRate'] . '</div>';
			$output[$oCount]['html']		.= '<div class="planName">' . $temp['title'] . '</div>';
			$output[$oCount]['html']		.= '<div class="planType">' . $temp['medicalDental'] . '</div>';
			$output[$oCount]['html']		.= '<div class="planYear">' . $temp['planYear'] . '</div>';
			$output[$oCount]['html']		.= '</div></div></div>';
			if((!empty($temp['deductibleAmount']) || !empty($temp['individualOOPMax'])) && $temp['medicalDental'] != 'dental') {
				$output[$oCount]['html']	.= '<div class="row"><div class="medium-12 columns"><p class="sm-heading-title">Individual</p></div></div>';
				$output[$oCount]['html']	.= '<div class="row">';
				if(!empty($temp['deductibleAmount'])) {
					$output[$oCount]['html'] .= '<div class="medium-6 columns"><p class="sm-title">Deductible <span data-tooltip aria-haspopup="true" class="has-tip tip-top" title="The amount you pay before your health plan starts to pay for certain medical bills. It resets every January 1."><i class="icon icon-info-circle"></i></span></p><p class="price bentonsansregular">' . $temp['deductibleAmount'] . '</p></div>';
				}
				if(!empty($temp['individualOOPMax'])) {
					$output[$oCount]['html'] .= '<div class="medium-6 columns"><p class="sm-title">Max Out-of-Pocket <span data-tooltip aria-haspopup="true" class="has-tip tip-top" title="The maximum amount you would pay out-of-pocket for covered health care services each year, including deductible, copayments, and coinsurance. After the out-of-pocket is met, covered services are paid at 100% by Blue Cross."><i class="icon icon-info-circle"></i></span></p><p class="price bentonsansregular">' . $temp['individualOOPMax'] . '</p></div>';
				}
				$output[$oCount]['html']	.= '</div>';
			}
			if((!empty($temp['familyDeductible']) || !empty($temp['familyOOPMax'])) && $temp['medicalDental'] != 'dental') {
				$output[$oCount]['html']	.= '<div class="row"><div class="medium-12 columns"><p class="sm-heading-title">Family</p></div></div>';
				$output[$oCount]['html']	.= '<div class="row">';
				if(!empty($temp['familyDeductible'])) {
					$output[$oCount]['html'] .= '<div class="medium-6 columns"><p class="sm-title">Deductible <span data-tooltip aria-haspopup="true" class="has-tip tip-top" title="The amount you pay before your health plan starts to pay for certain medical bills. It resets every January 1."><i class="icon icon-info-circle"></i></span></p><p class="price bentonsansregular">' . $temp['familyDeductible'] . '</p></div>';
				}
				if(!empty($temp['familyOOPMax'])) {
					$output[$oCount]['html'] .= '<div class="medium-6 columns"><p class="sm-title">Max Out-of-Pocket <span data-tooltip aria-haspopup="true" class="has-tip tip-top" title="The maximum amount you would pay out-of-pocket for covered health care services each year, including deductible, copayments, and coinsurance. After the out-of-pocket is met, covered services are paid at 100% by Blue Cross."><i class="icon icon-info-circle"></i></span></p><p class="price bentonsansregular">' . $temp['familyOOPMax'] . '</p></div>';
				}
				$output[$oCount]['html']	.= '</div>';
			}
			if(!empty($temp['adultDentalDeductible']) || !empty($temp['adultCalendarYearMax'])) {
				$output[$oCount]['html']	.= '<div class="row"><div class="medium-12 columns"><p class="sm-heading-title">Age 19 and Older</p></div></div>';
				$output[$oCount]['html']	.= '<div class="row">';
				if(!empty($temp['adultDentalDeductible'])) {
					$output[$oCount]['html'] .= '<div class="medium-6 columns"><p class="sm-title">Deductible <span data-tooltip aria-haspopup="true" class="has-tip tip-top" title="The amount you pay before your health plan starts to pay for certain dental bills. It resets every January 1."><i class="icon icon-info-circle"></i></span></p><p class="price bentonsansregular">' . $temp['adultDentalDeductible'] . '</p></div>';
				}
				if(!empty($temp['adultCalendarYearMax'])) {
					$output[$oCount]['html'] .= '<div class="medium-6 columns"><p class="sm-title">Calendar Year Max <span data-tooltip aria-haspopup="true" class="has-tip tip-top" title="The total amount that Blue Cross Dental will pay for services you receive in one year. After BCBSRI has paid this amount, you are responsible for all costs until the following calendar year."><i class="icon icon-info-circle"></i></span></p><p class="price bentonsansregular">' .$temp['adultCalendarYearMax'] . '</p></div>';
				}
				$output[$oCount]['html']	.= '</div>';
			}
			if(!empty($temp['adultDentalDeductible']) || !empty($temp['childDentalOOPMax'])) {
				$output[$oCount]['html']	.= '<div class="row"><div class="medium-12 columns"><p class="sm-heading-title">Under Age 19</p></div></div>';
				$output[$oCount]['html']	.= '<div class="row">';
				if(!empty($temp['childDentalDeductible'])) {
					$output[$oCount]['html'] .= '<div class="medium-6 columns"><p class="sm-title">Deductible <span data-tooltip aria-haspopup="true" class="has-tip tip-top" title="The amount you pay before your health plan starts to pay for certain dental bills. It resets every January 1."><i class="icon icon-info-circle"></i></span></p><p class="price bentonsansregular">' . $temp['childDentalDeductible'] . '</p></div>';
				}
				if(!empty($temp['childDentalOOPMax'])) {
					$output[$oCount]['html'] .= '<div class="medium-6 columns"><p class="sm-title">Max Out-of-Pocket <span data-tooltip aria-haspopup="true" class="has-tip tip-top" title="The maximum amount you would pay out-of-pocket for covered health care services each year, including deductible, copayments, and coinsurance. After the out-of-pocket is met, covered services are paid at 100% by Blue Cross."><i class="icon icon-info-circle"></i></span></p><p class="price bentonsansregular">' . $temp['childDentalOOPMax'] . '</p></div>';
				}
				$output[$oCount]['html']	.= '</div>';
			}
			if($temp['selected'] == 1) $output[$oCount]['html'] .= '<div class="row btn-selected"><div class="medium-12 columns text-center"> <a href="' . $temp['path'] . '" class="button expand radius"><i class="icon icon-check"></i> Selected</a></div></div>';
			else $output[$oCount]['html'] .= '<div class="row btn-secondary"><div class="medium-12 columns text-center"> <a href="' . $temp['path'] . '" class="button expand radius">Get Details</a></div></div>';
			$output[$oCount]['html']		.= '</div></div>';
			$oCount++;
		}
		return $output;
	}
	
	public function getJson() {
		return $this->json;
	}
	
	public function getJsonfile() {
		return $this->jsonFile;
	}
	
	public function getPlansSelected() {
		return $this->plansSelected;
	}
	
	public function getPulledCookie() {
		return $this->pulledCookie;
	}
	
	public function getOptions() {
		asort($this->personas);
		asort($this->metal);
		asort($this->plans);
		return array('featured' => $this->personas, 'metal' => $this->metal, 'planFamily' => $this->plans);
	}
	
	protected function getPricing ($plans) {
		include('bcbsri_online_application.pricing.modal.php');
		#$pricingIndexMedical[2016] = array('under21' => 0.635, '21' => 1.000, '22' => 1.000, '23' => 1.000, '24' => 1.000, '25' => 1.004, '26' => 1.024, '27' => 1.048, '28' => 1.087, '29' => 1.119, '30' => 1.135, '31' => 1.159, '32' => 1.183, '33' => 1.198, '34' => 1.214, '35' => 1.222, '36' => 1.230, '37' => 1.238, '38' => 1.246, '39' => 1.262, '40' => 1.278, '41' => 1.302, '42' => 1.325, '43' => 1.357, '44' => 1.397, '45' => 1.444, '46' => 1.500, '47' => 1.563, '48' => 1.635, '49' => 1.706, '50' => 1.786, '51' => 1.865, '52' => 1.952, '53' => 2.040, '54' => 2.135, '55' => 2.230, '56' => 2.333, '57' => 2.437, '58' => 2.548, '59' => 2.603, '60' => 2.714, '61' => 2.810, '62' => 2.873, '63' => 2.952, '64' => 3.000, '65+' => 3.000);
		#$pricingIndexDental[2016] = array('19-29' => 1.00, '30-39' => 1.00, '40-49' => 1.00, '50-59' => 1.10, '60+' => 1.25);
		if($this->covered == false) return false;
		$totPrice = 0;
		$pricing = $this->covered;
		$planYear = $plans['planYear'];
		if(strtolower($plans['planName']) == "dental direct standard" || strtolower($plans['planName']) == "dental direct plus") $pricingIndexDental[$planYear]['50-59'] = 1.099916;
		else $pricingIndexDental[$planYear]['50-59'] = 1.1;
		$planType = $plans['planType'];
		$planPrice = $plans['baseRate'];
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
					$totPrice += round($price, 2);
				}
			}
		}
		$dependentCount = 0;
		if(!empty($pricing['dependent'])) {
			usort($pricing['dependent'], array('BCBSRIPlanList','sort_by_age'));
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
						$totPrice += round($price, 2);
					}
				}
			}
		}
	
		return $totPrice;
	}
	
	protected function sort_by_age ($a, $b) {
		if((int)$a['age'] > (int)$b['age']) return -1;
		else if((int)$a['age'] < (int)$b['age']) return 1;
		else if((int)$a['age'] == (int)$b['age']) {
			if(!empty($a['number'])) {
				if((int)$a['number'] > (int)$b['number']) return 1;
				else return -1;
			} else return 0;
		} else return 0;
	}
	
	protected function sort_dental($a, $b) {
		$filter2 = array('weight');
		$filter1 = array('promoted', 1);
		$same = false;
		if(is_numeric($a[$filter1[0]])) {
			if((int)$a[$filter1[0]] == (int)$b[$filter1[0]]) $same = true;
			else {
				return ($a[$filter1[0]] > $b[$filter1[0]] ? -1 : 1);
			}
		} else {
			if($a[$filter1[0]] == $filter1[1] && $b[$filter1[0]] != $filter1[1]) return -1;
			else if($a[$filter1[0]] != $filter1[1] && $b[$filter1[0]] == $filter1[1]) return 1;
			else if($a[$filter1[0]] == $filter1[1] && $b[$filter1[0]] == $filter1[1]) $same = true;
		}
		if($same) {
			if(is_numeric($a[$filter2[0]])) {
				if((int)$a[$filter2[0]] == (int)$b[$filter2[0]]) return 0;
				else {
					return ($a[$filter2[0]] > $b[$filter2[0]] ? 1 : -1);
				}
			} else {
				if($a[$filter2[0]] == $filter2[1] && $b[$filter2[0]] != $filter2[1]) return 1;
				else if($a[$filter2[0]] != $filter2[1] && $b[$filter2[0]] == $filter2[1]) return -1;
				else if($a[$filter2[0]] == $filter2[1] && $b[$filter2[0]] == $filter2[1]) return 0;
				else return 0;
			}
		}
		return 0;
	}
	
	protected function sort_by_one($a, $b) {
		if($this->sort == 'default') $filter1 = array('promoted', 1);
		else $filter1 = explode(':', $this->sort[0]);
		$filter2 = array('weight');
		$same = false;
		if(is_numeric($a[$filter1[0]])) {
			if((int)$a[$filter1[0]] == (int)$b[$filter1[0]]) $same = true;
			else {
				return ($a[$filter1[0]] > $b[$filter1[0]] ? -1 : 1);
			}
		} else {
			if($a[$filter1[0]] == $filter1[1] && $b[$filter1[0]] != $filter1[1]) return -1;
			else if($a[$filter1[0]] != $filter1[1] && $b[$filter1[0]] == $filter1[1]) return 1;
			else if($a[$filter1[0]] == $filter1[1] && $b[$filter1[0]] == $filter1[1]) $same = true;
		}
		if($same) {
			if(is_numeric($a[$filter2[0]])) {
				if((int)$a[$filter2[0]] == (int)$b[$filter2[0]]) return 0;
				else {
					return ($a[$filter2[0]] > $b[$filter2[0]] ? 1 : -1);
				}
			} else {
				if($a[$filter2[0]] == $filter2[1] && $b[$filter2[0]] != $filter2[1]) return 1;
				else if($a[$filter2[0]] != $filter2[1] && $b[$filter2[0]] == $filter2[1]) return -1;
				else if($a[$filter2[0]] == $filter2[1] && $b[$filter2[0]] == $filter2[1]) return 0;
				else return 0;
			}
		}
		return 0;
		#if($a[$filter[0]] == $filter[1] && $b[$filter[0]] != $filter[1]) return 1;
		#else if($a[$filter[0]] != $filter[1] && $b[$filter[0]] == $filter[1]) return -1;
		#else return 0;
	}
	
	protected function sort_by_all($a, $b) {
		$filter2 = array('weight');
		$filter1 = array('promoted', 1);
		$same = false;
		if(is_numeric($a[$filter1[0]])) {
			if((int)$a[$filter1[0]] == (int)$b[$filter1[0]]) $same = true;
			else {
				return ($a[$filter1[0]] > $b[$filter1[0]] ? -1 : 1);
			}
		} else {
			if($a[$filter1[0]] == $filter1[1] && $b[$filter1[0]] != $filter1[1]) return -1;
			else if($a[$filter1[0]] != $filter1[1] && $b[$filter1[0]] == $filter1[1]) return 1;
			else if($a[$filter1[0]] == $filter1[1] && $b[$filter1[0]] == $filter1[1]) $same = true;
		}
		if($same) {
			if(is_numeric($a[$filter2[0]])) {
				if((int)$a[$filter2[0]] == (int)$b[$filter2[0]]) return 0;
				else {
					return ($a[$filter2[0]] > $b[$filter2[0]] ? 1 : -1);
				}
			} else {
				if($a[$filter2[0]] == $filter2[1] && $b[$filter2[0]] != $filter2[1]) return 1;
				else if($a[$filter2[0]] != $filter2[1] && $b[$filter2[0]] == $filter2[1]) return -1;
				else if($a[$filter2[0]] == $filter2[1] && $b[$filter2[0]] == $filter2[1]) return 0;
				else return 0;
			}
		}
		return 0;
		#if($a[$filter1[0]] == $filter1[1] && $b[$filter1[0]] != $filter1[1]) return -1;
		#else if($a[$filter1[0]] != $filter1[1] && $b[$filter1[0]] == $filter1[1]) return 1;
		#else if($a[$filter1[0]] == $filter1[1] && $b[$filter1[0]] == $filter1[1]) {
		#	if(intval($a[$filter2[0]]) > intval($b[$filter2[0]])) return 1;
		#	else if(intval($a[$filter2[0]]) < intval($b[$filter2[0]])) return -1;
		#	else if(intval($a[$filter2[0]]) == intval($b[$filter2[0]])) return 0;
		#	else return 0;
		#}
		#else return 0;
	}
	
	protected function sort_by_two($a, $b) {
		$filter1 = explode(':', $this->sort[0]);
		$filter2 = explode(':', $this->sort[1]);
		$same = false;
		if(is_numeric($a[$filter1[0]])) {
			if((int)$a[$filter1[0]] == (int)$b[$filter1[0]]) $same = true;
			else {
				return ($a[$filter1[0]] > $b[$filter1[0]] ? -1 : 1);
			}
		} else {
			if($a[$filter1[0]] == $filter1[1] && $b[$filter1[0]] != $filter1[1]) return -1;
			else if($a[$filter1[0]] != $filter1[1] && $b[$filter1[0]] == $filter1[1]) return 1;
			else if($a[$filter1[0]] == $filter1[1] && $b[$filter1[0]] == $filter1[1]) $same = true;
		}
		if($same) {
			if(is_numeric($a[$filter2[0]])) {
				if((int)$a[$filter2[0]] == (int)$b[$filter2[0]]) return 0;
				else {
					return ($a[$filter2[0]] > $b[$filter2[0]] ? 1 : -1);
				}
			} else {
				if($a[$filter2[0]] == $filter2[1] && $b[$filter2[0]] != $filter2[1]) return 1;
				else if($a[$filter2[0]] != $filter2[1] && $b[$filter2[0]] == $filter2[1]) return -1;
				else if($a[$filter2[0]] == $filter2[1] && $b[$filter2[0]] == $filter2[1]) return 0;
				else return 0;
			}
		}
		return 0;
	}
	
	protected function sort_by_number($a, $b) {
		$sortBy = $this->sortBy;
		if((int)$a[$sortBy] < (int)$b[$sortBy]) return -1;
		else if((int)$a[$sortBy] > (int)$b[$sortBy]) return 1;
		else return 0;
	}
	
	protected function sort_by_string($a, $b) {
		return strcmp($a["name"], $b["name"]);
	}
}