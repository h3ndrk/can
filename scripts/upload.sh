#!/bin/bash

if [ $# -eq 2 ]; then
	# Remove old files in bin directory and copy new files into
	ssh root@$1 rm -Rf /usr/share/nginx/html/*
	rsync -rv  $2/bin/* root@$1:/usr/share/nginx/html/
else
	# Echo usage if parameter-size is lower than 2
	echo "Usage: upload <IP> <gitPath>"
fi
