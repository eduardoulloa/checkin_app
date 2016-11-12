/*$.fn.serializeObject = function() {
	var o = {};
	var a = this.serializeArray();
	$.each(a, function() {
		if (o[this.name]) {
			if (!o[this.name].push) {
				o[this.name] = [o[this.name]];
			}
			o[this.name].push(this.value || '');
		} else {
			o[this.name] = this.value || '';
		}
	});
	return o;
};*/

// Variable to control the timeout check.
var globalTimeCheck;

/*
 * Timeout Check, returns true or false.
 * 
 * Runs every minute on loop until timed out, as long as auth code has been generated.
 * TODO: Might have to add to data the auth code
 */
function bcbsri_timeout_passed() {
  var checkTimeoutUrl = "/shop/application/auth/timeout-check";
  var timeCheck = $.ajax({type: 'POST', url: checkTimeoutUrl, data: "", dataType: "json"});
  
  timeCheck.done(function (data) {
    /*
     * data should return as the following:
     * data.curTimestamp - Timestamp format since epoc
     * data.curTime - Time in string readable format
     * data.expireTime - Time limit that can be reached before clearing
     * data.modTimestamp - Timestamp format since epoc
     * data.modTime - Time in string readable format
     * data.timeElapsed - Time that has elapased since last change
     */
    if(data.success && data.expired){
        // Clear the interval (which is initially set on the 'on ready' function).
        clearInterval(globalTimeCheck);
        // Clear the info, i.e., the cookie.
        var clearDataUrl = "/shop/application/clear";
        $.ajax({type: 'POST', url: clearDataUrl, data: "", dataType: "json"});
        if( $('.app-config .timeoutCounter').length > 0 && data.timeRemaining <= 0){
          // Display the modal.
           $('#appTimeout').foundation('reveal', 'open');
        }
        return true;
    } else if(data.success && data.expired == false){
       // Update the DOM element here, with the remaining time in minutes.
       if( $('.app-config .timeoutCounter').length > 0){
         $('.app-config .timeoutCounter').html(data.timeRemaining);
       } 
    }
    // Time out hasn't expired.
    return false;
    
  });
}

/*
 * Timeout update
 * Updates the end date of auth code.
 * TODO: Might have to add to data the auth code
 */
function bcbsri_timeout_update() {
  var url = "/shop/application/auth/timeout-update";
  var timeUpdate = $.ajax({type: 'POST', url: url, data: "", dataType: "json"});
  timeUpdate.done(function (data) {
    if(data.success) return true;
    else return false
  });
}

/*
 * product - The list of products purchased in json. 
 *   EX: {plan: '2016 VanatageBlue 1000/2000', price: '0.00', apptype: 'Direct Pay or Medicare', productType: 'New or Plan Change'}
 * auth - Is not used because we are only getting a sense of the page loads.
 * quoteFor - This is the overall quote breakdown. Medicare will allways be myself.
 *            Ex: myself, myself-spouse, myself-kids
 *
 * Must be called prior to ga('send', 'pageview')
 */
function bcbsri_gaecommerce_addImpression (product, auth, quoteFor) {
  ga('ec:addImpression', {
    //'id': id,  // optional if name is set
    'name': product.plan, // optional if id is set
    'category': product.appType,
    'brand': product.productType,
    'price': product.price,
    'variant': quoteFor
  });
}

/*
 * product - The list of products purchased in json. 
 *   EX: {plan: '2016 VanatageBlue 1000/2000', price: '0.00', apptype: 'Direct Pay or Medicare', productType: 'New or Plan Change'}
 * auth - is the transaction that was used by the application database.
 * quoteFor - This is the overall quote breakdown. Medicare will allways be myself.
 *            Ex: myself, myself-spouse, myself-kids
 */
function bcbsri_gaecommerce_addProduct (product, auth, quoteFor) {
  ga('ec:addProduct', {
    //'id': id,  // optional if name is set
    'name': product.plan, // optional if id is set
    'category': product.appType,
    'brand': product.productType,
    'price': product.price,
    'variant': quoteFor
  });
  ga('ec:setAction', 'add', {
    'id': auth
  });
  ga('send', 'event', 'Shopping', 'click', 'add to cart');
}

/*
 * product - The list of products purchased in json. 
 *   EX: {plan: '2016 VanatageBlue 1000/2000', price: '0.00', apptype: 'Direct Pay or Medicare', productType: 'New or Plan Change'}
 * auth - is the transaction that was used by the application database.
 * quoteFor - This is the overall quote breakdown. Medicare will allways be myself.
 *            Ex: myself, myself-spouse, myself-kids
 */
function bcbsri_gaecommerce_removeProduct (product, auth, quoteFor) {
  ga('ec:addProduct', {
    //'id': id,  // optional if name is set
    'name': product.plan, // optional if id is set
    'category': product.appType,
    'brand': product.productType,
    'price': product.price,
    'variant': quoteFor
  });
  ga('ec:setAction', 'remove', {
    'id': auth
  });
  ga('send', 'event', 'Shopping', 'click', 'removed from cart');
}

/*
 * product - The list of products purchased in json. 
 *           EX: {medical: 'Plan Name,Price', dental: 'Plan Name,Price', apptype: '', productType: ''}
 * auth - is the transaction that was used by the application database.
 * affiliation - Is the person that submitted. Ex: Self, Retail, ...
 * quoteFor - This is the overall quote breakdown. Medicare will allways be myself.
 *            Ex: myself, myself-spouse, myself-kids
 */
function bcbsri_gaecommerce_productPurchased (product, auth, affiliation, enrollmentReason, quoteFor) {
  var total = 0;
  if(product.medical != '') {
    var p = product.medical.split(',');
    total += parseInt(p[1]);
    ga('ec:addProduct', {
      //'id': id,  // optional if name is set
      'name': p[0], // optional if id is set
      'category': product.appType,
      'brand': product.productType,
      'price': p[1],
      'variant': quoteFor
    });
  }
  if(product.dental != '') {
    var p = product.dental.split(',');
    total += parseInt(p[1]);
    ga('ec:addProduct', {
      //'id': id,  // optional if name is set
      'name': p[0], // optional if id is set
      'category': product.appType,
      'brand': product.productType,
      'price': p[1],
      'variant': quoteFor
    });
  }
  ga('ec:setAction', 'purchase', {
    'id': auth,
    'affiliation': affiliation,
    'revenue': total,
    'list': enrollmentReason
  });
}

/*
 * Additional way to measure checkout process.
 * Must be called prior to ga('send', 'pageview')
 *
 * auth - is the transaction that was used by the application database.
 * appType - Should be sent in the format of the application type with type of app. ex: Direct Pay New or Direct Pay Plan Change
 * appYear - Is the year of the application
 * ga_page - is the same as I send for events.
 */
function bcbsri_gaecommerce_checkoutSteps (auth, page, appType, appYear, ga_page) {
  ga('ec:setAction', 'checkout', {
    'id': auth,
    'step': page,
    'option': 'ONLINE_APP__' + appYear + '_' + appType + '_' + ga_page
  });
}

/*
 * This function registers the scripts that must be run on page load.
 *
 * Among its purposes, it starts and controls the timeout of an existing application with a loop. 
 * The interval is set to 60 seconds to check if the timeout has expired due to inactivity, in which
 * case it clears the user data and notifies the user about it.
 *
 * It also delegates the update of the timeout of an existin application to the events
 * associated with user interaction (i.e., clicks, text enters, etc.).
 */
 jQuery(document).ready(function() {
     
    //Hide error messages for the resume app.
    $('#resumeError').css('display', 'none');
    $('#invalidNameResume').css('display', 'none');
    $('#invalidDobResume').css('display', 'none');
    $('#invalidResumeCode').css('display', 'none'); 
    $('#resumeErrorData').css('display', 'none');
    $('#resumeErrorServer').css('display', 'none');
    $('#invalidEmail').css('display', 'none');
    $('#invalidName').css('display', 'none');
    $('#invalidDob').css('display', 'none');
    $('#confirmed').css('display', 'none');
    $('#confirmedResend').css('display', 'none');
    
    // Register callback functions upon closing of the modal.
    $(document).on('closed.fndtn.reveal', '#continuecode[data-reveal]', function () {
      $('#invalidEmail').css('display', 'none');
      $('#resumeErrorData').css('display', 'none');
      $('#resumeErrorServer').css('display', 'none');
      $('#invalidName').css('display', 'none');
      $('#invalidDob').css('display', 'none');
      $('#confirmedResend').css('display', 'none');
    });
     
    // Check to see if there's an existing application.
    var authUrl = "/shop/application/auth";
    var auth = $.ajax({type: 'POST', url: authUrl, data: "", dataType: "json"});
    auth.done(function() {
      //console.log('the auth that was fetched is '+auth.auth);
      // Update the timeout of an application, if it exists.
      bcbsri_timeout_update();
       
      // Set the interval for the timeout check to 60 s.
      globalTimeCheck = setInterval(bcbsri_timeout_passed, 60000);
      
      // Identify the device.
      var ua = navigator.userAgent, event = (ua.match(/iPad/i)) ? "touchstart" : "click";
       
      // Delegate the timeout update to user interaction events.
      
      $(document).delegate('input', 'change', function (e) {
        bcbsri_timeout_update();
      });
      $(document).delegate('input[type=radio], input[type=checkbox]', 'change', function (e) {
        bcbsri_timeout_update();
      });
      $(document).delegate('.submitSFButton', event, function (e) {
        bcbsri_timeout_update();
      });
      $(document).delegate('.edit-page', event, function (e) {
        bcbsri_timeout_update();
      });
      $(document).delegate('select', 'change', function (e) {
        bcbsri_timeout_update();
      });
      $(document).delegate('.close-reveal-modal', event, function (e) {
        bcbsri_timeout_update();
      });
      $(document).delegate('.quotesubmit', event, function (e) {
        bcbsri_timeout_update();
      });
      $(document).delegate('a.button', event, function (e) {
        bcbsri_timeout_update();
      });
      $(document).delegate('.quoteedit', event, function (e) {
        bcbsri_timeout_update();
      });
      $(document).delegate('#resume_continue_button', event, function (e) {
        
        // Clear errors. 
        $('#resumeError').css('display', 'none');
        $('#invalidNameResume').css('display', 'none');
        $('#invalidDobResume').css('display', 'none');
        $('#invalidResumeCode').css('display', 'none');
        $('#confirmed').css('display', 'none');
         /***/
         var resumek = $('#resumek').val();
         var name = $('#resume_name').val();
         var dob = $('#resume_dob').val();
         
         if(($('#resume_name').length > 0 && $('#resume_name').val() != '') && ($('#resume_dob').length > 0 && $('#resume_dob').val() != '') && ($('#resumek').length > 0 && $('#resumek').val() != '')) {
            var url = "/shop/resumesaved";
            var dataToSend = 'resumek='+resumek+'&name='+name+'&dob='+dob;
            var check = $.ajax({type: 'POST', url: url, data: dataToSend, dataType: "json"});
             check.done(function (data) {
                if(data.success == true){
                  var apptype = '';
                  cookieUrl = '/shop/application/cookie/get';
                  dataToBeSent = 'auth=' + data.auth + '&form_year=' + data.app_year + '&form_cookie=planDetails';
                  var planDetails = $.ajax({type: 'POST', url: cookieUrl, data: dataToBeSent, dataType: "json"});
                  var shopUrl;
                  
                  if(data.app_type == 'medicare'){
                    url = '/medicare/shop/'+data.app_year+'/bluechip-for-medicare/application';
                    shopUrl = '/medicare/shop/'+data.app_year+'/bluechip-for-medicare';
                  }else if (data.app_type == 'medicare-plan-change'){
                    url = '/medicare/shop/'+data.app_year+'/bluechip-for-medicare/plan-change';
                    shopUrl = '/medicare/shop/'+data.app_year+'/bluechip-for-medicare';
                  }else if(data.app_type == 'directpay'){
                    url = '/shop/'+data.app_year+'/individuals-and-families/application';
                    shopUrl = '/shop-for-plan/'+data.app_year;
                  }else if(data.app_type == 'directpay-plan-change'){
                    url = '/shop/'+data.app_year+'/individuals-and-families/plan-change';
                    shopUrl = '/shop-for-plan/'+data.app_year;
                  }else if(data.app_type == 'plan65'){
                    url = '/medicare/shop/'+data.app_year+'/plan65/application';
                    shopUrl = '/medicare/shop/'+data.app_year+'/bluechip-for-medicare';
                  }else if(data.app_type == 'plan65-plan-change'){
                    url = '/medicare/shop/'+data.app_year+'/plan65/plan-change';
                    shopUrl = '/medicare/shop/'+data.app_year+'/bluechip-for-medicare';
                  }
                  
                  window.location.assign(shopUrl);
                  window.open(url);
                }else{
                  
                  if(data.message == 'Confirmed.'){
                    $('#confirmed').css('display', 'block');
                  }else{
                    $('#resumeError').css('display', 'block');
                  }
                }
             });
         }else{
           if(typeof $('#resume_name').val() === 'undefined' || $('#resume_name').val() == ''){
              $('#invalidNameResume').css('display', 'block');
            }
            if(typeof $('#resume_dob').val() === 'undefined' || $('#resume_dob').val() == ''){
              $('#invalidDobResume').css('display', 'block');
            }
            if(typeof $('#resumek').val() === 'undefined' || $('#resumek').val() == ''){
              $('#invalidResumeCode').css('display', 'block');
            }
         }
      });
      /*
        var emailUrl = "/shop/application/send-resume-email";
        var dataToSend = 'rk='+resumeCode+'&saveapp_email='+sendTo;
        var emailSending = $.ajax({type: 'POST', url: emailUrl, data: dataToSend, dataType: "json"});
        emailSending.done(function (data) {
          if(data.success){
            //$('.saveappstate2').addClass('hide');
            $('.saveappstate2').css('display', 'none');
            //$('.saveappstate3').removeClass('hide');
            $('.saveappstate3').css('display', 'block');
          }else{
            $('.errorDiv.errorEmail').css('display', 'block');
          }
        });
      */
      $(document).delegate('#resend_resume_email', event, function (e) {
        //Clear all error messages.
        $('#resumeErrorData').css('display', 'none');
        $('#resumeErrorServer').css('display', 'none');
        $('#invalidEmail').css('display', 'none');
        $('#invalidName').css('display', 'none');
        $('#invalidDob').css('display', 'none');
        $('#confirmedResend').css('display', 'none');
        
        if(($('#resend_name').length > 0 && $('#resend_name').val() != '') && ($('#resend_dob').length > 0 && $('#resend_dob').val() != '') && ($('#resend_email').length > 0 && $('#resend_email').val() != '')) {
            var name = $('#resend_name').val();
            var dob = $('#resend_dob').val();
            
            //if(typeof $('#resend_email').val() !== 'undefined' && $('#resend_email').val() != ''){
            
            if (bcbsri_online_application_email_validation($('#resend_email').val())){
               email = $('#resend_email').val();
               var resumeKeyUrl = "/shop/application/search-resume-key";
               var dataToSend = 'name='+name+'&dob='+dob+'&email='+email;
               var resumeKey = $.ajax({type: 'POST', url: resumeKeyUrl, data: dataToSend, dataType: "json"});
                resumeKey.done(function (data) {
                  if(data.success == false){
                    if(data.message == 'Not found.'){
                      $('#resumeErrorData').css('display', 'block');
                    }else if(data.message == 'Confirmed.'){
                      $('#confirmedResend').css('display', 'block');
                    }else{
                      $('#resumeErrorServer').css('display', 'block');
                    }
                  }else if(data.success == true){
                    //$('#resumeErrorData').css('display', 'none');
                    $('#continuecode').foundation('reveal', 'close');
                  }
                });
            }else{
              $('#invalidEmail').css('display', 'block');
            }
          
        }else{
          // error for name, DOB or e-mail
          if(typeof $('#resend_name').val() === 'undefined' || $('#resend_name').val() == ''){
            $('#invalidName').css('display', 'block');
          }
          if(typeof $('#resend_dob').val() === 'undefined' || $('#resend_dob').val() == ''){
            $('#invalidDob').css('display', 'block');
          }
          if(typeof $('#resend_email').val() === 'undefined' || $('#resend_email').val() == ''){
            $('#invalidEmail').css('display', 'block');
          }
        }
      });
    });
 }); // End On Document Ready.
 
/*
 * This function validates if a particular application already exists.
 * It checks to see if the name of the applicant matches the record on the database.
 */
function bcbsri_validate_application_against_db(){
  var url = "/shop/application/validate";
  var formData = JSON.stringify($('form').serializeObject());
  var dataToBeSent = 'd=' + formData;
  var validation = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
  validation.done(function (data) {
    if(data.success && data.app_match) return true;
    else return false
  });
}

/*
 * This function verifies that the application data is correct. It checks for the accuracy of the data
 * on a plan change, in the event that the spouse or dependents change.
 */
function bcbsri_verify_plan_change(){
  var url = "/shop/application/verify-plan-change";
  var formData = JSON.stringify($('form').serializeObject());
  var dataToBeSent = 'd=' + formData;
  var validation = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
  validation.done(function (data) {
    if(data.success && data.info_correct) return true;
    else return false
  });
}

/*
 * This function gets an application's fields from the database.
 */
function bcbsri_get_application_fields(){
  var url = "/shop/application/get-data";
  var validation = $.ajax({type: 'POST', url: url, data: "", dataType: "json"});
  validation.done(function (data) {
    if(data.success) return data.fields;
    else return false
  });
}

function bcbsri_online_application_email_validation(email) {
	isValid = true;
	atIndex = email.indexOf("@");
	domainParts = email.split('@');
	if(domainParts.length != 2) {
		isValid = false;
	} else {
		domain = domainParts[1];
		local = domainParts[0];
		localLen = local.length;
		domainLen = domain.length;
		localRemoveSlashes = local.replace("\\\\","");
		var re = /^(([^<>()[\]\\.,;:\s@"]+(\.[^<>()[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/;
		isValid = re.test(email);
		if (localLen < 1 || localLen > 64) {
			// local part length exceeded
			isValid = false;
		} else if (domainLen < 1 || domainLen > 255) {
			// domain part length exceeded
			isValid = false;
		} else if (local[0] == '.' || local[localLen - 1] == '.') {
			// local part starts or ends with '.'
			isValid = false;
		} 
	    else if (!localRemoveSlashes.match(/^(\\\\.|[A-Za-z0-9!#%&`_=\\/$\'*+?^{}|~.-])+$/)) {
			// character not valid in local part unless 
			// local part is quoted
			if (!localRemoveSlashes.match(/^"(\\\\"|[^"])+"$/)) {
				isValid = false;
			}
		} 
		
	}
	return isValid;
}