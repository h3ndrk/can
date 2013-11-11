<?php
	session_start();
	
	$title = 'Entfernungsmessung';
	$style = 'body
			{
				margin: 10px;
				font-family: Arial, sans, sans-serif;
				font-size: 15px;
				color: #000000;
			}
			#image
			{
				position: absolute;
				top: 45px;
				bottom: 10px;
				left: 10px;
				right: 220px;
				box-shadow: 0 1px 2px rgba(0,0,0,0.5);
				overflow: auto;
				background-color: #EEEEEE;
				text-align: center;
			}
			#image img
			{
				max-width: 100%;
				max-heigth: 100%;
			}
			#imageInformations
			{
				position: absolute;
				top: 45px;
				bottom: 10px;
				right: 10px;
				width: 200px;
				overflow: auto;
			}';
	$script = 'function getDistance()
			{
				write(\'<span style="color: #AAAAAA;">Befehl wird ausgef&uuml;hrt...</span>\');
				if(document.getElementById("thresholdMin").value != "" && document.getElementById("thresholdMax").value != "" && document.getElementById("clipping").value != "")
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
						http.open("GET","./distance_AJAX.php?thresholdMin=" + document.getElementById("thresholdMin").value + "&thresholdMax=" + document.getElementById("thresholdMax").value + "&clipping=" + document.getElementById("clipping").value,true);
						http.onreadystatechange = function()
						{
							if(http.readyState == 4)
							{
								document.getElementById("imageInner").src = http.responseText.split("|")[0].replace("/usr/share/nginx/html","");
								write(http.responseText.replace("|","<br />"));
							}
						}
						http.send(null);
					}
				}
				else
				{
					write(\'<span style="color: #AA0000;">Eingaben &uuml;berpr&uuml;fen</span>\');
				}
			}
			function takePicture()
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
					http.open("GET","./distance_AJAX.php?takePicture",true);
					http.onreadystatechange = function()
					{
						if(http.readyState == 4)
						{
							document.getElementById("imageInner").src = http.responseText.split("|")[0].replace("/usr/share/nginx/html","");
							write(http.responseText.split("|")[0] + "<br />" + http.responseText.split("|")[1]);
						}
					}
					http.send(null);
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
								document.getElementById("imageInner").src = http.responseText.split("|")[0].replace("/usr/share/nginx/html","");
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
				document.getElementById("imageInformations").innerHTML = text;
			}';
	$body = '<button onClick=\'getDistance();\'>Neue Messung</button> <button onClick=\'takePicture();\'>Neues Bild</button> <input id="filename" type="text" placeholder="Dateiname" /> <input style="width: 50px;" id="thresholdMin" type="text" placeholder="min" /> <input style="width: 50px;" id="thresholdMax" type="text" placeholder="max" /> <input style="width: 75px;" id="clipping" type="text" placeholder="clipping" /> <button onClick=\'analyze();\'>Analysieren</button>
		<div id="image">
			<img id="imageInner" src="" alt="" />
		</div>
		<div id="imageInformations">Bildinformationen</div>';
	
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
