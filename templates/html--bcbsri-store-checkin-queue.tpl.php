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
      <div class="large-9 large-push-3 columns" id="table">
          
          <div class="row">
            <div class="small-12 columns">
              <div>
                <h4 id="storename"></h4>
              </div>
            </div>
          </div>
      
          <div class="row">
            <div class="small-6 columns select">
              <label for="waittimevar">Set Wait Time</label>
              <select id="waittimevar" class="radius"> 
                <option value="0 - 15 minutes">0 - 15 m</option>
                <option value="15 - 30 minutes">15 - 30 m</option>
                <option value="30 - 45 minutes">30 - 45 m</option>
                <option value="45 - 60 minutes">45 - 60 m</option>
                 <option value="60 - 75 minutes">60 - 75 m</option>
                 <option value="75 - 90 minutes">75 - 90 m</option>
                 <option value="90 - 105 minutes">90 - 105 m</option>
                 <option value="105 - 120 minutes">105 - 120 m</option>
                 <option value="120+ minutes">120+ m</option>
              </select>
            </div>
          </div>
          <div class="row">
            <div class="small-12 columns">
              <div class="btn-primary submitButton" data-reveal-id="checkin" id="checkinbutton">
                <div class="button small radius">Check-In</div>
              </div>
            </div>
          </div>
          
          <div class="row">
            <div class="small-12 columns"><h3>Walk-In Wait List</h3></div>
          </div>
          
          <div id="table">
            <div class="row">
              <div class="small-2 columns">Name</div>
              <div class="small-2 columns">Reason for Visit</div>
              <div class="small-2 columns">Product</div>
              <div class="small-2 columns">Segment</div>
              <div class="small-4 columns">Wait Time</div>
            </div>
            <hr />
          </div>
          
          <div id="visitorcontainer" class="row">
          </div>
          
      </div>
      
      <div class="large-3 large-pull-9 columns">
        <div class="panel">
          <section class="section">
            <p class="title"><a href="#" id="queuelink">Queues</a></p>
          </section>
          <section class="section">
            <p class="title"><a href="/yourbluestore/yhXS7LpB/admin">Admin</a></p>
          </section>
          <section class="section">
            <p class="title"><a href="#" id="storelink">Store Display</a></p>
          </section>
        </div>        
      </div>
    </div>
    

            <!-- Modal answer with form -->
            <div id="checkin" class="reveal-modal medium" data-reveal aria-labelledby="continuecode" aria-hidden="true" role="dialog">
              <form>
                <div class="row">
                  <div class="medium-12 columns">
                    <p class="lead">Customer Information</p>
                  </div>
                </div>
                <div class="row">
                  <div class="medium-12 columns">
                    <div id="nameError" class="errorDiv nameError" style="display:none">Please enter a first name.</div>
                    <label>First Name<span class="required">*</span></label>
                    <input type="text" placeholder="First Name" class="radius"  id="first_name"/>
                    
                  </div>
                </div>
                <div class="row">
                  <div class="medium-12 columns">
                    <div id="lnameError" class="errorDiv lnameError" style="display:none">Please enter a last name.</div>
                    <label>Last Name<span class="required">*</span></label>
                    <input type="text" placeholder="Last Name" class="radius"  id="last_name"/>
                    
                  </div>
                </div>
                <div class="row">
                  <div class="medium-12 columns">
                    <div id="emailError" class="errorDiv emailError" style="display:none">Please enter a valid e-mail address.</div>
                    <label>E-mail address</label>
                    <input type="text" placeholder="E-mail address" class="radius"  id="email"/>
                  </div>
                </div>
                <div class="row">
                  <div class="medium-12 columns">
                    <label>Phone number</label>
                    <input type="text" placeholder="Phone" class="radius"  id="phone"/>
                  </div>
                </div>
                <div class="row">
                  <div class="medium-12 columns">
                    <p class="lead">Visit Details</p>
                  </div>
                </div>
                <div class="row">
                  <div class="medium-12 columns">
                    <div id="reasonError" class="errorDiv reasonError" style="display:none">Please select a reason for visit.</div>
                    <label>Reason for visit<span class="required">*</span></label>
                    <input name="reason_for_visit" type="radio" class="radius"  id="reason_cs" value="cs"/>
                    <label for="reason_cs">Customer Service</label>
                    <input name="reason_for_visit" type="radio" class="radius"  id="reason_sales" value="s"/>
                    <label for="reason_sales">Sales</label>
                  </div>
                </div>
                <div class="row">
                  <div class="medium-12 columns select">
                    <label>Product<span class="required">*</span></label>
                    <div id="productError" class="errorDiv productError" style="display:none">Please select a product.</div>
                    <select id="product" name="product" class="radius">
                      <option value="selectone" selected>Select One</option>
                      <option value="Medical">Medical</option>
                      <option value="Dental">Dental</option>
                      <option value="Medical + Dental">Medical + Dental</option>
                      <option value="Vision">Vision</option>
                      <option value="Wellness">Wellness</option>
                      <option value="BillPay">Bill Pay</option>
                    </select>
                  </div>
                </div>
                <div class="row">
                  <div class="medium-12 columns select">
                    <div id="segmentError" class="errorDiv segmentError" style="display:none">Please select a segment.</div>
                    <label>Segment<span class="required">*</span></label>
                    <select id="segment" name="segment" class="radius">
                      <option value="selectone" selected>Select One</option>
                      <option value="Medicare Advantage">Medicare Advantage</option>
                      <option value="Plan 65">Plan 65</option>
                      <option value="Commercial">Commercial</option>
                      <option value="Direct">Direct</option>
                      <option value="HSRI">HSRI</option>
                      <option value="Provider">Provider</option>
                    </select>
                  </div>
                </div>
                <div class="row">
                  <div class="medium-6 columns">
                    <input type="checkbox" class="radius"  id="signature"/>
                    <label for="signature">Signature Received</label>
                  </div>
                </div>
                <div class="row">
                  <div class="medium-6 columns">
                    <input type="checkbox" class="radius"  id="appointment"/>
                    <label for="appointment">Appointment</label>
                  </div>
                </div>
                <div class="row">
                  <div class="medium-12 columns">
                    <label>Appointment with</label>
                    <input type="text" placeholder="Appointment with" class="radius"  id="appointment_with"/>
                  </div>
                </div>
                 <div class="row">
                  <div class="medium-12 columns">
                    <label>Appointment time</label>
                    <input type="time" placeholder="00:00 A.M." class="radius"  id="appointment_time"/>
                  </div>
                </div>
                <div class="row">
                  <div class="medium-12 columns text-center">
                    <div class="btn-primary" id="check-in"><a href="#" class="button radius">Check-In</a> </div>
                    <div class="btn-primary" id="edit-visitor"><a href="#" class="button radius">Edit Info</a> </div>
                  </div>
                </div>
                <div id="visitor_id" style="display:none"></div>
              </form>
              <a class="close-reveal-modal" aria-label="Close">&#215;</a>
            </div>
            
    <div id="visitorinfo" class="reveal-modal small" data-reveal aria-labelledby="continuecode" aria-hidden="true" role="dialog">
      <div class="row">
        <div class="medium-6 columns">
          <label>
            <b>Customer Name</b><br/>
            <div id="visitor_name"></div>
          </label>
        </div>
      </div>
      <br />
      <div class="row">
        <div class="medium-6 columns">
          <label>
            <b>Phone</b><br/>
            <div id="visitor_phone"></div>
          </label>
        </div>
      </div>
      <br />
      <div class="row">
        <div class="medium-6 columns">
          <label>
            <b>E-mail Address</b><br/>
            <div id="visitor_email"></div>
          </label>
        </div>
      </div>
      <br />
      <div class="row">
        <div class="medium-6 columns">
          <label>
            <b>Appointment with</b><br/>
            <div id="visitor_appointmentwith"></div>
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