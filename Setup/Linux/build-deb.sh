#!/bin/bash

mkdir -p build/.dpkg/DEBIAN
mkdir -p build/.dpkg/tmp/icst.tmp/vst3
mkdir -p build/.dpkg/tmp/icst.tmp/lv2
mkdir -p build/.dpkg/tmp/icst.tmp/templates
cp -r build/bin/VST3/* build/.dpkg/tmp/icst.tmp/vst3 || exit -1;
cp -r build/bin/LV2/* build/.dpkg/tmp/icst.tmp/lv2 || exit -1;
cp -r Setup/Templates/* build/.dpkg/tmp/icst.tmp/templates || exit -1

cp $(dirname "$0")/postinst build/.dpkg/DEBIAN/ || exit -1;
chmod +x build/.dpkg/DEBIAN/postinst

PACKAGE_NAME="$1"
VERSION="$2"
OUTPUT_DIR="$3"
DESCRIPTION=$(sed '/^$/d; s/^/ /' "$(dirname "$0")/../readme.txt")
DEBIAN_DIR="build/.dpkg/DEBIAN"

# Control-File erstellen
CONTROL_FILE="$DEBIAN_DIR/control"
cat <<EOF > "$CONTROL_FILE"
Package: $PACKAGE_NAME
Version: $VERSION
Section: base
Priority: optional
Architecture: all
Maintainer: ICST <christian.schweizer@zhdk.ch>
Description: $DESCRIPTION
EOF

mkdir -p "$OUTPUT_DIR"
dpkg-deb --build build/.dpkg "$OUTPUT_DIR"
