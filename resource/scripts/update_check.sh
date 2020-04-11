#!/bin/sh

if ls /usr/share/iAloy/.temp/iAloy_update_package/installer.sh > /dev/null; then
	chmod 777 /usr/share/iAloy/.temp/iAloy_update_package/installer.sh
	/usr/share/iAloy/.temp/iAloy_update_package/installer.sh
	rm -rf /usr/share/iAloy/.temp/*
fi
