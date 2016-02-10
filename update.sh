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
	put header.hpp
	put nets.cpp
	put README.md
	put server.2.0.cpp
	put server.cpp
	put sbuf.cpp
	put sbuf.h
	put client.cpp
	put tester.cpp
	bye
EOF    
	echo "done"
# done

