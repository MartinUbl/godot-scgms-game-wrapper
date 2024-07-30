## Notes

This directory will contain the following:

- GDExtension shared object/dynamic library:
    - `godot-scgms-game-wrapper-debug.win.x86_64.dll` - GDExtension library for Windows (debug version)
    - `godot-scgms-game-wrapper-release.win.x86_64.dll` - GDExtension library for Windows (release version)
    - `godot-scgms-game-wrapper-debug.linux.x86_64.so` - GDExtension library for GNU/Linux (debug version)
    - `godot-scgms-game-wrapper-release.linux.x86_64.so` - GDExtension library for GNU/Linux (release version)
    - in future, we may also support more platforms; but for now, we selected Windows and GNU/Linux (on x86_64)
- `scgms.dll` (Windows), `libscgms.so` (GNU/Linux) - SmartCGMS entry point
- `filters` - directory containing SmartCGMS filters

Compiled binaries are not part of this repository. To get them, please, visit [https://diabetes.zcu.cz/smartcgms](https://diabetes.zcu.cz/smartcgms) for further instructions.