# you can find these in the platformio.ini
_target_environment='esp32-s2-kaluga-1'

./run_pio.sh run --target upload \
                 --target monitor \
                 --environment "$_target_environment"