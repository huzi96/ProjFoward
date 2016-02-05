#!/bin/bash
HOST=10.211.55.7
USER=huzi
PASS=mncvoqdf   
# while true
# do
	echo "Starting to sftp..."  
	/usr/local/bin/lftp -u ${USER},${PASS} sftp://${HOST} <<EOF   
	cd /home/huzi/ProjFWD/
	put Makefile
	put tsh.c
	bye
EOF    
	echo "done"
# done

