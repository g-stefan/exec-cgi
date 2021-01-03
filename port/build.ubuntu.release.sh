#!/bin/sh
# Public domain
# http://unlicense.org/
# Created by Grigore Stefan <g_stefan@yahoo.com>

echo "-> release exec-cgi"

cmdX(){
	if ! "$@" ; then
		echo "Error: release"
		exit 1
	fi
}

export XYO_PATH_RELEASE=release

cmdX /bin/sh ./port/build.ubuntu.sh make
cmdX /bin/sh ./port/build.ubuntu.sh install
cmdX /bin/sh ./port/build.ubuntu.sh install-release
cmdX xyo-cc exec-cgi --archive-release-sha512 --version-file=source/exec-cgi.version.ini
