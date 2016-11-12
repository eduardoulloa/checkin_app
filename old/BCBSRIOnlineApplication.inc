<?php
class BCBSRIOnlineApplication {
	protected $config = array();
	protected $content = array();
	protected $content_bottom = array();
	protected $dentalApp = false;
	protected $display = '';
	protected $footer = array();
	protected $json;
	protected $reviewPage;
	protected $sidebar = array();
	protected $step = array();
	protected $year;
	
	public function __construct($data, $app, $year, $dental = false, $view = 'full') {
		$this->json = json_decode(file_get_contents($data), true);
		$this->year = $year;
		$this->dentalApp = $dental;
		$this->createDisplay($app, $view);
	}

	protected function createAnswersArray($data, $view = 'full') {
		$ans = array();
		$id = 1;
		foreach($data as $d) {
            // The id will now function as the order: simply count the number of answers.
			$id = count($ans);
			$ans[$id]['id'] = (!empty($d['id']) ? $d['id'] : "");
			if(!empty($d['alert_help_text']))			$ans[$id]['alert_help_text'] = $d['alert_help_text'];
			if(!empty($d['class']))						$ans[$id]['class'] = $d['class'];
			if(!empty($d['dependency'])) {
				if(!empty($d['dependency'][0]['hide'])) $ans[$id]['dependency']['hide'] = $d['dependency'][0]['hide'];
				if(!empty($d['dependency'][0]['show'])) $ans[$id]['dependency']['show'] = $d['dependency'][0]['show'];
			}
			if(!empty($d['error']))						$ans[$id]['error'] = $d['error'];
			if(!empty($d['name']))						$ans[$id]['name'] = $d['name'];
			if(!empty($d['label_tooltip']))				$ans[$id]['label_tooltip'] = $d['label_tooltip'];
			if(!empty($d['placeholder']))				$ans[$id]['placeholder'] = $d['placeholder'];
			$ans[$id]['required'] = (!empty($d['required']) ? $d['required'] : 0);
			if(!empty($d['selected']))					$ans[$id]['selected'] = $d['selected'];
			if(!empty($d['type']))						$ans[$id]['type'] = $d['type'];
			$ans[$id]['value'] = (!empty($d['value']) ? $d['value'] : '');
			$ans[$id]['label'] = (!empty($d['label']) ? $d['label'] : '');
			$ans[$id]['linebreak'] = (!empty($d['linebreak']) ? $d['linebreak'] : 0);
			$ans[$id]['min'] = (!empty($d['min']) ? $d['min'] : 0);
			$ans[$id]['max'] = (!empty($d['max']) ? $d['max'] : 0);
			$ans[$id]['rows'] = (!empty($d['rows']) ? $d['min'] : 0);
			$ans[$id]['cols'] = (!empty($d['cols']) ? $d['max'] : 0);
			$ans[$id]['open_enrollment'] = (!empty($d['open_enrollment_only']) ? $d['open_enrollment_only'] : 0);
			$ans[$id]['label_loc'] = (!empty($d['label_loc']) ? (strtolower($d['label_loc']) == 'bottom' || strtolower($d['label_loc']) == 'below' ? 'bottom' : strtolower($d['label_loc'])) : 'top');
			$ans[$id]['linebreak'] = (!empty($d['linebreak']) ? strtolower($d['linebreak']) : 0);
			$ans[$id]['readonly'] = (!empty($d['readonly']) ? $d['readonly'] : 0);
			$ans[$id]['pattern'] = (!empty($d['pattern']) ? strtolower($d['pattern']) : (!empty($d['placeholder']) ? strtolower($d['placeholder']) : '' ));
			switch($ans[$id]['pattern']) {
				case 'email@domain.com':
					$ans[$id]['pattern'] = '';
				case '000-000-0000':
					$ans[$id]['pattern'] = '\b(\(?\d{3}\)?[\-\.\s]?\d{3}[\-\.\s]?\d{4})\b';
					//$ans[$id]['pattern'] = '(\(?\d{3}\)?[\-\.\s]?\d{3}[\-\.\s]?\d{4})';
					break;
				case '00000':
				case '00000-0000':
					$ans[$id]['pattern'] = '(\d{5}(\-\d{4})?)';
					break;
				case 'mm/dd/yyyy':
				case 'mm-dd-yyyy':
					/* Pattern is for date in mm/dd/yyyy or yyyy/mm/dd */
					$ans[$id]['pattern'] = '((\d{2}[\-\.\/]\d{2}[\-\.\/]\d{4})|(\d{4}[\-\.\/]\d{2}[\-\.\/]\d{2}))';
					break;
				case '000-00-0000':
				    /* May 22, 2016 - added limitation of unused numbers according to https://www.ssa.gov/employer/randomization.html & https://www.ssa.gov/employer/randomizationfaqs.html#&a0=6
					   Limits SSN to not have all zeros in any part of the SSN or to start with 666 or 9xx
					   Old regex: (\d{3}(\-)?\d{2}(\-)?\d{4}) */
					$ans[$id]['pattern'] = '((?!000|666)[0-8][0-9]{2}(\-)?(?!00)[0-9]{2}(\-)?(?!0000)[0-9]{4})';
					break;
				case '000-00-0000a':
					$ans[$id]['pattern'] = '(\d{3}(\-)?\d{2}(\-)?\d{4}(\-)?[a-zA-Z])';
					break;
				case 'aaa000000000':
					$ans[$id]['pattern'] = '([a-zA-Z0-9]{3}[0-9]{9})';
					break;
				default:
					$ans[$id]['pattern'] = '';
					break;
			}
            // Increment the number of answers by one.
            $id++;
		}
		return $ans;
	}

	protected function createCellArray($data, $view = 'full') {
		$cell = array();
		$id = 0;
		foreach($data as $d) {
			#$this->display .= var_export($d, true) . '<br />';
			if(!empty($d['answers']))	$cell[$id]['answers'] = $this->createAnswersArray($d['answers'], $view);
			if(!empty($d['row']))		$cell[$id]['row'] = $this->createSubRowArray($d['row']);
			if(!empty($d['subtitle']))	$cell[$id]['subtitle'] = $d['subtitle'];
			if(!empty($d['text']))		$cell[$id]['text'] = $d['text'];
			if(!empty($d['title']))		$cell[$id]['title'] = $d['title'];
			$cell[$id]['class'] = (!empty($d['class']) ? $d['class'] : 'medium-12');
			$cell[$id]['textloc'] = (!empty($d['textloc']) ? (strtolower($d['textloc']) == 'bottom' || strtolower($d['textloc']) == 'below' ? 'bottom' : strtolower($d['textloc'])) : 'top');
			$id++;
		}
		#$this->display .= var_export($cell, true) . '<br /><br />';
		return $cell;
	}

	protected function createDisplay($app, $view) {
		foreach($this->json as $key => $val) {
			if($app == $key) {
				//foreach($this->json[$key]['config'] as $k => $v) {
					$this->config['date_format'] = (isset($this->json[$key]['config'][0]['date_format']) ? $this->json[$key]['config'][0]['date_format'] : 'm/d/Y');
					foreach($this->json[$key]['config'][0]['open_enrollment'] as $oe) {
						if($oe['year'] == $this->year) {
              $this->config['application_form']             = $oe['application_form'];
              $this->config['eft_form']                     = $oe['eft_form'];
              $this->config['cms_ohic_code']                = $oe['cms_ohic_code'];
              $this->config['contact_info']                 = $oe['contact_info'];
              $this->config['error_customer_service']       = $oe['error_customer_service'];
              $this->config['error_customer_service_name']  = $oe['error_customer_service_name'];
              $this->config['last_modified']                = $oe['last_modified'];
              $this->config['oe_end']                       = $oe['open_enrollment_end'];
              $this->config['oe_start']                     = $oe['open_enrollment_start'];
              $this->config['progress_steps']               = $oe['progress_steps'];
              $this->config['plan_change_form']             = $oe['plan_change_form'];
              if(!empty($oe['dependentpage'])) $this->config['dependentpage'] = $oe['dependentpage'];
              if(!empty($oe['spousepage'])) $this->config['spousepage'] = $oe['spousepage'];
						}
					}
				//}
				foreach($this->json[$key]['content'] as $k => $v) {
					$this->createRowArray($v['row'], 'content');
				}
				foreach($this->json[$key]['content_bottom'] as $k => $v) {
					$this->createRowArray($v['row'], 'content_bottom');
				}
				foreach($this->json[$key]['footer'] as $k => $v) {
					$this->createRowArray($v['row'], 'footer');
				}
				$this->createSidebarArray($this->json[$key]['sidebar']);
				$this->createStepsArray($this->json[$key]['steps']);
			}
		}
	}
    
    protected function createRowArray($data, $variable, $view = 'full') {
		$o = 1;
		#$id = 1;
		foreach($data as $d) {
			$step = (!empty($d['config'][0]['step']) ? $d['config'][0]['step'] : $d['step']); 
            //If the step cell in the array is empty, assign a 0. Otherwise, count the number of elements, and
            //assign it.
            $order = (empty($this->{$variable}[$step]) ? 0 : count($this->{$variable}[$step]));
			#$id = (!empty($d['id']) ? $d['id'] : (!empty($this->{$variable}[$step]) ? count($this->{$variable}[$step]) : 0));
			#$order = (!empty($d['config'][0]['order']) ? $d['config'][0]['order'] : $o);
			#$this->{$variable}[$step][$order][$id]['id'] = $id;
			if(!empty($d['cell']))                  $this->{$variable}[$step][$order]['cell'] = $this->createCellArray($d['cell']);
			if(!empty($d['class']))                 $this->{$variable}[$step][$order]['class'] = $d['class'];
			if(!empty($d['id']))                    $this->{$variable}[$step][$order]['id'] = $d['id'];
			if(!empty($d['modal'][0]['reveal_id']))	$this->{$variable}[$step][$order]['modal']['reveal_id'] = strtolower($d['modal'][0]['reveal_id']);
			if(!empty($d['modal'][0]['text']))      $this->{$variable}[$step][$order]['modal']['text'] = $d['modal'][0]['text'];
			$this->{$variable}[$step][$order]['panel'] = (!empty($d['panel']) ? $d['panel'] : 0);
			$this->{$variable}[$step][$order]['required'] = (!empty($d['config'][0]['required']) ? $d['config'][0]['required'] : 0);
			$this->{$variable}[$step][$order]['config']['startyear'] = (!empty($d['config'][0]['startyear']) ? $d['config'][0]['startyear'] : 1900);
			$this->{$variable}[$step][$order]['config']['endyear'] = (!empty($d['config'][0]['endyear']) ? $d['config'][0]['endyear'] : 9999);
			$this->{$variable}[$step][$order]['config']['medical_only'] = (!empty($d['config'][0]['medical_only']) ? $d['config'][0]['medical_only'] : 0);
			$this->{$variable}[$step][$order]['config']['open_enrollment_only'] = (!empty($d['config'][0]['open_enrollment_only']) ? $d['config'][0]['open_enrollment_only'] : 0);
			$o++;
			#$id++;
		}
	}
	
	protected function createSubRowArray($data, $view = 'full') {
		$o = 1;
		$id = 1;
		$sRow = array();
		foreach($data as $d) {
			#$order = (!empty($d['config'][0]['order']) ? $d['config'][0]['order'] : $o);
            $order = $o;
			#$id = (!empty($d['id']) ? $d['id'] : (!empty($sRow[$order]) ? count($sRow[$order]) : 0));
			#$sRow[$order][$id]['id'] = $id;
			if(!empty($d['cell']))					$sRow[$order]['cell'] = $this->createCellArray($d['cell']);
			if(!empty($d['class']))					$sRow[$order]['class'] = $d['class'];
			if(!empty($d['modal'][0]['reveal_id']))	$sRow[$order]['modal']['reveal_id'] = strtolower($d['modal'][0]['reveal_id']);
			if(!empty($d['modal'][0]['text']))		$sRow[$order]['modal']['text'] = $d['modal'][0]['text'];
            if(!empty($d['id']))                    $sRow[$order]['id'] = $d['id'];
			$sRow[$order]['panel'] = (!empty($d['panel']) ? $d['panel'] : 0);
			$sRow[$order]['required'] = (!empty($d['config'][0]['required']) ? $d['config'][0]['required'] : 0);
			$sRow[$order]['config']['startyear'] = (!empty($d['config'][0]['startyear']) ? $d['config'][0]['startyear'] : 1900);
			$sRow[$order]['config']['endyear'] = (!empty($d['config'][0]['endyear']) ? $d['config'][0]['endyear'] : 9999);
			$sRow[$order]['config']['medical_only'] = (!empty($d['config'][0]['medical_only']) ? $d['config'][0]['medical_only'] : 0);
			$sRow[$order]['config']['open_enrollment_only'] = (!empty($d['config'][0]['open_enrollment_only']) ? $d['config'][0]['open_enrollment_only'] : 0);
			$sRow[$order]['sub'] = 1;
			$o++;
			#$id++;
		}
		return $sRow;
	}

	protected function createSidebarArray($data, $view = 'full') {
		$id = 1;
		$o = 1;
		foreach($data as $d) {
			$step = (!empty($d['config'][0]['step']) ? $d['config'][0]['step'] : 0);
            // The id is now used as the order. Simply count the number of elements in the 'step' cell of the
            // sidebar array.
			$id = (!empty($this->sidebar[$step]) ? count($this->sidebar[$step]) : 0);
			$this->sidebar[$step][$id]['id'] = $d['id'];
			if(!empty($d['answers']))							$this->sidebar[$step][$id]['answers'] = $this->createAnswersArray($d['answers'], $view);
			if(!empty($d['class']))								$this->sidebar[$step][$id]['class'] = $d['class'];
			if(!empty($d['row']))								$this->sidebar[$step][$id]['row'] = $this->createSubRowArray($d['row']);
			if(!empty($d['subtitle']))							$this->sidebar[$step][$id]['subtitle'] = $d['subtitle'];
			if(!empty($d['text']))								$this->sidebar[$step][$id]['text'] = $d['text'];
			if(!empty($d['title']))								$this->sidebar[$step][$id]['title'] = $d['title'];
			$this->sidebar[$step][$id]['textloc'] = (!empty($d['textloc']) ? (strtolower($d['textloc']) == 'bottom' || strtolower($d['textloc']) == 'below' ? 'bottom' : strtolower($d['textloc'])) : 'top');
			$this->sidebar[$step][$id]['config']['endyear'] = (!empty($d['config'][0]['endyear']) ? $d['config'][0]['endyear'] : 9999);
			$this->sidebar[$step][$id]['config']['medical_only'] = (!empty($d['config'][0]['medical_only']) ? $d['config'][0]['medical_only'] : 0);
			$this->sidebar[$step][$id]['config']['open_enrollment_only'] = (!empty($d['config'][0]['open_enrollment_only']) ? $d['config'][0]['open_enrollment_only'] : 0 );
			$this->sidebar[$step][$id]['config']['startyear'] = (!empty($d['config'][0]['startyear']) ? $d['config'][0]['startyear'] : 1900);
			$id++;
			$o++;
		}
	}
	
	protected function createStepsArray($data, $view = 'full') {
		$cell = array();
		foreach($data as $d) {
			$this->step[$d['id']]['back_text'] = (!empty($d['back_text']) ? $d['back_text'] : '');
			$this->step[$d['id']]['ga_page_view'] = (!empty($d['ga_page_view']) ? $d['ga_page_view'] : '');
			$this->step[$d['id']]['progress'] = (!empty($d['progress']) ? $d['progress'] : '');
			$this->step[$d['id']]['reviewpage'] = (!empty($d['reviewpage']) ? $d['reviewpage'] : '');
			$this->step[$d['id']]['step_number'] = (!empty($d['shown_step_number']) ? $d['shown_step_number'] : '');
			$this->step[$d['id']]['submit_text'] = (!empty($d['submit_text']) ? $d['submit_text'] : '');
			$this->step[$d['id']]['submit_helper_text']['success'] = (!empty($d['submit_helper_text'][0]['success']) ? $d['submit_helper_text'][0]['success'] : '');
			$this->step[$d['id']]['submit_helper_text']['error'] = (!empty($d['submit_helper_text'][0]['error']) ? $d['submit_helper_text'][0]['error'] : '');
			$this->step[$d['id']]['subtext'] = (!empty($d['subtext']) ? $d['subtext'] : '');
			$this->step[$d['id']]['title'] = (!empty($d['title']) ? $d['title'] : '');
            $this->step[$d['id']]['show_on_review'] = (!empty($d['show_on_review']) ? $d['show_on_review'] : '');
			if(!empty($d['reviewpage']) && $d['reviewpage'] == 1) $this->reviewPage = $d['id'];
		}
	}
	//Generates the HTML.
	protected function getAnswerOutput($answers, $view = 'full', $review = false, $finalReview = false, $emailReview = false) {
		$output = array();
		$alert = array();
		$i = 0;
		foreach($answers as $a) {
			if($a['open_enrollment'] == 0 || ($a['open_enrollment'] == 1 && $this->isOpenEnrollment())) {
				$output[$i] = '<div class="' . ($review != false ? 'review-' : '') . 'answer-' . $a['id'] . ' answer-' . $a['type'] . '" id="' . $a['id'] . '">';
				if(!empty($a['dependency']) && $review == false) {
					$output[$i] .= '<div class="hide-show-logic hide">';
					if(!empty($a['dependency']['hide'])) $output[$i] .= '<div class="hide">' . $a['dependency']['hide'] . '</div>';
					if(!empty($a['dependency']['show'])) $output[$i] .= '<div class="show">' . $a['dependency']['show'] . '</div>';
					$output[$i] .= '</div>';
				}
				$tName = str_replace(array('[', ']'), '', $a['name']);
				$name = ($review ? 'review' . ($finalReview ? '2' : '') . '-' . $tName : $tName);
				$label = '<label';
				$label .= (!empty($a['name']) ? ' for="' . ($review != false ? 'review-' : '') . $name . '-' . $a['value'] . '"' : '');
				$label .= '>' . $a['label'];
				$label .= (!empty($a['required']) && $a['required'] == 1 ? ($review == false ? '<span class="required">*</span>' : '') : '');
				$label .= (!empty($a['label_tooltip']) && $review == false ? $a['label_tooltip'] : '');
				$label .= '</label>';
				switch($a['type']) {
					case 'text':
					case 'password':
					case 'number':
					case 'email':
					case 'tel':
					case 'range':
					case 'date':
					case 'month':
					case 'week':
					case 'time':
					case 'datetime':
					case 'datetime-local':
					case 'url':
					case 'color':
						if($emailReview != false) {
							if(strpos($a['class'], 'datepicker')) {
								$eClass = explode('datepicker', $a['class']);
								$a['class'] = join('', $eClass);
							}
							$output[$i] .= $a['label'] . (!empty($a['required']) && $a['required'] == 1 ? '<span class="required">*</span>' : '') . ': <span id="' . $name . '" class="answer-' . $a['id'] . (isset($a['class']) ? ' ' . $a['class'] : '') . ' ' . $name;
							$output[$i] .= ($review != false ? ' review-answer-' . $a['id'] . '"' : '"') . ' data-hj-masked>';
							$output[$i] .= '</span>';
						} else {
							if($a['label_loc'] == 'top') $output[$i] .= $label;
							$output[$i] .= '<input type="' . $a['type'] . '"';
							$output[$i] .= ' name="' . $name . '"';
							$output[$i] .= ' id="' . $name . '"';
							$output[$i] .= ' class="answer-' . $a['id'] . (isset($a['class']) ? ' ' . $a['class'] : '') . ' ' . $name;
							$output[$i] .= ($review != false ? ' review-answer-' . $a['id'] . '"' : '"');
							if(!empty($a['value'])) {
								if($a['type'] == 'date' && $a['value'] == 'now') $output[$i] .= ' value="' . date('Y-m-d') . '"';
								else if($a['type'] == 'month' && $a['value'] == 'now') $output[$i] .= ' value="' . date('Y-m-01') . '"';
								else $output[$i] .= ' value="' . $a['value'] . '"';
							}
							$output[$i] .= (!empty($a['max']) && $a['max'] != 0 && $a['type'] != 'number' ? ' maxlength="' . $a['max'] . '"' : '');
							$output[$i] .= (!empty($a['min']) && $a['type'] == 'number' ? ' min="' . $a['max'] . '"' : '');
							$output[$i] .= (!empty($a['max']) && $a['max'] != 0 && $a['type'] == 'number' ? ' max="' . $a['max'] . '"' : '');
							$output[$i] .= (!empty($a['placeholder']) ? ' placeholder="' . $a['placeholder'] . '"' : '');
							$output[$i] .= (!empty($a['pattern']) ? ' pattern="' . $a['pattern'] . '"' : '');
							$output[$i] .= (!empty($a['required']) && $a['required'] == 1 ? ($review == false ? ' required' : '') : '');
							$output[$i] .= (!empty($a['readonly']) && $a['readonly'] == 1 ? ' readonly="readonly"' : '');
							$output[$i] .= ' data-hj-masked />';
							$output[$i] .= (!empty($a['error']) && $review == false ? '<small class="error">' . $a['error'] . '</small>' : '');
							if($a['label_loc'] == 'bottom') $output[$i] .= $label;
						}
						break;
					case 'select':
					case 'datalist':
						if($emailReview != false) {
							$output[$i] .= $a['label'] . (!empty($a['required']) && $a['required'] == 1 ? '<span class="required">*</span>' : '') . ': <span id="' . $name . '" class="answer-' . $a['id'] . (isset($a['class']) ? ' ' . $a['class'] : '') . ' ' . $name;
							$output[$i] .= ($review != false ? ' review-answer-' . $a['id'] . '" data-hj-masked' : '"') . '>';
							$output[$i] .= (!empty($a['selected']) ? $a['selected'] : '') . '</span>';
						} else {
							if($a['label_loc'] == 'top') $output[$i] .= $label;
							$output[$i] .= ($a['type'] == 'select' ? '<select' : '<datalist');
							$output[$i] .= ' name="' . $name . '"';
							$output[$i] .= ' id="' . $name . '-' . $a['value'] . '"';
							$output[$i] .= ' class="answer-' . $a['id'] . (isset($a['class']) ? ' ' . $a['class'] : '') . ' ' . $name;
							$output[$i] .= ($review != false ? ' review-answer-' . $a['id'] . '"' : '"');
							$output[$i] .= ($review != false ? ' disabled' : '');
							$output[$i] .= ' data-hj-masked>';
							foreach(explode('|', $a['value']) as $key => $value) {
								if($a['type'] == 'select') {
									$output[$i] .= '<option value="' . $value . '"';
									$output[$i] .= (!empty($a['selected']) && $a['selected'] == $value ? ' selected' : '');
									$output[$i] .= '>' . $value . '</option>';
								} else $output[$i] .= '<option value="' . $value . '">';
							}
							$output[$i] .= '</select>';
							$output[$i] .= ($a['type'] == 'select' ? '</select>' : '</datalist>');
							$output[$i] .= (!empty($a['error']) && $review == false ? '<small class="error">' . $a['error'] . '</small>' : '');
							if($a['label_loc'] == 'bottom') $output[$i] .= $label;
						}
						break;
					case 'checkbox':
					case 'radio':
						if($emailReview != false) {
							$output[$i] .= '<span id="' . $name . '" class="answer-' . $a['id'] . (isset($a['class']) ? ' ' . $a['class'] : '') . ' ' . $name . ' ' . $name . '-' . $a['value'];
							$output[$i] .= ($review != false ? ' review-answer-' . $a['id'] . '"' : '"') . ' data-hj-masked>' . (!empty($a['selected']) && $a['selected'] == 'yes' ? '(X)' : '(_)');
							$output[$i] .= '</span> ' . $a['label'];
						} else {
							$output[$i] .= '<input type="' . ($a['type'] == 'radio' ? 'radio' : 'checkbox') . '"';
							$output[$i] .= ' name="' . $name . '"';
							$output[$i] .= ' id="' . $name . '-' . $a['value'] . '"';
							$output[$i] .= (!empty($a['value']) ? ' value="' . $a['value'] . '"' : '');
							$output[$i] .= ' class="answer-' . $a['id'] . (isset($a['class']) ? ' ' . $a['class'] : '') . ' ' . $name . '-' . $a['value'];
							$output[$i] .= ($review != false ? ' review-answer-' . $a['id'] . '"' : '"');
							$output[$i] .= (!empty($a['selected']) && $a['selected'] == 'yes' ? ' checked' : '');
							$output[$i] .= ($review != false ? ' disabled' : '');
							$output[$i] .= ' data-hj-masked />';
							$output[$i] .= $label;
						}
						break;
					case 'textarea':
						if($emailReview != false) {
							$output[$i] .= $a['label'] . (!empty($a['required']) && $a['required'] == 1 ? '<span class="required">*</span>' : '') . ': <span id="' . $name . '" class="answer-' . $a['id'] . (isset($a['class']) ? ' ' . $a['class'] : '') . ' ' . $name;
							$output[$i] .= ($review != false ? ' review-answer-' . $a['id'] . '"' : '"') . ' data-hj-masked></span>';
						} else {
							if($a['label_loc'] == 'top') $output[$i] .= $label;
							$output[$i] .= '<textarea';
							$output[$i] .= ' name="' . $name . '"';
							$output[$i] .= ' id="' . $name . '"';
							$output[$i] .= ' class="answer-' . $a['id'] . (isset($a['class']) ? ' ' . $a['class'] : '');
							$output[$i] .= ($review != false ? ' review-answer-' . $a['id'] . '"' : '"');
							//$output[$i] .= (!empty($a['value']) ? ' value="' . $a['value'] . '"' : '');
							$output[$i] .= (!empty($a['rows']) && $a['rows'] != 0 ? ' rows="' . $a['rows'] . '"' : '');
							$output[$i] .= (!empty($a['cols']) && $a['cols'] != 0 ? ' cols="' . $a['cols'] . '"' : '');
							$output[$i] .= (!empty($a['required']) && $a['required'] == 1 ? ($review == false ? ' required' : '') : '');
							$output[$i] .= ($review != false ? ' disabled' : '');
							$output[$i] .= ' data-hj-masked></textarea>';
							$output[$i] .= (!empty($a['error']) && $review == false ? '<small class="error">' . $a['error'] . '</small>' : '');
							if($a['label_loc'] == 'bottom') $output[$i] .= $label;
						}
						break;
				}
				if(!empty($a['alert_help_text']) && $review == false) {
					$alert[$i] .= '<div class="hide clear alert-' . $name . ' alert-' . $name . '-' . $a['value'] . '"><div data-alert="" class="alert-box">';
					$alert[$i] .= '<p>' . $a['alert_help_text'] . '</p>';
					$alert[$i] .= '</div></div>';
				}
				$output[$i] .= '</div>';
				$output[$i] .= ($a['linebreak'] == 1 ? '<div class="clear"></div>' : '');
				$i++;
			}
		}
		$output = join('', $output);
		$output .= join('', $alert);
		return $output;
	}
	
	protected function getCellOutput($cell, $panel, $modal, $view = 'full', $review = false, $finalReview = false, $emailReview = false, $rowRequired = false) {
		$output = array();
		$i = 0;
		foreach($cell as $c) {
			$output[$i] = '<div class="' . (!empty($c['class']) ? $c['class'] . ' ' : '') . 'columns">';
			$output[$i] .= ($panel == 1 ? '<div class="panel">' : '');
			$output[$i] .= (!empty($c['title']) ? '<h2>' . $c['title'] . '</h2>' : '');
			$output[$i] .= (!empty($c['subtitle']) ? '<h3>' . $c['subtitle'] . '</h3>' : '');
			$output[$i] .= (!empty($c['text']) && $c['textloc'] == 'top' ? $c['text'] : '');
			$output[$i] .= ($rowRequired && !empty($c['text']) ? '<span class="required">*</span>' : '');
			$output[$i] .= (!empty($c['text']) && $c['textloc'] == 'top' ? '<div class="clear"></div>' : '');
			if(!empty($c['answers'])) $output[$i] .= $this->getAnswerOutput($c['answers'], $view, $review, $finalReview, $emailReview);
			if(!empty($modal) && $modal != false && $review == false) $output[$i] .= '<div id="' . $modal['reveal_id'] . '" class="reveal-modal" data-reveal aria-labelledby="' . $modal['reveal_id'] . 'Title" aria-hidden="true" role="dialog">' . $modal['text'] . '<a class="close-reveal-modal" aria-label="Close">&#215;</a></div>';
			if(!empty($c['row'])) $output[$i] .= $this->getRowOutput2($c['row'], $view, $review, $finalReview, $emailReview);
			$output[$i] .= (!empty($c['text']) && $c['textloc'] == 'bottom' ? $c['text'] : '');
			$output[$i] .= ($panel == 1 ? '<div class="clear"></div></div>' : '');
			$output[$i] .= '</div>';
			$i++;
		}
		return join('', $output);
	}
	
	protected function getContentBottomOutput($page, $view = 'full', $review = false, $finalReview = false, $emailReview = false) {
		$output = array();
		if(isset($this->content_bottom[0])) {
			$output[0] = $this->getRowOutput(0, 'content_bottom', $view, $review, $finalReview, $emailReview);
		}
		if(isset($this->content_bottom[$page])) {
			$output[1] = $this->getRowOutput($page, 'content_bottom', $view, $review, $finalReview, $emailReview);
		}
		return join('', $output);
	}
	
	public function getDisplayPage($page, $view = 'full') {
		$dis = array();
		$steps = $this->getNumberofSteps();
		
		$dis['page'] = $page;
		$dis['view'] = $view;
		$review = (!empty($this->step[$page]['reviewpage']) && $this->step[$page]['reviewpage'] == 1 ? true : false);
		$emailReview = ($steps == $page ? true : false);
		$finalReview = ($steps == $page ? true : false);
		$dis['ga_page_view'] = (!empty($this->step[$page]['ga_page_view']) ? $this->step[$page]['ga_page_view'] : strtolower(str_replace(array(' ', ',', '!', '?'), '', $this->step[$page]['title'])));
		$dis['progress'] = (!empty($this->step[$page]['progress']) ? $this->step[$page]['progress'] : '');
		$dis['step_number'] = (!empty($this->step[$page]['step_number']) ? $this->step[$page]['step_number'] : '');
		$dis['submit_text'] = (!empty($this->step[$page]['submit_text']) ? $this->step[$page]['submit_text'] : '');
		$dis['back_text'] = (!empty($this->step[$page]['back_text']) ? $this->step[$page]['back_text'] : '');
		$dis['submit_helper_text_success'] = (!empty($this->step[$page]['submit_helper_text']['success']) ? $this->step[$page]['submit_helper_text']['success'] : '');
		$dis['submit_helper_text_error'] = (!empty($this->step[$page]['submit_helper_text']['error']) ? $this->step[$page]['submit_helper_text']['error'] : '');
		
		$dis['content'] = (!empty($this->step[$page]['title']) ? '<h1>' . $this->step[$page]['title'] . '</h1>' : '');
		$dis['content'] .= (!empty($this->step[$page]['subtext']) ? '<p class="lead">' . $this->step[$page]['subtext'] . '</p>' : '');
		$dis['content'] .= '<hr /><p class="bentonsanslightitalic"><span class="required">*</span> = required field</p>';
		$dis['content'] .= (!empty($this->content[$page]) ? $this->getRowOutput($page, 'content', $view, $review, $finalReview, false) : '');
		$dis['final_review'] = '';
		$dis['email_review'] = '';
		
		if($steps == $page || $review == true) {
			for($i = 1; $i < $this->reviewPage; $i++) {
                if(!empty($this->step[$i]['show_on_review']) && $this->step[$i]['show_on_review'] == 1){
                    if($i > 1) $dis['final_review'] .= '<hr />';
                    $dis['final_review'] .= '<div class="page-' . $i . '">';
                    if($review == true) {
                        $dis['final_review'] .= '<a class="edit-section edit-page page-' . $i . '">Edit</a>';
                    }
                    $dis['final_review'] .= (!empty($this->step[$i]['title']) ? '<h1>' . $this->step[$i]['title'] . '</h1>' : '');
                    $dis['final_review'] .= (!empty($this->step[$i]['subtext']) ? '<p class="lead">' . $this->step[$i]['subtext'] . '</p>' : '');
                    $dis['final_review'] .= $this->getRowOutput($i, 'content', $view, true, $finalReview, false) . '</div>';
                }
				
			}
			if($emailReview) {
				for($i = 2; $i < $this->reviewPage; $i++) {
					if($i > 2) $dis['email_review'] .= '<hr />';
					$dis['email_review'] .= '<div class="page-' . $i . '">';
					$dis['email_review'] .= (!empty($this->step[$i]['title']) ? '<h1>' . $this->step[$i]['title'] . '</h1>' : '');
					$dis['email_review'] .= (!empty($this->step[$i]['subtext']) ? '<p class="lead">' . $this->step[$i]['subtext'] . '</p>' : '');
					$dis['email_review'] .= $this->getRowOutput($i, 'content', $view, true, false, $emailReview) . '</div>';
				}
			}
		}
		
		$dis['content_bottom'] = $this->getContentBottomOutput($page, $view, $review, $finalReview, $emailReview);
		
		$dis['sidebar'] = $this->getSidebarOutput($page, $view, $review, $finalReview, $emailReview);
		
		$dis['footer'] = $this->getFooterOutput($page, $view, $review, $finalReview, $emailReview);
		
		return json_encode($dis);
	}
	
	protected function getFooterOutput($page, $view = 'full', $review = false, $finalReview = false, $emailReview = false) {
		$output = array();
		if(!empty($this->footer[0])) {
			$output[0] = $this->getRowOutput(0, 'footer', $view, $review, $finalReview, $emailReview);
		}
		if(!empty($this->footer[$page])) {
			$output[1] = $this->getRowOutput($page, 'footer', $view, $review, $finalReview, $emailReview);
		}
		return join('', $output);
	}
	
	protected function getRowOutput($page, $content, $view = 'full', $review = false, $finalReview = false, $emailReview = false) {
		$output = array();
        $i = 0;
		foreach($this->{$content}[$page] as $p) {
			//foreach($o as $p) {
				/* Pass start/end year check and OE. Now show the questions */
				if(($p['config']['startyear'] <= $this->year && $this->year <= $p['config']['endyear']) && ($p['config']['open_enrollment_only'] == 0 || ($p['config']['open_enrollment_only'] == 1 && $this->isOpenEnrollment()))) {
					/* pass if dental only app */
					if(($this->dentalApp == true && $p['config']['medical_only'] != 1) || $this->dentalApp == false) {
						$output[$i] = '<div class="row ' . ($review != false ? 'review-' : '') . '' . (!empty($p['sub']) && $p['sub'] == 1 ? ' sub-row' : '') . '' . (!empty($p['class']) ? ' ' . $p['class'] : '') . '' . (!empty($p['required']) && $p['required'] == 1 ? ($review == false ? ' row-required' : '') : '') . '"' . (!empty($p['id']) ? ' id="' . $p['id'] . '"' : '') . '>';
						$output[$i] .= (!empty($p['cell']) ? $this->getCellOutput($p['cell'], $p['panel'], (!empty($p['modal']) ? $p['modal'] : false), $view, $review, $finalReview, $emailReview, (!empty($p['required']) && $p['required'] == 1 ? true : false)) : '');
						$output[$i] .= '</div>';
						$i++;
					}
				}
			//}
		}
		return join('', $output);
	}
	
	protected function getRowOutput2($row, $view = 'full', $review = false, $finalReview = false, $emailReview = false) {
		$output = array();
		$i = 0;
		foreach($row as $p) {
			//foreach($o as $p) {
				/* Pass start/end year check and OE. Now show the questions */
				if(($p['config']['startyear'] <= $this->year && $this->year <= $p['config']['endyear']) && ($p['config']['open_enrollment_only'] == 0 || ($p['config']['open_enrollment_only'] == 1 && $this->isOpenEnrollment()))) {
					/* pass if dental only app */
					if(($this->dentalApp == true && $p['config']['medical_only'] != 1) || $this->dentalApp == false) {
						$output[$i] = '<div class="row ' . (!empty($p['sub']) && $p['sub'] == 1 ? ' sub-row' : '') . '' . (!empty($p['class']) ? ' ' .$p['class'] : '') . '' . (!empty($p['required']) && $p['required'] == 1 ? ($review == false ? ($review == false ? ' row-required' : '') : '') : '') . '"' . (!empty($p['id']) ? ' id="' . $p['id'] . '"' : '') . '>';
						$output[$i] .= (!empty($p['cell']) ? $this->getCellOutput($p['cell'], $p['panel'], (!empty($p['modal']) ? $p['modal'] : false), $view, $review, $finalReview, $emailReview, (!empty($p['required']) && $p['required'] == 1 ? true : false)) : '');
						$output[$i] .= '</div>';
						$i++;
					}
				}
			//}	
		}
		return join('', $output);
	}
	
	protected function getSidebarOutput($page, $view = 'full', $review = false, $finalReview = false, $emailReview = false) {
		$output = array();
		if(!empty($this->sidebar[0])) {
			$output[0] = $this->getSidebarOutput2(0, $view, $review);
		}
		if(!empty($this->sidebar[$page])) {
			$output[1] = $this->getSidebarOutput2($page, $view, $review);
		}
		return join('', $output);
	}
	
	protected function getSidebarOutput2($page, $view = 'full', $review = false, $finalReview = false, $emailReview = false) {
		$output = array();
		$i = 0;
		foreach($this->sidebar[$page] as $c) {
			/* Pass start/end year check and OE. Now show the questions */
			if(($c['config']['startyear'] <= $this->year && $this->year <= $c['config']['endyear']) && ($c['config']['open_enrollment_only'] == 0 || ($c['config']['open_enrollment_only'] == 1 && $this->isOpenEnrollment()))) {
				/* pass if dental only app */
				if(($this->dentalApp == true && $c['config']['medical_only'] != 1) || $this->dentalApp == false) {
					$output[$i] = '<div class="row' . (isset($c['id']) ? ' sidebar-' . $c['id'] : '') . '"><div class="' . (isset($c['class']) ? $c['class'] . ' ' : '') . ' columns" ' . (!empty($c['id']) ? ' id="' . $c['id'] . '"' : '') . '>';
					$output[$i] .= (!empty($c['title']) ? '<h2>' . $c['title'] . '</h2>' : '');
					$output[$i] .= (!empty($c['subtitle']) ? '<h3>' . $c['subtitle'] . '</h3>' : '');
					$output[$i] .= (!empty($c['text']) && $c['textloc'] == 'top' ? $c['text'] : '');
					if(!empty($c['answers'])) $output[$i] .= $this->getAnswerOutput($c['answers'], $view, $review, $finalReview, $emailReview);
					if(!empty($modal) && $modal != false) $output[$i] .= '<div id="' . $modal['reveal_id'] . '" class="reveal-modal" data-reveal aria-labelledby="' . $modal['reveal_id'] . '" aria-hidden="true" role="dialog">' . $modal['text'] . '<a class="close-reveal-modal" aria-label="Close">&#215;</a></div>';
					if(!empty($c['row'])) $output[$i] .= $this->getRowOutput2($c['row'], $view, $review, $finalReview, $emailReview);
					$output[$i] .= (!empty($c['text']) && $c['textloc'] == 'bottom' ? $c['text'] : '');
					$output[$i] .= '</div></div>';
					$i++;
				}
			}
		}
		return join('', $output);
	}
	
	public function getConfig() {
		return json_encode($this->config);
	}
	
	public function getEmail ($data) {
		$email = '';
		for($o = 1; $o <= count($this->step); $o++) {
			$email .= (!empty($this->step[$o]['title']) ? '<h1>' . $this->step[$o]['title'] . '</h1>' : '');
			$email .= (!empty($this->step[$o]['subtext']) ? '<p class="lead">' . $this->step[$o]['subtext'] . '</p>' : '');
			$email .= (!empty($this->content[$o]) ? $this->getEmailRow('content', $o, $data) : '');
			$email .= $this->getEmailContentBottom($o, $data);
			$email .= $this->getEmailFooter($this->footer, $o, $data);
			$email .= '<hr />';
		}
		return $email;
	}
	
	protected function getEmailAns ($ans, $data) {
		$output = array();
		$i = 0;
		foreach($ans as $a) {
			if($a['open_enrollment'] == 0 || ($a['open_enrollment'] == 1 && $this->isOpenEnrollment())) {
				$output[$i] = '<table>';
				$dValue = '';
				foreach($data as $k => $v) {
					if($k = $a['name']) $dValue = $v;
				}
				switch($a['type']) {
					case 'text':
					case 'password':
					case 'number':
					case 'email':
					case 'tel':
					case 'range':
					case 'date':
					case 'month':
					case 'week':
					case 'time':
					case 'datetime':
					case 'datetime-local':
					case 'url':
					case 'color':
					case 'select':
					case 'datalist':
					case 'textarea':
						$output[$i] .= '<tr><td>';
						$output[$i] .= (!empty($a['label']) ? $a['label'] : '');
						$output[$i] .= '</td><td>';
						$output[$i] .= $dValue;
						$output[$i] .= '</td></tr>';
					case 'checkbox':
					case 'radio':
						$output[$i] .= '<tr><td>';
						$output[$i] .= (!empty($a['label']) ? $a['label'] : '');
						$output[$i] .= '</td><td>';
						$output[$i] .= (!empty($a['value']) && $dValue == $a['value'] ? 'checked' : '');
						$output[$i] .= '</td></tr>';
						break;
				}
				$output[$i] .= '</table>';
				$output[$i] .= ($a['linebreak'] == 1 ? '<br />' : '');
				$i++;
			}
		}
		return join('', $output);
	}
	
	protected function getEmailCol ($col, $panel, $data) {
		$output = array();
		$i = 0;
		foreach($col as $c) {
			$output[$i] = '';
			$output[$i] .= ($panel == 1 ? '<table style="background:#ccc;">' : '');
			$output[$i] .= (!empty($c['title']) ? '<h3>' . $c['title'] . '</h3>' : '');
			$output[$i] .= (!empty($c['subtitle']) ? '<h4>' . $c['subtitle'] . '</h4>' : '');
			$output[$i] .= (!empty($c['text']) && $c['textloc'] == 'top' ? $c['text'] : '');
			if(!empty($c['answers'])) $output[$i] .= $this->getEmailAns($c['answers'], $data);
			if(!empty($c['row'])) $output[$i] .= $this->getEmailRow2($c['row'], $data);
			$output[$i] .= (!empty($c['text']) && $c['textloc'] == 'bottom' ? $c['text'] : '');
			$output[$i] .= ($panel == 1 ? '</table>' : '');
			$i++;
		}
		return join('', $output);
	}
	
	protected function getEmailContentBottom ($page, $data) {
		$output = '';
		$output .= (!empty($this->content_bottom[0]) ? $this->getEmailRow('content_bottom', 0, $data) : '');
		$output .= (!empty($this->content_bottom[$page]) ? $this->getEmailRow('content_bottom', $page, $data) : '');
		return $output;
	}
	
	protected function getEmailFooter ($cFoot, $page, $data) {
		$output = '';
		$output .= (!empty($this->footer[0]) ? $this->getEmailRow('footer', 0, $data) : '');
		$output .= (!empty($this->footer[$page]) ? $this->getEmailRow('footer', $page, $data) : '');
		return $output;
	}
	
	protected function getEmailRow ($content, $page, $data) {
		$output = array();
		$i = 0;
		foreach($this->{$content}[$page] as $o) {
			foreach($o as $p) {
					/* Pass start/end year check and OE. Now show the questions */
					if(($p['config']['startyear'] <= $this->year && $this->year <= $p['config']['endyear']) && ($p['config']['open_enrollment_only'] == 0 || ($p['config']['open_enrollment_only'] == 1 && $this->isOpenEnrollment()))) {
						/* pass if dental only app */
						if(($this->dentalApp == true && $p['config']['medical_only'] != 1) || $this->dentalApp == false) {
							$output[$i] = (!empty($p['cell']) ? $this->getEmailCol($p['cell'], $p['panel'], $data) : '');
							$i++;
						}
					}
			}
		}
		return join('', $output);
	}
	
	protected function getEmailRow2 ($row, $data) {
		$output = array();
		$i = 0;
		foreach($row as $o) {
			foreach($o as $p) {
				/* Pass start/end year check and OE. Now show the questions */
				if(($p['config']['startyear'] <= $this->year && $this->year <= $p['config']['endyear']) && ($p['config']['open_enrollment_only'] == 0 || ($p['config']['open_enrollment_only'] == 1 && $this->isOpenEnrollment()))) {
					/* pass if dental only app */
					if(($this->dentalApp == true && $p['config']['medical_only'] != 1) || $this->dentalApp == false) {
						$output[$i] = (!empty($p['cell']) ? $this->getEmailCol($p['cell'], $p['panel'], $data) : '');
						$i++;
					}
				}
			}	
		}
		return join('', $output);
	}
	
	public function getNumberofSteps() {
		return count($this->step);
	}
	
	protected function isOpenEnrollment() {
		$oeStart = DateTime::createFromFormat($this->config['date_format'],$this->config['oe_start']);
		$oeEnd = DateTime::createFromFormat($this->config['date_format'],$this->config['oe_end']);
		$today = new DateTime;
		return($oeStart < $today && $today < $oeEnd ? true : false);
	}
	
	/* Test Functions Below */
	
	public function getArray($arr) {
		return $this->{$arr};
	}
	
	public function getDisplay() {
		return $this->display;
	}
}