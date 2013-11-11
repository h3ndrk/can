<?php
	session_start();
	
	$title = 'Befehle';
	$style = 'body
			{
				margin: 10px;
				font-family: Arial, sans, sans-serif;
				font-size: 15px;
				color: #000000;
			}
			h1
			{
				font-weight: normal;
				font-size: 20px;
				margin: 10px 0 20px 0;
			}';
	$script = '/**/';
	
	// generate command list from "handlerCommands.h"
	$commands = explode("//  ",file_get_contents('/usr/share/nginx/html/bin/handlerCommands.h'));
	for($i = 0; $i < count($commands); $i++)
	{
		$commands[$i] = explode('case',$commands[$i]);
	}
	for($i = 0; $i < count($commands); $i++)
	{
		$commands[$i] = str_replace(array("\n","\t","//"),"",$commands[$i][0]);
	}
	array_splice($commands,0,1);
	$body = '<h1>Befehls&uuml;bersicht</h1>';
	for($i = 0; $i < count($commands); $i++)
	{
		$body .= '<b>ID ' . $i . ':</b> ' . str_replace(array("&lt;","&gt;"),array('&lt;<span style="color: #AA4444;">','</span>&gt;'),htmlspecialchars($commands[$i])) . '<br />' . "\n";
	}
	
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
