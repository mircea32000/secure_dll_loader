<?php
$login_logic = function()
{
    $link = mysqli_connect("localhost", "id15705445_qjaws", "=(h{g}[#!%Su0DdT");
    $database = mysqli_select_db($link, "id15705445_qjawshook");
    $user = '';
    $password = '';
    $plain_pass = '';
    $hwid = '';

    if (isset($_POST['username']))
    {
    	$user = $_POST['username'];
    }

    if (isset($_POST['password']))
    {
    	$password = $_POST['password'];
    }

    if (isset($_POST['hwid']))
    {
    	$hwid = $_POST['hwid'];
    }
    
    $tables = "mybb_users";

    $sql = "SELECT * FROM ".$tables ." WHERE username = '".mysqli_real_escape_string($link, $user) ."'";
    $result = $link->query($sql);
    if ($result->num_rows > 0)
    {
	// Outputting the rows
    	while ($row = $result->fetch_assoc())
    	{
		$password = $row['password'];
		$salt = $row['salt'];
		$plain_pass = $_POST['password'];
		$stored_pass = md5(md5($salt).md5($plain_pass));

		function Redirect($url, $permanent = false)
		{
			if (headers_sent === false)
			{
				header('Location: ' . $url, true, ($permanent === true) ? 301 : 302);
			}
			exit();
		}
		if ($stored_pass != $row['password'])
		{
			echo base64_encode("p0<br>"); // Wrong pass, user exists
		}
		else
		{
			echo base64_encode("p1<br>"); // Correct pass

			echo base64_encode("g".$row['usergroup'] . "<br>");

			if (!empty($row['hwid']))
			{
				if ($hwid != $row['hwid'])
				{
					echo base64_encode("h2"); // Wrong
				}
				else
				{
					echo base64_encode("h1"); // Correct
					$_SESSION["loggedin"] = true;
				}
			}
			else
			{
				$sql = "UPDATE ".$tables ." SET hwid='$hwid' WHERE username='$user'";
				if (mysqli_query($link, $sql))
				{
					echo $row['hwid'];
					echo base64_encode("h3"); // HWID Set
				}
				else
				{
					echo base64_encode("h4"); // Else errors
				}
			}
		}
	}
  }
  return;
};


?>