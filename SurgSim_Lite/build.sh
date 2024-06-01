#!/bin/bash

os_name=$(uname)

if [[ $os_name == *"Darwin"* ]]; then
  additional_flags="-I../../lib"
else
  additional_flags="-I../../lib"
  # export BUILD_PKG_CONFIG_MODULES='openal'
fi

../../lib/Core/build.sh pilot_episode "$1" "${additional_flags[@]}"
