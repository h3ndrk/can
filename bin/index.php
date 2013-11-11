<?php
	session_start();
	if(isset($_GET["login"]) && empty($_SESSION["loggedIn"]) && !empty($_POST["key"]))
	{
		if(md5(htmlspecialchars($_POST["key"])) == "2332229a2f3e5a72f7a5863bfe8d07bf")
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
	}
	if(!empty($_SESSION["loggedIn"])) {
?><!DOCTYPE html>
<html>
	<head>
		<meta http-equiv="content-type" content="text/html; charset=utf-8">
		<title>Roboter CAN</title>
		<script src="canvas.js.php"></script>
		<!--<script src="canvasImage.js.php"></script>-->
		<script src="websocket.js.php"></script>
		<script src="script.js.php"></script>
		<style type="text/css">
			body
			{
				margin: 0;
				overflow: hidden;
				font-family: Arial, sans, sans-serif;
				font-size: 15px;
				color: #000000;
				background-color: #F2F2F2;
			}
			#header
			{
				position: absolute;
				top: 20px;
				left: 20px;
			}
			#header a img
			{
				border: none;
				text-decoration: none;
			}
			#name
			{
				font-size: 20px;
				text-shadow: 0 0 5px #F2F2F2, 0 0 5px #F2F2F2, 0 0 5px #F2F2F2;
			}
			#connected
			{
				font-size: 20px;
				text-shadow: 0 0 5px #F2F2F2, 0 0 5px #F2F2F2, 0 0 5px #F2F2F2;
			}
			#connected.connected
			{
				color: #00CC00;
			}
			#connected.disconnected
			{
				color: #CC0000;
			}
			#reconnect
			{
				text-shadow: 0 0 5px #F2F2F2, 0 0 5px #F2F2F2, 0 0 5px #F2F2F2;
				font-size: 13px;
				color: #888888;
			}
			#box
			{
				position: absolute;
				background-color: #FFFFFF;
				width: 300px;
				top: 0;
				bottom: 0;
				left: 0;
				box-shadow: 1px 0 2px rgba(0,0,0,0.125), 2px 0 7px rgba(0,0,0,0.125), 3px 0 15px rgba(0,0,0,0.125), 5px 0 25px rgba(0,0,0,0.125), 10px 0 30px rgba(0,0,0,0.125), 25px 0 75px rgba(0,0,0,0.125);
				overflow-x: hidden;
				overflow-y: auto;
				visibility: visible;
				opacity: 1;
				-moz-transition: all 0.25s ease-out;
				-webkit-transition: all 0.25s ease-out;
				-o-transition: all 0.25s ease-out;
				-ms-transition: all 0.25s ease-out;
				transition: all 0.25s ease-out;
			}
			#box.hidden
			{
				left: -300px;
				visibility: hidden;
				opacity: 0;
			}
			#box .boxLine
			{
				padding: 5px 20px;
				color: #444444;
				background-color: #F2F2F2;
				box-shadow: inset 0 1px 1px rgba(0,0,0,0.125);
			}
			#box .boxContent
			{
				padding: 20px;
				color: #222222;
			}
			#box .boxContent.noPadding
			{
				padding: 0 20px 20px 20px;
			}
			#box .boxContent span
			{
				color: #666666;
			}
			#box button
			{
				background-color: #DDDDDD;
				box-shadow: 0 2px 0 #BBBBBB;
				padding: 4px 15px 3px 15px;
				font-family: Arial, sans, sans-serif;
				font-size: 15px;
				color: #444444;
				border: none;
				border-radius: 2px;
				width: 100%;
				cursor: pointer;
				display: block;
			}
			#box button:hover
			{
				background-color: #EEEEEE;
			}
		</style>
	</head>
	<body onload="canvasInit();websocketInit();">
		<div id="header">
			<table>
				<tr>
					<td><a href="#" onClick="showBox();"><img src="canLogo.png" alt="" /></a></td>
					<td><div id="name">Roboter CAN</div><div id="connected" class="disconnected">NOT CONNECTED</div><div id="reconnect"></div></td>
				</tr>
			</table>
		</div>
		<div id="box" class="hidden">
			<div class="boxLine">General</div>
			<div class="boxContent">
				<button onClick="document.location.href='?logout';">Logout</button>
			</div>
			<div class="boxContent noPadding">
				Hostname: <span id="generalHostname">?</span><br />
				Kernel: <span id="generalKernel">?</span><br />
				Architecture: <span id="generalArchitecture">?</span><br />
				System: <span id="generalSystem">?</span><br />
				Date: <span id="generalDate">?</span><br />
				Uptime: <span id="generalUptime">?</span><br />
			</div>
			<div class="boxLine">Position</div>
			<div class="boxContent">
				Robot X: <span id="robotX">?</span><span> mm</span><br />
				Robot Y: <span id="robotY">?</span><span> mm</span><br />
				Robot ANGLE: <span id="robotAngle">?</span><span> &deg;</span><br />
				Robot target X: <span id="robotTargetX">?</span><span> mm</span><br />
				Robot target Y: <span id="robotTargetY">?</span><span> mm</span>
			</div>
			<div class="boxLine">CPU</div>
			<div class="boxContent">
				Temperature: <span id="cpuTemp">?</span><span> &deg;C</span><br />
				Volts: <span id="cpuVolts">?</span><span> Volt</span><br />
				Clock: <span id="cpuClock">?</span><span> MHz</span>
			</div>
		</div>
		<canvas style="cursor: default; margin: 0; padding: 0;" height="947" width="1680" id="canvas"></canvas>
	</body>
</html><?php } else { ?><!DOCTYPE html>
<html>
	<head>
		<meta http-equiv="content-type" content="text/html; charset=utf-8">
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
				padding: 0;
				background-color: #F2F2F2;
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
				background-color: #FFFFFF;
				border-radius: 2px;
				box-shadow: 0 1px 2px rgba(0,0,0,0.125), 0 1px 7px rgba(0,0,0,0.125), 0 1px 15px rgba(0,0,0,0.125), 0 1px 25px rgba(0,0,0,0.125);
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
				background-image: url(input.png);
				background-position: center bottom;
				background-repeat: no-repeat;
				width: 300px;
				padding: 10px;
				border: none;
				font-family: Arial, sans, sans-serif;
				font-size: 15px;
				color: #444444;
			}
			div#login input[type="submit"]
			{
				cursor: pointer;
				margin: 10px auto 10px auto;
				display: block;
				background-color: #DDDDDD;
				width: 320px;
				padding: 10px 10px 8px 10px;
				border: none;
				border-radius: 2px;
				box-shadow: 0 2px 0 #CCCCCC;
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
			<?php if(!isset($_GET["error"])) { ?><h1>Roboter CAN</h1><?php } else { ?><h1 style="color: #AA0000;">Access denied</h1><?php } ?>
			<form action="?login" method="post">
				<input type="password" name="key" placeholder="Access-Key" autofocus />
				<input type="submit" value="Anmelden" />
			</form>
			<h2>&copy; 2013 by Hendrik Sieck, NIPE-SYSTEMS.de<br />Team Partyzone Glinde</h2>
		</div></div>
	</body>
</html><?php } ?>
