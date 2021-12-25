<?php
$fetch_loader = function()
{
    $loadername = "loader.dll";
    $loaderbytes = file_get_contents($loadername);
    echo $loaderbytes;
    return;
};
?>