<?php
	session_start();
	if(!empty($_SESSION["loggedIn"]))
	{
		if(!empty($_GET["filename"]) && !empty($_GET["thresholdMin"]) && !empty($_GET["thresholdMax"]) && !empty($_GET["clipping"]))
		{
			$file = fopen("/usr/share/nginx/html/bin/imageRecognitionValues/thresholdMin","w");
			fwrite($file,htmlspecialchars($_GET["thresholdMin"]));
			fclose($file);
			$file = fopen("/usr/share/nginx/html/bin/imageRecognitionValues/thresholdMax","w");
			fwrite($file,htmlspecialchars($_GET["thresholdMax"]));
			fclose($file);
			$file = fopen("/usr/share/nginx/html/bin/imageRecognitionValues/clipping","w");
			fwrite($file,htmlspecialchars($_GET["clipping"]));
			fclose($file);
			echo shell_exec("php -f ../bin/imageRecognition.php " . htmlspecialchars($_GET["filename"]));
		}
		elseif(!isset($_GET["filename"]) && !empty($_GET["thresholdMin"]) && !empty($_GET["thresholdMax"]) && !empty($_GET["clipping"]))
		{
			echo shell_exec("can-handler - 0 " . htmlspecialchars($_GET["thresholdMin"]) . " " . htmlspecialchars($_GET["thresholdMax"]) . " " . htmlspecialchars($_GET["clipping"]));
		}
		elseif(isset($_GET["takePicture"]))
		{
			$time = time();
			echo '/usr/share/nginx/html/sensor-images/' . $time . '.jpg|' . shell_exec("raspistill -w 640 -h 480 -n -t 0 -o /usr/share/nginx/html/sensor-images/" . $time . ".jpg");
		}
	}
?>
