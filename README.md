# SurgSim Lite

![GitHub License](https://img.shields.io/github/license/razterizer/SurgSim_Lite?color=blue)
![Static Badge](https://img.shields.io/badge/C%2B%2B-20-yellow)

[![build ubuntu](https://github.com/razterizer/SurgSim_Lite/actions/workflows/build-ubuntu.yml/badge.svg)](https://github.com/razterizer/SurgSim_Lite/actions/workflows/build-ubuntu.yml)
[![build macos](https://github.com/razterizer/SurgSim_Lite/actions/workflows/build-macos.yml/badge.svg)](https://github.com/razterizer/SurgSim_Lite/actions/workflows/build-macos.yml)
[![build windows](https://github.com/razterizer/SurgSim_Lite/actions/workflows/build-windows.yml/badge.svg)](https://github.com/razterizer/SurgSim_Lite/actions/workflows/build-windows.yml)

![Top Languages](https://img.shields.io/github/languages/top/razterizer/SurgSim_Lite)
![GitHub repo size](https://img.shields.io/github/repo-size/razterizer/SurgSim_Lite)
![](https://tokei.rs/b1/github/razterizer/SurgSim_Lite)
![Commit Activity](https://img.shields.io/github/commit-activity/t/razterizer/SurgSim_Lite)
![Last Commit](https://img.shields.io/github/last-commit/razterizer/SurgSim_Lite?color=blue)
![Contributors](https://img.shields.io/github/contributors/razterizer/SurgSim_Lite?color=blue)

<img width="568" alt="image" src="https://github.com/razterizer/SurgSim_Lite/assets/32767250/2d356a54-6117-4d6b-aeca-4a025e8ae372">
<img width="568" alt="image" src="https://github.com/razterizer/SurgSim_Lite/assets/32767250/1cf2f435-88b3-4c41-bafd-a7cabd30ca75">
<img width="567" alt="image" src="https://github.com/razterizer/SurgSim_Lite/assets/32767250/0c7f3528-00d8-492c-9609-9bef63f8e797">
<img width="567" alt="image" src="https://github.com/razterizer/SurgSim_Lite/assets/32767250/47cf2c18-c9cf-46a5-9787-bfd3ad69540a">
<img width="567" alt="image" src="https://github.com/razterizer/SurgSim_Lite/assets/32767250/443646ef-9819-41e2-a370-28ab5ee10231">

![ubuntu_lffeb5yfso_360](https://github.com/user-attachments/assets/96302a4b-1359-40ce-87e4-01c79c6af759)

## Build & Run Instructions

There are two options on dealing with repo dependencies:

### Repo Dependencies Option 1

This method will ensure that you are running the latest stable versions of the dependencies that work with `SurgSim_Lite`.

The script `fetch-dependencies.py` used for this was created by [Thibaut Buchert](https://github.com/thibautbuchert).
`fetch-dependencies.py` is used in the following scripts below:

After a successful build, the scripts will then prompt you with the question if you want to run the program.

When the script has been successfully run for the first time, you can then go to sub-folder `SurgSim_Lite` and use the `build.sh` / `build.bat` script instead, and after you have built, just run the `run.sh` or `run.bat` script.

#### Windows

Run the following script:
```sh
setup_and_build.bat
```

#### MacOS / Linux

Run the following script:
```sh
setup_and_build.sh
```

### Repo Dependencies Option 2

In this method we basically outline the things done in the `setup_and_build`-scripts in Option 1.

This method is more suitable for development as we're not necessarily working with "locked" dependencies.

You need the following header-only libraries:
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

You can also build it by going to `<my_source_code_dir>/SurgSim_Lite/SurgSim_Lite/` and build with `build.bat`.
Then you run by typing `run.bat`.

### MacOS / Linux

Then goto `<my_source_code_dir>/SurgSim_Lite/SurgSim_Lite/` and build with `./build.sh`.

Then run by typing `./run.sh`.
