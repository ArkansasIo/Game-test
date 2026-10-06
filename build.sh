#!/usr/bin/env bash
# Build helper for Labyrinth of the Dragon.
# Usage: ./build.sh [make-target...]   (default target: all)
#
# On Windows, run this from Git Bash: it sets GBDK_HOME/PATH and locates the
# GnuWin32 make.exe. On Unix-like systems it simply forwards to make.
set -e

cd "$(dirname "$0")"

if [ -z "$GBDK_HOME" ]; then
  for candidate in "$HOME/gbdk" "/c/gbdk" "$HOME/gbdk-2020"; do
    if [ -x "$candidate/bin/lcc" ] || [ -x "$candidate/bin/lcc.exe" ]; then
      GBDK_HOME="$candidate"
      break
    fi
  done
fi

if [ -z "$GBDK_HOME" ]; then
  echo "error: GBDK_HOME is not set and no GBDK install was found." >&2
  echo "       Set it like: GBDK_HOME=/c/Users/you/gbdk ./build.sh" >&2
  exit 1
fi
export GBDK_HOME
export PATH="$GBDK_HOME/bin:$PATH"

# Locate make: prefer whatever is on PATH, then the GnuWin32 install location.
MAKE="$(command -v make || true)"
if [ -z "$MAKE" ]; then
  for candidate in "/c/Program Files (x86)/GnuWin32/bin/make.exe" \
                   "/c/Program Files/GnuWin32/bin/make.exe"; do
    if [ -x "$candidate" ]; then
      MAKE="$candidate"
      break
    fi
  done
fi

if [ -z "$MAKE" ]; then
  echo "error: GNU Make was not found." >&2
  echo "       On Windows: winget install --id GnuWin32.Make -e" >&2
  exit 1
fi

echo "GBDK_HOME = $GBDK_HOME"
echo "make      = $MAKE"
echo

TARGETS=("$@")
if [ ${#TARGETS[@]} -eq 0 ]; then
  TARGETS=(all)
fi

"$MAKE" "${TARGETS[@]}"
