<?php
/* ********************************************************
 * Functions to get, set, and clear cookies
 ******************************************************** */
function bcbsri_online_application_clear_cookies () {
	/* $year = bcbsri_online_application_get_year();
	$alias = bcbsri_online_application_get_alias();
	$sessionCookie = session_get_cookie_params();
	setcookie('bcbsri_prod_alias', '', time() -3600, '/', $_SERVER['HTTP_HOST'], $sessionCookie['secure'], TRUE);
	setcookie('bcbsri_prod_year', '', time() -3600, '/', $_SERVER['HTTP_HOST'], $sessionCookie['secure'], TRUE);
	foreach($_COOKIE['bcbsri_' . $year . '_' . $alias] as $key => $value) {
		setcookie('bcbsri_' . $year . '_' . $alias . '[' . $key . ']', '', time() -3600, '/', $_SERVER['HTTP_HOST'], $sessionCookie['secure'], TRUE);
	}*/
  // Only one cookie that should be being set at themoment.
	$sessionCookie = session_get_cookie_params();
	setcookie('bcbsri_oa_aid', '', time() -3600, '/', $_SERVER['HTTP_HOST'], $sessionCookie['secure'], TRUE);
}

function bcbsri_online_application_get_alias ($auth) {
  $result = bcbsri_online_application_db_get('bcbsri_online_application_udata', array('app_type'), $auth, 'auth');
  foreach($result as $r) {
    $val= $r->app_type;
  }
  return $val;
}

function bcbsri_online_application_set_alias ($auth, $alias) {
  bcbsri_online_application_db_set('bcbsri_online_application_udata', array('app_type' => $alias, 'auth' => $auth), 'auth');
}

function bcbsri_online_application_get_year ($auth) {
  $result = bcbsri_online_application_db_get('bcbsri_online_application_udata', array('app_year'), $auth, 'auth');
  foreach($result as $r) {
    $val= (int)$r->app_year;
  }
  return $val;
}

function bcbsri_online_application_set_year ($auth, $year) {
  bcbsri_online_application_db_set('bcbsri_online_application_udata', array('app_year' => $year, 'auth' => $auth), 'auth');
}

function bcbsri_online_application_get_cookie ($name) { /*, $alias = NULL, $year = NULL) {
	if(empty($alias)) $alias = bcbsri_online_application_get_alias();
	if(empty($year)) $alias = bcbsri_online_application_get_year(); */
  // only one cookie should be set.
	if(isset($_COOKIE[$name])) return urldecode($_COOKIE[$name]);
	else return false;
}

/* ********************************************************
 * $args is an array of values
 *    - $args[0]: Name of the cookie
 *    - $args[1]: Value of the cookie
 *    - $args[2]: Expiration time in standard strtotime fashion
 *           Examples: now
 *                     +1 day
 *                     +1 week
 *                     +1 week 2 days 4 hours 2 seconds
 *                     next Monday
 ******************************************************** */
function bcbsri_online_application_set_cookie ($args) {
	$name = $args[0];
	$value = $args[1];
	//$year = (!empty($args[2]) ? $args[2] : bcbsri_online_application_get_year());
	//$alias = (!empty($args[3]) ? $args[3] : bcbsri_online_application_get_alias());
  // Changed to 0 becuase timeout check was causing auth to fail when setting to 15
  //$expire = 0; #(empty($args[2]) ? 0 : strtotime($args[2]));
  $expire = (empty($args[2]) ? 0 : strtotime($args[2]));;
	$sessionCookie = session_get_cookie_params();
	// setcookie('bcbsri_' . $year . '_' . $alias . '[' . $name . ']', $value, 0, '/', $_SERVER['HTTP_HOST'], $sessionCookie['secure'], TRUE);
  setcookie($name, $value, $expire, '/', $_SERVER['HTTP_HOST'], $sessionCookie['secure'], TRUE);
}

/**
 * This function's purpose is to provide the functionality
 * for the determination of a current or new member. It sets the cookie's expiration time to
 * 15 minutes, and it also updates the corresponding record on the
 * database to indicate membership status.
 */
function bcbsri_online_application_setdata(){
  //$auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
  //bcbsri_online_application_set_cookie(array('bcbsri_oa_aid', $auth, '+15 minutes'));
  //call db set
  if($_SERVER['REQUEST_METHOD'] == 'POST') {
    if(isset($_REQUEST['existing_member'])) {
      $sessionCookie = session_get_cookie_params();
      $expire = strtotime('+4 hours'); #15 minutes');
      setcookie('bcbsri_oa_em', $_REQUEST['existing_member'], $expire, '/', $_SERVER['HTTP_HOST'], $sessionCookie['secure'], TRUE);
      //setcookie('bcbsri_oa_aid', '', time() -3600, '/', $_SERVER['HTTP_HOST'], $sessionCookie['secure'], TRUE);
      $auth = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
      bcbsri_online_application_db_set('bcbsri_online_application_udata', array('existing_member' => $_REQUEST['existing_member'], 'auth' => $auth), 'auth');
      return array('success' => true);
    }else{
      return array('success' => false, 'reason' => 'Data not sent.');
    }
  }
}

function bcbsri_online_application_clear_cookie ($name) {
  // only one cookie should be being set
	$sessionCookie = session_get_cookie_params();
	setcookie($name, '', time() -3600, '/', $_SERVER['HTTP_HOST'], $sessionCookie['secure'], TRUE);
}

function bcbsri_online_application_set_quote ($auth, $data) {
  bcbsri_online_application_db_set('bcbsri_online_application_udata', array('quote' => $data, 'auth' => $auth), 'auth');
}

function bcbsri_online_application_get_quote ($auth) {
  $result = bcbsri_online_application_db_get('bcbsri_online_application_udata', array('quote'), $auth, 'auth');
  foreach($result as $r) {
    $val = $r->quote;
  }
  return $val;
}

function bcbsri_online_application_set_covered ($auth, $data) {
  bcbsri_online_application_db_set('bcbsri_online_application_udata', array('covered' => $data, 'auth' => $auth), 'auth');
}

function bcbsri_online_application_get_covered ($auth) {
  $result = bcbsri_online_application_db_get('bcbsri_online_application_udata', array('covered'), $auth, 'auth');
  foreach($result as $r) {
    $val = $r->covered;
  }
  if(empty($val)) return false;
  else return $val;
}

//include('bcbsri_online_application.process.php');

/* ********************************************************
 * This function is used to create a truely unique auth
 * code. This checks each one against the database to make
 * sure each is unique.
 *
 * TODO: Update to pull from cookie and check against database. If exists pull that in
 ******************************************************** */
function bcbsri_online_application_appauth ($appStart = FALSE) {
  drupal_add_http_header(
    'Cache-Control', 
    'no-store, no-cache, must-revalidate, post-check=0, pre-check=0', 
    FALSE
  );
  try {
    if($_SERVER['REQUEST_METHOD'] == 'POST'){
      /* *****************************************************
       * Check if auth if good and if it is during app start. 
       * We should not be setting the auth on app start. 
       ***************************************************** */
      if($appStart == FALSE && bcbsri_online_application_auth_verify(bcbsri_online_application_get_cookie('bcbsri_oa_aid')) == false) {
        /* *****************************************************
         * get a random auth that does not exist in the database already
         ***************************************************** */
        do {
          $token = bcbsri_online_application_authgen();
          $num_rows = bcbsri_online_application_check_auth($token);
        } while ($num_rows > 0);
        $sToken = bcbsri_online_application_authgen();
        $time = time();
        /* *****************************************************
         * set auth cookie and add it to the tables
         ***************************************************** */
        bcbsri_online_application_set_cookie(array('bcbsri_oa_aid', $token, '+15 minutes'));
        bcbsri_online_application_db_set('bcbsri_online_application_udata', array('auth' => $token, 'auths' => $sToken, 'start_time' => $time, 'end_time'=>$time));
        bcbsri_online_application_db_set('bcbsri_online_application_ufields', array('auth' => $token));
        bcbsri_online_application_db_set('bcbsri_online_application_plans', array('auth' => $token));
        
        /* *****************************************************
         * return auth start and end/modified time
         ***************************************************** */
        return array('success' => true, 'auth' => $token, 'stime' =>  $time, 'mtime' =>  $time, 'continued' => false);
      } else {
        $token = bcbsri_online_application_get_cookie('bcbsri_oa_aid');
        if(bcbsri_online_application_auth_verify($token)) {
          $time = time();
          $result = bcbsri_online_application_db_get('bcbsri_online_application_udata', array('start_time'), $token, 'auth');
          foreach($result as $r) {
            $sTime = (int)$r->start_time;
          }
          bcbsri_online_application_set_cookie(array('bcbsri_oa_aid', $token, '+15 minutes'));
          bcbsri_online_application_db_set('bcbsri_online_application_udata', array('end_time' => $time, 'auth' => $token), 'auth');
          return array('success' => true, 'auth' => $token, 'stime' => $sTime, 'mtime' =>  $time, 'continued' => true);
        } else return array('success' => false, 'error' => 'Auth is invalid');
      }
    } else return array('success' => false);
  } catch (Exception $e) {
    return array('success' => false, 'error' => var_dump($e, true));
  }
}

/* ********************************************************
 * This function is used to verify the existence of an
 * application. For an existing application it verifies
 * the last time the application was accessed against the
 * threshold criteria.
 
 * Ultimately, this function is used to decide whether to
 * load an existing application or to create a new one.
 ******************************************************** */
function bcbsri_online_application_auth_verify($auth){
  if(!$auth) return false;
  else{
    $result = bcbsri_online_application_db_get('bcbsri_online_application_udata', array('end_time'), $auth, 'auth');
    // Time threshold criteria
    foreach($result as $r) {
      $eTime = (int)$r->end_time;
    }
    $threshold = strtotime('-15 minutes');
    if($threshold <= $eTime) return true;
    else return false;
  }
}

/* ********************************************************
 * This function is used to create a truely unique auth
 * code. The possibilities using the fall back are 10^42 
 * or 10.40879 tredecillion. Possible for openssl to have same.
 * Cron will remove auth after 6 months for reusablitiy
 ******************************************************** */
function bcbsri_online_application_authgen ($length = 32) {
	/*if(function_exists('openssl_random_pseudo_bytes')) {
		$strong = TRUE;
		$token = base64_encode(openssl_random_pseudo_bytes($length, $strong));
		if($strong == TRUE) {
			//base64 is about 33% longer, so we need to truncate the result
			$token = strtr(substr($token, 0, $length), '+/=', '-_,');
		}
	} else { */
    //fallback to mt_rand if php < 5.3 or no openssl available
    $characters = '0123456789';
    $characters .= 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'; 
    $charactersLength = strlen($characters)-1;
    $token = '';
    //select some random characters
    for ($i = 0; $i < $length; $i++) {
      $token .= $characters[mt_rand(0, $charactersLength)];
    }
	#}
	return $token;
}


/* ********************************************************
 * This function is used to create a truely unique auth
 * code. The possibilities using the fall back are 8^32 
 * or over 1.099 trillion. Possible for openssl to have same.
 * Cron will remove resumekey after 6 months for reusablitiy
 ******************************************************** */
function bcbsri_online_application_resumekey ($length = 8) {
	/*if(function_exists('openssl_random_pseudo_bytes')) {
		$strong = TRUE;
		$token = base64_encode(openssl_random_pseudo_bytes($length, $strong));
		if($strong == TRUE) {
			//base64 is about 33% longer, so we need to truncate the result
			$token = strtr(substr($token, 0, $length), '+/=', '-_,');
		}
	} else { */
    //fallback to mt_rand if php < 5.3 or no openssl available
    $characters = '23456789';
    $characters .= 'abcdefghijkmnpqrstuvwxyz'; 
    $charactersLength = strlen($characters)-1;
    $token = '';
    //select some random characters
    for ($i = 0; $i < $length; $i++) {
      $token .= $characters[mt_rand(0, $charactersLength)];
    }
	#}
	return $token;
}

/******************************************************************
 * $table - database table name
 * $record - array that represents the record to update
 * $updateby - array that contains the primary key or keys
 ******************************************************************/
function bcbsri_online_application_db_set($table, $record, $updateby = NULL) {
  $record = (object)$record;
  if(!empty($updateby)) drupal_write_record($table, $record, $updateby);
  else drupal_write_record($table, $record);
}

/******************************************************************
 * $table - database table name
 * $fields - array of table columns
 * $data - value to filter by
 * $condition - column to filter by
 ******************************************************************/
function bcbsri_online_application_db_get($table, $fields, $data, $condition) {
	$query = db_select($table, 'boad');
	$query->fields('boad', $fields);
  if(!empty($condition) && !empty($data))	{
    $c = 'boad.' . $condition;
    $query->condition($c, $data);
  }
	$result = $query->execute();
	return $result; #->fetchAll();
}

/******************************************************************
 * Checks to see if the item exists in the table
 ******************************************************************/
function bcbsri_online_application_check_auth ($auth) {
	// return 1;
	//$query = db_select('bcbsri_online_application_data', 'boad');
	$query = db_select('bcbsri_online_application_udata', 'boad');
	$query->fields('boad', array('auth'));
	$query->condition('boad.auth', $auth);
	$result = $query->execute();
	return $result->rowCount();
}

/******************************************************************
 * Checks to see if the item exists in the table
 ******************************************************************/
function bcbsri_online_application_check_resumekey ($resumekey) {
	// return 1;
	//$query = db_select('bcbsri_online_application_data', 'boad');
	$query = db_select('bcbsri_online_application_udata', 'boad');
	$query->fields('boad', array('resumedk'));
	$query->condition('boad.resumedk', $resumekey);
	$result = $query->execute();
	return $result->rowCount();
}

function bcbsri_online_application_get_json ($file) {
	$cPath = dirname(__FILE__);
	$json = file_get_contents($cPath . "/" . $file . ".json");
	$json = json_decode($json, true);
	return $json;
}

/* *******************************************
 * (de)ecrpyt methods
 *
 * TODO: see if we can get $key to work from database
 ******************************************* */
function bcbsri_online_application_encrypt($data, $pass) {
	$result = bcbsri_online_application_db_get('bcbsri_online_application_udata', array('auths'), $pass, 'auth');
  foreach($result as $r) {
      $skey = $r->auths;
  }
	return openssl_encrypt($data, "AES-256-CBC", $pass, false, $skey);
}

function bcbsri_online_application_decrypt($data, $pass) {
	$result = bcbsri_online_application_db_get('bcbsri_online_application_udata', array('auths'), $pass, 'auth');
  foreach($result as $r) {
      $skey = $r->auths;
  }
	return openssl_decrypt($data, "AES-256-CBC", $pass, false, $skey);
}