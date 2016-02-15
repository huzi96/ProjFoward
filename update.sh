#!/bin/bash
HOST=192.168.1.101
USER=huzi
PASS=huzi96
# while true
# do
	echo "Starting to sftp..."  
	/usr/local/bin/lftp -u ${USER},${PASS} sftp://${HOST} <<EOF   
	cd /home/huzi/ProjFoward/
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
	put encoding.cpp
	put tester.cpp
	put broadcast.cpp
	put trial.cpp
	put nets_lite.cpp
	bye
EOF    
	echo "done"
# done

