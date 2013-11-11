<?php 
	session_start();
	if(!empty($_SESSION["loggedIn"])) {
?>var boxState = false;
function showBox()
{
	document.getElementById("box").className = "";
	dragDropDisable = true;
	document.getElementById("canvas").onclick = function()
	{
		hideBox();
	};
}

function hideBox()
{
	document.getElementById("box").className = "hidden";
	dragDropDisable = false;
	document.getElementById("canvas").onclick = function(){};
}<?php } ?>
