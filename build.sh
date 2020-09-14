#!/bin/sh
# Public domain
# http://unlicense.org/
# Created by Grigore Stefan <g_stefan@yahoo.com>

if [ "$1" = "" ]; then
	echo "build [platform] [mode]"
	exit 0
fi
if ! [ -f "./port/build.$1.sh" ]; then
	DEFAULT_PLATFORM=
	if [ "$XYO_PLATFORM" = "ubuntu-18" ]; then
		DEFAULT_PLATFORM=ubuntu
	fi
	if [ "$XYO_PLATFORM" = "ubuntu-20" ]; then
		DEFAULT_PLATFORM=ubuntu
	fi
	if [ "$DEFAULT_PLATFORM" = "" ]; then
		echo "build [platform] [mode]"
		exit 0
	fi	
	. ./port/build.$DEFAULT_PLATFORM.sh $1
	RETV=$?
	if [ "$RETV" = "1" ]; then
		exit 1
	fi
	exit 0
fi
. ./port/build.$1.sh $2
RETV=$?
if [ "$RETV" = "1" ]; then
	exit 1
fi
exit 0
