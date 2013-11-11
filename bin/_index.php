<?php
	/*session_start();
	if(isset($_GET["login"]) && empty($_SESSION["loggedIn"]) && !empty($_POST["pwd"]))
	{
		if(md5(htmlspecialchars($_POST["pwd"])) == "2332229a2f3e5a72f7a5863bfe8d07bf")
		{
			$_SESSION["loggedIn"] = 1;
			header("Location: .");
		}
		else
		{
			header("Location: ?error");
		}
	}
	if(isset($_GET["logout"]))
	{
		$_SESSION["loggedIn"] = 0;
		$_SESSION = array();
		session_destroy();
		header("Location: .");
	}
	if(!empty($_SESSION["loggedIn"])) {
?>
<!DOCTYPE html>
<html>
	<head>
		<title>Roboter CAN</title>
		<script>
			var windows = new Array();
			function show(parameter)
			{
				windows.push(window.open("./templates/" + parameter + ".php","","innerWidth=1000,innerHeight=562,dependent=yes,location=no,resizeable=no,status=no,toolbar=no"));
			}
			function hideAll()
			{
				for(i = 0; i < windows.length; i++)
				{
					if(windows[i].closed == false)
						windows[i].close();
				}
			}
		</script>
		<style type="text/css">
			html
			{
				width: 100%;
				height: 100%;
			}
			body
			{
				font-family: Arial, sans, sans-serif;
				font-size: 15px;
				width: 100%;
				height: 100%;
				margin: 0;
				padding: 0 0 0 0;
				background-image: url(background.png);
				background-repeat: repeat;
				box-shadow: inset 0 0 500px rgba(0,0,0,0.25);
			}
			#bar
			{
				position: absolute;
				top: 0;
				bottom: 0;
				left: 0;
				right: 0;
				height: 35px;
				background-color: rgba(0,0,0,0.0625);
				box-shadow: inset 0 -3px 3px -3px rgba(0,0,0,0.25), inset 0 -10px 10px -10px rgba(0,0,0,0.25), inset 0 -35px 35px -35px rgba(0,0,0,0.25);
				overflow-y: auto;
				overflow-x: hidden;
				-moz-transition: all 0.25s ease-in-out;
				-webkit-transition: all 0.25s ease-in-out;
				-o-transition: all 0.25s ease-in-out;
				-ms-transition: all 0.25s ease-in-out;
				transition: all 0.25s ease-in-out;
			}
			#bar span
			{
				display: inline-block;
				padding: 8px 15px;
				color: #222222;
				text-shadow: 0 1px 0 rgba(255,255,255,0.25);
			}
			#bar a
			{
				display: inline-block;
				padding: 8px 15px;
				color: #444444;
				text-shadow: 0 1px 0 rgba(255,255,255,0.25);
				text-decoration: none;
				float: right;
			}
			#bar a:hover
			{
				background-color: rgba(255,255,255,0.25);
			}
			#content
			{
				position: absolute;
				top: 35px;
				bottom: 0;
				left: 0;
				right: 0;
				overflow: auto;
				padding: 20px;
				color: #444444;
			}
			#content h1
			{
				font-weight: normal;
				font-size: 20px;
				color: #222222;
				margin: 0 0 20px 0;
			}
			#content p
			{
				font-weight: normal;
				font-size: 15px;
				color: #444444;
				margin: 15px 0 20px 0;
			}
			#content table
			{
				border-spacing: 0;
				margin: 0;
				padding: 0;
				width: 100%;
				height: 100%;
			}
			#content table tr
			{
				margin: 0;
				padding: 0;
			}
			#content table tr td
			{
				margin: 0;
				padding: 0;
				font-size: 25px;
				color: #444444;
				text-align: center;
				width: 33.333%;
				height: 50%;
				cursor: pointer;
			}
			#content table tr td:hover
			{
				box-shadow: inset 0 0 0 1px rgba(0,0,0,0.125);
			}
			#content table tr td pre
			{
				margin: 0;
				padding: 0;
			}
		</style>
	</head>
	<body onUnload='hideAll();'>
		<div id="bar" onMouseover='document.getElementById("bar").className="active";document.getElementById("content").className="active";' onMouseout='document.getElementById("bar").className="";document.getElementById("content").className="";'>
			<span>Roboter CAN</span><a class="right" href="?logout">Abmelden</a>
		</div>
		<div id="content">
			<table>
				<tr>
					<td onClick='show("informations");'>Informationen</td>
					<td onClick='show("distance");'>Entfernungsmessung</td>
					<td onClick='show("commands");'>Befehle</td>
				</tr>
				<tr>
					<td onClick='show("images");'>Sensorbilder</td>
					<td onClick='show("network");'>Netzwerk</td>
				</tr>
			</table>
		</div>
	</body>
</html>
<?php } else { ?>
<!DOCTYPE html>
<html>
	<head>
		<title>Roboter CAN</title>
		<style type="text/css">
			html
			{
				width: 100%;
				height: 100%;
			}
			body
			{
				font-family: Arial, sans, sans-serif;
				width: 100%;
				height: 100%;
				margin: 0;
				padding: 0 0 0 0;
				background-image: url(background.png);
				background-repeat: repeat;
				box-shadow: inset 0 0 500px rgba(0,0,0,0.25);
			}
			div#overlay
			{
				top: 0;
				bottom: 0;
				padding: 100px 0 0 0;
			}
			div#login
			{
				width: 320px;
				padding: 20px;
				margin: 0 auto 0 auto;
				background-color: rgba(0,0,0,0.125);
				border-radius: 3px;
				box-shadow: inset 0 1px 3px rgba(0,0,0,0.125);
				text-align: center;
			}
			div#login h1
			{
				font-size: 25px;
				font-weight: normal;
				color: #666666;
				text-shadow: 0 1px 0 rgba(255,255,255,0.75);
				margin: 10px 0 30px 0;
			}
			div#login input[type="password"]
			{
				margin: 10px auto;
				display: block;
				background-color: #FFFFFF;
				width: 300px;
				padding: 10px;
				border: none;
				border-radius: 3px;
				box-shadow: 0 1px 2px rgba(0,0,0,0.25);
				font-family: Arial, sans, sans-serif;
				font-size: 15px;
				color: #444444;
			}
			div#login input[type="submit"]
			{
				margin: 10px auto 10px auto;
				display: block;
				background-color: rgba(255,255,255,0.5);
				width: 320px;
				padding: 10px;
				border: none;
				border-radius: 3px;
				box-shadow: 0 1px 2px rgba(0,0,0,0.25);
				font-family: Arial, sans, sans-serif;
				font-size: 15px;
				color: #444444;
			}
			div#login h2
			{
				font-size: 13px;
				font-weight: normal;
				color: #888888;
				text-shadow: 0 1px 0 rgba(255,255,255,0.75);
				margin: 20px 0 0 0;
			}
			div#login h2 a
			{
				color: #666666;
			}
			div#login h2 a:hover
			{
				color: #0065FF;
			}
		</style>
	</head>
	<body>
		<div id="overlay"><div id="login">
			<img src="canLogo.png" alt="" />
			<?php if(!isset($_GET["error"])) { ?><h1>Roboter CAN</h1><?php } else { ?><h1 style="color: #AA0000;">Zugriff verweigert</h1><?php } ?>
			<form action="?login" method="post">
				<input type="password" name="pwd" placeholder="Passwort" autofocus />
				<input type="submit" value="Anmelden" />
			</form>
			<h2>&copy; <?php date_default_timezone_set("Europe/Berlin"); echo date("Y"); ?> by Hendrik Sieck, <a href="http://www.nipe-systems.de" target="_blank">NIPE-SYSTEMS.de</a></h2>
		</div></div>
	</body>
</html>
<?php }*/ ?>
