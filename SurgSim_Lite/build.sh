#!/bin/bash

os_name=$(uname)

if [[ $os_name == *"Darwin"* ]]; then
  additional_flags="-I../../lib \
    -I../../lib/Termin8or/include"
else
  additional_flags="-I../../lib \
    -I../../lib/Termin8or/include"
  # export BUILD_PKG_CONFIG_MODULES='openal'
fi

../../lib/Core/build.sh surgsim_lite "$1" "${additional_flags[@]}"

# Capture the exit code of Core/build.sh
exit_code=$?

if [ $exit_code -ne 0 ]; then
  echo "Core/build.sh failed with exit code $exit_code"
  exit $exit_code
fi

### Post-Build Actions ###

cp title.txt bin/

mkdir -p bin/fonts/
cp ../../lib/Termin8or/include/Termin8or/title/fonts/* bin/fonts/
