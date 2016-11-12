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
<body>
    <?php print $page; ?>
	<div class="associationTagging hide"></div>
    <header>
      <div class="row show-for-medium-up">
        <div class="medium-4 columns"><a href="/"><img src="/sites/default/files/bluecross_logo.png" alt="Blue Cross and Blue Shield of RI" /></a></div>
        <div class="medium-8 columns header-info">&nbsp;</div>
      </div>
      <div class="row show-for-small-only">
        <div class="small-8 columns"><a href="/"><img src="/sites/default/files/bluecross_logo.png" alt="Blue Cross and Blue Shield of RI" /></a></div>
        <div class="small-4 columns header-info">&nbsp;</div>
      </div>
    </header><!-- /header -->
    
    <div class="errorHandling hide">
        <div class="row">
            <div class="small-1 columns exclamation"><i class="icon icon-exclamation"></i></div>
            <div class="small-11 columns errortxt"></div>
        </div>
    </div>
    
    <div class="row">      
      <!--
      <div class="medium-3 columns">
        <div class="panel">
          <section class="section">
            <p class="title">Admin</p>
          </section>
        </div>
      </div>
      -->
      
      <div class="large-9 large-push-3 columns">
          <div class="row">
            <div class="small-12 columns">
              <div id="addbutton" class="btn-primary submitButton" data-reveal-id="addstore">
                <div class="button radius">Add Store</div>
              </div>
            </div>
          </div>
          
          <div class="row">
            <div class="small-12 columns">
              <div id="removed_stores" data-reveal-id="removed_stores_modal">
                <div><a href="#">List previously removed stores</a></div>
              </div>
            </div>
          </div>
          
          <div class="row" id="storerecords">
          </div>
          
          <div class="row">
            <div class="small-12 columns">
              <div class="btn-secondary" id="gethistory">
                <a href="/yourbluestore/yhXS7LpB/gethistory"><div class="button radius expand">Get History</div></a>
              </div>
            </div>
          </div>
      </div>

      <div class="large-3 large-pull-9 columns">
        <div class="panel">
          <section class="section-q">
            <p class="title"><a href="#">Queues</a></p>
          </section>
          <section class="section">
            <p class="title"><a href="/yourbluestore/yhXS7LpB/admin">Admin</a></p>
          </section>
          <section class="section-d">
            <p class="title"><a href="#">Store Display</a></p>
          </section>
        </div>
      </div>
      
    </div>
    
<div id="removed_stores_modal" class="reveal-modal small" data-reveal aria-labelledby="continuecode" aria-hidden="true" role="dialog">
</div>

            <!-- Modal answer with form -->
            <div id="addstore" class="reveal-modal small" data-reveal aria-labelledby="continuecode" aria-hidden="true" role="dialog">
              <form>
                
                <div class="row">
                  <div class="medium-12 columns">
                    <div id="noname" class="errorDiv noname" style="display:none">Please enter a store name.</div>
                    <div id="exists" class="errorDiv exists" style="display:none">A store with that name already exists.</div>
                    <label>Store Name<span class="required">*</span></label>
                    <input type="text" placeholder="Store Name" class="radius"  id="store_name"/>
                  </div>
                </div>
                <div class="row">
                  <div class="medium-12 columns">
                    <label>Green Threshold</label>
                    <input type="text" placeholder="hh:mm:ss" class="radius"  id="green_threshold"/>
                  </div>
                </div>
                <div class="row">
                  <div class="medium-12 columns">
                    <label>Orange Threshold</label>
                    <input type="text" placeholder="hh:mm:ss" class="radius"  id="orange_threshold"/>
                  </div>
                </div>
                <div class="hide" id="store_id">
                </div>
                <div class="row">
                  <div class="medium-12 columns text-center">
                    <div class="btn-primary" id="submitstore"><a href="#" class="button radius">Submit</a> </div>
                    <div class="btn-primary" id="submitstoreedit" style="display:none"><a href="#" class="button radius">Edit</a> </div>
                  </div>
                </div>
              </form>
              <div class="row">
                <div class="medium-3 columns">
                  <a href="#" style="color:#E35205" id="removestore">Remove Store</a>
                </div>
              </div>
              <a class="close-reveal-modal" aria-label="Close">&#215;</a>
              
            </div>
            
    <div id="visitorinfo" class="reveal-modal small" data-reveal aria-labelledby="continuecode" aria-hidden="true" role="dialog">
      <div class="row">
        <div class="medium-6 columns">
          <label>
            <b>Customer Name</b><br/>
            John Smith
          </label>
        </div>
      </div>
      <br />
      <div class="row">
        <div class="medium-6 columns">
          <label>
            <b>Phone</b><br/>
            411-555-5555
          </label>
        </div>
      </div>
      <br />
      <div class="row">
        <div class="medium-6 columns">
          <label>
            <b>E-mail Address</b><br/>
            sample@sample.com
          </label>
        </div>
      </div>
      <br />
      <div class="row">
        <div class="medium-6 columns">
          <label>
            <b>Appointment with</b><br/>
            Rafael
          </label>
        </div>
      </div>
      <a class="close-reveal-modal" aria-label="Close">&#215;</a>
    </div>
    
    <div id="appTimeout" class="reveal-modal small" data-reveal="" aria-labelledby="appTimeout" aria-hidden="true" role="dialog">
        <p class="lead">For your information security, after 15 minutes of no activity, the data that you entered has been cleared.</p>
        <div class="btn btn-primary text-center">
            <div class="button radius submitButton timeoutButton">Close Window</div>
        </div>
        <!-- <a class="close-reveal-modal" aria-label="Close">×</a> -->
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