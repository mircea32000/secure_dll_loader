<?php
$fetch_bypass = function()
{
    $bypassname = "bypass.dll";
    if(isset($_SESSION['loggedin']))
    {
        $bypassbytes = file_get_contents($bypassname);
        echo $bypassbytes;
    }
};
?>