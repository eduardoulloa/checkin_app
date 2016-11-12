<?php
$sessionCookie = session_get_cookie_params();
$pD = 'planDetails';
setcookie('bcbsri_2016_medicare[' . $pD . ']', 'medical:BlueCHiP for Medicare Value|HMO-POS Network|16.00,dental:', 0, $sessionCookie['path'], $sessionCookie['domain'], $sessionCookie['secure'], TRUE);
header('Location: /shop/2016/bluechip-for-medicare/application');