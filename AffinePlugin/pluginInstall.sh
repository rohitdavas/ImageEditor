#!/bin/bash
# use this script to build and
clear

# create build directory 
mkdir build
cd build 

# generate makefile and make
qmake -makefile ../
make

# move the library
echo "enter the library to move -> "
read filename
echo "filename entered $filename"
cp $filename ../../ImageEditorApp/plugins 

# show the ImageEditor plugins. Keep an eye on modiified time.
ls ../../ImageEditorApp/plugins -l
