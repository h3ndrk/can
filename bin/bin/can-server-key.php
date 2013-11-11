<?php
	if(count($argv) > 1)
	{
		echo strlen(base64_encode(sha1($argv[1] . "258EAFA5-E914-47DA-95CA-C5AB0DC85B11",true))) . " " . base64_encode(sha1($argv[1] . "258EAFA5-E914-47DA-95CA-C5AB0DC85B11",true)) . "\n";
	}
	else
	{
		echo "error\n";
	}
?>
