#!/bin/sh

if [ $(cat /proc/cpuinfo | grep 'model name' | awk '{print tolower($4)}' | cut -c-5 | head -n1 | cut -c1-3) = "arm" ]; then
	if ls iAloy_update_package > /dev/null; then
		rm -rf iAloy_update_package/
	fi

	mkdir iAloy_update_package
	mkdir iAloy_update_package/resource/
	mkdir iAloy_update_package/bin/

	cp -r ../resource/fonts/ iAloy_update_package/resource/
	cp -r ../resource/photos/ iAloy_update_package/resource/
	cp -r ../resource/scripts/ iAloy_update_package/resource/
	cp -r ../resource/hex/ iAloy_update_package/resource/

	if ls ../src/iAloy_Desktop_App > /dev/null; then
		cp ../src/iAloy_Desktop_App iAloy_update_package/bin/iAloy
	else
		cd ../ && ./configure && make && cd -
		cp ../src/iAloy_Desktop_App iAloy_update_package/bin/iAloy
	fi

	cp installer.sh iAloy_update_package/

	chmod 777 iAloy_update_package/resource/scripts/*
	chmod 777 iAloy_update_package/bin/*
	chmod 777 installer.sh


	now="$(date +'%d-%m-%Y-%H-%M-%S')"

	zip -r iAloy_update_package_$now.zip iAloy_update_package/

	cd iAloy_update_package/ && tree && cd -

	rm -rf iAloy_update_package/
else
	echo "\nThe CPU architecture is not perfect for build, Try with ARM CPU.\n"
fi