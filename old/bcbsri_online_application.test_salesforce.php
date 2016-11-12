<?php
if (isset($_SERVER['AH_SITE_ENVIRONMENT']) && $_SERVER['AH_SITE_ENVIRONMENT'] != 'prod') {
	$to  = 'james.williams@bcbsri.org';
	$to .= ', jawilliams346614@gmail.com';
	$siteEnv = 'Sandbox';
} else {
	#$to  = 'james.williams@bcbsri.org' . ', '; 
	#$to .= 'andy.garcia@bcbsri.org';
	$to  = 'james.williams@bcbsri.org';
	$siteEnv = 'Production';
}

$subject = 'Salesforce Pulse Check Failed';
$message = '
<html>
<head>
  <title>Salesforce Pulse Check Failed</title>
</head>
<body>
  <p>Below is the information for the Salesforce Instance</p>
  <p><strong>Application:' . $year . ' ' . $alias . '<br />
  <strong>Username: </strong>' . $sfdcUsername . '<br />
  <strong>Environment: </strong>' . $siteEnv . '<br />
  <strong>WSDL Location: </strong>' . $sfWsdlPath.basename($arrSfCache['entr_wsdl']) . '<br />
  <strong>Date/Time: </strong>' . date("m-d-Y H:i:s") . '</p>
  <p><strong>Error Output: </strong>';

// To send HTML mail, the Content-type header must be set
$headers  = 'MIME-Version: 1.0' . "\r\n";
$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";

// Additional headers
#$headers .= 'To: ' . $to . "\r\n";
$headers .= 'From: BCBSRI Website<website@bcbsri.com>' . "\r\n";

global $errors;
$errors = (!empty($e->faultstring) ? $e->faultstring : curl_error($curl)) . "</p>" . (!empty($sfArray['message']) ? "<p>Code Message: " . $sfArray['message'] . "</p>" : '');
$message .= $errors . '</body></html>';
mail($to, $subject, $message, $headers);
