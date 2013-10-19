#!/bin/bash

if [ $# -eq 3 ]; then
	# Generate current timestamp
	current=$(date +%y%m%d_%H%M%S)
	# Make new backup directory and back up
	mkdir -p $2/$current/html
	mkdir -p $2/$current/root
	rsync -rv root@$1:/usr/share/nginx/html/* $2/$current/html
	rsync -rv root@$1:/root/* $2/$current/root
	# Remove old files in git directory and copy new files into
	rm -Rf $3/bin/*
	rm -Rf $3/bin/*
	rsync -rv root@$1:/usr/share/nginx/html/* $3/bin
	rsync -rv root@$1:/root/* $3/root
else
	# Echo usage if parameter-size is lower than 3
	echo "Usage: download <IP> <backupPath> <gitPath>"
fi
