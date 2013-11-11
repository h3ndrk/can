<?php 
	session_start();
	if(!empty($_SESSION["loggedIn"])) {
?>// websocket
var websocketHost = <?php echo '"' . $_SERVER["SERVER_ADDR"] . '"'; ?>;
var websocketPort = "1027";
var websocketConnected = false;
var websocketReconnectTimeout = 10; // in seconds
var websocketReconnecting = false; // true = client will reconnect, false = client won't reconnect
var websocket = null;
var websocketGlobalKey = 9997110;

var websocketDelimiter = "|";

function websocketInit()
{
	window.onbeforeunload = function()
	{
		// disable onclose handler first
		websocket.onclose = function(){};
		
		// close the connection client-side
		websocket.close();
	};
	try
	{
		console.info("Connect to 'ws://" + websocketHost + ":" + websocketPort + "'");
		updateHeader();
		websocket = new WebSocket("ws://" + websocketHost + ":" + websocketPort);
		websocket.onopen = function(e)
		{
			websocketConnected = true;
			console.info("Connected to websocket server 'ws://" + websocketHost + ":" + websocketPort + "'");
			updateHeader();
			canvasDraw();
			write(14,"0");
		};
		websocket.onmessage = function(e)
		{
			console.log("Received: " + e.data);
			if(e.data.search(/general/gi) != -1)
			{
				var temp = e.data.split(websocketDelimiter);
				generalHostname = temp[1];
				generalKernel = temp[2];
				generalArchitecture = temp[3];
				generalSystem = temp[4];
				generalDate = temp[5];
				generalUptime = temp[6];
				updateGeneral();
			}
			else if(e.data.search(/position/gi) != -1)
			{
				var temp = e.data.split(websocketDelimiter);
				robotX = parseFloat(temp[1]);
				robotY = parseFloat(temp[2]);
				robotAngle = parseFloat(temp[3]);
				updatePosition();
			}
			else if(e.data.search(/cpu/gi) != -1)
			{
				var temp = e.data.split(websocketDelimiter);
				cpuTemp = parseFloat(temp[1]);
				cpuVolts = parseFloat(temp[2]);
				cpuClock = parseInt(temp[3]);
				updateCpu();
			}
		};
		websocket.onclose = function(e)
		{
			websocketConnected = false;
			console.info("Disconnected from websocket server 'ws://" + websocketHost + ":" + websocketPort + "'");
			updateHeader();
			canvasDraw();
			if(websocketReconnecting)
			{
				console.info("Reconnect in " + websocketReconnectTimeout + " seconds");
				setTimeout(function(){websocketInit();},websocketReconnectTimeout * 1000);
			}
			else
			{
				console.info("Reconnecting disabled");
			}
			generalHostname = "?";
			generalKernel = "?";
			generalArchitecture = "?";
			generalSystem = "?";
			generalDate = "?";
			generalUptime = "?";
			robotX = "?";
			robotY = "?";
			robotAngle = "?";
			cpuTemp = "?";
			cpuVolts = "?";
			cpuClock = "?";
			updateGeneral();
			updatePosition();
			updateCpu();
		};
		websocket.onerror = function(e)
		{
			console.error("Error: " + e.data);
			updateHeader();
			canvasDraw();
		};
	}
	catch(ex)
	{
		console.error(ex);
	}
}

var generalHostname;
var generalKernel;
var generalArchitecute;
var generalSystem;
var generalDate;
var generalUptime;
function updateGeneral()
{
	document.getElementById("generalHostname").innerHTML = generalHostname;
	document.getElementById("generalKernel").innerHTML = generalKernel;
	document.getElementById("generalArchitecture").innerHTML = generalArchitecture;
	document.getElementById("generalSystem").innerHTML = generalSystem;
	document.getElementById("generalDate").innerHTML = generalDate;
	document.getElementById("generalUptime").innerHTML = generalUptime;
}

function updatePosition()
{
	document.getElementById("robotX").innerHTML = robotX;
	document.getElementById("robotY").innerHTML = robotY;
	document.getElementById("robotAngle").innerHTML = robotAngle;
	canvasDraw();
}

var cpuTemp;
var cpuVolts;
var cpuClock;
function updateCpu()
{
	document.getElementById("cpuTemp").innerHTML = cpuTemp;
	document.getElementById("cpuVolts").innerHTML = cpuVolts;
	document.getElementById("cpuClock").innerHTML = (cpuClock=="?")?("?"):(Math.round(cpuClock / 1000.0 / 1000.0));
	canvasDraw();
}

function updateHeader()
{
	if(websocketConnected)
	{
		document.getElementById("connected").innerHTML = "CONNECTED";
		document.getElementById("connected").className = "connected";
		document.getElementById("reconnect").innerHTML = "";
	}
	else
	{
		document.getElementById("connected").innerHTML = "NOT CONNECTED";
		document.getElementById("connected").className = "disconnected";
		if(websocketReconnecting)
			document.getElementById("reconnect").innerHTML = "Reconnecting every " + websocketReconnectTimeout + " seconds";
		else
			document.getElementById("reconnect").innerHTML = "Reconnecting disabled";
	}
}

function write(commandId,commandParam)
{
	websocket.send(websocketGlobalKey + ":" + commandId + ":" + commandParam);
}<?php } ?>
