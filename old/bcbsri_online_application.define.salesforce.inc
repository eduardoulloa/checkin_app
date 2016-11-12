<?php
define("_SF_REDIRECT_URI_", "https://localhost/resttest/oauth_callback.php");
define("_SF_SERVICES_URI_", "/services/apexrest");
define("_SF_MA_LOOKUP_URI_", _SF_SERVICES_URI_ . "/wsifLookup");
define("_SF_MA_APP_URI_", _SF_SERVICES_URI_ . "/wsifNewApp");
define("_SF_MA_PC_URI_", _SF_SERVICES_URI_ . "/wsifPlanChg");
if(isset($_SERVER['AH_SITE_ENVIRONMENT']) && $_SERVER['AH_SITE_ENVIRONMENT'] != 'prod') $sfArray['debug'] = true;
else $sfArray['debug'] = false;
$sfLogin = array(
  'production' => array(
    'sf_username' => 'apiuser@bcbsri.com',
    'sf_password' => '@p1u5er!',
    'sf_token' => '',
    'sf_wsdl' => 'enterprise.prod.wsdl',
    'sf_client_id' => '',
    'sf_client_secret' => '',
    'sf_url' => 'https://login.salesforce.com'
	),
  'sandbox' => array(
#    'sf_username' => 'apiuser@bcbsri.com.bcbsrifull',
#    'sf_password' => 'welcome$12',
    'sf_username' => 'apiuser@bcbsri.com.devma',
    'sf_password' => '@p1u5er!',
    'sf_token' => '',
    'sf_wsdl' => 'enterprise.sandbox.wsdl',
    'sf_client_id' => '3MVG98EE59.VIHmzjrHCmcjXLysPBSJNYFX0AzoqNxk5F7VF.KBRR4j.yGGG1sEMFe5.C_Zg9x6lZzGPCh2yt',
    'sf_client_secret' => '7628006478708365834',
    'sf_url' => 'https://test.salesforce.com'
  )
 );
