# you can find these in the platformio.ini
_target_environment='espressif32-dev'

./run_pio.sh run --target upload \
                 --target monitor \
                 --environment "$_target_environment"