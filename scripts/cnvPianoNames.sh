#!/bin/bash
sdir=$ARDUINO_PROJECTS/data/NewNumbers
ddir=/Volumes/WAVTRIG
count=1
for f in `ls $sdir/*.wav`
do
	n=`printf %03d $count`
	echo cp $sdir/$f $ddir/$n-$f
	count=$((count + 1))
done