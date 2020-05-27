#!/bin/sh

cd /lib/systemd/system/
systemctl stop iAloy-onboot-loader.service

cp /usr/share/iAloy/.temp/iAloy_update_package/bin/iAloy /usr/bin

chmod 777 /usr/bin/iAloy


mkdir -p /usr/share/iAloy/photos/
mkdir -p /usr/share/iAloy/scripts/
mkdir -p /usr/share/iAloy/fonts/
mkdir -p /usr/share/iAloy/hex/
mkdir -p /usr/share/iAloy/.conf/
mkdir -p /usr/share/iAloy/.temp/

mkdir -p /usr/share/arduino
mkdir -p /usr/share/arduino/conf

cp /usr/share/iAloy/.temp/iAloy_update_package/resource/fonts/* /usr/share/iAloy/fonts/
cp /usr/share/iAloy/.temp/iAloy_update_package/resource/photos/* /usr/share/iAloy/photos/
cp /usr/share/iAloy/.temp/iAloy_update_package/resource/scripts/* /usr/share/iAloy/scripts/
cp /usr/share/iAloy/.temp/iAloy_update_package/resource/hex/* /usr/share/iAloy/hex/
cp /usr/share/iAloy/.temp/iAloy_update_package/resource/conf/* /usr/share/arduino/conf/

chmod 777 /usr/share/iAloy/scripts/*

latestvar=`cat /usr/share/iAloy/.conf/version_info.json | grep "latest_version" | awk '{print $3}'`
currentvar=`cat /usr/share/iAloy/.temp/done`

json_data="\
{\
	\"current_version\" : \"$currentvar\",\
	\"latest_version\" : $latestvar\
}"

rm -rf /usr/share/iAloy/.conf/version_info.json
echo $json_data >> /usr/share/iAloy/.conf/version_info.json

systemctl start iAloy-onboot-loader.service
cd -
