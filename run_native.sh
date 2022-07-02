# build
_environment='native-dev'
if ./run_pio.sh run --environment "$_environment"
then
    # run
    ./.pio/build/$_environment/program
fi