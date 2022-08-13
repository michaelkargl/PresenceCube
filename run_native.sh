_environment='native-dev'

if ./build_env.sh --environment "$_environment"
then
    # run
    ./.pio/build/$_environment/program
fi
