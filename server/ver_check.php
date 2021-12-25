<?php
$version_check = function()
{
    $versionfile = "client_ver.txt";
    $actual_version = file_get_contents($versionfile);
    echo $actual_version;
    return;
};
?>