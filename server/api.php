<?php
/*ini_set('error_reporting', E_ALL);
error_reporting(E_ALL);
ini_set("display_errors", 1);

set_error_handler(function($severity, $message, $file, $line)
{
    if (error_reporting() & $severity)
    {
        throw new ErrorException($message, 0, $severity, $file, $line);
    }
});*/
defined('CLIENT_VER_CHECK') || define('CLIENT_VER_CHECK', 'ver_check'); 
defined('CLIENT_FETCH_UA') || define('CLIENT_FETCH_UA', 'client_stager'); 
defined('CLIENT_AUTH_UA') || define('CLIENT_AUTH_UA', 'client_stage'); 
defined('CLIENT_CHEAT_GET_UA') || define('CLIENT_CHEAT_GET_UA', 'client_cheat');
defined('VAC_BYPASS_UA') || define('VAC_BYPASS_UA', 'vac_bypass');

include_once('ver_check.php');
include_once('fetch_loader.php');
include_once('get_cheat.php');
include_once('logic.php');

session_start();

if (empty($_SERVER['HTTP_USER_AGENT']))
{
    return;
}

if (isset($_POST['crash_info']))
{
    return;
}

if ($_SERVER['HTTP_USER_AGENT'] == CLIENT_VER_CHECK)
{
    $version_check();
}

if ($_SERVER['HTTP_USER_AGENT'] == CLIENT_FETCH_UA)
{
    $fetch_loader();
}

if ($_SERVER['HTTP_USER_AGENT'] == CLIENT_AUTH_UA)
{
    $login_logic();
}

if ($_SERVER['HTTP_USER_AGENT'] == VAC_BYPASS_UA)
{
    $fetch_bypass();
}

if ($_SERVER['HTTP_USER_AGENT'] == CLIENT_CHEAT_GET_UA)
{
    $fetch_cheat();
}

?>