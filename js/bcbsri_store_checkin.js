$(document).ready(function() {
  $('body#display').css('height','auto');
  var ua = navigator.userAgent, event = (ua.match(/iPad/i)) ? "touchstart" : "click";
  
  var time = $('#time'),
    //seconds = 0, minutes = 0, hours = 0,
    t,
    adminTimer;
  
  //getStoreWaitTime();
  startTime();  
  updateVisitorList();
  getStoreLists();
  getAdminData();
  geturls();
  getRemovedStores();
  $(document).foundation('reflow');
  var sname = $('#sname').text();
  
  $('#storename').text(sname);
  var storelink = '/yourbluestore/yhXS7LpB/'+sname+'/display';
  var queuelink = '/yourbluestore/yhXS7LpB/'+sname+'/queue';
  $('#storelink').attr('href', storelink);
  $('#queuelink').attr('href', queuelink);

  function add() {
    
    times = $('.time');
  
      $.each(times, function(){
        secondsObject = $(this).find('.s');
        minutesObject = $(this).find('.m');
        hoursObject = $(this).find('.h');
        
        seconds = parseInt($(this).find('.s').text());
        minutes = parseInt($(this).find('.m').text());
        hours = parseInt($(this).find('.h').text());
        
        seconds++;
        if (seconds >= 60) {
            seconds = 0;
            minutes++;
            if (minutes >= 60) {
                minutes = 0;
                hours++;
            }
        }
        
        (hours ? (hours > 9 ? hoursObject.text(hours) : hoursObject.text("0" + hours)) : hoursObject.text("00"));
        (minutes ? (minutes > 9 ? minutesObject.text(minutes) : minutesObject.text("0" + minutes)) : minutesObject.text("00"));
        (seconds > 9 ? secondsObject.text(seconds) : secondsObject.text("0" + seconds));
        
      });
      
  
      timer();
  }
  
  function addTimeAdmin() {
    
    times = $('.admintime');
  
      $.each(times, function(){
        secondsObject = $(this).find('.s');
        minutesObject = $(this).find('.m');
        hoursObject = $(this).find('.h');
        
        seconds = parseInt($(this).find('.s').text());
        minutes = parseInt($(this).find('.m').text());
        hours = parseInt($(this).find('.h').text());
        
        seconds++;
        if (seconds >= 60) {
            seconds = 0;
            minutes++;
            if (minutes >= 60) {
                minutes = 0;
                hours++;
            }
        }
        
        (hours ? (hours > 9 ? hoursObject.text(hours) : hoursObject.text("0" + hours)) : hoursObject.text("00"));
        (minutes ? (minutes > 9 ? minutesObject.text(minutes) : minutesObject.text("0" + minutes)) : minutesObject.text("00"));
        (seconds > 9 ? secondsObject.text(seconds) : secondsObject.text("0" + seconds));
        
      });
      
  
      admintimer();
  }
  
  function timer() {
      t = setTimeout(add, 1000);
  }
  
  function admintimer() {
      adminTimer = setTimeout(addTimeAdmin, 1000);
  }
  
  function getStoreWaitTime(){
    url = "/yourbluestore/yhXS7LpB/getstorewaittime";
    dataToSend = 'store='+$('#store_id').text();
    storeWaitTime = $.ajax({type: 'POST', url: url, data: dataToSend, dataType: "json"});
    storeWaitTime.done(function (data) {
      if(data.success == true){
        $('#storewaittime').text(data.wait_time);
      }else{
        if(window.console) console.log('Could not get the current wait time for this store.');
      }
      $(document).foundation('reflow');
    });
  }
  
  function updateVisitorList(){
    
    
    url = "/yourbluestore/yhXS7LpB/updatevisitorlist";
    dataToSend = 'store='+$('#store_id').text();
    getList = $.ajax({type: 'POST', url: url, data: dataToSend, dataType: "json"});
    getList.done(function (data) {
      if(data.success == true){
        clearTimeout(t);
        output = '';
        $.each(data.results, function(){
          
          output += '<div class="visitor row" id="'+this.id+'">';
          output += '<div class="small-2 columns">';
          output += '<div class="small-12 columns">';
          output += '<a href="#" data-reveal-id="checkin" class="visitoranchor">'+this.name+'</a></div>';
          if(typeof this.appointment !== 'undefined' && this.appointment == 1){
            output += '<div class="small-12 columns"><i class="icon icon-calendar-clock"></i>';
            if(typeof this.appointment_time !== 'undefined'){
              output += '<span style="font-size:9px"> '; //10:00 A.M.
              output += this.appointment_time;
              output += '</span></div>';
            }
          }
          output += '</div>';
          output += '<div class="small-2 columns">'+(this.reason_for_visit == 'cs' ? "Customer Service" : "Sales")+'</div>';
          output += '<div class="small-2 columns">'+this.product+'</div>';
          output += '<div class="small-2 columns">'+this.segment+'</div>';
          output += '<div class="time small-4 columns">';
          
          var seconds = this.elapsed;
          var minutes =  Math.floor(seconds / 60);
          var hours = Math.floor(seconds / 3600);
          
          if(seconds >= 60){
            seconds = seconds % 60;
            minutes = Math.floor(this.elapsed / 60);
            //hours = 0;
            if(minutes >= 60){ 
              minutesElapsed = minutes;
              minutes = minutes % 60;
              hours = Math.floor(minutesElapsed / 60);
            }
          }
          
          output += '<span class="h">'+(hours > 9 ? hours : "0" + hours)+'</span>:';
          output += '<span class="m">'+(minutes > 9 ? minutes : "0" + minutes)+'</span>:';
          output += '<span class="s">'+(seconds > 9 ? seconds : "0" + seconds)+'</span>';
          output += '<div class="btn-secondary buttongroup">';
          output += '<div class="seen button tiny radius bs'+this.id+'">Seen</div>';
          output += '&nbsp;&nbsp;<div class="notseen button tiny radius bn'+this.id+'">Not Seen</div>';
          output += '</div>';
          output += '</div>';
          
          output += '<div name="customer_details" class="hide">';
          output += '<div class="name">';
          output += this.name;
          output += '</div>';
          if(typeof this.phone !== 'undefined'){
            output += '<div class="phone">';
            output += this.phone;
            output += '</div>';
          }
          
          if(typeof this.email !== 'undefined'){
            output += '<div class="email">';
            output += this.email;
            output += '</div>';
          }
          
          if(typeof this.appointment_with !== 'undefined'){
            output += '<div class="appointment_with">';
            output += this.appointment_with;
            output += '</div>';
          }
          
          if(typeof this.appointment_time !== 'undefined'){
            output += '<div class="appointment_time">';
            output += this.appointment_time;
            output += '</div>';
          }
          
          output += '<div class="first_name">';
          output += this.first_name;
          output += '</div>';
          
          output += '<div class="last_name">';
          output += this.last_name;
          output += '</div>';
          
          output += '<div class="signature_received">';
          output += this.signature_received;
          output += '</div>';
          
          output += '<div class="appointment">';
          output += this.appointment;
          output += '</div>';
          
          output += '<div class="product">';
          output += this.product;
          output += '</div>';
          
          output += '<div class="segment">';
          output += this.segment;
          output += '</div>';
          
          output += '<div class="reason_for_visit">';
          output += this.reason_for_visit;
          output += '</div>';
          
          output += '<div class="visitor_id">';
          output += this.id;
          output += '</div>';
         
          output += '</div>';
          output += '</div>';
          
        });
        /*$('#visitorcontainer').html('');
        $('#visitorcontainer').addClass('hide');
        $('#visitorcontainer').removeClass('hide');*/
        $('#visitorcontainer').html(output);
        
        timer();
        $('#waittimevar').val(data.wait_time);
      }else{
        if(window.console) console.log('did not find data');
      }
      $(document).foundation('reflow');
    });
  }
  
 function getRemovedStores(){
   url = "/yourbluestore/yhXS7LpB/getremovedstores";
   adminCall = $.ajax({type: 'POST', url: url, data: '', dataType: "json"});
   adminCall.done(function (adminData){
     output = '';
     $.each(adminData.storedata, function(){
       output += '<div class="row" id="'+this.id+'">';
       output += '<div class="medium-6 columns">';
       output += '<h4>'+this.location+'</h4></div>';
       output += '<div class="medium-6 columns">';
       output += '<div class="button radius recover">Recover</div></div></div>';
     });
     output += '<a class="close-reveal-modal" aria-label="Close">&#215;</a>';
     $('#removed_stores_modal').html(output);
   });
 }
 
 function getAdminData() {
   clearTimeout(adminTimer);
   url = "/yourbluestore/yhXS7LpB/getadmindata";
   adminCall = $.ajax({type: 'POST', url: url, data: '', dataType: "json"});
   adminCall.done(function (adminData){
     if(adminData.success == true){
       
       $('#storerecords').html('');
       $.each(adminData.visitorsInQueue, function(index){
         output = '<div class="small-12 columns storecontainer">';
         output += '<div class="row">';
         output += '<div class="small-12 columns">';
         output += '<h3>'+index+'&nbsp;<span class="small">';
         output += '<a href="#" data-reveal-id="addstore" class="editstore">(Edit)</a></span></h3>';
         output += '</div>';
         output += '</div>';
         output += '<div class="row">';
         // PRINT THE CURRENT WAIT TIME
         output += '<div class="small-4 columns">';
         output += '<p class="lead">Current wait</p>';
         output += '<div class="admintime">';
         
         if(typeof this.elapsed !== 'undefined'){
            var green_time = this.greenThreshold.split(':');
            var orange_time = this.orangeThreshold.split(':');
            var green_h = parseInt(green_time[0]);
            var green_m = parseInt(green_time[1]);
            var green_s = parseInt(green_time[2]);
            
            var orange_h = parseInt(orange_time[0]);
            var orange_m = parseInt(orange_time[1]);
            var orange_s = parseInt(orange_time[2]);
            
            var seconds = this.elapsed;
            var minutes =  Math.floor(seconds / 60);
            var hours = Math.floor(seconds / 3600);
            
            if(seconds >= 60){
              seconds = seconds % 60;
              minutes = Math.floor(this.elapsed / 60);
              //hours = 0;
              if(minutes >= 60){ 
                minutesElapsed = minutes;
                minutes = minutes % 60;
                hours = Math.floor(minutesElapsed / 60);
              }
            }
            var gt_seconds = (green_h * 3600) + (green_m * 60) + green_s;
            var ot_seconds = (orange_h * 3600) + (orange_m * 60) + orange_s;
            var elapsed_seconds = (hours * 3600) + (minutes * 60) + seconds;
            
            t_class ='h_green';
            
            if(elapsed_seconds >= ot_seconds){
              t_class = 'h_red';
            }else if (elapsed_seconds < ot_seconds && elapsed_seconds > gt_seconds){
              t_class = 'h_orange';
            }
            
            output += '<p class="bentonsansmedium '+t_class+'">';
            output += '<span class="h">'+(hours > 9 ? hours : "0" + hours)+'</span>:';
            output += '<span class="m">'+(minutes > 9 ? minutes : "0" + minutes)+'</span>:';
            output += '<span class="s">'+(seconds > 9 ? seconds : "0" + seconds)+'</span>';
         }
         output += '</p>';
         output += '</div>';
         output += '</div>';
         
         // PRINT THE VISITORS IN QUEUE.
         output += '<div class="small-4 columns">';
         output += '<p class="lead">In Queue</p>';
         output += '<p class="bentonsansmedium">';
         output += this.inQueue;
         output += '</p>';
         output += '</div>';
         
         // PRINT THE NUMBER OF VISITORS THAT HAVE BEEN SEEN TODAY.
         output += '<div class="small-4 columns">';
         output += '<p class="lead">Seen Today</p>';
         output += '<p class="bentonsansmedium">';
         output += this.seenToday;
         output += '</p>';
         output += '</div>';
         output += '</div><hr />';
         
         output += '<div class="hide" id="storedata">';
         
         output += '<div class="storeId">';
         output += this.id;
         output += '</div>';
         
         output += '<div class="storeName">';
         output += index;
         output += '</div>';
         
         output += '<div class="gThreshold">';
         if(typeof this.greenThreshold !== 'undefined'){
           output += this.greenThreshold;
         }
         output += '</div>';
         
         output += '<div class="oThreshold">';
         if(typeof this.orangeThreshold !== 'undefined'){
           output += this.orangeThreshold;
         }
         output += '</div>';
         
         output += '</div>';
         
         output += '</div>';
         
         $('#storerecords').append(output);
       });
       $(document).foundation('reflow');
       admintimer();
     }else{
       console.log('ajax call failed');
     }
   });
 }
 
 $(document).delegate('#removestore', event, function (e) {
    url = "/yourbluestore/yhXS7LpB/removestore";
    dataToBeSent = 'id='+$('#store_id').val();
    removeStore = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
    removeStore.done(function (data) {
      if(data.success == true){
        $('#addstore').foundation('reveal', 'close');
        getAdminData();
        geturls();
        getRemovedStores();
      }else{
        if(window.console) console.log('There was an error removing this store.');
      }
    });
 });
 
 $(document).delegate('.recover', event, function (e) {
   url = "/yourbluestore/yhXS7LpB/recoverstore";
   storeid = $(this).parent().parent().prop('id');
   dataToSend = 'id='+storeid;
   adminCall = $.ajax({type: 'POST', url: url, data: dataToSend, dataType: "json"});
   adminCall.done(function (adminData){
     if(adminData.success == true){
       $('#removed_stores_modal').foundation('reveal', 'close');
       getAdminData();
       geturls();
       getRemovedStores();
     }
   });
 });
 
 $(document).delegate('.visitoranchor', event, function (e) {
   
   $('#check-in').css('display','none');
   $('#edit-visitor').css('display','block');
   
   e = $(this).parent().parent().parent();
   customerDetails = e.find('.hide');
   name = customerDetails.find('.name').text();
   first_name = customerDetails.find('.first_name').text();
   last_name = customerDetails.find('.last_name').text();
   appointment = customerDetails.find('.appointment').text();
   product = customerDetails.find('.product').text();
   segment = customerDetails.find('.segment').text();
   signature_received = customerDetails.find('.signature_received').text();
   appointment_time = customerDetails.find('.appointment_time').text();
   phone = customerDetails.find('.phone').text();
   email = customerDetails.find('.email').text();
   reason_for_visit = customerDetails.find('.reason_for_visit').text();
   appointment_with = customerDetails.find('.appointment_with').text();
   visitor_id = customerDetails.find('.visitor_id').text();
   
   $('#visitor_name').text(name);
   $('#visitor_phone').text(phone);
   $('#visitor_email').text(email);
   $('#visitor_appointmentwith').text(appointment_with);
   $('#first_name').val(first_name);
   $('#last_name').val(last_name);
   $('#email').val(email);
   $('#phone').val(phone);
   $('#appointment_with').val(appointment_with);
   $('#appointment_time').val(appointment_time);
   if(reason_for_visit == 'cs'){
     $('#reason_cs').prop('checked',true);
   }else{
     $('#reason_sales').prop('checked',true);
   }
   $('#product').val(product);
   $('#segment').val(segment);
   if(signature_received == 1){
     $('#signature').prop('checked',true);
   }
   if(appointment == 1){
     $('#appointment').prop('checked',true);
   }
   $('#visitor_id').text(visitor_id);
 });
  
 $(document).delegate('#list', event, updateVisitorList);
 
 globalIntervalChecker = setInterval(updateVisitorList, 10000);
 
 globalStoreChecker = setInterval(getStoreLists, 10000);
 
 globalAdminChecker = setInterval(getAdminData, 10000);
 
 $(document).delegate('#submitstoreedit', event, function (e) {
    //Clear all previous error messages.
    $('#exists').css('display', 'none');
    $('#noname').css('display', 'none');   
   
    url = "/yourbluestore/yhXS7LpB/editstore";
    if($('#store_name').val() != '' && $('#store_name').length > 0){
      dataToBeSent = 'id='+$('#store_id').val()+'&l='+$('#store_name').val()+'&g='+$('#green_threshold').val()+'&o='+$('#orange_threshold').val();
      updateStore = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
      updateStore.done(function (data) {
        if(data.success == true){
          $('#exists').css('display', 'none');
          $('#addstore').foundation('reveal', 'close');
          getAdminData();
          geturls();
        }else{
          if(data.reason == 'Exists'){
            $('#exists').css('display', 'block');
          }
          if(window.console) console.log('There was an error updating the data for this store.');
        }
      });
    }else{
      $('#noname').css('display', 'block');
    }
 });
 
 $(document).delegate('.notseen', event, function (e) {
   id = $(this).parent().parent().parent().attr('id');
   parent = $(this).parent().parent().parent();
   //id = parent.prop('id');
    url = "/yourbluestore/yhXS7LpB/updatevisitor";
    dataToBeSent = 'notseen=1'+'&id='+$(parent).prop('id');
    $('#'+id).css('display', 'none');
    updateVisitor = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
    updateVisitor.done(function (data) {
      if(data.success == true){
        $(parent).remove();
        updateVisitorList();
        url = "/yourbluestore/yhXS7LpB/sendtomarketo";
        dataToBeSent += '&store='+$('#store_id').text();
         sendToMarketo = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
         sendToMarketo.done(function (marketoData) {
           if(marketoData.success == true){
            console.log(marketoData);
           }else{
             if(window.console) console.log('There was an error sending the data to Marketo.');
           }
         });
      }else{
        if(window.console) console.log('There was an error updating the data for this visitor.');
      }
      $(document).foundation('reflow');
    });
 });
 
 $(document).delegate('.seen', event, function (e) {
   /*var cl = $(this).attr('class');
   
   //var str = "button seen bs13432432";
   var n = cl.indexOf("bs");
   var s = cl.substring(n+2); 

   console.log($('#'+s));*/
   
   id = $(this).parent().parent().parent().attr('id');
   parent = $(this).parent().parent().parent();
   //id = parent.prop('id');
    url = "/yourbluestore/yhXS7LpB/updatevisitor";
    dataToBeSent = 'seen=1'+'&id='+$(parent).prop('id');
    $('#'+id).css('display', 'none');
    //$(document).foundation('reflow');
    updateVisitor = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
    updateVisitor.done(function (data) {
      if(data.success == true){
        //$(parent).css('display', 'none');
        $(parent).remove();
        updateVisitorList();
        url = "/yourbluestore/yhXS7LpB/sendtomarketo";
        dataToBeSent += '&store='+$('#store_id').text();
         sendToMarketo = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
         sendToMarketo.done(function (marketoData) {
           if(marketoData.success == true){
            console.log(marketoData);
           }else{
             if(window.console) console.log('There was an error sending the data to Marketo.');
           }
         });
      }else{
        if(window.console) console.log('There was an error updating the data for this visitor.');
      }
      
    });
 });
 
  /*$(document).delegate('.notseen', event, function (e) {
   parent = $(this).parent().parent().parent();
   //id = parent.prop('id');
    url = "/yourbluestore/yhXS7LpB/updatevisitor";
    dataToBeSent = 'notseen=1'+'&id='+$(parent).prop('id');
    updateVisitor = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
    updateVisitor.done(function (data) {
      if(data.success == true){
        $(parent).remove();
        url = "/yourbluestore/yhXS7LpB/sendtomarketo";
        dataToBeSent += '&store='+$('#store_id').text();
        sendToMarketo = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
        sendToMarketo.done(function (marketoData) {
         if(marketoData.success == true){
          console.log(marketoData);
         }else{
           if(window.console) console.log('There was an error sending the data to Marketo.');
         }
        });
      }else{
        if(window.console) console.log('There was an error updating the data for this visitor.');
      }
      $(document).foundation('reflow');
    });
 });*/
  
  $(document).delegate('#submitstore', event, function (e) {
    var storeName = $('#store_name').val();
    var greenThreshold = $('#green_threshold').val();
    var orangeThreshold = $('#orange_threshold').val();
    
    //Clear all previous error messages.
    $('#exists').css('display', 'none');
    $('#noname').css('display', 'none');
    
    url = "/yourbluestore/yhXS7LpB/addstore";
    dataToBeSent = 'sn='+storeName+'&gt='+greenThreshold+'&ot='+orangeThreshold;
    if($('#store_name').val() != '' && $('#store_name').length > 0){
      addStore = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
      addStore.done(function (data) {
        if(data.success == true){
          $('#exists').css('display', 'none');
          $('#submitstore').foundation('reveal', 'close');
          getAdminData();
          geturls();
        }else{
          if(data.reason == 'Exists'){
            $('#exists').css('display', 'block');
          }
          if(window.console) console.log('error! Could not store the data');
        }
        $(document).foundation('reflow');
      });
    }else{
       $('#noname').css('display', 'block');
    }
  });
  
  $(document).delegate('#addbutton', event, function (e) {
    $('#store_name').val('');
    $('#green_threshold').val('');
    $('#orange_threshold').val('');
    $('#store_id').val('');
    $('#exists').css('display', 'none');
    $('#submitstoreedit').css('display', 'none');
    $('#noname').css('display', 'none');
    $('#submitstore').css('display', 'block');
  });
  
  $(document).delegate('.editstore', event, function (e) {
    //Clear form values.
    $('#store_name').val('');
    $('#green_threshold').val('');
    $('#orange_threshold').val('');
    $('#store_id').val('');
    
    //Clear error message.
    $('#exists').css('display', 'none');
    $('#noname').css('display', 'none');
    
    $('#submitstoreedit').css('display', 'block');
    $('#submitstore').css('display', 'none');
    
    /*var container = $(this).parentsUntil('.storecontainer');
    console.log(container);*/
    
    var store_name = $(this).parent().parent().parent().parent().parent().find('.storeName').html();
    var green_threshold = $(this).parent().parent().parent().parent().parent().find('.gThreshold').html();
    var orange_threshold = $(this).parent().parent().parent().parent().parent().find('.oThreshold').html();
    var store_id = $(this).parent().parent().parent().parent().parent().find('.storeId').html();
    
    $('#store_id').val(store_id);
    $('#store_name').val(store_name);
    if(green_threshold != 'null'){
      $('#green_threshold').val(green_threshold);
    }
    if(orange_threshold != 'null'){
     $('#orange_threshold').val(orange_threshold);
    }
  });
  
  $(document).delegate('#waittimevar', event, function (e) {
    url = "/yourbluestore/yhXS7LpB/setstorewaittime";
    dataToBeSent = 'waittime='+$('#waittimevar').val()+'&store='+ $('#store_id').text();
    waitTime = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
    waitTime.done(function (data) {
      if(data.success == true){
        if(window.console) console.log('Sucessfully set the wait time.');
      }else{
        if(window.console) console.log('Wait time update failed.');
      }
      $(document).foundation('reflow');
    });
  });
  
  function getStoreLists() {
    
    url = "/yourbluestore/yhXS7LpB/getstorelists";
    dataToBeSent = 'store='+$('#store_id').text();
    visitorsList = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
    visitorsList.done(function (data) {
      if(data.success == true){
        $('#storewaittime').text(data.wait_time+'');
        i_cs = 1;
        $.each(data.results_cs, function(){
          if(i_cs < 13){
            li = '.cs'+i_cs;
            $(li).removeClass('hide');
            if(this.appointment == 1){
              $(li).html(this.name+'&nbsp;<i class="icon icon-calendar-clock"></i>');
            }else{
              $(li).text(this.name);
            }
          }
          i_cs++;
        });
        
        for(i_cs; i_cs<13; i_cs++){
          li = '.cs'+i_cs;
          $(li).text('');
          $(li).addClass('hide');
        }
        
        i_s = 1;
        $.each(data.results_s, function(){
          if(i_s < 13){
            li = '.s'+i_s;
            $(li).removeClass('hide');
            if(this.appointment == 1){
              $(li).html('<i class="icon icon-calendar-clock"></i>&nbsp;'+this.name);
            }else{
              $(li).text(this.name);
            }
          }
          i_s++;
        });
        for(i_s; i_s<13; i_s++){
          li = '.s'+i_s;
          $(li).text('');
          $(li).addClass('hide');
        }
      }else{
        if(window.console) console.log('Could not fetch visitor data.');
      }
      $(document).foundation('reflow');
    });
  };
  
  function geturls(){
     url = '/yourbluestore/yhXS7LpB/getstoreurls';
     addStore = $.ajax({type: 'POST', url: url, data: '', dataType: "json"});
     addStore.done(function (data) {
       if(data.success == true){
          outputq = '<ul>';
          outputd = '<ul>';
          $.each(data.storedata, function(){
            outputq += '<li><a href="/yourbluestore/yhXS7LpB'+this.url+'/queue">'+this.location+'</a></li>';
            outputd += '<li><a href="/yourbluestore/yhXS7LpB'+this.url+'/display" target="_blank">'+this.location+'</a></li>';
          });
          outputq += '</ul>';
          outputd += '</ul>';
          $('.section-q').html('<p class="title"><a href="#">Queues</a></p>');
          $('.section-d').html('<p class="title"><a href="#">Store Display</a></p>');
          $('.section-q').append(outputq);
          $('.section-d').append(outputd);
       }
     });
  }
  
  $(document).delegate('#checkinbutton', event, function (e) {
    $('#check-in').css('display','block');
    $('#edit-visitor').css('display','none');
    $('#first_name').val('');
    $('#last_name').val('');
    $('#email').val('');
    $('#phone').val('');
    $('#reason_cs').attr('checked', false);
    $('#reason_sales').attr('checked', false);
    $('#product').val('selectone');
    $('#segment').val('selectone');
    $('#emailError').css('display', 'none');
    $('#signature').attr('checked', false);
    $('#appointment').attr('checked', false);
    $('#appointment_with').val('');
    $('#appointment_time').val('');
    
    //Also, clear error messages.
    $('#nameError').css('display', 'none');
    $('#lnameError').css('display', 'none');
    $('#emailError').css('display', 'none');
    $('#reasonError').css('display', 'none');
    $('#segmentError').css('display', 'none');
    $('#productError').css('display', 'none');
  });
  
  /**
   * Checks in a visitor.
   */
  $(document).delegate('#check-in', event, function (e) {
    var firstName = $('#first_name').val();
    var lastName = $('#last_name').val();
    var email = $('#email').val();
    var phone = $('#phone').val();
    var waittime = $('#waittimevar').val();
    var reasonForVisit = $('input[name="reason_for_visit"]:checked').val();
    var product = $("select[name='product'] option:selected").val();
    var segment = $("select[name='segment'] option:selected").val();
    var signature = ($('#signature').prop('checked') == true ? 1 : 0);
    var appointment = ($('#appointment').prop('checked') == true ? 1 : 0);
    var appointment_with = $('#appointment_with').val();
    var appointment_time = $('#appointment_time').val();
    var storeid = $('#store_id').text();
    var checkin = new Date();
    
    //Clear any previous error messages.
    $('#nameError').css('display', 'none');
    $('#lnameError').css('display', 'none');
    $('#emailError').css('display', 'none');
    $('#reasonError').css('display', 'none');
    $('#segmentError').css('display', 'none');
    $('#productError').css('display', 'none');
    
    url = "/yourbluestore/yhXS7LpB/addvisitor";
    dataToBeSent = 'fn='+firstName+'&ln='+lastName+'&email='+email+
    '&phone='+phone+'&rv='+reasonForVisit+'&product='+product+
    '&segment='+segment+'&signature='+signature+'&appt='+appointment+
    '&awith='+appointment_with+'&store='+storeid+'&w='+waittime+'&appt_time='+appointment_time;
    
    if(($('#first_name').length > 0 && $('#first_name').val() != '') && ($('#last_name').length > 0 && $('#last_name').val() != '') && typeof $('input[name="reason_for_visit"]:checked').val() !== 'undefined' && (($('#product').val() != 'selectone' && $('#product').val() != 'BillPay' && $('#segment').val() != 'selectone') || $('#product').val() == 'BillPay')){
      //if(bcbsri_store_checkin_email_validation(email)){
        addStore = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
        addStore.done(function (data) {
          if(data.success == true){
            $('#checkin').foundation('reveal', 'close');
            output = '<div class="visitor row">';
            output += '<div class="small-2 columns"><a href="#" data-reveal-id="visitorinfo">'+firstName+' '+lastName+'</a></div>';
            output += '<div class="small-2 columns">'+reasonForVisit+'</div>';
            output += '<div class="small-2 columns">'+product+'</div>';
            output += '<div class="small-2 columns">'+segment+'</div>';
            output += '<div class="time small-4 columns">';
            output += '<span class="h">00</span>:';
            output += '<span class="m">00</span>:';
            output += '<span class="s">00</span>';
            output += '<div class="buttonwrapper">';
            output += '<div class="seen btn-primary"><div class="button radius">Seen</div></div>';
            output += '<div class="notseen btn-secondary"><div class="button radius">Not Seen</div></div>';
            output += '</div>';
            output += '</div>';
            output += '</div>';
            //$('#visitorcontainer').append(output);
            updateVisitorList();
          }else{
            if(window.console) console.log('error! Could not store the data');
          }
          
          $(document).foundation('reflow');
          
        });
      /*}else{
         $('#emailError').css('display', 'block');
      }*/
    }else{
      if(typeof $('#first_name').val() === 'undefined' || $('#first_name').val() == ''){
        $('#nameError').css('display', 'block');
      }
      if(typeof $('#last_name').val() === 'undefined' || $('#last_name').val() == ''){
        $('#lnameError').css('display', 'block');
      }
      if(typeof $('input[name="reason_for_visit"]:checked').val() === 'undefined'){
        $('#reasonError').css('display', 'block');
      }
      if($('#segment').val() == 'selectone'){
        $('#segmentError').css('display', 'block');
      }
      if($('#product').val() == 'selectone'){
        $('#productError').css('display', 'block');
      }
    } 
  });
  
  /**
 * Updates a visitor record.
 */
$(document).delegate('#edit-visitor', event, function (e) {
  var firstName = $('#first_name').val();
  var lastName = $('#last_name').val();
  var email = $('#email').val();
  var phone = $('#phone').val();
  var waittime = $('#waittimevar').val();
  var reasonForVisit = $('input[name="reason_for_visit"]:checked').val();
  var product = $("select[name='product'] option:selected").val();
  var segment = $("select[name='segment'] option:selected").val();
  var signature = ($('#signature').prop('checked') == true ? 1 : 0);
  var appointment = ($('#appointment').prop('checked') == true ? 1 : 0);
  var appointment_with = $('#appointment_with').val();
  var appointment_time = $('#appointment_time').val();
  var storeid = $('#store_id').text();
  var visitorid = $('#visitor_id').text();
  
  //Clear any previous error messages.
  $('#nameError').css('display', 'none');
  $('#lnameError').css('display', 'none');
  $('#emailError').css('display', 'none');
  $('#reasonError').css('display', 'none');
  $('#segmentError').css('display', 'none');
  $('#productError').css('display', 'none');
  
  url = "/yourbluestore/yhXS7LpB/updatevisitor";
  dataToBeSent = 'fn='+firstName+'&ln='+lastName+'&email='+email+
  '&phone='+phone+'&rv='+reasonForVisit+'&product='+product+
  '&segment='+segment+'&signature='+signature+'&appt='+appointment+
  '&awith='+appointment_with+'&w='+waittime+'&appt_time='+appointment_time+
  '&edit=1&store='+storeid+'&vid='+visitorid;
  
  if(($('#first_name').length > 0 && $('#first_name').val() != '') && ($('#last_name').length > 0 && $('#last_name').val() != '') && typeof $('input[name="reason_for_visit"]:checked').val() !== 'undefined' && (($('#product').val() != 'selectone' && $('#product').val() != 'BillPay' && $('#segment').val() != 'selectone') || $('#product').val() == 'BillPay')){
    //if(bcbsri_store_checkin_email_validation(email)){
      addStore = $.ajax({type: 'POST', url: url, data: dataToBeSent, dataType: "json"});
      addStore.done(function (data) {
        if(data.success == true){
          $('#checkin').foundation('reveal', 'close');
          $('#edit-visitor').css('display', 'none');
          $('#edit-visitor').css('display', 'block');
          output = '<div class="visitor row">';
          output += '<div class="small-2 columns"><a href="#" data-reveal-id="visitorinfo">'+firstName+' '+lastName+'</a></div>';
          output += '<div class="small-2 columns">'+reasonForVisit+'</div>';
          output += '<div class="small-2 columns">'+product+'</div>';
          output += '<div class="small-2 columns">'+segment+'</div>';
          output += '<div class="time small-4 columns">';
          output += '<span class="h">00</span>:';
          output += '<span class="m">00</span>:';
          output += '<span class="s">00</span>';
          output += '<div class="buttonwrapper">';
          output += '<div class="seen btn-primary"><div class="button radius">Seen</div></div>';
          output += '<div class="notseen btn-secondary"><div class="button radius">Not Seen</div></div>';
          output += '</div>';
          output += '</div>';
          output += '</div>';
          $('#visitorcontainer').append(output);
          $('#'+visitorid).remove();
          updateVisitorList();
        }else{
          if(window.console) console.log('error! Could not store the data');
        }
        
        $(document).foundation('reflow');
        
      });
    /*}else{
       $('#emailError').css('display', 'block');
    }*/
    }else{
      if(typeof $('#first_name').val() === 'undefined' || $('#first_name').val() == ''){
        $('#nameError').css('display', 'block');
      }
      if(typeof $('#last_name').val() === 'undefined' || $('#last_name').val() == ''){
        $('#lnameError').css('display', 'block');
      }
      if(typeof $('input[name="reason_for_visit"]:checked').val() === 'undefined'){
        $('#reasonError').css('display', 'block');
      }
      if($('#segment').val() == 'selectone'){
        $('#segmentError').css('display', 'block');
      }
      if($('#product').val() == 'selectone'){
        $('#productError').css('display', 'block');
      }
    } 
  });
  
  function startTime() {
    var today = new Date();
    var h = today.getHours();
    var am_or_pm = 'AM';
    if(h >= 12){
      am_or_pm = 'PM';
    }
    if(h > 12){
       h = h - 12;
    }

    var m = today.getMinutes();
    var s = today.getSeconds();
    m = checkTime(m);
    s = checkTime(s);
    $('#clock').text(
    h + ":" + m + " "+am_or_pm);
    var t = setTimeout(startTime, 500);
  }

  function checkTime(i) {
      if (i < 10) {i = "0" + i};  // add zero in front of numbers < 10
      return i;
  }

    
});



function bcbsri_store_checkin_email_validation(email) {
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