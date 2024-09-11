# SurgSim Lite

![GitHub License](https://img.shields.io/github/license/razterizer/SurgSim_Lite?color=blue)
[![Build Check](https://github.com/razterizer/SurgSim_Lite/actions/workflows/main.yml/badge.svg)](https://github.com/razterizer/SurgSim_Lite/actions/workflows/main.yml)

<img width="568" alt="image" src="https://github.com/razterizer/SurgSim_Lite/assets/32767250/2d356a54-6117-4d6b-aeca-4a025e8ae372">
<img width="568" alt="image" src="https://github.com/razterizer/SurgSim_Lite/assets/32767250/1cf2f435-88b3-4c41-bafd-a7cabd30ca75">
<img width="567" alt="image" src="https://github.com/razterizer/SurgSim_Lite/assets/32767250/0c7f3528-00d8-492c-9609-9bef63f8e797">
<img width="567" alt="image" src="https://github.com/razterizer/SurgSim_Lite/assets/32767250/47cf2c18-c9cf-46a5-9787-bfd3ad69540a">
<img width="567" alt="image" src="https://github.com/razterizer/SurgSim_Lite/assets/32767250/443646ef-9819-41e2-a370-28ab5ee10231">

## Build & Run Instructions

There are two options on dealing with repo dependencies:
### Repo Dependencies Option 1

Run the following command from `<my_source_code_dir>`.
```sh
./SurgSim_Lite/fetch-dependencies ./SurgSim_Lite/dependencies
```
This will make sure you are running the latest stable versions that work with `SurgSim_Lite`.

This script was created by [Thibaut Buchert](https://github.com/thibautbuchert).

### Repo Dependencies Option 2

You need the following header-only libraries that I've made:
* https://github.com/razterizer/Core
* https://github.com/razterizer/Termin8or

Make sure the folder structure looks like this:
```
<my_source_code_dir>/lib/Core/                   ; Core repo workspace/checkout goes here.
<my_source_code_dir>/lib/Termin8or/              ; Termin8or repo workspace/checkout goes here.
<my_source_code_dir>SurgSim_Lite/                ; SurgSim_Lite repo workspace/checkout goes here.
```

These repos are not guaranteed to all the time work with the latest version of `SurgSim_Lite`. If you want the more stable aproach then look at Option 1 instead.

### Windows

Then just open `<my_source_code_dir>/SurgSim_Lite/SurgSim_Lite/SurgSim_Lite.sln` and build and run. That's it!

### MacOS

Goto `<my_source_code_dir>/SurgSim_Lite/SurgSim_Lite/` and build with `./build.sh`.

Then run by typing `./bin/surgsim_lite`.

### Linux (Ubuntu)

Goto `<my_source_code_dir>/SurgSim_Lite/SurgSim_Lite/` and build with `./build.sh`.

Then run by typing `./bin/surgsim_lite`.
