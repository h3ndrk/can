#!/bin/bash

if [ $# -eq 2 ]; then
	# Remove old files in bin directory and copy new files into it
	rsync -razuv --exclude-from $2/scripts/rsyncExlude --delete $2/bin/ root@$1:/usr/share/nginx/html/
else
	# Echo usage if parameter-size is lower than 2
	echo "Usage: upload <IP> <gitPath>"
fi
