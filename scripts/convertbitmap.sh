#!/bin/bash

echo bash script $1 $ARDUINO_PROJECTS
infile="$ARDUINO_PROJECTS/data/bitmaps/input/$1.c"
outfile="$ARDUINO_PROJECTS/data/bitmaps/output/$1.cpp"

if [ ! -e $infile ] 
	then echo $infile does not exist; exit -1
fi


sed "s+BITFILE___+$infile+" convert_template.cpp  > do_convert.cpp

make do_convert

./do_convert $1 > $outfile


rm do_convert do_convert.cpp


