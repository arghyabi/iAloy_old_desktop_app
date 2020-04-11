#!/bin/sh

cd /lib/systemd/system/
systemctl stop iAloy-onboot-loader.service

cp /usr/share/iAloy/.temp/iAloy_update_package/bin/iAloy /usr/bin

chmod 777 /usr/bin/iAloy

cp /usr/share/iAloy/.temp/iAloy_update_package/resource/fonts/* /usr/share/iAloy/fonts/
cp /usr/share/iAloy/.temp/iAloy_update_package/resource/photos/* /usr/share/iAloy/photos/
cp /usr/share/iAloy/.temp/iAloy_update_package/resource/scripts/* /usr/share/iAloy/scripts/

chmod 777 /usr/share/iAloy/scripts/*

latestvar=`cat /usr/share/iAloy/.conf/version_info.json | grep "latest_version" | awk '{print $3}'`

json_data="\
{\
	\"current_version\" : $latestvar,\
	\"latest_version\" : $latestvar\
}"

rm -rf /usr/share/iAloy/.conf/version_info.json
echo $json_data >> /usr/share/iAloy/.conf/version_info.json

systemctl start iAloy-onboot-loader.service
cd -
