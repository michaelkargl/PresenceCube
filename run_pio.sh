#!/bin/bash

_pio_cli="pio"

if ! which "$_pio_cli"
then
    # by default, the pio cli isn't exposed to $PATH
    # try to locate it in the users home directory
    _pio_base_dir="$HOME/.platformio"
    _pio_cli=$(find "$_pio_base_dir" -name pio)
fi

$_pio_cli $@