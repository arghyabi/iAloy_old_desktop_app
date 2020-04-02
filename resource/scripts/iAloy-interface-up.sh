ip_address="8.8.8.8"

echo -e "checking internet connection..."
ping -q -c2 "$ip_address" > /dev/null

if [ $? -ne 0 ]
then
	ifdown eth0
	ifup eth0
fi
