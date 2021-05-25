#! /usr/bin/zsh
port=8080
ip4=$(/sbin/ip -o -4 addr list eth0 | awk '{print $4}' | cut -d/ -f1)
echo "http://$ip4:$port"
python3 -m http.server $port
