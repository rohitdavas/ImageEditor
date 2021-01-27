#!/bin/bash
clear
echo "the script starts now"

qmake -makefile ../
make
echo "enter the library to move -> "
read filename
echo "filename entered $filename"
cp $filename ../../ImageEditorApp/plugins 

ls ../../ImageEditorApp/plugins -l
