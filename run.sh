#!/usr/bin/env bash
set -euo pipefail

usage() {
    echo "Usage: $0 <d|r>"
    echo "  d  build and run Debug"
    echo "  r  build and run Release"
}

if [ "$#" -ne 1 ]; then
    usage
    exit 1
fi

case "$1" in
    d) config=debug;   dir=Debug ;;
    r) config=release; dir=Release ;;
    *) usage; exit 1 ;;
esac

cd "$(dirname "$0")"

premake5 gmake
make -C build config="$config"
"./bin/$dir/damsel"
