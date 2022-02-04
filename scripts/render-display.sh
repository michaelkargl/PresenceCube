SCREEN_WIDTH=50 #176
SCREEN_HEIGHT=264
SCREEN_DUMP='./display.bin'

curl --request GET 'http://192.168.8.119/screen' --output $SCREEN_DUMP
python3 render-display.py
