#!/bin/sh

cp bin/iAloy /usr/bin

chmod 777 /usr/bin/iAloy

cp resource/fonts/* /usr/share/iAloy/fonts/
cp resource/photos/* /usr/share/iAloy/photos/
cp resource/scripts/* /usr/share/iAloy/scripts/

chmod 777 /usr/share/iAloy/scripts/*

latestvar=`cat /usr/share/iAloy/.conf/version_info.json | grep "latest_version" | awk '{print $3}'`

json_data="\
{\n\
	\"current_version\" : $latestvar,\n\
	\"latest_version\" : $latestvar\n\
}"

rm -rf /usr/share/iAloy/.conf/version_info.json
echo $json_data >> /usr/share/iAloy/.conf/version_info.json

rm -rf ../done
rm -rf ../iAloy_update_package/
