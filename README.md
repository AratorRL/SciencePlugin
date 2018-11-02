# SciencePlugin

Plugin for BakkesMod that can be used to perform scientific experiments within Rocket League. It only works in freeplay and is not meant to be used outside of that.

# Features
- Real-time displaying of information about the car and/or the ball, such as location, velocity and rotation, as well as player controller inputs.
- Recording of RBState data of car and/or ball. 
- Overriding player input (limited).
- Bunch of setters and getters

# Preparation
- Compile with Visual studio, using the BakkesMod SDK.
- Place the dll in `Binaries\Win32\bakkesmod\plugins` (within the RL installation folder).
- To use the in-game settings menu (F2) for this plugin, the plugin should load on start-up of the game. For this, add a line with `plugin load scienceplugin` to `Binaries\Win32\bakkesmod\cfg\plugins.cfg`.
- For the input override to work, add an empty file called `debug.txt` to `Binaries\Win32\bakkesmod`.

# How to use in-game
- To show/hide panels, use the settings in the bakkesmod menu (F2) (only works when plugin loaded on start-up). Alternatively, use the `showHUD`/`hideHUD` commands.
- To record e.g. car data, set `recordCarInfo` to 1, and to 0 to stop recording. I recommend adding hotkeys for these commands such that recording and stopping can be done with simple button presses.

For questions, feel free to contact me on discord: Arator#5686.

Big thanks to Bakkes for creating BakkesMod and its SDK.
