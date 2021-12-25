<?php
$fetch_cheat = function()
{
    $cheatname = "csgo.dll";
    if(isset($_SESSION['loggedin']))
    {
        $cheatbytes = file_get_contents($cheatname);
        echo $cheatbytes;
    }
};
?>