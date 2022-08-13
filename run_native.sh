_environment='native-dev'

if ./build_env --environment "$_environment"
then
    # run
    ./.pio/build/$_environment/program
fi