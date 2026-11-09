#!/usr/bin/env bash
set -euo pipefail

usage() {
    echo "Usage: $0 <d|r|c>"
    echo "  d  build and run Debug"
    echo "  r  build and run Release"
    echo "  c  clean the build and obj directories"
}

if [ "$#" -ne 1 ]; then
    usage
    exit 1
fi

case "$1" in
    d) config=debug;   dir=Debug ;;
    r) config=release; dir=Release ;;
    c)
        make -C build clean
        find ./obj -delete
        exit 0
        ;;
    *) usage; exit 1 ;;
esac

cd "$(dirname "$0")"

premake5 gmake
make -C build config="$config"
SECONDS=0
./bin/$dir/damsel | tee output.txt
echo -e "\ntook $SECONDS\ndiff: "
diff -q ./output.txt example/example-output.txt
echo "diff DONE\nexiting..."
