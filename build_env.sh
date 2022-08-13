if [ $# -lt 2 ]; then
    echo "Unexpected arguments received: $@"
    echo "   build_env --environment <environment>"
fi

environment="$2"
./run_pio.sh run --environment "$environment"