$.fn.serializeObject = function() {
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
};

var confirmOnPageExit = function (e) {
    /* If we haven't been passed the event get the window.event */
    e = e || window.event;
    var message = 'For your security, closing the application will clear all of your personal information from the form. Click OK to clear the form or Cancel to continue with the application.';
    /* For IE6-8 and Firefox prior to version 4 */
    if (e) e.returnValue = message;
    /* For Chrome, Safari, IE8+ and Opera 12+ */
    return message;
};

function bcbsri_online_application_app_start(url) {
	var ua = navigator.userAgent;
  url += '/start';
	dataToBeSent = "uastring=" + ua;
	return $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
}

function bcbsri_online_application_checkTimeout() {
	url = "/shop/application/clear";
	//count = parseInt($('.app-config .timeoutCounter').html()) + 1;
	if(bcbsri_timeout_passed()) {
		$('#appTimeout').foundation('reveal', 'open');
		clearInterval(timeoutCounter);
    clearInterval(globalTimeCheck);
		dataToBeSent = '';
		timeoutSent = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
		timeoutSent.done(function (tData) {
			$('form').find('input').each(function(index, element) {
				if($.inArray($(this).attr('type'), ['text', 'password', 'number', 'email', 'tel', 'range', 'date', 'month', 'week', 'time', 'datetime', 'datetime-local', 'url', 'color']) >= 0) $(this).val('');
				if($.inArray($(this).attr('type'), ['radio', 'checkbox']) >= 0 && $(this).is(":checked")) $(this).prop('checked', false);
			});
			window.onbeforeunload = null;
		});
	} //else $('.app-config .timeoutCounter').html(count);
}

function bcbsri_online_application_disableNext(page) {
	$('.app-form-next>.page-content-' + page + ' .submitButton').removeClass('next-button');
	$('.app-form-next>.page-content-' + page + ' .submitButton').addClass('disabled');
}

function bcbsri_online_application_enableNext(page) {
	$('.app-form-next>.page-content-' + page + ' .submitButton').removeClass('disabled');
	$('.app-form-next>.page-content-' + page + ' .submitButton').addClass('next-button');
}

function bcbsri_online_application_errorHandling(msg, page) {
	$('.errorHandling .errortxt').html(msg);
	$('.errorHandling').show();
	bcbsri_online_application_disableNext(page);
}

function bcbsri_online_application_gaecommerce(auth, appalias, opp) {
	/* GA eCommerce */
	affiliation = '';
	category = '';
	medical_price = 0;
	dental_price = 0;
	total_price = 0;
	total = 0;
	if(opp == 1) opp = 'OPP-';
	else {
		opp = '';
		if($('.plan-medical-details .planselect').text() != '') medical_price = $('.plan-medical-details .price').text().replace("$", "");
		if($('.plan-dental-details .planselect').text() != '') dental_price = $('.plan-dental-details .price').text().replace("$", "");
		total_price = Number(medical_price) + Number(dental_price);
		if($('input[name="completing_this_application"]:checked').val() == 'broker_agent' || $('input[name="completing_this_application"]:checked').val() == 'broker_agent_other') {
			affiliation = $('select[name="broker_agent_channel"]').val();
		} else affiliation = $('input[name="completing_this_application"]:checked').val();
		category = '';
		if($('input[name="select_option_best_fits_your_needs"]').length > 0 && typeof($('input[name="select_option_best_fits_your_needs"]:checked').val()) == "string") $('input[name="select_option_best_fits_your_needs"]:checked').each(function () {category += $(this).val() + '/'});
		category = category.substr(0, category.length - 1);
		if($('input[name="something_has_changed"]').length > 0 && typeof($('input[name="something_has_changed"]:checked').val()) == "string") category += '/' + $('input[name="something_has_changed"]:checked').val();
		if($('input[name="my_coverage_changed_recently"]').length > 0 && typeof($('input[name="my_coverage_changed_recently"]:checked').val()) == "string") category += '/' + $('input[name="my_coverage_changed_recently"]:checked').val();
		if($('input[name="receive_state_assistance"]').length > 0 && typeof($('input[name="receive_state_assistance"]:checked').val()) == "string") category += '/' + $('input[name="receive_state_assistance"]:checked').val();
		if($('input[name="recently_moved_or_will_move_soon"]').length > 0 && typeof($('input[name="recently_moved_or_will_move_soon"]:checked').val()) == "string") category += '/' + $('input[name="recently_moved_or_will_move_soon"]:checked').val();
		affiliation = affiliation.replace(/_|-|\s-\s/g, " ");
		category = category.replace(/_|-|\s-\s/g, " ");
	}
	if(opp == 1) total_price = 0;
	ga('ecommerce:addTransaction', {
	  'id': opp + appalias + '-' + auth,  /* Transaction ID. Required. */
	  'affiliation': affiliation,         /* Affiliation or store name. */
	  'revenue': total_price,             /* Grand Total. */
	  /*'shipping': '5', */               /* Shipping. */
	  /*'tax': '1.29' */                  /* Tax. */
	});
	ga('ecommerce:send');
	if($('.plan-medical-details .planselect').text() != '') {
		ga('ecommerce:addItem', {
		  'id': opp + appalias + '-' + auth,                               /* Transaction ID. Required. */
		  'name': opp + $('.plan-medical-details .planselect').text(),     /* Product name. Required. */
		  /*'sku': 'DD23444',*/                                            /* SKU/code. */
		  'category': category,                                            /* Category or variation. */
		  'price': medical_price,                                          /* Unit price. */
		  'quantity': '1'                                                  /* Quantity. */
		});
		/*total += Number(medical_price);*/
		ga('ecommerce:send');
	}
	if($('.plan-dental-details .planselect').text() != '') {
		ga('ecommerce:addItem', {
		  'id': opp + appalias + '-' + auth,                           /* Transaction ID. Required. */
		  'name': opp + $('.plan-dental-details .planselect').text(),  /* Product name. Required. */
		  /*'sku': 'DD23444',*/                                        /* SKU/code. */
		  'category': category,                                        /* Category or variation. */
		  'price': dental_price,                                       /* Unit price. */
		  'quantity': '1'                                              /* Quantity. */
		});
		/* total += Number(dental_price); */
		ga('ecommerce:send');
	}
}

function bcbsri_online_application_gasend(year, app, ga_page, dental) {
	ga('send', 'pageview', 'ONLINE_APP__' + year + '_' + app + '_' + ga_page);
	ga('send', 'event', 'ONLINE_APP__' + year + '_' + app, ga_page);
	newrelic.addPageAction('app-page');
	newrelic.setCustomAttribute('pageTitle', 'ONLINE_APP__' + year + '_' + app + '_' + ga_page);
}

function bcbsri_online_application_get_config(auth, url, form, year, dental) {
  url += '/get-form-config';
	dataToBeSent = 'auth=' + auth + '&form=' + form + '&form_year=' + year + '&dental=' + dental;
	return $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
}

function bcbsri_online_application_get_cookie(auth, url, year, cookie) {
  url += '/cookie/get';
	dataToBeSent = 'auth=' + auth + '&form_year=' + year + '&form_cookie=' + cookie;
	return $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
}

function bcbsri_online_application_get_covered(auth, url) {
  url += '/cart/get-coverage';
	dataToBeSent = '';
	return $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
}

function bcbsri_online_application_get_pages(auth, url, form, year, dental) {
  url += '/get-form-number-of-pages';
	dataToBeSent = 'auth=' + auth + '&form=' + form + '&form_year=' + year + '&dental=' + dental;
	return $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
}

function bcbsri_online_application_get_page(auth, url, form, page, view, year, dental) {
  url += '/get-form-pages';
	dataToBeSent = 'auth=' + auth + '&form=' + form + '&form_page=' + page + '&form_view=' + view + '&form_year=' + year + '&dental=' + dental;
	return $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
}

function bcbsri_online_application_maskValue(val, type) {
	var masked = '***-**-*';
	if(type == 'medicare_claim_number') masked += val.substr(val.length - 4);
	else if (type == 'ssn') masked += val.substr(val.length - 3);
	else masked = '**********';
	return masked;
}

function bcbsri_online_application_nextpage (currentPage, steps, year, app_alias, spousePage, depPage, medicalSelected, dentalSelected) {
	nextPage = parseInt(currentPage) + 1;
  var goToNext = false;
	if(bcsri_online_application_verify_form(currentPage)) {
		if(currentPage != steps) {
      if(app_alias == 'medicare-plan-change' && $('.page-content-' + currentPage + ' input[name="bcbsri_member_id"]').length > 0 && $('.page-content-' + currentPage + ' input[name="medicare_claim_number"]').length > 0) {
        var mid = $('input[name="bcbsri_member_id"]').val();
        var mid2 = $('input[name="medicare_claim_number"]').val();
        if(mid.length != 12 || mid.substring(0,3).toLowerCase() != 'zbm') {
          msg = '<strong>Let’s try that again</strong><br />The Member ID that you entered is not a valid BlueCHiP for Medicare ID';
          bcbsri_online_application_errorHandling(msg, currentPage);
          bcbsri_online_application_enableNext(currentPage);
          goToNext == false;
        }else {
          mid = mid.substring(3);
          dataToBeSent = 'mid=' + mid2 + '&lid=' + mid;
          lkupUrl = '/shop/application/check/member';
          var lkup = $.ajax({type: 'POST', url: lkupUrl, data: dataToBeSent, dataType: "json"});
          lkup.done(function (data) {
            if(data.success) {
              var hasDental = false
              mData = data.mLookup;
              if(mData.isActiveMember) {
                if(typeof(mData.hasDental) === 'undefined' || mData.hasDental == 'null' || mData.hasDental == null || mData.hasDental == '') hasDental = false
                else hasDental = mData.hasDental
                $('input[name="first_name"]').val(mData.fName);
                $('input[name="middle_initial"]').val(mData.mName);
                $('input[name="last_name"]').val(mData.lName);
                $('.app-config').append('<div class="hasDental">' + hasDental + '</div>');
                $('.app-config').append('<div class="currrentProd">' + mData.currProduct + '</div>');
                //var dob = mData.dob.split('-');
                //$('input[name="date_of_birth"]').val(dob[1] + '/' + dob[2] + '/' + dob[0]);
                $('input[name="date_of_birth"]').val(mData.dob);
                if(mData.hasDental && dentalSelected) nextPage += 1;
                $('.page-content-' + currentPage).hide();
                $('.page-content-' + nextPage).show();
                currentPage = nextPage;
                $('.app-config .currentPage').text(currentPage);
              }
            } else {
              // TODO: show error and goToNext should be false.
              goToNext = true;
            }
          });
          lkup.error(function (data) {
            /*if(data.errorCode == "APEX_ERROR")
              msg = '<strong>Let’s try that again</strong><br />We’re having some difficulty connecting to your plan in our systems. But that’s OK, you can still proceed with making a change to your plan.';
            } else { */
              msg = '<strong>Let’s try that again</strong><br />We’re having some difficulty connecting to your plan in our systems. But that’s OK, you can still proceed with making a change to your plan.';
            //}
            bcbsri_online_application_errorHandling(msg, currentPage);
            bcbsri_online_application_enableNext(currentPage);
            //return currentPage;
          });
        }
      } else goToNext = true;

			/* Turn it on - assign the function that returns the string */
			if(currentPage == 1) window.onbeforeunload = confirmOnPageExit;
			$("html, body").animate({ scrollTop: "0px" });
      if(goToNext) {
        $('.page-content-' + currentPage).hide();
        /* change helper text -- no spouse page */
        if(app_alias == 'directpay' && ($('.app-config #spouseAge').text() == '' || $('.app-config #spouseAge').text() == 'none') && nextPage == spousePage - 1) {
          $('.app-form-next .page-content-' + nextPage + ' .small .success').text($('.app-form-next .page-content-' + spousePage + ' .small .success').text());
          $('.app-form-next .page-content-' + (spousePage + 1) + ' .nav-previous .back-button').html($('.app-form-next .page-content-' + spousePage + ' .nav-previous .back-button').html());
          $('.review .page-' + spousePage).addClass('hide');
        }
        
        /* change helper text -- no dep page */
        if(app_alias == 'directpay' && parseInt($('.app-config #kidcount').text()) == 0 && nextPage == depPage - 1) {
          $('.app-form-next .page-content-' + nextPage + ' .small .success').text($('.app-form-next .page-content-' + depPage + ' .small .success').text());
          $('.app-form-next .page-content-' + (depPage + 1) + ' .nav-previous .back-button').html($('.app-form-next .page-content-' + depPage + ' .nav-previous .back-button').html());
          $('.review .page-' + depPage).addClass('hide');
        }
        
        /* change helper text -- no dep and no spouse page */
        if(app_alias == 'directpay' && parseInt($('.app-config #kidcount').text()) == 0 && ($('.app-config #spouseAge').text() == '' || $('.app-config #spouseAge').text() == 'none') && nextPage == spousePage - 1) {
          $('.app-form-next .page-content-' + nextPage + ' .small .success').text($('.app-form-next .page-content-' + depPage + ' .small .success').text());
          $('.app-form-next .page-content-' + (depPage + 1) + ' .nav-previous .back-button').html($('.app-form-next .page-content-' + depPage + ' .nav-previous .back-button').html());
          $('.review .page-' + depPage).addClass('hide');
          $('.review .page-' + spousePage).addClass('hide');
        }
        
        /* skip pages if no spouse or dependents */
        if(app_alias == 'directpay' && ($('.app-config #spouseAge').text() == '' || $('.app-config #spouseAge').text() == 'none') && nextPage == spousePage) {
          nextPage = parseInt(spousePage) + 1;
          //$('.review page-' + spousePage).addClass('hide');
        }
        if(app_alias == 'directpay' && parseInt($('.app-config #kidcount').text()) == 0 && nextPage == depPage) {
          nextPage = parseInt(depPage) + 1;
          //$('.review page-' + depPage).addClass('hide');
        }
        /*hide remove link if its not initial page in medicare part*/
        if(app_alias == 'medicare' && $('.plan-dental-details .removelink').not(':hidden') && currentPage > 0) {
          $('.plan-dental-details .removelink').hide();
        }
        $('.page-content-' + nextPage).show();
        currentPage = nextPage;
        bcbsri_online_application_gasend(year, app_alias, $('.app-config .ga-view-' + currentPage).html());
        bcbsri_online_application_progress(currentPage);
        if(($('#first_name').length > 0 && $('#first_name').val() != '') && ($('#last_name').length > 0 && $('#last_name').val() != '') && ($('#date_of_birth').length > 0 && $('#date_of_birth').val() != '' && $('#medicare_claim_number').length > 0 && $('#medicare_claim_number').val() != '') && year > 2016) {
          submitUrl = '/shop/application/process/send-to-salesforce';
          formData = JSON.stringify($('form').serializeObject());
          dataToBeSent = 'completed=0&sfTracking=' + $('.app-config .sfTracking').text() + '&d=' + formData;
          var sfSubmit = $.ajax({type: 'POST', url: submitUrl, data: dataToBeSent, dataType: "json"});
          sfSubmit.done(function (sfData) {
            $('.app-config .sfTracking').text(sfData.sfTracking)
          });
        }
      }
		}
	} /* No else since error handling is in verify_form */
  //bcbsri_timeout_update();
	return currentPage;
}

function bcbsri_online_application_prevpage (currentPage, steps, year, app_alias, spousePage, depPage, medicalSelected, dentalSelected) {
	nextPage = parseInt(currentPage) - 1;
	if(currentPage > 0) {
		$("html, body").animate({ scrollTop: "0px" });
		$('.page-content-' + currentPage).hide();
		if(app_alias == 'directpay' && parseInt($('.app-config #kidcount').text()) == 0 && nextPage == depPage) nextPage = parseInt(depPage) - 1;
		if(app_alias == 'directpay' && ($('.app-config #spouseAge').text() == '' || $('.app-config #spouseAge').text() == 'none') && nextPage == spousePage) nextPage = parseInt(spousePage) - 1;
    hasDentalQuestionCheck = nextPage - 1;
    if(app_alias == 'medicare-plan-change' && $('.app-config .hasDental').text().toLowerCase() == dentalSelected && $('.page-content-' + hasDentalQuestionCheck + ' input[name="bcbsri_member_id"]').length > 0 && $('.page-content-' + hasDentalQuestionCheck + ' input[name="medicare_claim_number"]').length > 0) nextPage = hasDentalQuestionCheck;
		currentPage = nextPage;
		$('.page-content-' + currentPage).show();
		bcbsri_online_application_gasend(year, app_alias, $('.app-config .ga-view-' + currentPage).html());
		bcbsri_online_application_progress(currentPage);
	} else currentPage = 1;
  bcbsri_timeout_update();
	return currentPage;
}


function bcbsri_online_application_nopcp(option, currentPage, steps, year, app_alias) {
	var choice = '', closeModal = false;
	if(option == 'closed') {
		if($('.app-config .nopcp').html() == 'continue') choice = 'continue';
		else choice = 'cancel';
	} else choice = option;
	$('.app-config .nopcp').html(choice)
	if(choice = 'continue') {
		currentPage = bcbsri_online_application_nextpage($('.app-config .currentPage').text().text(), steps, year, app_alias, spousePage, depPage, medicalSelected, dentalSelected);
    $('.app-config .currentPage').text(currentPage);
		//$('.app-config .timeoutCounter').html(0);
		closeModal = true;
	} else {
		if(option != 'closed' && choice == 'cancel') closeModal = true;
	}
	if(closeModal) {
		//$('#nopcp').foundation('reveal').foundation('reveal', 'close');
		$('div#nopcp').removeClass('open').hide().css('visibility', 'hidden');
    	$('div.reveal-modal-bg').hide();	
	}
	return currentPage;
}

function bcbsri_online_application_progress(page) {
	pBars = $('.app-config #progress_steps').html();
	for(i = 1; i <= pBars; i++) $('#progress-bar-' + i + ' .meter').css({width: '0%'});
	pProgress = $('.app-config .progress-' + page).html();
	step = 100 / pBars;
	if(typeof pProgress !== 'undefined' && pProgress != '') {
		pFull = Math.floor(pProgress / step); pNext = pFull + 1;
		pMod = pProgress % step; pMod = (pMod / step) * 100;
		for(i = 1; i <= pFull; i++) $('#progress-bar-' + i + ' .meter').css({width: '100%'});
		$('#progress-bar-' + pNext + ' .meter').css({width: pMod + '%'});
		$('.progress-bars .steps').html('Step ' + $('.app-config .step-number-' + page).html());
		$('.progress-bars').show();
	} else $('.progress-bars').hide();
}

function bcbsri_online_application_sfCheck (url, auth, year, currentPage, applicationForm, phone) {
  url += '/check-sf';
	dataToBeSent = 'auth=' + auth + '&form_year=' + year;
	msg = 'We apologize, but we are unable to process online applications at this time. Please try again later, or download the <a href="' + applicationForm + '" target="_blank">application</a>. If you prefer, you can contact us at ' + phone +'.';
	var sfCheck = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
	sfCheck.done(function (sfData) {
		if(!sfData.success) bcbsri_online_application_errorHandling(msg, currentPage);
	});
	sfCheck.fail(function (sfData) { bcbsri_online_application_errorHandling(msg, currentPage);	});
}

function bcbsri_online_application_showhide(items, showHide, step, $input) {
	items = items.split(',');
	for(ii = 0; ii < items.length; ii++) {
		if(items[ii].indexOf('modal_') >= 0) {
			ic = items[ii].split('_');
			$('#' + ic[1]).foundation('reveal', 'open');
		} else if(items[ii].indexOf('alert') >= 0) {
			bcbsri_online_application_showhideAlert($input);
		} else if(items[ii].indexOf('disable_page') >= 0) {
			bcbsri_online_application_disableNext(step);
		} else if(items[ii].indexOf('enable_page') >= 0) {
			bcbsri_online_application_enableNext(step);
		} else {
			/*rowsplit = new Array(); answersplit = 0; pagesplit = 0;
			splittxt = '.app-form-content>'; splittxt2 = '.app-form-content .review'; splittxt3 = '.final-review-cell .review';
			if(items[ii].indexOf('|') >= 0 || items[ii].indexOf('~') >= 0) {
				if(items[ii].indexOf('|') >= 0) {
					tmpsplit = items[ii].split('|');
					answersplit = tmpsplit[1];
					tmpsplit = tmpsplit[0];
				} else tmpsplit = items[ii];
				if(tmpsplit.indexOf('~') >= 0) {
					psplit = tmpsplit.split('~');
					rowsplit = psplit[1].split(':');
					pagesplit = psplit[0];
				} else rowsplit = tmpsplit.split(':');
			} else rowsplit = items[ii].split(':');
			if(pagesplit != 0) {
				splittxt += '.page-content-' + pagesplit;
				splittxt2 += ' .page-' + pagesplit;
				splittxt3 += ' .page-' + pagesplit;
			} else {
				splittxt += '.page-content-' + step;
				splittxt2 += ' .page-' + step;
				splittxt3 += ' .page-' + step;
			}
			for(ri = 0; ri < rowsplit.length; ri++) {
				if(ri > 0) {
					splittxt += ' .row-' + rowsplit[ri];
					splittxt2 += ' .row-' + rowsplit[ri];
					splittxt3 += ' .row-' + rowsplit[ri];
				} else {
					splittxt += '>.row-' + rowsplit[ri];
					splittxt2 += '>.review-row-' + rowsplit[ri];
					splittxt3 += '>.review-row-' + rowsplit[ri];
				}
			}
			if(answersplit !=0) {
				splittxt += ' div.answer-' + answersplit;
				splittxt2 += ' div.review-answer-' + answersplit;
				splittxt3 += ' div.review-answer-' + answersplit;
			}*/
			if(showHide == 'show') {
				$(items[ii]).removeClass('hide');
				//$(splittxt2).removeClass('hide');
				//$(splittxt3).removeClass('hide');
			} else {
				$(items[ii]).addClass('hide');
				//$(splittxt2).addClass('hide');
				//$(splittxt3).addClass('hide');
				$(items[ii]).find('input').each(function(index, element) {
                    if($.inArray($(this).attr('type'), ['text', 'password', 'number', 'email', 'tel', 'range', 'date', 'month', 'week', 'time', 'datetime', 'datetime-local', 'url', 'color']) >= 0) $(this).val('');
					if($.inArray($(this).attr('type'), ['radio', 'checkbox']) >= 0 && $(this).is(":checked")) $(this).prop('checked', false);
                });
			}
		}
	}
}

function bcbsri_online_application_showhideAlert($input) {
	name = $input.attr('name');
	val = $input.val();
	if($('.alert-' + name).length > 0) {
		$('.alert-' + name).each(function(index, element) {
            $(this).addClass('hide');
        });
		if($('.alert-' + name + '-' + val).length > 0) $('.alert-' + name + '-' + val).removeClass('hide');
	}
}

function bcsri_online_application_verify_form(page) {
	var verified = true, firstError = false, $firstErrorRow = 'undefined', pcpContinue = false;
	$('.app-content .page-content-' + page).find('input.error, div.error').each(function (e) { $(this).removeClass('error'); });
	$('.app-content .page-content-' + page).find('small.error').hide();
	$('.app-content .page-content-' + page + ' input').each(function (e) {
		if($(this).parents('.hide').length == 0) {
			if(!$(this).prop('required')) {
				if($(this).parents('.row-required').length > 0) {
					$parent = $(this).closest('.row-required'); childrenVerified = $('input[name="' + $(this).attr('name') + '"]:checked').length; error = false;
					$parent.find('input').each(function (f) {
						if($.inArray($(this).attr('type'), ['text', 'password', 'number', 'email', 'tel', 'range', 'date', 'month', 'week', 'time', 'datetime', 'datetime-local', 'url', 'color']) >= 0 && $.trim($(this).val()) != '') {
							if($(this).prop('pattern')) {
								if($(this).val().match($(this).prop('pattern'))) childrenVerified++;
								else {
									$(this).parent('.row').addClass('error'); $(this).addClass('error');
									error = true; $(this).parent().find('small.error').css({display: 'block'});
								}
							}
						}
					});
					if(childrenVerified <= 0 || error == true) {
						if(!firstError) {
							firstError = true; $firstErrorRow = $(this).closest('.row');
						}
						verified = false; $parent.addClass('error');
					}
				}
			} else {
				
				if($.inArray($(this).attr('type'), ['radio', 'checkbox']) >= 0 && $('input[name="' + $(this).attr('name') + '"]:checked').length < 1) {					
					verified = false;
					if(!firstError) {
						firstError = true; $firstErrorRow = $(this).closest('.row');
					}
					
				}
				
				if($.inArray($(this).attr('type'), ['text', 'password', 'number', 'email', 'tel', 'range', 'date', 'month', 'week', 'time', 'datetime', 'datetime-local', 'url', 'color']) >= 0 && $.trim($(this).val()) != '') {

					if($(this).prop('pattern')) {
						if(!$(this).val().match($(this).prop('pattern'))) {
							$(this).parent('.row').addClass('error'); $(this).addClass('error');
							verified = false; $(this).parent().find('small.error').css({display: 'block'});
							if(!firstError) {
								firstError = true; $firstErrorRow = $(this).closest('.row');
							}
						}
					}
				} 
				
				if($.inArray($(this).attr('type'), ['text', 'password', 'number', 'email', 'tel', 'range', 'date', 'month', 'week', 'time', 'datetime', 'datetime-local', 'url', 'color']) >= 0 && $.trim($(this).val()) == '') {
					verified = false; $(this).parent('.row').addClass('error'); $(this).parent().find('small.error').css({display: 'block'}); $(this).addClass('error');
					if(!firstError) {
						firstError = true; $firstErrorRow = $(this).closest('.row');
					}
				}
				
				
			}
		}
	});
	
	//for the broker agent channel select
	if(($('.app-content .page-content-' + page + ' select[name="broker_agent_channel"]').length > 0) && ($('input[name="completing_this_application"]:checked').val() == 'broker_agent' || $('input[name="completing_this_application"]:checked').val() == 'broker_agent_other')) {
					var select_val = $("select[name='broker_agent_channel'] option:selected").val().toLowerCase();	
					if(select_val == "choose one") {
						verified = false; 
						$("select[name='broker_agent_channel']").parent('.row').addClass('error'); 
						$("select[name='broker_agent_channel']").parent().find('small.error').css({display: 'block'}); 
						$("select[name='broker_agent_channel']").addClass('error');
						if(!firstError) {
							firstError = true; $firstErrorRow = $("select[name='broker_agent_channel']").closest('.row');
						}
					}
				}
	
	if($('.app-content .page-content-' + page + ' select[name="name_current_medical_insurance_carrier"]').length > 0) {
		var select_val = $("select[name='name_current_medical_insurance_carrier'] option:selected").val().toLowerCase();		
		if(select_val == "choose one") {
			verified = false; 
			$("select[name='name_current_medical_insurance_carrier']").parent('.row').addClass('error'); 
			$("select[name='name_current_medical_insurance_carrier']").parent().find('small.error').css({display: 'block'}); 
			$("select[name='name_current_medical_insurance_carrier']").addClass('error');
			if(!firstError) {
				firstError = true; $firstErrorRow = $("select[name='name_current_medical_insurance_carrier']").closest('.row');
			}
		}
	}
	
	if($('.app-content .page-content-' + page + ' select[name="name_current_dental_insurance_carrier"]').length > 0) {
		var select_val = $("select[name='name_current_dental_insurance_carrier'] option:selected").val().toLowerCase();	
		if(select_val == "choose one") {
			verified = false; 
			$("select[name='name_current_dental_insurance_carrier']").parent('.row').addClass('error'); 
			$("select[name='name_current_dental_insurance_carrier']").parent().find('small.error').css({display: 'block'}); 
			$("select[name='name_current_dental_insurance_carrier']").addClass('error');
			if(!firstError) {
				firstError = true; $firstErrorRow = $("select[name='name_current_dental_insurance_carrier']").closest('.row');
			}
		}
	}
	
	if($('.app-content .page-content-' + page + ' input[name="confirm_email_address"]').length > 0) {
		if($('input[name="confirm_email_address"]').val() != $('input[name="email_address"]').val() || ($('input[name="email_address"]').val() != '' && !bcbsri_online_application_email_validation($('input[name="email_address"]').val()))) {
			$('input[name="email_address"]').closest('.row').addClass('error');
			$('input[name="email_address"]').parent().find('small.error').css({display: 'block'});
			if(!firstError) {
				verified = false; firstError = true;
				$firstErrorRow = $('input[name="email_address"]').closest('.row');
			}
			if($('input[name="confirm_email_address"]').val() != $('input[name="email_address"]').val()) $('input[name="email_address"]').parent().find('small.error').html('Email and Confirm email do not match.');
			else $('input[name="email_address"]').parent().find('small.error').html('Please enter a valid email.');
		}
	}
	/*if($('.app-content .page-content-' + page + ' input[name="provide_pcp_name"]').length > 0 && $('.app-content .page-content-' + page + ' input[name="provide_pcp_name"]').val() == '') {
		if($('.app-content .page-content-' + page + ' input[name="provide_pcp_name"]').closest('.row').find('.hide-show-logic').children('.show').length > 0) {
			show = $('.app-content .page-content-' + page + ' input[name="provide_pcp_name"]').closest('.row').find('.hide-show-logic').children('.show').html();
			bcbsri_online_application_showhide(show, 'show', page, $('.app-content .page-content-' + page + ' input[name="provide_pcp_name"]'));
			pcpContinue = true;
		}
		pcpContinue = true;
		$('#nopcp').foundation('reveal').foundation('reveal', 'open');
	}
	if(pcpContinue == false) {*/
		if($firstErrorRow != 'undefined') oSet = $firstErrorRow.offset().top;
		else oSet = 0;
		if(!verified) $('html,body').animate({scrollTop: oSet});
		return verified;
	//} else return false;
}

$(document).ready(function() {
	var ua = navigator.userAgent, event = (ua.match(/iPad/i)) ? "touchstart" : "click", dataFlds = '';
	$('.app-loaded').hide();
	var appStart, auth, url, app_alias, app, year, pages, steps, answerNumber = 0, currentPage = 1, nextPage, appDownload = false, initialLoad = false, loaded = false, applicationForm, phoneCS, nameCS, medicalSelected = false, dentalSelected = false, numKids = 0, depPage = -1, spousePage = -1, saveAppState = false, resumeCode;
	url = "/shop/application";
	/* application start to get auth and other app details */
	appStart = bcbsri_online_application_app_start(url);
	appStart.done( function (appData) {
		if(appData.success) {
			auth = appData.auth;
			app_alias = appData.alias;
			$('.app-content').addClass(app_alias);
			app = appData.app;
			year = appData.year;
      if(year == 2017){
        //bcbsri_online_application_appauth
        // Call to the server to fetch the 'flds' field.
        var fldsUrl = "/shop/application/get-data";
        getData = $.ajax({type: 'POST', url: fldsUrl, data: "", dataType: "json"});
        getData.done(function (data) {
          if(data.success == true) dataFlds = data.fields
        });
      }
			dental = appData.dental;
			/* Setup config data */
			if(app_alias == 'directpay') $('.associationTagging').html('<iframe src="https://3042219.fls.doubleclick.net/activityi;src=3042219;type=begin249;cat=ri-be005;ord=[SessionID]?" width="1" height="1" frameborder="0" style="display:none"></iframe>');
			var appConfig = bcbsri_online_application_get_config(auth, url, app_alias, year, dental);
			appConfig.done( function (configData) {
        if(configData.success == true) {
          var items = [];
          $.each( configData, function( key, val ) {items.push( "<div id='" + key + "'>" + val + "</div>" );});
          $( "<div/>", {html: items.join("")}).appendTo( ".app-config" );
          $("<div class='sfTracking'></div>").appendTo(".app-config");
          $('.header-info').html($('.app-config #contact_info').html());
          applicationForm = $('.app-config #application_form').text();
          phoneCS = $('.app-config #error_customer_service').html();
          nameCS = $('.app-config #error_customer_service_name').html();
          if($('.app-config #kidcount').length > 0) numKids = parseInt($('.app-config #kidcount').text());
          if($('.app-config #dependentpage').length > 0) depPage = parseInt($('.app-config #dependentpage').text());
          if($('.app-config #spousepage').length > 0) spousePage = parseInt($('.app-config #spousepage').text());
          $('.app-config').append('<div class="currentPage">1</div>');
          /* Progress bars */
          pBars = $('.app-config #progress_steps').html()
          pWidth = 100 / pBars;
          for(pi = 1; pi <= pBars; pi++) {
            if(pi == pBars) pClass = ' last';
            else pClass = '';
            $('.progress-bars .progress-column').append('<div id="progress-bar-' + pi + '" class="progress progress-bar success' + pClass + '" style="width:' + pWidth + '%;"><span class="meter success"></span></div>');
          }
          
          /* Get plans we are applying for */
          plans = bcbsri_online_application_get_cookie(auth, url, year, 'planDetails');
          plans.done( function (planData) {
            if(planData.success) {
              if(planData.medical) {
                $('.plan-medical-details .plantype').html('Medical Plan');
                $('.plan-medical-details .planselect').html(year + ' ' + planData.medicalPlan);
                $('.plan-medical-details .network').html(planData.medicalNetwork);
                $('.plan-medical-details .sm-title').html('Monthly Premium');
                $('.plan-medical-details .price').html(planData.medicalCost);
                $('.plan-medical-details').show();
                medicalSelected = true;
              } else $('.plan-dental-details').hide();
              if(planData.dental) {
                $('.plan-dental-details .plantype').html('Dental Plan');
                $('.plan-dental-details .planselect').html(year + ' ' + planData.dentalPlan);
                $('.plan-dental-details .sm-title').html('Monthly Premium');
                $('.plan-dental-details .price').html(planData.dentalCost);
                $('.plan-dental-details').show();
                dentalSelected = true;
              } else $('.plan-dental-details').hide();
              bcbsri_online_application_gaecommerce(auth, app_alias, 1);
              if(planData.dental && !planData.medical) dental = true;
              else dental = false;
            } else {
              msg = 'It looks as though you haven\'t selected any plans yet. View all the options for <a href="/shop-for-plan/2016">Individuals and Families</a> or <a href="/medicare/shop/2016">Medicare</a>. Or you can call us at 1-855-690-2583.'; //'We apologize, we are unable to process online applications at this time. Please try again later, or download the <a href="' + applicationForm + '" target="_blank">application</a>. If you prefer, you can contact us at ' + phoneCS +'.';
              bcbsri_online_application_errorHandling(msg, currentPage);
            }
          });
          plans.fail( function (planData) {
            msg = 'We apologize, but we are unable to process online applications at this time. Please try again later, or download the <a href="' + applicationForm + '" target="_blank">application</a>. If you prefer, you can contact us at ' + phoneCS +'.';
            bcbsri_online_application_errorHandling(msg, currentPage);
          });
          pages = bcbsri_online_application_get_pages(auth, url, app_alias, year, dental);
          
          pages.done( function (pData) {
            review = false;
            if(pData.success) {
              steps = pData.numSteps;
              for(i = 1; i <= steps; i++) {
                currentStep = i;
                pageContent = bcbsri_online_application_get_page(auth, url, app_alias, i, 'full', year, dental);
                pageContent.done( function (pcData) {
                  $('.app-form-content').append('<div class="page-content-' + pcData.page + '">' + pcData.content + '</div>');
                  $('.app-form-content-bottom').append('<div class="page-content-' + pcData.page + '">' + pcData.content_bottom + '</div>');
                  $('#planYear').html(year);
                  $('.app-form-sidebar').append('<div class="page-content-' + pcData.page + '">' + pcData.sidebar + '</div>');
                  $('.app-form-footer').append('<div class="page-content-' + pcData.page + '">' + pcData.footer + '</div>');
                  if(pcData.page == steps && pcData.final_review != '') $('.final-review-cell').append('<div class="review">' + pcData.final_review + '</div>');
                  if(pcData.page == steps && pcData.email_review != '') $('body').append('<div class="email_review hide">' + pcData.email_review + '</div>');
                  else if(pcData.final_review != '') $('.app-form-content .page-content-' + pcData.page).append('<div class="review">' + pcData.final_review + '</div>');	
        
                  if(pcData.submit_text != '') {
                    next = '<div class="page-content-' + pcData.page + ' row btn btn-primary">';
                    if(pcData.back_text != '') next += '<div class="medium-4 columns nav-previous"><ul class="icon-ul"><li><a class="back-link back-button"><i class="icon-li icon icon-chevron-left"></i>' + pcData.back_text + '</a></li></ul></div><div class="medium-8 columns"><div class="row"><div class="medium-5 columns text-center">';
                    else next += '<div class="medium-12 columns text-center">';
                    next += '<a class="next-' + pcData.page + ' button radius submitButton';
                    if(pcData.ga_page_view == 'signsubmit') next += ' submitSFButton';
                    else next += ' next-button';
                    next += '">' + pcData.submit_text + '</a>';
                    if(pcData.submit_helper_text_success != '' || pcData.submit_helper_text_error != '') {
                      next += '<p class="small">';
                      if(pcData.submit_helper_text_success != '') next += '<span class="success">' + pcData.submit_helper_text_success + '</span>';
                      if(pcData.submit_helper_text_error != '') next += '<span class="failure">' + pcData.submit_helper_text_error + '</span>';
                      next += '</p>';
                    }							
                    
                    next += '</div></div></div></div>';
                    $('.app-form-next').append(next);
                  }
                  $('.app-form-footer .page-content-' + pcData.page + ' .last_modified').html($('.app-config #last_modified').html());
                  $('.app-form-footer .page-content-' + pcData.page + ' .cms_ohic_code').html($('.app-config #cms_ohic_code').html());
                  $('.app-config').append('Page ' + pcData.page + ' loaded<br />');
                  $('.app-config').append('<div class="ga-view-' + pcData.page + '">' + pcData.ga_page_view + '</div>');
                  $('.app-config').append('<div class="progress-' + pcData.page + '">' + pcData.progress + '</div>');
                  $('.app-config').append('<div class="step-number-' + pcData.page + '">' + pcData.step_number + '</div>');
                  if(pcData.page != 1) {
                    $('.page-content-' + pcData.page).hide();
                  } else {
                    bcbsri_online_application_gasend(year, app_alias, pcData.ga_page_view);
                    bcbsri_online_application_progress(currentPage);
                  }
                  
                  //console.log(typeof dataFlds);
                  if(dataFlds !== ''){
                    $.each( dataFlds, function( key, val ) {
                      
                      if (key.indexOf('saveapp') == -1){
                        // Validate if it's an input field.
                        if($('input[name="' + key +'"]').length > 0 && val != '') {
                          
                          // Validate if the field type is a text field.
                          if($.inArray($('input[name="' + key +'"]').attr('type'), ['text', 'password', 'number', 'email', 'tel', 'range', 'date', 'month', 'week', 'time', 'datetime', 'datetime-local', 'url', 'color']) >= 0) {
                            // Populate the field accordingly.
                            $('input[name="' + key +'"]').val(val);
                            if(key == 'medicare_claim_number') emailVal = bcbsri_online_application_maskValue(val, key);
                            else if(key.indexOf('social_security') >= 0) emailVal = bcbsri_online_application_maskValue(val, key);
                            else if($.inArray($(this).attr('type'), ['password']) >= 0) emailVal = bcbsri_online_application_maskValue(val, key);
                            else emailVal = val;
                            $('input[name="review-' + key + '"]').val(val);
                            $('input[name="review2-' + key + '"]').val(val);
                            $('.email_review span.review-' + key).each(function () { $(this).html(emailVal); });
                          }
                          
                          // Validate if the field type is a radio or a checkbox.
                          if($.inArray($('input[name="' + key +'"]').attr('type'), ['radio', 'checkbox']) >= 0){
                            
                            // Handle the show/hide logic.
                            
                           $('input[name="' + key +'"][value="'+val+'"]').attr('checked',true);
                            if($('input[name="' + key +'"][value="'+val+'"]').parent('div').children('.hide-show-logic').length > 0 && $.inArray($('input[name="' + key +'"]').attr('type'), ['radio', 'checkbox']) >= 0 && $('input[name="'+ key +'"]:checked').val() == val){
                              if($('input[name="' + key +'"][value="'+val+'"]').parent('div').children('.hide-show-logic').children('.show').length > 0) {
                                show = $('input[name="' + key +'"][value="'+val+'"]').parent('div').children('.hide-show-logic').children('.show').html();
                                bcbsri_online_application_showhide(show, 'show', pcData.step_number, $('input[name="' + key +'"][value="'+val+'"]'));
                              }
                              if($('input[name="' + key +'"][value="'+val+'"]').parent('div').children('.hide-show-logic').children('.hide').length > 0) {
                                hide = $('input[name="' + key +'"][value="'+val+'"]').parent('div').children('.hide-show-logic').children('.hide').html();
                                //if($('input[name="' + key +'"]').val() == val){
                                  bcbsri_online_application_showhide(hide, 'hide', pcData.step_number, $('input[name="' + key +'"][value="'+val+'"]'));
                                //}
                              }
                            }
                            
                            // Handle the review pages' fields.
                            var id = key +'-'+val+'[value="'+val+'"]';
                            var emailId = key + '-' + val;
                            var emailValue = '(X)';
                            //Before submission.
                            $('.app-form-content .review #review-' + id).prop('checked', true);
                            // Thank you page.
                            $('.final-review-cell #review2-' + id).prop('checked', true);
                            if(name != 'prefer_information_in_another_language' && name != 'select_option_best_fits_your_needs') $('.email_review span.review-' + id).each(function() { $('input[name="' + key +'"]').html('(_)'); });
                            //Everything that goes into the e-mail.
                            $('.email_review span.review-' + emailId ).html(emailValue);
                            
                          }
                            
                        }
                        // Validate if the field type is a select element.
                        if($('select[name="' + key +'"]').length >= 0){
                         $('select[name="' + key +'"]').val(val).change();
                        }
                     }
                      
                    });
                  }
                  
                  if($('.app-form-content .page-content-1').length > 0 && $('.app-form-content .page-content-2').length > 0 && initialLoad == false) {
                    if(appDownload == false && $('.app-download').length > 0 && $('.app-config #plan_change_form').length > 0 && $('.plan_change_form').length > 0) {
                      $('.app-download').attr('href', $('.app-config #application_form').text());
                      $('.plan_change_form').attr('href', $('.app-config #plan_change_form').text());
                      appDownload == true;
                    }
                    initialLoad = true;
                    $('.app-loading').hide();
                    $('.app-loaded').show();
                    $('.app-config').append('<div class="timeoutCounter">0</div>');
                    timeoutCounter = setInterval(bcbsri_online_application_checkTimeout, 60000);
                    bcbsri_online_application_sfCheck(url, auth, year, 1, applicationForm, phoneCS);
                    if($('.noDental').length > 0 && dentalSelected) $('.noDental').addClass('hide');
                    if($('input[name="bluechip_dental"]').length > 0 && dentalSelected) {
                      $('input[name="bluechip_dental"]').prop('checked', true);
                      $('.bluechip-dental').each(function () { $(this).addClass('hide')});
                    }
                    if(app_alias == 'directpay' && !dentalSelected) {
                      $('.app-form-sidebar .noDental').removeClass('hide');
                    }
                    /* If no plan is selected disable next button */
                    if(medicalSelected == false && dentalSelected == false) {
                      bcbsri_online_application_disableNext(1);
                    }
                  }
                  if($('.app-form-content .page-content-7').length > 0) {
                    if($('.eft-download').length > 0) {
                      $('.eft-download').attr('href', $('.app-config #eft_form').text());
                    }
                  }
                  if(pcData.page == depPage) {
                    var coveredAjax = bcbsri_online_application_get_covered(auth, url);
                    coveredAjax.done(function (caData) {
                      if(caData.success) {
                        cdep = caData.covered.dependent;
                        numDep = 0;
                        for(ik = 0; ik < numKids; ik++) {
                          temp = ik + 1;
                          //if(cdep[ik].medical == 'yes' || cdep[ik].dental == 'yes') {
                          if((cdep[ik].medical == 'yes' && medicalSelected) || (cdep[ik].dental == 'yes' && dentalSelected)) {
                            numDep++;
                            $('.depAge_' + numDep).text(cdep[ik].age);
                            if(cdep[ik].medical != 'yes' || !medicalSelected) $('.depCoveredMedical_' + numDep + ' .icon').removeClass('icon-check').addClass('icon-times');
                            if(cdep[ik].dental != 'yes' || !dentalSelected) $('.depCoveredDental_' + numDep + ' .icon').removeClass('icon-check').addClass('icon-times');
                            //if(cdep[ik].medical != 'yes' || cdep[ik].dental != 'yes') numDep++;
                          }
                        }
                        numKids = numDep;
                        $('.app-config #kidcount').text(numKids);
                        for(ik = 1; ik <= 9; ik++) {
                          if(ik > numKids) {
                            $('.enterDependent' + ik).each(function () {
                              $(this).addClass('hide');
                            });
                          }
                        }
                      }
                    });
                  }
                  if(pcData.page == spousePage) {
                    var coveredAjax = bcbsri_online_application_get_covered(auth, url);
                    coveredAjax.done(function (caData) {
                      if(caData.success) {
                        cspouse = caData.covered.spouse;
                        if((cspouse.medical == 'yes' && medicalSelected) || (cspouse.dental == 'yes' && dentalSelected)) {
                          if(cspouse.medical != 'yes' || !medicalSelected) $('.spouseCoveredMedical .icon').removeClass('icon-check').addClass('icon-times');
                          if(cspouse.dental != 'yes' || !dentalSelected) $('.spouseCoveredDental .icon').removeClass('icon-check').addClass('icon-times');
                        } else $('.app-config #spouseAge').text('');
                      }
                    });
                  }
                  if($('.final-review-cell').length > 0) $('.final-review-cell').hide();
                  if (!Modernizr.inputtypes.date) {
                    // no native support for <input type="date"> :(
                    $( ".datepicker" ).datepicker({
                    changeMonth: true,
                    changeYear: true,
                    yearRange: "-150 Years:+1 Year"
                    });
                  }
                  $(document).foundation('reflow');
                });
                pageContent.fail( function(pData) {
                  msg = 'We apologize, but we are unable to process online applications at this time. Please try again later, or download the <a href="' + applicationForm + '" target="_blank">application</a>. If you prefer, you can contact us at ' + phoneCS +'.';
                  bcbsri_online_application_errorHandling(msg, currentPage);
                });
              }
              
              
              try {
                hj('tagRecording', ['application']);
              } catch(e) {
                /* no errors output if hotjar is not available */
              }
            }/* end steps loop */
            else {
              msg = 'We apologize, but we are unable to process online applications at this time. Please try again later, or download the <a href="' + applicationForm + '" target="_blank">application</a>. If you prefer, you can contact us at ' + phoneCS +'.';
              bcbsri_online_application_errorHandling(msg, currentPage);
            }
          });
          pages.fail( function(pData) {
            msg = 'We apologize, but we are unable to process online applications at this time. Please try again later, or download the <a href="' + applicationForm + '" target="_blank">application</a>. If you prefer, you can contact us at ' + phoneCS +'.';
            bcbsri_online_application_errorHandling(msg, currentPage);
          });
        } else {
          msg = 'It looks as though you haven\'t selected any plans yet. View all the options for <a href="/shop-for-plan/2016">Individuals and Families</a> or <a href="/medicare/shop/2016">Medicare</a>. Or you can call us at 1-855-690-2583.';
          bcbsri_online_application_errorHandling(msg, currentPage);
        }
			});
			appConfig.fail( function(pData) {
				msg = 'Application config failed to load. Please try again later.';
				bcbsri_online_application_errorHandling(msg, currentPage);
			});
		} else {
			/* TODO: get error msg to show if app start fails */
			msg = 'Application failed to load. Please try again later.';
			bcbsri_online_application_errorHandling(msg, currentPage);
		}
	});
	appStart.fail( function(pData) {
		/* TODO: get error msg to show if app start fails */
		msg = 'Application failed to load. Please try again later.';
		bcbsri_online_application_errorHandling(msg);
	});
	
	/* Navigation buttons */
	$(document).delegate('.submitSFButton', event, function (e) {
		e.preventDefault();
		var submitPage = currentPage;
		$('.errorHandling').hide();
		$('.app-form-next>.page-content-' + submitPage + ' .submitButton').addClass('disabled').removeClass('submitSFButton');
		$('.sigError').addClass('hide');
		formData = JSON.stringify($('form').serializeObject());
    submitUrl = url + '/process/send-to-salesforce';
		dataToBeSent = 'completed=1&sfTracking=' + $('.app-config .sfTracking').text() + '&d=' + formData;
		var nameArray = $('input[name="enrollee_signature_name"]').val().trim().toLowerCase().split(' ');
		//var sigName = nameArray[0] + ' ' + nameArray[nameArray.length - 1];
		var sigName = $('input[name="enrollee_signature_name"]').val().trim();
		regex = '^';
		regex += '(' + $('input[name="first_name"]').val().trim() + '\\s)';
		regex += ($('input[name="middle_initial"]').val().trim() != '' ? '(' + $('input[name="middle_initial"]').val() + '\\.\\s)?(' + $('input[name="middle_initial"]').val() + '\\s)?' : '([a-zA-Z]\\.\\s)?([a-zA-Z]\\s)?');
		regex += '(' + $('input[name="last_name"]').val().trim() + ')';
		if($('input[name="suffix"]').val().trim() != '') {
			regex += '(\\s' + $('input[name="suffix"]').val().trim() + ')?(\\.)?';
		} else {
			regex += '(\\s[a-zA-Z\\.]*)?';
		}
		regex += '$';
		//sigName = '';
		sigMatch = false;
		/* if(nameArray.length >=3) {
			if(nameArray[1].length > 1) sigName = nameArray[0] + ' ' + nameArray[1];
			else sigName = nameArray[0] + ' ' + nameArray[2];
		} else sigName = nameArray[0] + ' ' + nameArray[1]; */
		if($('input[name="completing_this_application"]:checked').val() == 'authorized_representative') {
			var appName = $('input[name="authorized_rep_name"]').val().trim().toLowerCase(); // .split(' ');
			//appName = appName[0] + ' ' + appName[appName.length - 1];
			if(sigName.toLowerCase() == appName) sigMatch = true;
		} else if($('input[name="completing_this_application"]:checked').val() == 'applicant17') {
			//var appName = $('input[name="first_name"]').val().trim().toLowerCase() + ' ' + $('input[name="last_name"]').val().trim().toLowerCase();
			//if(sigName != appName) sigMatch = true;
			if(!sigName.match(new RegExp(regex, 'ig'))) sigMatch = true;
		} else {
			if($('input[name="how_old_applicant"]:selected').val() != 'undefined'  && $('input[name="how_old_applicant"]:selected').val() == '17younger') {
				//var appName = $('input[name="first_name"]').val().trim().toLowerCase() + ' ' + $('input[name="last_name"]').val().trim().toLowerCase();
				//if(sigName != appName) sigMatch = true;
				if(!sigName.match(new RegExp(regex, 'ig'))) sigMatch = true;
			} else {
				//var appName = $('input[name="first_name"]').val().trim().toLowerCase() + ' ' + $('input[name="last_name"]').val().trim().toLowerCase();
				//if(sigName == appName) sigMatch = true;
				if(sigName.match(new RegExp(regex, 'ig'))) sigMatch = true;
			}
		}
		if(sigMatch && $('input[name="enrollee_signature_checkbox"]').is(':checked')) {
			var sfSubmit = $.ajax({type: 'POST', url: submitUrl, data: dataToBeSent, dataType: "json"});
			sfSubmit.done(function (sfData) {
				if(sfData.success) {
					// Turn off window leave notification
					window.onbeforeunload = null;
					if(app_alias == 'directpay') $('.associationTagging').html('<iframe src="https://3042219.fls.doubleclick.net/activityi;src=3042219;type=compl491;cat=ri-co339;ord=[SessionID]?" width="1" height="1" frameborder="0" style="display:none"></iframe>');
					$('.confirmationNumber').html(sfData.confirmation);
					formData = JSON.stringify($('form').serializeObject());
          processUrl = url + '/process/data';
					dataToBeSent = 'auth=' + auth + '&form_confirmation=' + sfData.confirmation + '&form_year=' + year + '&form_alias=' + app_alias + '&d=' + formData;
					$.ajax({type: 'POST', url: processUrl, data: dataToBeSent, dataType: "json"});
					if($('input[name="email_address"]').val() != '') {
						formData = $('.email_review').html(); // JSON.stringify($('form').serializeObject());
						formDataPlans = $('.plan-medical-details').html();
            emailUrl = url + '/send-email';
						if($('.plan-dental-details .plantype').text() != '') formDataPlans += $('.plan-dental-details').html();
						dataToBeSent = 'auth=' + auth + '&form_confirmation=' + sfData.confirmation + '&form_year=' + year + '&dental=' + dental + '&form_email=' + $('input[name="email_address"]').val() + '&d=' + formData + '&dplans=' + formDataPlans;
						$.ajax({type: 'POST', url: emailUrl, data: dataToBeSent, dataType: "json"});
						$('.tySentEmail').html($('input[name="email_address"]').val());
						$('.unsentEmail').hide();
						$('.sentEmail').show();
					} else {
						$('.unsentEmail').show();
						$('.sentEmail').hide();
					}
					/* GA eCommerce */
					bcbsri_online_application_gaecommerce(auth, app_alias, 0);
					nextPage = parseInt(currentPage) + 1;
					$("html, body").animate({ scrollTop: "0px" }); $('.page-content-' + currentPage).hide();
					$('.page-content-' + nextPage).show(); currentPage = nextPage;
					bcbsri_online_application_gasend(year, app_alias, $('.app-config .ga-view-' + currentPage).html());
					bcbsri_online_application_progress(currentPage);
				} else {
					$("html, body").animate({ scrollTop: "0px" });
					msg = 'There was a problem submitting your application. Please try again by clicking the Submit button below. If this problem persists, download the <a href="' + applicationForm + '" target="_blank">application</a>. If you prefer, you can contact us at ' + phoneCS +'.';
					bcbsri_online_application_errorHandling(msg, currentPage);
					$('.app-form-next>.page-content-' + submitPage + ' .submitButton').addClass('submitSFButton').removeClass('disabled');
				}
			});
			sfSubmit.fail(function (sfData) {
				$("html, body").animate({ scrollTop: "0px" });
				msg = 'There was a problem submitting your application. Please try again by clicking the Submit button below. If this problem persists, download the <a href="' + applicationForm + '" target="_blank">application</a>. If you prefer, you can contact us at ' + phoneCS +'.';
				bcbsri_online_application_errorHandling(msg, currentPage);
				$('.app-form-next>.page-content-' + submitPage + ' .submitButton').addClass('submitSFButton').removeClass('disabled');
			});
		} else {
			$('input[name="enrollee_signature_name"]').closest('.row').addClass('error');
			$('.sigError').removeClass('hide');
			$('.app-form-next>.page-content-' + submitPage + ' .submitButton').addClass('submitSFButton').removeClass('disabled');

		}
	});
	$(document).delegate('.timeoutButton', event, function (e) {
		e.preventDefault();
		window.close();
		//window.location.href = '/';
	});
	$(document).on('closed.fndtn.reveal', '#appTimeout[data-reveal]', function () {
		var modal = $(this);
		window.close();
		//window.location.href = '/';
	});
	/* $(document).on('closed.fndtn.reveal', '#nopcp[data-reveal]', function () {
		bcbsri_online_application_nopcp('closed', currentPage, steps, year, app_alias);
	});
	$(document).delegate('.pcp_select_continue', 'click', function () {
		currentPage = bcbsri_online_application_nopcp('continue', currentPage, steps, year, app_alias);
	});
	$(document).delegate('.pcp_select_cancel', 'click', function () {
		currentPage = bcbsri_online_application_nopcp('cancel', currentPage, steps, year, app_alias);
	}); */
	$(document).delegate('.edit-page', event, function (e) {
		e.preventDefault();
		classes = $(this).prop('class');
		classes = classes.split(' ');
		for(i = 0; i < classes.length; i++) {
			test = classes[i].split('-');
			if(test[0] == 'page') {
				nextPage = test[1];
				break;
			}
		}
		$("html, body").animate({ scrollTop: "0px" });
		$('.page-content-' + currentPage).hide();
		$('.page-content-' + nextPage).show();
		currentPage = nextPage;
		bcbsri_online_application_gasend(year, app_alias, $('.app-config .ga-view-' + currentPage).html());
		bcbsri_online_application_progress(currentPage);
	});
	$(document).delegate('.next-button', event, function (e) {
		e.preventDefault();
		currentPage = bcbsri_online_application_nextpage($('.app-config .currentPage').text(), steps, year, app_alias, spousePage, depPage, medicalSelected, dentalSelected);
    $('.app-config .currentPage').text(currentPage);
		if($('.ga-view-' + currentPage).html() == 'signsubmit') bcbsri_online_application_sfCheck(url, auth, year, currentPage, applicationForm, phoneCS);
		//$('.app-config .timeoutCounter').html(0);
	});
	$(document).delegate('.back-button', event, function (e) {
		e.preventDefault();
		currentPage = bcbsri_online_application_prevpage($('.app-config .currentPage').text(), steps, year, app_alias, spousePage, depPage, medicalSelected, dentalSelected);
    $('.app-config .currentPage').text(currentPage);
		//$('.app-config .timeoutCounter').html(0);
	});
	
	/* Update review sections */
	$(document).delegate('.enrollSummary', event, function (e) {
		e.preventDefault();
		//var reviewHtml = $('.review').html();
		//$('.final-review-cell').html('<div class="review">' + reviewHtml + '</div>');
		$('.final-review-cell').show();
		sPos = $('.final-review-cell').offset();
		$("html, body").animate({ scrollTop: sPos['top'] + "px" });
	});
	$(document).delegate('.tyEmailSubmit', event, function (e) {
		e.preventDefault();
		formData = $('.email_review').html(); // JSON.stringify($('form').serializeObject());
		formDataPlans = $('.plan-medical-details').html();
		if($('.plan-dental-details .plantype').text() != '') formDataPlans += $('.plan-dental-details').html();
    emailUrl = url + '/send-email';
		dataToBeSent = 'auth=' + auth + '&form_confirmation=' + $('.confirmationNumber').text() + '&form_year=' + year + '&dental=' + dental + '&form_email=' + $('input[name="tyEmail"]').val() + '&d=' + formData + '&dplans=' + formDataPlans;
		$.ajax({type: 'POST', url: emailUrl, data: dataToBeSent, dataType: "json"});
		/* formData = $('.email_review').html(); // JSON.stringify($('form').serializeObject());
		dataToBeSent = 'process=send-email&auth=' + auth + '&form_confirmation=' + $('.confirmationNumber').text() + '&form_year=' + year + '&dental=' + dental + '&form_email=' + $('input[name="tyEmail"]').val() + '&d=' + formData;
		$.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"}); */
		$('.tySentEmail').html($('input[name="tyEmail"]').val());
		$('.unsentEmail').hide();
		$('.sentEmail').show();
	});
	$(document).delegate('.tyEmailSubmit2', event, function (e) {
		e.preventDefault();
		formData = $('.email_review').html(); // JSON.stringify($('form').serializeObject());
		formDataPlans = $('.plan-medical-details').html();
		if($('.plan-dental-details .plantype').text() != '') formDataPlans += $('.plan-dental-details').html();
    emailUrl = url + '/send-email';
		dataToBeSent = 'auth=' + auth + '&form_confirmation=' + $('.confirmationNumber').text() + '&form_year=' + year + '&dental=' + dental + '&form_email=' + $('input[name="tyEmailDiff"]').val() + '&d=' + formData + '&dplans=' + formDataPlans;
		$.ajax({type: 'POST', url: emailUrl, data: dataToBeSent, dataType: "json"});
		/* formData = $('.email_review').html(); // JSON.stringify($('form').serializeObject());
		dataToBeSent = 'process=send-email&auth=' + auth + '&form_confirmation=' + $('.confirmationNumber').text() + '&form_year=' + year + '&dental=' + dental + '&form_email=' + $('input[name="tyEmailDiff"]').val() + '&d=' + formData;
		$.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"}); */
		$('.tySentEmail').html($('input[name="tyEmailDiff"]').val());
		$('.unsentEmail').hide();
		$('.sentEmail').show();
	});
	$(document).delegate('input', 'change', function (e) {
		if($(this).attr("name").indexOf('review-') < 0 && $(this).attr("name").indexOf('saveapp') < 0) {
			classes = $(this).attr('class'); classes = classes.split(" ");
			for(ic = 0; ic < classes.length; ic++) {
				sc = classes[ic].split('-');
				if(classes[ic].indexOf('answer') >= 0) {
					answerNumber = sc[1];
					sc = classes[ic].split('-');
				}
			}
			if(answerNumber) {
				if($(this).parent('div.answer-' + answerNumber).children('.hide-show-logic').length > 0 && $.inArray($(this).attr('type'), ['radio', 'checkbox']) >= 0) {
					if($(this).parent('div.answer-' + answerNumber).children('.hide-show-logic').children('.show').length > 0) {
						show = $(this).parent('div.answer-' + answerNumber).children('.hide-show-logic').children('.show').html();
						bcbsri_online_application_showhide(show, 'show', currentPage, $(this));
					}
					if($(this).parent('div.answer-' + answerNumber).children('.hide-show-logic').children('.hide').length > 0) {
						hide = $(this).parent('div.answer-' + answerNumber).children('.hide-show-logic').children('.hide').html();
						bcbsri_online_application_showhide(hide, 'hide', currentPage, $(this));
					}
				}
			}
			var name = $(this).attr("name");
			var id = $(this).attr("id");
			var value = $(this).val();
			if($.inArray($(this).attr('type'), ['text', 'password', 'number', 'email', 'tel', 'range', 'date', 'month', 'week', 'time', 'datetime', 'datetime-local', 'url', 'color']) >= 0) {
				val = $(this).val();
				if(name == 'medicare_claim_number') emailVal = bcbsri_online_application_maskValue(val, name);
				else if(name.indexOf('social_security') >= 0) emailVal = bcbsri_online_application_maskValue(val, name);
				else if($.inArray($(this).attr('type'), ['password']) >= 0) emailVal = bcbsri_online_application_maskValue(val, name);
				else emailVal = val;
				$('input[name="review-' + name + '"]').val(val);
				$('input[name="review2-' + name + '"]').val(val);
				$('.email_review span.review-' + name).each(function () { $(this).html(emailVal); });
			}
			if($.inArray($(this).attr('type'), ['radio', 'checkbox']) >= 0) {
				var isChecked = $(this).prop('checked');
				//$('.review-' + name + '-' + value).each(function() { $(this).prop('checked', isChecked)}); $('#review-' + name + '-' + value).prop('checked', $(this).prop('checked'));
				//$('.app-form-content .review #review-' + name + '-' + value).prop('checked', isChecked);
				//$('.final-review-cell #review2-' + name + '-' + value).prop('checked', isChecked);
				$('.app-form-content .review #review-' + id).prop('checked', isChecked);
				$('.final-review-cell #review2-' + id).prop('checked', isChecked);
				if(isChecked) val = '(X)';
				else val = '(_)';
				if(name != 'prefer_information_in_another_language' && name != 'select_option_best_fits_your_needs') $('.email_review span.review-' + name).each(function() { $(this).html('(_)'); });
				$('.email_review span.review-' + name + '-' + value).each(function() { $(this).html(val); });
			} 
			//$('.app-config .timeoutCounter').html(0);
		}
		if(app_alias == 'directpay' && ($('input[name="completing_this_application"]:checked').val() == 'applicant17' || ($('input[name="how_old_applicant"]:selected').val() != undefined  && $('input[name="how_old_applicant"]:selected').val() == '17younger'))) {
			$('.sigUnder17').removeClass('hide');
			$('.sigDepName').text($('input[name="first_name"]').val() + ' ' + $('input[name="last_name"]').val());
		} else $('.sigUnder17').addClass('hide');
    
    /*var first_name = $('#first_name').val();
    var last_name = $('#last_name').val();
    var date_of_birth = $('#date_of_birth').val();
    console.log('the values are ' + first_name + ' ' + last_name + ' ' + date_of_birth);*/
    if(($('#first_name').length > 0 && $('#first_name').val() != '') && ($('#last_name').length > 0 && $('#last_name').val() != '') && ($('#date_of_birth').length > 0 && $('#date_of_birth').val() != '') && saveAppState == false &&  year >= 2017) {
      var resumeKeyUrl = "/shop/application/get-resume-key";
      var resumeKey = $.ajax({type: 'POST', url: resumeKeyUrl, data: "", dataType: "json"});
      resumeKey.done(function (data) {
        if(data.success){
          $('.saveappstate3').each(function () {
            //$(this).addClass('hide');
            $(this).css('display', 'none');
          });
          $('.saveappstate2').each(function () {
            //$(this).addClass('hide');
            $(this).css('display', 'none');
          });
          $('.saveappstate1').each(function () {
            //$(this).removeClass('hide');
            $(this).css('display', 'block');
          });
          $('.saveapp-col-wrapper').each(function () {
            $(this).removeClass('hide');
            //$(this).css('display', 'block');
          });
          $('input[name="saveappid"]').val(data.resume_key);
          resumeCode = data.resume_key;
           saveAppState = true;
        }
      });
    }
	});
   $(document).delegate('.saveappstate1 .button', event, function (e) {
      //$('.saveappstate1').addClass('hide');
      $('.saveappstate1').css('display', 'none');
      //$('.saveappstate2').removeClass('hide');
      $('.saveappstate2').css('display', 'block');
      $('.errorDiv.invalidEmail').css('display', 'none');
      $('.errorDiv.errorEmail').css('display', 'none');
   });
   $(document).delegate('.saveappstate2 .button', event, function (e) {
      
      //$('.errorEmailError').addClass('hide');
      var sendTo = '';
      $('input[name="saveappemail"]').each(function () {
        // Sanitize the inputed e-mail address.
        if(typeof $(this).val() !== 'undefined' && $(this).val() != ''){
          if (bcbsri_online_application_email_validation($(this).val())){
             sendTo = $(this).val();
          }else{
            $('.errorDiv.invalidEmail').css('display', 'block');
          }
        }
      });
      
      //console.log('the value of sendto is ' + sendTo);
      
      if(sendTo != ''){
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
        submitUrl = '/shop/application/process/send-to-salesforce';
        formData = JSON.stringify($('form').serializeObject());
        dataToBeSent = 'completed=0&sfTracking=' + $('.app-config .sfTracking').text() + '&d=' + formData;
        var sfSubmit = $.ajax({type: 'POST', url: submitUrl, data: dataToBeSent, dataType: "json"});
        sfSubmit.done(function (sfData) {
          $('.app-config .sfTracking').text(sfData.sfTracking)
        });
      }
   });
	$(document).delegate('select', 'change', function (e) {
		var name = $(this).attr("name");
		$('.app-form-content .review select[name="review-' + name + '"]').val($(this).val());
		$('.final-review-cell select[name="review2-' + name + '"]').val($(this).val());
		$('.email_review span.review-' + name).text($(this).val());
		//$('.app-config .timeoutCounter').html(0);
	});
	$(document).delegate('input[name="bluechip_dental"]', 'change', function (e) {
    dentalUrl = url + '/cart/add-dental';
		if($(this).is(':checked')) {
			planValue = $(this).val().split('--');
			dataToBeSent = 'cookie_name=planDetails&form_alias=' + app_alias + '&form_year=' + year + '&cookie_value=' + planValue[0] + '||' + planValue[1];
			addDental = jQuery.ajax({type: 'POST', url: dentalUrl, data: dataToBeSent, dataType: "json"});
			addDental.done(function(dData) {
				$('.plan-dental-details .plantype').html('Dental Plan');
				$('.plan-dental-details .planselect').html(planValue[0]);
				$('.plan-dental-details .sm-title').html('Monthly Premium');
				planCost = planValue[1].split('.');
				if(planCost[1] === typeof undefined) planCost[1] = '00';
				$('.plan-dental-details .price').html('$' + planCost[0] + '<sup>.' + planCost[1] + '</sup>');
				$('.plan-dental-details .removelink').show();
				$('.plan-dental-details').show();
				$('.app-form-sidebar').hide();
			});
			addDental.fail(function(dData) { $(this).prop('checked', false); });
		} else {
			dataToBeSent = 'cookie_name=planDetails&form_alias=' + app_alias + '&form_year=' + year + '&cookie_value=';
			removeDental = jQuery.ajax({type: 'POST', url: dentalUrl, data: dataToBeSent, dataType: "json"});
			removeDental.done(function(dData) {
				if(dData.success) {
					$('.plan-dental-details .plantype').html('');
					$('.plan-dental-details .planselect').html('');
					$('.plan-dental-details .sm-title').html('');
					$('.plan-dental-details .price').html('');
					$('.plan-dental-details').hide();				

				} else $(this).prop('checked', true);
			});
			removeDental.fail(function(dData) { $(this).prop('checked', true); });
		}
	});	
	$(document).delegate('.plan-dental-details .removelink', 'click', function (e) {
    dentalUrl = url + '/cart/add-dental';
    dataToBeSent = '&cookie_name=planDetails&form_alias=' + app_alias + '&form_year=' + year + '&cookie_value=';
    removeDental = jQuery.ajax({type: 'POST', url: dentalUrl, data: dataToBeSent, dataType: "json"});
    removeDental.done(function(dData) {
      if(dData.success) {
        $('.plan-dental-details .plantype').html('');
        $('.plan-dental-details .planselect').html('');
        $('.plan-dental-details .sm-title').html('');
        $('.plan-dental-details .price').html('');
        $('.plan-dental-details').hide();
        $('.app-form-sidebar').show();
        $('input[name="bluechip_dental"]').attr('checked',false);			

      }
    });
	});
	/*removelink action part*/
	$(document).delegate('.plan-dental-details .removelink', 'click', function (e) {
    dentalUrl = url + '/cart/add-dental';
    dataToBeSent = 'cookie_name=planDetails&form_alias=' + app_alias + '&form_year=' + year + '&cookie_value=';
    removeDental = jQuery.ajax({type: 'POST', url: dentalUrl, data: dataToBeSent, dataType: "json"});
    removeDental.done(function(dData) {
      if(dData.success) {
        $('.plan-dental-details .plantype').html('');
        $('.plan-dental-details .planselect').html('');
        $('.plan-dental-details .sm-title').html('');
        $('.plan-dental-details .price').html('');
        $('.plan-dental-details').hide();
        $('.app-form-sidebar').show();
        $('input[name="bluechip_dental"]').attr('checked',false);			
      }
    });
	});	
	/* Process Data */
	$(document).delegate('input', 'focusout', function (e) {
    processDataUrl = url + '/process/data';
		formData = JSON.stringify($('form').serializeObject());
		dataToBeSent = 'auth=' + auth + '&form_year=' + year + '&form_alias=' + app_alias + '&d=' + formData;
		$.ajax({type: 'POST', url: processDataUrl, data: dataToBeSent, dataType: "json"});
	});
	$(document).delegate('input[type=radio], input[type=checkbox]', 'change', function (e) {
    processDataUrl = url + '/process/data';
		formData = JSON.stringify($('form').serializeObject());
		dataToBeSent = 'auth=' + auth + '&form_year=' + year + '&form_alias=' + app_alias + '&d=' + formData;
		$.ajax({type: 'POST', url: processDataUrl, data: dataToBeSent, dataType: "json"});
	});
	$('input[name="permanent_street_address"]').on('input propertychange paste', function (e) {
		if($('input[name="mailing_address_different_from_permanent"]').val() == 'no') $('input[name="mailing_street_address"]').val($('input[name="permanent_street_address"]').val());
		if($('input[name="billing_address_different_from_permanent_or_mailing"]').val() == 'no') $('input[name="billing_street_address"]').val($('input[name="permanent_street_address"]').val());
	});
	$('input[name="permanent_address_city"]').on('input propertychange paste', function (e) {
		if($('input[name="mailing_address_different_from_permanent"]').val() == 'no') $('input[name="mailing_address_city"]').val($('input[name="permanent_address_city"]').val());
		if($('input[name="billing_address_different_from_permanent_or_mailing"]').val() == 'no') $('input[name="billing_address_city"]').val($('input[name="permanent_address_city"]').val());
	});
	$('input[name="permanent_address_city"]').on('input propertychange paste', function (e) {
		if($('input[name="mailing_address_different_from_permanent"]').val() == 'no') $('input[name="mailing_address_state"]').val($('input[name="permanent_address_state"]').val());
		if($('input[name="billing_address_different_from_permanent_or_mailing"]').val() == 'no') $('input[name="billing_address_state"]').val($('input[name="permanent_address_state"]').val());
	});
	$('input[name="permanent_address_zip_code"]').on('input propertychange paste', function (e) {
		if($('input[name="mailing_address_different_from_permanent"]').val() == 'no') $('input[name="mailing_address_zip"]').val($('input[name="permanent_address_zip_code"]').val());
		if($('input[name="billing_address_different_from_permanent_or_mailing"]').val() == 'no') $('input[name="billing_address_zip"]').val($('input[name="permanent_address_zip_code"]').val());
	});
	$(document).delegate('input[name="mailing_address_different_from_permanent"]', 'change', function (e) {
		if($(this).val() == 'no') {
			$('input[name="mailing_street_address"]').val($('input[name="permanent_street_address"]').val());
			$('input[name="mailing_address_city"]').val($('input[name="permanent_address_city"]').val());
			$('select[name="mailing_address_state"]').val($('select[name="permanent_address_state"]').val());
			$('input[name="mailing_address_zip"]').val($('input[name="permanent_address_zip_code"]').val());
		} else {
			$('input[name="mailing_street_address"]').val('');
			$('input[name="mailing_address_city"]').val('');
			$('select[name="mailing_address_state"]').val('RI');
			$('input[name="mailing_address_zip"]').val('');
		}
	});
	$(document).delegate('.contact-link', event, function (e) {
		e.preventDefault();
		sPos = $('.app-form-footer .page-content-' + currentPage).offset();
		$("html, body").animate({ scrollTop: sPos['top'] + "px" });
	});
	$(document).delegate('input[name="billing_address_different_from_permanent_or_mailing"]', 'change', function (e) {
		if($(this).val() == 'no') {
			$('input[name="billing_street_address"]').val($('input[name="permanent_street_address"]').val());
			$('input[name="billing_address_city"]').val($('input[name="permanent_address_city"]').val());
			$('select[name="billing_address_state"]').val($('select[name="permanent_address_state"]').val());
			$('input[name="billing_address_zip"]').val($('input[name="permanent_address_zip_code"]').val());
		} else {
			$('input[name="billing_street_address"]').val('');
			$('input[name="billing_address_city"]').val('');
			$('select[name="billing_address_state"]').val('RI');
			$('input[name="billing_address_zip"]').val('');
		}
	});
});//end document ready

/* **************************************************
 * Validate an email address. Provide email address (raw input).
 * Returns true if the email address has the email address format.
 * Cannot verify domain in Javascript
 * This is as close to the RFC spec that we can get in javascript
 ************************************************** */
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