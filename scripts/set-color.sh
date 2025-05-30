#!/bin/bash

if [ $# -lt 2 ]; then
	echo 'Missing arguments:'
        echo 'Usage: set-color -ip <hostname | ip-address>'
        exit 1
fi

IP_ADDRESS="$2"
SERVER_URL="http://$IP_ADDRESS"


# Builds the body for setting remote led colors
function _build_led_color_body {
   local led_id=$2
   local red=$4
   local green=$6
   local blue=$8

   jq --null-input --compact-output \
      --argjson ledId "$led_id" \
      --argjson red "$red" \
      --argjson green "$green" \
      --argjson blue "$blue" \
      '{"id":$ledId,"r":$red,"g":$green,"b":$blue}'
}


# Issues a request to set the led color for the requested led id
# Use `_list_available_colors` to get a list of ids
function _set_led_color {
   local led_id=$2
   local red=$4
   local green=$6
   local blue=$8

   local body=$(_build_led_color_body --led-id $led_id\
	                              --red $red \
                                      --green $green \
                                      --blue $blue);
   local url="$SERVER_URL/led"
   echo -n "Setting color '$body' => '$url': "
   curl --silent --request POST -d "$body" "$url"
   echo
}


# Lists all available leds
function _list_available_leds {
   local url="$SERVER_URL/leds"
   curl --silent --request GET "$url"
}


# Iterates and sets the color for every available LED
function _set_all_led_colors {
   local red=$2
   local green=$4
   local blue=$6

   local leds=$(_list_available_leds)
   local led_count=$(echo $leds | jq '. | length')

   for i in $(seq 0 $(("$led_count"-1)))
   do
      local led_id=$(echo "$leds" | jq ".[$i].id")
      _set_led_color --led-id "$led_id" --red "$red" --green "$green" --blue "$blue"
   done
}


function _get_random_color {
   echo $(($RANDOM % 255))
}


function _start_setting_colors {
   local interval=$2

   _set_all_led_colors --red 255 --green 0 --blue 0
   sleep $interval
   echo

   _set_all_led_colors --red 0 --green 255 --blue 0
   sleep $interval
   echo

   _set_all_led_colors --red 0 --green 0 --blue 255
   sleep $interval
   echo
}



_start_setting_colors -interval 3
