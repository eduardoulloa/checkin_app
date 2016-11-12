<?php
$curPath = dirname(__FILE__); /*getcwd();*/
include_once('bcbsri_online_application.helper.inc');

function bcbsri_online_application_report ($page = 1) {
	/*if(!is_numeric($page)) $page = 0;
	else $page--;
	$offset = $page * 20;*/
	$query = db_select('bcbsri_online_application_data', 'boad');
	$query->fields('boad', array('id', 'app_type', 'app_confirm', 'start_time', 'end_time'));
	$query->orderBy('boad.start_time', 'DESC');
	//$query->range($offset, 20);
	$result = $query->execute();

	$query2 = db_select('bcbsri_online_application_data', 'boad');
	$query2->fields('boad', array('id', 'app_type', 'app_confirm', 'start_time', 'end_time'));
	$query2->orderBy('boad.start_time', 'DESC');
	$result2 = $query2->execute();
	$count = $result2->rowCount();
		
	$header = array(t('ID'), t('Application Type'), t('Application Confirmation'), t('Start Time'), t('End Time'), t('Operations'));
	$rows = array();// Make a table for them.
	
	foreach($result as $r) {
		$rows[] = array(
			$r->id,
			$r->app_type,
			($r->app_confirm == 'NULL' ? '' : $r->app_confirm), 
			date("F j, Y g:i:s a", $r->start_time), 
			($r->end_time != 0 ? date("F j, Y g:i:s a", $r->end_time) : ''),
			t('<a href="@url">view</a>', array('@url' => url('admin/reports/applications/view/' . $r->id)))
		);
	}
	
	/*$pager = '<div style="float:right;">';
	$nextPage = $page + 2;
	$prevPage = $page;
	if($count > 20 && $offset > 20) $pager .= '<a href="admin/reports/applications/view/' . $prevPage . '"><< Previous Page</a>';
	if($count > 20 && $offset > 20 && $count < $offset - 20) $pager .= ' | ';
	if($count > 20 && $count < $offset - 20) $pager .= '<a href="admin/reports/applications/view/' . $nextPage . '">Next Page >></a>';
	$pager .= '</div>';
	$output .= $pager;*/
    $output = theme('table', array('header' => $header, 'rows' => $rows));
	return $output;
}

function bcbsri_online_application_resume ($page = 1) {
	/*if(!is_numeric($page)) $page = 0;
	else $page--;
	$offset = $page * 20;*/
	/*$query = db_select('bcbsri_online_application_data', 'boad');
	$query->fields('boad', array('id', 'app_type', 'app_confirm', 'start_time', 'end_time'));
	$query->orderBy('boad.start_time', 'DESC');
	//$query->range($offset, 20);
	$result = $query->execute();

	$query2 = db_select('bcbsri_online_application_data', 'boad');
	$query2->fields('boad', array('id', 'app_type', 'app_confirm', 'start_time', 'end_time'));
	$query2->orderBy('boad.start_time', 'DESC');
	$result2 = $query2->execute();
	$count = $result2->rowCount();
		
	$header = array(t('ID'), t('Application Type'), t('Application Confirmation'), t('Start Time'), t('End Time'), t('Operations'));
	$rows = array();// Make a table for them.
	
	foreach($result as $r) {
		$rows[] = array(
			$r->id,
			$r->app_type,
			($r->app_confirm == 'NULL' ? '' : $r->app_confirm), 
			date("F j, Y g:i:s a", $r->start_time), 
			($r->end_time != 0 ? date("F j, Y g:i:s a", $r->end_time) : ''),
			t('<a href="@url">view</a>', array('@url' => url('admin/reports/applications/view/' . $r->id)))
		);
	}*/
	
	/*$pager = '<div style="float:right;">';
	$nextPage = $page + 2;
	$prevPage = $page;
	if($count > 20 && $offset > 20) $pager .= '<a href="admin/reports/applications/view/' . $prevPage . '"><< Previous Page</a>';
	if($count > 20 && $offset > 20 && $count < $offset - 20) $pager .= ' | ';
	if($count > 20 && $count < $offset - 20) $pager .= '<a href="admin/reports/applications/view/' . $nextPage . '">Next Page >></a>';
	$pager .= '</div>';
	$output .= $pager;*/
  
    //$output = theme('table', array('header' => $header, 'rows' => $rows));
  $output = theme('table', array());
  $output = '
  <script src="https://use.fonticons.com/d26ce12b.js"></script>
<link href="http://cloud.webtype.com/css/4d7cb697-6eb7-4c81-9c78-fd05514f26c4.css" rel="stylesheet" type="text/css" />
<link href="http://cloud.webtype.com/css/553a5185-da3f-4310-8435-07d23252e5d7.css" rel="stylesheet" type="text/css" />
<script src="js/vendor/modernizr.js"></script>
<script src="js/bcbsri_online_application.js"></script>
</head>
<body>
<!--
<header>
  <div class="row show-for-medium-up">
    <div class="medium-6 columns">
      <div class="pull-left logo"><a href="/"><img src="img/bcbsri_logo.png" alt="Blue Cross and Blue Shield of RI" /></a></div>
      <div class="pull-left">
        <h1>Medicare</h1>
      </div>
    </div>
    <div class="medium-6 columns">
      <p class="contact"><i class="icon icon-lg icon-phone"></i> <a href="#contact">Contact the BlueCHiP for Medicare Sales Department</a></p>
    </div>
  </div>
  <div class="row show-for-small-only">
    <div class="small-8 columns">
      <div class="logo"><a href="/"><img src="img/bcbsri_logo.png" alt="Blue Cross and Blue Shield of RI" /></a></div>
    </div>
    <div class="small-4 columns">
      <p class="contact"><a a href="tel:+18005052583"><i class="icon icon-phone icon-3x"></i></a></p>
    </div>
  </div>
</header>
-->
<!-- /header -->

<div class="row saveapp-content">
  <div class="medium-9 medium-centered columns">
    <h1 class="text-center">Welcome back!</h1>
    <div class="panel">
      <div class="row form-wrapper">
        <div class="medium-12 columns">
          <p class="lead text-center">We saved your form right where you left off. Enter the information required below and we’ll get you going again.</p>
          <hr />
        </div>
      </div>
      <form>
        <div class="row form-wrapper">
          <div class="medium-6 columns">
            <label>First and Last Name<span class="required">*</span></label>
            <input type="text" placeholder="First and Last Name" class="radius" id="resume_name"/>
            <div class="errorDiv" id="invalidNameResume" style="display:none">Please enter a name.</div>
          </div>
          <div class="medium-6 columns">
            <label>Date of Birth<span class="required">*</span></label>
            <input type="text" placeholder="MM/DD/YYYY" pattern="month_day_year" class="radius" id="resume_dob"/>
            <div class="errorDiv" id="invalidDobResume" style="display:none">Please enter a valid date of birth.</div>
          </div>
        </div>
        <div class="row form-wrapper">
          <div class="medium-12 columns">
            <label>Continue Code</label>
            <input type="text" class="radius" id="resumek" placeholder="25jP7x2"/>
            <div class="errorDiv" id="invalidResumeCode" style="display:none">Please enter a continue code.</div>
            <div class="errorInput" id="resumeError" style="display:none">The information that you provided doesn&rsquo;t match our records. Please make sure the data is correct.</div>
            <div class="errorInput" id="confirmed" style="display:none">The application you&rsquo;re trying to resume has already been submitted. Please contact us if you have any questions.</div>
          </div>
        </div>
        <div class="row">
          <div class="medium-12 columns text-center">
            <p class="saveapp-text">Your First and Last Name and Date of Birth must match what we have on file.</p>
          </div>
        </div>
        <div class="row form-wrapper">
          <div class="medium-12 columns text-center">
            <div class="btn-primary" id="resume_continue_button"><a href="#" class="button radius">Continue</a> </div>
          </div>
        </div>
      </form>
      
      <!-- Modal Link -->
            <p class="saveapp-text"><a href="#" data-reveal-id="continuecode">Need your Continue Code?</a></p>

            <!-- Modal answer with form -->
            <div id="continuecode" class="reveal-modal small" data-reveal aria-labelledby="continuecode" aria-hidden="true" role="dialog">
              <form>
                <div class="row">
                  <div class="medium-12 columns">
                    <p class="lead">If you dont have your Continue Code on hand, well resend it to your email address.</p>
                  </div>
                </div>
                <div class="row">
                  <div class="medium-6 columns">
                    <label>First and Last Name<span class="required">*</span></label>
                    <input type="text" placeholder="First and Last Name" class="radius"  id="resend_name"/>
                    <div class="errorDiv" id="invalidName">Please enter a name.</div>
                  </div>
                  <div class="medium-6 columns">
                    <label>Date of Birth<span class="required">*</span></label>
                    <input type="text" placeholder="MM/DD/YYYY" pattern="month_day_year" class="radius" id="resend_dob"/>
                    <div class="errorDiv" id="invalidDob">Please enter a date of birth.</div>
                  </div>
                </div>
                <div class="row">
                  <div class="medium-12 columns">
                    <label>Email<span class="required">*</span></label>
                    <input type="text" placeholder="Email" class="radius" value=""/ id="resend_email">
                      <div class="errorDiv" id="invalidEmail">Please enter a valid email address.</div>
                      <div class="errorInput" id="resumeErrorData">The information that you provided doesn&rsquo;t match our records. Please make sure the data is correct.</div>
                      <div class="errorInput" id="confirmedResend" style="display:none">The application you&rsquo;re trying to resume has already been submitted. Please contact us if you have any questions.</div>
                      <div class="errorInput" id="resumeErrorServer">There was a problem processing your request. Please try again later.</div>
                  </div>
                </div>
                <div class="row">
                  <div class="medium-12 columns text-center">
                    <div class="btn-primary" id="resend_resume_email"><a href="#" class="button radius">Resend Email</a> </div>
                  </div>
                </div>
              </form>
              <a class="close-reveal-modal" aria-label="Close">&#215;</a>
            </div>
    </div>
  </div>
</div><!-- /saveapp-content -->

<footer>
  <div class="row">
    <div class="medium-12 columns text-center"> <a name="contact"></a>
      <h1>Questions?</h1>
      <p class="contact">Contact the BlueCHiP for Medicare Sales Department at 1-800-505-BLUE (2583) (TTY users should call 711).</p>
      <p class="dates">Hours are Monday - Friday, 8:00 a.m. to 8:00 p.m. You can use our automated answering system outside of these hours.</p>
      <p class="disclaimers">Copyright &copy; 2016 Blue Cross & Blue Shield of Rhode Island. All Rights Reserved. All trademarks unless otherwise noted are the property of Blue Cross & Blue Shield of Rhode Island or the Blue Cross and Blue Shield Association. You must continue to pay your Medicare Part B premium. This information is not a complete description of benefits. Contact the plan for more information. Limitations, copayments, and restrictions may apply. Benefits, premiums and/or co-payments/co-insurance may change on January 1 of each year. The formulary, pharmacy network, and/or provider network may change at any time. You will be notified when necessary. Blue Cross & Blue Shield of Rhode Island is an HMO plan with a Medicare contract. Enrollment in Blue Cross & Blue Shield of Rhode Island depends on contract renewal. An independent licensee of the Blue Cross and Blue Shield Association.</p>
    </div>
  </div>
</footer>
<!-- /footer --> 

<!-- jQuery & Foundation JS --> 
<script src="js/vendor/jquery.js"></script> 
<script src="js/foundation.min.js"></script> 
<script>
      $(document).foundation();
    </script>
</body>
        ';
	return $output;
}

function bcbsri_online_application_reportview ($id) {
	$query = db_select('bcbsri_online_application_data', 'boad');
	$query->fields('boad', array('id', 'app_type', 'app_confirm', 'auth', 'skey', 'fn', 'ln', 'email', 'phone', 'enter_by', 'start_time', 'end_time', 'fields', 'ua_info'));
	$query->condition('boad.id', $id);
	$result = $query->execute();
	
	$output = '';
		
	foreach($result as $r) {
		$output .= '<p><strong>Name:</strong> ' . (!empty($r->fn) ? bcbsri_online_application_decrypt($r->fn, $r->auth, $r->skey) : '') . ' ' . (!empty($r->ln) ? bcbsri_online_application_decrypt($r->ln, $r->auth, $r->skey) : '') . '</p>';
		$output .= '<p><strong>Phone:</strong> ' . (!empty($r->phone) ? bcbsri_online_application_decrypt($r->phone, $r->auth, $r->skey) : '') . '</p>';
		$output .= '<p><strong>Email:</strong> ' . (!empty($r->email) ? bcbsri_online_application_decrypt($r->email, $r->auth, $r->skey) : '') . '</p>';
		$output .= '<p><strong>Application Start Time:</strong> ' . ($r->start_time != 0 ? date("F j, Y g:i:s a", $r->start_time) : '') . '</p>';
		$output .= '<p><strong>Application Last Modified Time:</strong> ' . ($r->end_time != 0 ? date("F j, Y g:i:s a", $r->end_time) : '') . '</p>';
		$output .= '<p><strong>Application Confirmation Number:</strong> ' . (!empty($r->app_confirm) ? $r->app_confirm : '') . '</p>';
		$output .= '<hr /><p><strong>Entered By:</strong></p>';
		if(!empty($r->enter_by)) {
			$eBy = explode('|', bcbsri_online_application_decrypt($r->enter_by, $r->auth, $r->skey));
			$output .= '<ul style="list-style-type: none; display: table;">';
			foreach($eBy as $e) $output .= '<li>' . $e . '</li>';
			$output .= '</ul>';
		}
		$output .= '<hr /><p><strong>Fields Entered:</strong></p>';
		if(!empty($r->fields)) {
			$output .= '<ul style="list-style-type: none; display: table;">';
			$data = json_decode($r->fields, true);
			foreach($data as $key => $value) {
				if(!empty($value)) $output .= '<li style="float: left;width: 33%;">' . $key . '</li>';
			}
			$output .= '</ul>';
		}
		$output .= '<hr /><p><strong>User String:</strong> ' . $r->ua_info . '</p>';
		/* if(!empty($r->ua_info)) {
			$output .= '<ul style="list-style-type: none;display:table;">';
			$data = json_decode($r->ua_info, true);
			foreach($data as $key => $value) {
				if(!empty($value)) $output .= '<li style="float: left;width: 33%;"><strong>' . $key . ':</strong> ' . $value . '</li>';
			}
			$output .= '</ul>';
		} */
	}
	
    return $output;
}