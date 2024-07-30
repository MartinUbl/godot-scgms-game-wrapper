# <img src="https://diabetes.zcu.cz/img/icon.png" width="24" height="24" /> <img src="https://godotengine.org/assets/press/icon_color.png" width="24" height="24" /> Godot Engine - GDExtension for SmartCGMS

This repository contains GDExtension for Godot Engine to allow a seamless integration of SmartCGMS game wrapper.

Repository contents:
- `demo` - demo project for Godot; please note, that you have to manually prepare the `bin/` subdirectory of the extension
- `src` - source code of the extension

## Build

Once you clone this repository, do not forget to fetch the submodules by using e.g., the following command:

```git submodule update --init```

After that, you can use the CMake to build the solution:

```
cmake .
cmake --build .
```

This command generates debug variant of the extension for your current platform. For instructions how to switch platforms/configs, please, refer to the CMake manual.

## License

The SmartCGMS software and its components are distributed under the Apache license, version 2. When publishing any derivative work or results obtained using this software, you agree to cite the following paper:

_Tomas Koutny and Martin Ubl_, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science, Volume 177, pp. 354-362, 2020

See attached LICENSE file for full licencing information.

|![University of West Bohemia](https://www.zcu.cz/en/assets/logo.svg)|![Department of Computer Science and Engineering](https://www.kiv.zcu.cz/site/documents/verejne/katedra/dokumenty/dcse-logo-barevne.png)|
|--|--|