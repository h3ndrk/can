<?php
	if(count($argv) > 1 && file_exists($argv[1]))
	{
		$image = imagecreatefromjpeg($argv[1]);
	
		$width = imagesx($image);
		$height = imagesy($image);
	
		$thresholdMin = intval(file_get_contents("/usr/share/nginx/html/bin/imageRecognitionValues/thresholdMin"));
		$thresholdMax = intval(file_get_contents("/usr/share/nginx/html/bin/imageRecognitionValues/thresholdMax"));
		
		$clipping = intval(file_get_contents("/usr/share/nginx/html/bin/imageRecognitionValues/clipping"));
	
		$averageX = 0;
		$averageY = 0;
		$amount = 0;
		
		if(count($argv) > 2 && $argv[2] == "d")
			echo "\n";
	
		for($threshold = $thresholdMin; $threshold < $thresholdMax; $threshold++)
		{
			for($y = (($height / 2) - $clipping); $y < (($height / 2) + $clipping); $y++)
			{
				for($x = 0; $x < $width; $x++)
				{
					$rgb = imagecolorat($image,$x,$y);
					$red = ($rgb >> 16) & 0xFF;
					$green = ($rgb >> 8) & 0xFF;
					$blue = $rgb & 0xFF;
				
					if((($red + $green + $blue) / 3) >= $threshold)
					{
						$averageX += $x;
						$averageY += $y;
						$amount += 1;
						if(count($argv) > 2 && $argv[2] == "d")
							echo "Laser match: [X: " . $x . ", Y: " . $y . "] = R: " . $red . ", G: " . $green . ", B: " . $blue . ", T: " . $threshold . "\n";
					}
				}
			}
		}
		
		if($amount != 0)
		{
			$averageX = ($averageX / $amount) + 1;
			$averageY = ($averageY / $amount) + 1;
		
			echo $argv[1] . "|" . $averageX . "," . $averageY . "\n";
		}
		else
		{
			echo $argv[1] . "|laser out of range\n";
		}
	
		imagedestroy($image);
	}
	else
	{
		echo "no such file or directory\n";
	}
?>
