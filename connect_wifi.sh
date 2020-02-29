function print_usage() {
    echo "Usage:"
    echo "    $0 ssid passwd"
}

if [ $# != 2 ]
then
    print_usage
    exit -1
fi
ssid=$1
ssid=$2
echo "ssid: $1, passwd: $2"

wpa_cli -i wlan0 set_network 0 ssid \"$1\"
wpa_cli -i wlan0 set_network 0 psk \"$2\"
wpa_cli -i wlan0 save_config
wpa_cli -i wlan0 enable_network 0