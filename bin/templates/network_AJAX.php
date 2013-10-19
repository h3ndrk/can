<?php
	session_start();
	if(!empty($_SESSION["loggedIn"]))
	{
		if(isset($_GET["getInformations"]))
		{
			echo str_replace("          ","&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;",str_replace("\n","<br />",shell_exec("ifconfig")));
		}
		elseif(isset($_GET["save"]) && !empty($_POST["text"]))
		{
			echo shell_exec("sudo echo -e -n \"" . str_replace("\n",'\n',$_POST["text"]) . "\" > " . /*$_POST["filename"]*/"/var/www/testfile");
		}
	}
?>
