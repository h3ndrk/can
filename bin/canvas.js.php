<?php 
	session_start();
	if(!empty($_SESSION["loggedIn"])) {
?>// canvas
var canvas = null; // html object
var ctx = null; // canvas context

// robot
var robotX = 0; // robot X position in mm
var robotY = 0; // robot Y position in mm
var robotAngle = 90; // robot angle in degree
var robotRadius = 50; // robot radius in mm
var robotTargetX = -1; // robot target position X (-1 = not set) in mm
var robotTargetY = -1; // robot target position Y (-1 = not set) in mm

// view
var viewX = 0; // center X coord in mm
var viewY = 0; // center Y coord in mm
var viewZoomX = 1000; // zoom of X axis (width of screen); set to 1000 mm
var viewZoomY = 0; // DO NOT EDIT, zoom of Y axis (height of screen)
var viewDrawEveryLine = 0; // DO NOT EDIT, draw every X line for grid
var viewScaleUnit = new Array("1mm","1cm","10cm","1m","10m","100m","1km"); // scale units, displayed in the left bottom corner

// drag and drop
var dragDropDisable = false; // disables drag & drop for box overlay
var dragDropStarted = false; // false = no drag, true = drag started
var dragDropLastX = -1; // last mouse position X for drag & drop
var dragDropLastY = -1; // last mouse position Y for drag & drop
var dragDropStartX = 0; // mouse start position X
var dragDropStartY = 0; // mouse start position Y

// draw interval for drag and drop
var dragDropInterval = null; // interval variable
var dragDropIntervalSpeed = 25; // interval period

// key interval for zooming
var keyZoomInInterval = null; // interval variable
var keyZoomOutInterval = null; // interval variable
var keyZoomIntervalSpeed = 50; // interval period

// some other variables
var debugPointVisibility = false;

function canvasInit()
{
	canvas = document.getElementById("canvas");
	
	canvas.width = window.innerWidth;
	canvas.height = window.innerHeight;
	window.onresize = function(e)
	{
		canvas.width = window.innerWidth;
		canvas.height = window.innerHeight;
		canvasDraw();
	};
	window.onkeydown = keyDown;
	window.onmousedown = dragDropDown;
	window.onmouseup = dragDropUp;
	window.onmousemove = dragDropMove;
	canvasDraw();
	
	// adding the event listerner for Mozilla
	if(window.addEventListener)
		document.addEventListener('DOMMouseScroll',zoomHandler,false);
	
	// for IE/OPERA etc
	document.onmousewheel = zoomHandler;
	
	console.info("Canvas initialized");
}

function clickHandler(pixelX,pixelY)
{
	robotTargetX = convertPixelToPointX(pixelX);
	robotTargetY = convertPixelToPointY(canvas.height - pixelY);
	canvasDraw();
}

function zoomHandler(e)
{
	var delta = 0;
	if(!e)
		e = window.event;
	
	// normalize the delta
	if(e.wheelDelta)
	{
		// IE and Opera
		delta = e.wheelDelta / 60;
	}
	else if(e.detail)
	{
		// W3C
		delta = -e.detail / 2;
	}
	
	if(!dragDropDisable)
	{
		if(delta >= 0)
			zoomIn();
		else
			zoomOut();
	}
}

function keyDown(e)
{
	var keycode = e.which?e.which:e.keyCode;
	if(!dragDropDisable && keycode == 82)
	{
		debugPointVisibility = !debugPointVisibility;
		console.info("Debug point is now " + ((debugPointVisibility)?("visible"):("hidden")));
		canvasDraw();
	}
}

function zoomIn()
{
	viewZoomX /= 1.1;
	canvasDraw();
}

function zoomOut()
{
	viewZoomX *= 1.1;
	canvasDraw();
}

// converts point to pixel
function convertPointToPixelX(pointX)
{
	return (canvas.width / 2) - (((viewX - pointX) / viewZoomX) * canvas.width);
}

// converts pixel to point
function convertPixelToPointX(pixelX)
{
	return viewX - (viewZoomX / 2) + ((pixelX / canvas.width) * viewZoomX);
}

// converts point to pixel
function convertPointToPixelY(pointY)
{
	return (canvas.height / 2) - (((pointY - viewY) / viewZoomY) * canvas.height);
}

// converts pixel to point
function convertPixelToPointY(pixelY)
{
	return viewY - (viewZoomY / 2) + ((pixelY / canvas.height) * viewZoomY);
}

// converts pixel distance to point distance
function convertPixelDToPointDX(pixelX)
{
	return (pixelX / canvas.width) * viewZoomX;
}

// converts pixel distance to point distance
function convertPixelDToPointDY(pixelY)
{
	return (pixelY / canvas.height) * viewZoomY;
}

// converts point distance to pixel distance
function convertPointDToPixelDX(pointX)
{
	return (pointX / viewZoomX) * canvas.width;
}

// converts point distance to pixel distance
function convertPointDToPixelDY(pointY)
{
	return (pointY / viewZoomY) * canvas.height;
}

function dragDropDown(e)
{
	if(!dragDropDisable)
	{
		dragDropStarted = true;
		dragDropInterval = setInterval(function() { canvasDraw() },dragDropIntervalSpeed);
		canvas.style.cursor = "move";
	}
	
	// click
	/*dragDropStartX = e.pageX;
	dragDropStartY = e.pageY;*/
}

function dragDropUp(e)
{
	dragDropLastX = -1;
	dragDropLastY = -1;
	dragDropStarted = false;
	clearInterval(dragDropInterval);
	canvas.style.cursor = "crosshair";
	
	// click
	/*if((e.pageX - dragDropStartX) == 0 && (e.pageY - dragDropStartY) == 0)
	{
	clickHandler(e.pageX,e.pageY);
	}*/
}

function dragDropMove(e)
{
	if(dragDropStarted)
	{
		if(dragDropLastX == -1 && dragDropLastY == -1)
		{
			dragDropLastX = e.pageX;
			dragDropLastY = e.pageY;
		}
		viewX += convertPixelDToPointDX(dragDropLastX - e.pageX);
		viewY += convertPixelDToPointDY(e.pageY - dragDropLastY);
		dragDropLastX = e.pageX;
		dragDropLastY = e.pageY;
	}
}

function canvasDraw()
{
	if(canvas.getContext)
	{
		ctx = canvas.getContext("2d");
	
		// calculations
		viewZoomY = (canvas.height / canvas.width) * viewZoomX;
		viewDrawEveryLine = Math.pow(10,Math.ceil(Math.log(viewZoomX) / Math.LN10) - 2);
		
		// clear scene
		ctx.clearRect(0,0,canvas.width,canvas.height);
		
		// X- and Y-axis-steps
		ctx.lineWidth = 1;
		ctx.lineCap = "round";
		ctx.beginPath();
		ctx.strokeStyle = "#CCCCCC";
		for(i = (viewX - (viewZoomX / 2)) / viewDrawEveryLine; i < (viewX + (viewZoomX / 2)) / viewDrawEveryLine; i += 1)
		{
			ctx.moveTo(convertPointToPixelX(Math.round(i) * viewDrawEveryLine),0);
			ctx.lineTo(convertPointToPixelX(Math.round(i) * viewDrawEveryLine),canvas.height);
		}
		for(i = (viewY - (viewZoomY / 2)) / viewDrawEveryLine; i < (viewY + (viewZoomY / 2)) / viewDrawEveryLine; i += 1)
		{
			ctx.moveTo(0,convertPointToPixelY(Math.round(i) * viewDrawEveryLine));
			ctx.lineTo(canvas.width,convertPointToPixelY(Math.round(i) * viewDrawEveryLine));
		}
		ctx.stroke();
		
		// draw scale line
		ctx.strokeStyle = "#000000";
		ctx.lineWidth = 2;
		ctx.lineCap = "butt";
		ctx.beginPath();
		ctx.moveTo(25,canvas.height - 25);
		ctx.lineTo(convertPointDToPixelDX(viewDrawEveryLine) + 25,canvas.height - 25);
		ctx.stroke();
		ctx.font = "10px Arial,sans,sans-serif";
		ctx.fillText(((Math.ceil(Math.log(viewZoomX) / Math.LN10) - 2) < 0)?("too small"):(((Math.ceil(Math.log(viewZoomX) / Math.LN10) - 2) > 6)?("too big"):(viewScaleUnit[Math.ceil(Math.log(viewZoomX) / Math.LN10) - 2])),25,canvas.height - 30); // views "too small" or "too big" or scale unit
		
		// X- and Y-axes
		ctx.strokeStyle = "#000000";
		ctx.lineWidth = 2;
		ctx.lineCap = "round";
		ctx.beginPath();
		ctx.moveTo(convertPointToPixelX(0),0);
		ctx.lineTo(convertPointToPixelX(0),canvas.height);
		ctx.moveTo(0,convertPointToPixelY(0));
		ctx.lineTo(canvas.width,convertPointToPixelY(0));
		ctx.stroke();
		
		// axes-arrows
		ctx.lineCap = "round";
		ctx.beginPath();
		ctx.moveTo(convertPointToPixelX(0) - 7,10);
		ctx.lineTo(convertPointToPixelX(0),0);
		ctx.lineTo(convertPointToPixelX(0) + 7,10);
		ctx.moveTo(canvas.width - 10,convertPointToPixelY(0) - 7);
		ctx.lineTo(canvas.width,convertPointToPixelY(0));
		ctx.lineTo(canvas.width - 10,convertPointToPixelY(0) + 7);
		ctx.stroke();
		
		// axes-description
		ctx.font = "15px Arial,sans,sans-serif";
		ctx.fillText("x",canvas.width - 10,convertPointToPixelY(0) + 20);
		ctx.fillText("y",convertPointToPixelX(0) + 10,12);
		
		// draw robot direction pointer
		if(websocketConnected && (robotX > (viewX + (viewZoomX / 2)) || robotX < (viewX - (viewZoomX / 2)) || robotY > (viewY + (viewZoomY / 2)) || robotY < (viewY - (viewZoomY / 2))))
		{
			ctx.strokeStyle = "#AAAAAA";
			ctx.lineWidth = 1;
			ctx.lineCap = "round";
			ctx.beginPath();
			ctx.moveTo(convertPointToPixelX(viewX),convertPointToPixelY(viewY));
			ctx.lineTo(convertPointToPixelX(robotX),convertPointToPixelY(robotY));
			ctx.stroke();
		}
		
		// draw robot target position
		if(websocketConnected && robotTargetX != -1 && robotTargetY != -1)
		{
			ctx.strokeStyle = "#CC0000";
			ctx.lineWidth = 1;
			ctx.lineCap = "round";
			ctx.beginPath();
			ctx.moveTo(convertPointToPixelX(robotX),convertPointToPixelY(robotY));
			ctx.lineTo(convertPointToPixelX(robotTargetX),convertPointToPixelY(robotTargetY));
			ctx.stroke();
		}
		
		// draw center cross
		if(debugPointVisibility)
		{
			ctx.strokeStyle = "#000000";
			ctx.lineWidth = 2;
			ctx.lineCap = "round";
			ctx.beginPath();
			ctx.moveTo((canvas.width / 2),(canvas.height / 2) * 0.98);
			ctx.lineTo((canvas.width / 2),(canvas.height / 2) * 1.02);
			ctx.moveTo((canvas.width / 2) - ((canvas.height / 2) * 0.02),(canvas.height / 2));
			ctx.lineTo((canvas.width / 2) + ((canvas.height / 2) * 0.02),(canvas.height / 2));
			ctx.stroke();
			ctx.font = "13px Arial,sans,sans-serif";
			ctx.fillStyle = "#000000";
			ctx.fillText("( " + viewX.toFixed(3) + " | " + viewY.toFixed(3) + " )",(canvas.width / 2) + 5,(canvas.height / 2) + 20);
		}
		
		// draw robot
		if(websocketConnected)
		{
			ctx.strokeStyle = "#000000";
			ctx.lineWidth = 2;
			ctx.lineCap = "round";
			ctx.beginPath();
			ctx.arc(convertPointToPixelX(robotX),convertPointToPixelY(robotY),convertPointDToPixelDX(robotRadius),0,2 * Math.PI);
			ctx.moveTo(convertPointToPixelX(robotX),convertPointToPixelY(robotY));
			ctx.lineTo(convertPointToPixelX(robotX + (Math.cos(robotAngle * (Math.PI / 180.0)) * robotRadius)),convertPointToPixelY(robotY + (Math.sin(robotAngle * (Math.PI / 180.0)) * robotRadius)));
			ctx.stroke();
		}
	}
}<?php } ?>
