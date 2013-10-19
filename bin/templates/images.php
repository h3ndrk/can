<?php
	session_start();
	
	$title = 'Entfernungsmessung';
	$style = 'body
			{
				margin: 0;
				font-family: Arial, sans, sans-serif;
				font-size: 15px;
				color: #000000;
				background-color: #000000;
			}
			#imageBox
			{
				position: absolute;
				top: 0;
				bottom: 0;
				left: 0;
				right: 0;
				overflow-y: auto;
				overflow-x: hidden;
			}
			.imageContainer
			{
				display: inline-block;
				width: 245px;
				height: 150px;
				overflow: hidden;
				background-color: #000000;
				text-align: center;
			}
			.imageContainer .filename
			{
				position: absolute;
				width: 235px;
				height: 25px;
				margin: 100px 0 0 0;
				padding: 25px 10px 0 10px;
				text-align: center;
				color: #FFFFFF;
				font-size: 13px;
				font-weight: bold;
				text-shadow: 0 0 2px #000000, 0 0 5px #000000;
				background-image: url("imagesFilename.png");
				background-repeat: repeat-x;
				background-position: 0 0;
			}
			.imageContainer img
			{
				max-width: 245px;
				max-height: 150px;
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
	$body = '';
	$files = array();
	$dir = opendir("../sensor-images/");
	$cnt = 0;
	for($i = 0; $file = readdir($dir); $i++)
	{
		if(is_file("../sensor-images/" . $file) && !strpos($file,".php") && !strpos($file,".ht") && $file != ".." && $file != ".")
		{
			$files[$i] = $file;
		}
	}
	asort($files);
	echo '<div id="imageBox">';
	foreach($files as $file)
	{
		echo '<a href="../sensor-images/' . $file . '" target="_blank"><div class="imageContainer"><div class="filename">' . $file . '</div><img src="../sensor-images/' . $file . '" alt="" /></div></a>';
	}
	echo '</div>';
	
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
