<?php
$curPath = dirname(__FILE__);
?>
<!DOCTYPE html>
<!--[if IE 8 ]><html dir="ltr" lang="<?php print $language->language; ?>" class="no-js msie ie8"> <![endif]-->
<!--[if (gte IE 9)|(gt IEMobile 7)><html dir="ltr" lang="<?php print $language->language; ?>" class="no-js msie ie9"><![endif]-->
<!--[if !(IE)]><!--><html dir="ltr" lang="<?php print $language->language; ?>" class="no-js"><!--<![endif]-->
<head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
	<?php print $head; ?>
	<title><?php print $head_title; ?></title>
	<link rel="stylesheet" href="/misc/css/foundation.css" />
	<link rel="stylesheet" href="/sites/all/modules/bcbsri_store_checkin/css/styles.css" />
	<link rel="stylesheet" href="/<?php print drupal_get_path('module', 'bcbsri_online_application'); ?>/css/style.ie8.css" />
    <link href="//cloud.webtype.com/css/4d7cb697-6eb7-4c81-9c78-fd05514f26c4.css" rel="stylesheet" type="text/css" />
    <link href="//cloud.webtype.com/css/553a5185-da3f-4310-8435-07d23252e5d7.css" rel="stylesheet" type="text/css" />
    <link rel="stylesheet" href="//code.jquery.com/ui/1.11.4/themes/smoothness/jquery-ui.css">
	<script src="/misc/js/vendor/modernizr.js"></script>
    <!--[if lt IE 9]>
    	<script src="/misc/js/vendor/html5shiv.js"></script>
	<![endif]-->
    <script src="//use.fonticons.com/d26ce12b.js"></script>
</head>
<body id="display">
  <?php print $page; ?>
	<div class="associationTagging hide"></div>
    <div class="errorHandling hide">
        <div class="row">
            <div class="small-1 columns exclamation"><i class="icon icon-exclamation"></i></div>
            <div class="small-11 columns errortxt"></div>
        </div>
    </div>

      <div class="row">
        <div class="medium-6 columns" id="clock"></div>
        <div class="medium-6 columns text-right"><img class="logo" src="/sites/all/modules/bcbsri_store_checkin/img/bcbsri-logo.svg" alt="Your Blue Store" /></div>
      </div>
      <div class="row help">
        <div class="medium-12 columns text-center">
          <h1 class="title bentonsansmedium">Help is <span class="minutes bentonsansbold" id="storewaittime"></span> away</h1>
        </div>
    </div>
    
    <div class="row" data-equalizer>
      <div class="large-6 columns">
          <h3 class="bentonsansbold">Next up for customer service:</h3>
          <div class="panel medium-6 columns" data-equalizer-watch>
            <div class="cswait">
                 <ol>
                 	<li class="cs1 hide"><i class="icon icon-calendar-clock"></i></li>
                 	<li class="cs2 hide"><i class="icon icon-calendar-clock"></i></li>
                 	<li class="cs3 hide"></li>
                 	<li class="cs4 hide"></li>
                 	<li class="cs5 hide"></li>
                 	<li class="cs6 hide"></li>
                 </ol>
            </div>
          </div>
          <div class="panel medium-6 columns" data-equalizer-watch>
            <div class="cswait">
                   <ol start="7">
                    <li class="cs7 hide"><i class="icon icon-calendar-clock"></i></li>
                    <li class="cs8 hide"><i class="icon icon-calendar-clock"></i></li>
                    <li class="cs9 hide"></li>
                    <li class="cs10 hide"></li>
                    <li class="cs11 hide"></li>
                    <li class="cs12 hide"></li>
                   </ol>
              </div>
          </div>
      </div>
      <div class="large-6 columns">
          <h3 class="bentonsansbold">Next up for sales help:</h3>
          <div class="panel medium-6 columns" data-equalizer-watch>
            <div class="cswait">
                 <ol>
                 	<li class="s1 hide"><i class="icon icon-calendar-clock"></i></li>
                 	<li class="s2 hide"><i class="icon icon-calendar-clock"></i></li>
                 	<li class="s3 hide"></li>
                 	<li class="s4 hide"></li>
                 	<li class="s5 hide"></li>
                 	<li class="s6 hide"></li>
                 </ol>
            </div>
          </div>
          <div class="panel medium-6 columns" data-equalizer-watch>
            <div class="cswait">
                 <ol start="7">
                 	<li class="s7 hide"><i class="icon icon-calendar-clock"></i></li>
                 	<li class="s8 hide"><i class="icon icon-calendar-clock"></i></li>
                 	<li class="s9 hide"></li>
                 	<li class="s10 hide"></li>
                 	<li class="s11 hide"></li>
                 	<li class="s12 hide"></li>
                 </ol>
            </div>
          </div>
      </div>
    </div>
    
    <div class="row">
    	<div class="medium-12 columns legend">
            <i class="icon icon-calendar-clock"></i> - Scheduled appointment
        </div>
    </div>
    
    <script src="//code.jquery.com/jquery-1.11.3.min.js"></script>
    <script src="//code.jquery.com/ui/1.11.4/jquery-ui.js"></script>
    <script src="/misc/js/foundation.min.js"></script> 
    <script>
        $(document).foundation();
    </script>
    <script type="text/javascript">
    <!--//--><![CDATA[//><!--
    (function(i,s,o,g,r,a,m){i["GoogleAnalyticsObject"]=r;i[r]=i[r]||function(){(i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)})(window,document,"script","//www.google-analytics.com/analytics.js","ga");ga("create", "UA-30296451-1", {"cookieDomain":"<?php print (isset($_SERVER['AH_SITE_ENVIRONMENT']) && $_SERVER['AH_SITE_ENVIRONMENT'] != 'prod' ? ($_SERVER['AH_SITE_ENVIRONMENT'] != 'test' ? '.bcbsristg.prod.acquia-sites.com' : ($_SERVER['AH_SITE_ENVIRONMENT'] != 'test2' ? '.bcbsristg2.prod.acquia-sites.com' : '.bcbsridev.prod.acquia-sites.com')) : '.bcbsri.com'); ?>"});ga("set", "anonymizeIp", true);ga("send", "pageview");
    ga('require', 'ecommerce');
    //--><!]]>
    </script>
    <script src="/<?php print drupal_get_path('module', 'bcbsri_store_checkin'); ?>/js/bcbsri_store_checkin.js"></script>
</body>
</html>