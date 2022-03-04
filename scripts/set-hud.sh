ip_address=$2

function _create_message_body {
    local runlevel=$2
    local employees=$4
    local color=$6

    jq --null-input --compact-output \
        --arg top "$runlevel" \
        --arg bottom "$employees" \
        --argjson rgb888 $color \
        '{
            "top": $top,
            "bottom": $bottom,
            "rgb888": $rgb888
        }';
}

SERVER_URL="http://$ip_address"
HUD_URL="$SERVER_URL/hud"
INTERVAL=5

for i in $(seq 5 15)
do
    _body=$(_create_message_body \
        --runlevel "R$i" \
        --employees "E$i" \
        --color $RANDOM \
    );

    echo -n "Issuing hud update: $_body: "
    curl --silent --request POST -d "$_body" "$HUD_URL"
    echo

    sleep $INTERVAL
done
