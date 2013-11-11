<?php
	session_start();
	
	$title = 'Netzwerk';
	$style = 'body
			{
				margin: 0;
				font-family: Arial, sans, sans-serif;
				font-size: 15px;
				color: #000000;
			}
			h1
			{
				font-weight: normal;
				font-size: 20px;
				margin: 10px 0 20px 0;
			}
			p
			{
				margin: 10px 0 20px 0;
				font-size: 13px;
				color: #666666;
				padding: 0;
			}
			textarea
			{
				width: 950px;
				height: 300px;
			}
			#inner
			{
				position: absolute;
				top: 10px;
				bottom: 160px;
				left: 10px;
				right: 10px;
				overflow: auto;
			}
			#informations
			{
				position: absolute;
				bottom: 0;
				left: 0;
				right: 0;
				height: 140px;
				padding: 5px;
				box-shadow: inset 0 7px 7px -7px rgba(0,0,0,0.5);
				overflow: auto;
				background-color: #EEEEEE;
				font-family: mono, sans, sans-serif;
				font-size: 10px;
			}';
	$script = 'function getInformations()
			{
				write(\'<span style="color: #AAAAAA;">Befehl wird ausgef&uuml;hrt...</span>\');
				var http = null;
				if(window.XMLHttpRequest)
				{
					http = new XMLHttpRequest();
				}
				else if(window.ActiveXObject)
				{
					http = new ActiveXObject("Microsoft.XMLHTTP");
				}
				if(http != null)
				{
					http.open("GET","./network_AJAX.php?getInformations",true);
					http.onreadystatechange = function()
					{
						if(http.readyState == 4)
						{
							write(http.responseText);
						}
					}
					http.send(null);
				}
			}
			function save(filename)
			{
				write(\'<span style="color: #AAAAAA;">Befehl wird ausgef&uuml;hrt...</span>\');
				var http = null;
				if(window.XMLHttpRequest)
				{
					http = new XMLHttpRequest();
				}
				else if(window.ActiveXObject)
				{
					http = new ActiveXObject("Microsoft.XMLHTTP");
				}
				if(http != null)
				{
					http.open("POST","./network_AJAX.php?save",true);
					http.setRequestHeader("Content-type","application/x-www-form-urlencoded");
					http.onreadystatechange = function()
					{
						if(http.readyState == 4)
						{
							write(http.responseText);
						}
					}
					http.send("text=" + document.getElementById(filename.split("/")[filename.split("/").length - 1]).value + "&filename=" + filename);
				}
			}
			function analyze()
			{
				write(\'<span style="color: #AAAAAA;">Befehl wird ausgef&uuml;hrt...</span>\');
				if(document.getElementById("filename").value != "" && document.getElementById("thresholdMin").value != "" && document.getElementById("thresholdMax").value != "" && document.getElementById("clipping").value != "")
				{
					var http = null;
					if(window.XMLHttpRequest)
					{
						http = new XMLHttpRequest();
					}
					else if(window.ActiveXObject)
					{
						http = new ActiveXObject("Microsoft.XMLHTTP");
					}
					if(http != null)
					{
						http.open("GET","./distance_AJAX.php?filename=" + document.getElementById("filename").value + "&thresholdMin=" + document.getElementById("thresholdMin").value + "&thresholdMax=" + document.getElementById("thresholdMax").value + "&clipping=" + document.getElementById("clipping").value,true);
						http.onreadystatechange = function()
						{
							if(http.readyState == 4)
							{
								document.getElementById("imageInner").src = http.responseText.split("|")[0].replace("/var/www","");
								write(http.responseText.replace("|","<br />"));
							}
						}
						http.send(null);
					}
				}
				else
				{
					write(\'<span style="color: #AA0000;">Eingaben &uuml;berpr&uuml;fen</span>\');;
				}
			}
			function write(text)
			{
				document.getElementById("informations").innerHTML = text;
			}';
	$body = '<div id="informations"></div><div id="inner"><h1>Netzwerk-Informationen</h1><p>F&uuml;hrt den Befehl &quot;ifconfig&quot; aus.</p><p><button onClick=\'getInformations();\'>Abrufen</button></p><h1>DHCP-Konfiguration</h1><p>/etc/dhcpd.conf</p><p><textarea id="dhcpd.conf">' . shell_exec("sudo cat /etc/dhcpd.conf") . '</textarea></p></div>';
	
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
