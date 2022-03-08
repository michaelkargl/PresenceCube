ip_address=$2

if [ $# -lt 2 ]; then
   echo 'Argument error:'
   echo 'Usage: set-hud --ip <hostname|ip-address>'
   exit 1
fi

function _create_message_body {
    local runlevel=$2
    local employees=$4
    local color=$6

    # use --argjson for numbers
    # --argjson rgb888 "$color"
    jq --null-input --compact-output \
        --arg top "$runlevel" \
        --arg bottom "$employees" \
        --arg rgb888 "$color" \
        '{
            "top": $top,
            "bottom": $bottom,
            "rgbhex": $rgb888
        }';
}


function random_byte {
    echo "$((RANDOM % 0x100))"
}

function random_hex_color {
    local red=$(random_byte)
    local green=$(random_byte)
    local blue=$(random_byte)
    # 2 hex digits per channel
    printf '%02x%02x%02x' $red $green $blue
}

SERVER_URL="http://$ip_address"
HUD_URL="$SERVER_URL/hud"
INTERVAL=5

for i in $(seq 5 15)
do
    _body=$(_create_message_body \
        --runlevel "R$i" \
        --employees "E$i" \
        --color "$(random_hex_color)" \
    );

    echo -n "Issuing hud update: $_body: "
    curl --silent --request POST -d "$_body" "$HUD_URL"
    echo

    sleep $INTERVAL
done
