rm -r build
mkdir build
cd build
cmake ..
cmake --build . --config Release
../Setup/build_installer.sh -p macOS -t "0.0.1" -n AmbiPackage_0.0.1_20230906
packagesbuild ./osx_temp.pkgproj  