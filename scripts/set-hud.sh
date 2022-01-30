function _create_message_body {
    local top=$2
    local center_value=$4
    local center_label=$6
    local bottom=$8

    jq --null-input --compact-output \
        --arg top "$top" \
        --arg center_value "$center_value" \
        --arg center_label "$center_label" \
        --arg bottom "$bottom" \
        '{
            "centerValue": $center_value,
            "centerLabel": $center_label,
            "top": $top,
            "bottom": $bottom
        }';
}

SERVER_URL='http://192.168.8.119'
INTERVAL=5

for i in $(seq 5 15)
do
    top="Runlevel: $i"
    bottom="Time: $(date +'%H:%M:%S')"
    center_value="$i"
    center_label="Counter"

    _body=$(_create_message_body \
        --top "$top" \
        --center-value "$center_value" \
        --center-label "$center_label" \
        --bottom "$bottom"
    );

    echo -n "Issuing hud update: $_body: "
    curl --silent --request POST -d "$_body" "$SERVER_URL/hud"
    echo

    sleep $INTERVAL
done
