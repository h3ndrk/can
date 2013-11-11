<?php
	session_start();
	
	$title = 'Informationen';
	$style = 'body
			{
				margin: 10px;
				font-family: Arial, sans, sans-serif;
				font-size: 15px;
				color: #000000;
			}
			pre
			{
				display: inline-block;
				margin: 0 0 0 10px;
				color: #444444;
			}';
	$script = '/**/';
	$cpuValues = explode(" ",file_get_contents("/usr/share/nginx/html/bin/values/cpu"));
	$positionValues = explode(" ",file_get_contents("/usr/share/nginx/html/bin/values/position"));
	$body = '<span style="font-weight: bold;">Hostname:</span> <pre>' . str_replace("\n","",shell_exec("hostname")) . '</pre><br />
		<span style="font-weight: bold;">Kernel:</span> <pre>' . str_replace("\n","",shell_exec("uname -s")) . ', ' . str_replace("\n","",shell_exec("uname -r")) . '</pre><br />
		<span style="font-weight: bold;">Architektur:</span> <pre>' . str_replace("\n","",shell_exec("uname -m")) . '</pre><br />
		<span style="font-weight: bold;">System:</span> <pre>' . str_replace("\n","",shell_exec("uname -o")) . '</pre><br />
		<span style="font-weight: bold;">Datum:</span> <pre>' . str_replace("\n","",shell_exec("date")) . '</pre><br />
		<span style="font-weight: bold;">Uptime:</span> <pre>' . str_replace("\n","",shell_exec("uptime")) . '</pre><br />
		<span style="font-weight: bold;">CPU-Temperatur:</span> <pre>' . $cpuValues[0] . ' &deg;C</pre><br />
		<span style="font-weight: bold;">CPU-Spannung:</span> <pre>' . $cpuValues[1] . ' Volt</pre><br />
		<span style="font-weight: bold;">CPU-Frequenz:</span> <pre>' . round($cpuValues[2] / 1000.0 / 1000.0) . ' MHz</pre><br />
		<span style="font-weight: bold;">Position X:</span> <pre>' . str_replace(array("\n","\r"),"",$positionValues[0]) . ' mm</pre><br />
		<span style="font-weight: bold;">Position Y:</span> <pre>' . str_replace(array("\n","\r"),"",$positionValues[1]) . ' mm</pre><br />
		<span style="font-weight: bold;">Position ANGLE:</span> <pre>' . str_replace(array("\n","\r"),"",$positionValues[2]) . ' &deg;</pre>';
	
	function generateTemplate()
	{
		global $title,$style,$script,$body;
		return str_replace("%TITLE%",$title,str_replace("%STYLE%",$style,str_replace("%SCRIPT%",$script,str_replace("%BODY%",$body,file_get_contents("./body")))));
	}
	
	if(!empty($_SESSION["loggedIn"]))
	{
		echo generateTemplate();
	}
	else
	{
		echo '<h1>403 - Permission denied</h1><a href="/">Login</a>';
	}
?>
