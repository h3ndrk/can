<?php
	$file = fopen("/usr/share/nginx/html/bin/values/cpu","w");
	fputs($file,str_replace(array("\n","\r"),"",shell_exec("/opt/vc/bin/vcgencmd measure_temp | cut -d '=' -f2 | sed 's/..$//'")) . " " . str_replace(array("\n","\r"),"",shell_exec("/opt/vc/bin/vcgencmd measure_volts core | cut -d '=' -f2 | sed 's/.$//'")) . " " . str_replace(array("\n","\r"),"",shell_exec("/opt/vc/bin/vcgencmd measure_clock arm | cut -d '=' -f2")));
	fclose($file);
?>
