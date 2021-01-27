echo "enter enter plugin directory : "
read directory

mkdir $directory
cd $directory

touch "$directory.h" "$directory.cpp"
qmake -project
