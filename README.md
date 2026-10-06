# Labyrinth of the Dragon (GBC)
An 8-bit Adventure RPG with D&D Monsters!

## How to Build the ROM

### Depedencies
* [GBDK-2020](https://github.com/gbdk-2020/gbdk-2020) - The Game Boy Development
  kit. Includes the libraries and binaries for C development on the Game Boy.
* [GNU Make](https://gnuwin32.sourceforge.net/packages/make.htm) - Build system
  tool (installation should only be required on Windows).
* [NodeJS](https://nodejs.org) - Used to run custom tools I made in the course
  of developing the game.

### Use Make to Build the ROM
Define a shell variable named `GBDK_HOME` pointing to the directory where you
installed GBDK (the directory that contains `bin/`, `include/` and `lib/`). A
trailing slash is not required.

To build the ROM run the following commands:

* `npm install`
* `make assets`
* `make`

The finished ROM is written to `LabyrinthOfTheDragon.gbc` in the project root.

### Building on Windows
GBDK-2020 does not ship a Windows installer, so grab the `gbdk-win64.zip` asset
from the [releases page](https://github.com/gbdk-2020/gbdk-2020/releases) and
unzip it anywhere convenient (for example `C:\Users\<you>\gbdk`).

The Makefile's recipes are Unix-style (`mkdir -p`, `touch`, `rm -f`), so on
Windows the build must be run through **Git Bash** rather than `cmd.exe` or
PowerShell. The Makefile detects Windows automatically and points its recipe
shell at `sh.exe` from Git for Windows; the only requirement is that Git is
installed. A convenience wrapper is provided:

* Install dependencies: `winget install --id GnuWin32.Make -e`
* Build: `./build.sh` (or `./build.sh assets` to only regenerate assets)

`build.sh` sets `GBDK_HOME` and `PATH` for you, then invokes `make`. Override the
GBDK location with `GBDK_HOME=/c/path/to/gbdk ./build.sh` if you installed it
elsewhere.

Run `make usage` (or `./build.sh usage`) to print a per-bank ROM usage report,
and `make clean` to remove build artefacts.

### Character levels
The stat tables in `assets/tables.csv` cover levels up to `MAX_LEVEL` (see
`src/stats.h`), currently 100. Index 0 of each table is the level-0 baseline, so
the CSV holds `MAX_LEVEL + 1` data rows and `tools/tables2c` emits arrays of
that length. Every table getter clamps its level argument, and `level_up`
stops at the cap, so raising the level past the maximum cannot read outside the
tables.

`tables.csv` values are range-checked at generation time: a value that does not
fit its column type (for example an `exp_by_level` value above 65535, the
`uint16_t` maximum) fails the build rather than silently truncating.
