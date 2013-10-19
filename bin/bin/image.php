<?php
	/*if(count($argv) > 1 && file_exists($argv[1]))
	{*/
		$image = imagecreatefromstring(shell_exec("/opt/vc/bin/raspistill -w 640 -h 480 -n -t 0 -o -"/*"cat /usr/share/nginx/html/img.jpg"*/));
		
		imagepng($image,"/usr/share/nginx/html/img0.png");
		
		/*$width = imagesx($image);
		$height = imagesy($image);
		
		$thresholdMin = 240;//intval(file_get_contents("/usr/share/nginx/html/bin/imageRecognitionValues/thresholdMin"));
		$thresholdMax = 255;//intval(file_get_contents("/usr/share/nginx/html/bin/imageRecognitionValues/thresholdMax"));
		
		$clipping = 200;//intval(file_get_contents("/usr/share/nginx/html/bin/imageRecognitionValues/clipping"));
		
		$averageX = 0;
		$averageY = 0;
		$amount = 0;
		
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
					}
				}
			}
		}
		
		if($amount != 0)
		{
			$averageX = ($averageX / $amount) + 1;
			$averageY = ($averageY / $amount) + 1;
		
			echo $averageX . "," . $averageY . "\n";
		}
		else
		{
			$rgb = imagecolorat($image,245,169);
			$red = ($rgb >> 16) & 0xFF;
			$green = ($rgb >> 8) & 0xFF;
			$blue = $rgb & 0xFF;
			echo "laser out of range\n" . $red . ", " . $green . ", " . $blue . "\n";
		}*/
		
		imagefilter($image,IMG_FILTER_CONTRAST,-100);
		imagefilter($image,IMG_FILTER_GRAYSCALE);
		imagefilter($image,IMG_FILTER_CONTRAST,-100);
		
		$width = imagesx($image);
		$height = imagesy($image);
		
		$averageX = 0;
		$averageY = 0;
		$amount = 0;
		
		for($y = 0; $y < $height; $y+=1)
		{
			for($x = 0; $x < $width; $x+=1)
			{
				$rgb = imagecolorat($image,$x,$y);
				$red = ($rgb >> 16) & 0xFF;
				$green = ($rgb >> 8) & 0xFF;
				$blue = $rgb & 0xFF;
				
				if((($red + $green + $blue) / 3) >= 255)
				{
					$averageX += $x;
					$averageY += $y;
					$amount += 1;
				}
			}
		}
		
		if($amount != 0)
		{
			$averageX = ($averageX / $amount) + 1;
			$averageY = ($averageY / $amount) + 1;
			
			imagearc($image,$averageX,$averageY,50,50,0,360,imagecolorallocate($image,0,0,255));
			
			echo $averageX . "," . $averageY . "\n";
		}
		else
		{
			echo "laser out of range\n";
		}
		
		imagepng($image,"/usr/share/nginx/html/img1.png");
		
		imagedestroy($image);
	/*}
	else
	{
		echo "no such file or directory\n";
	}*/
?>
