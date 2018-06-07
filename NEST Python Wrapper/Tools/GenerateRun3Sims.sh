#!/bin/bash

# User inputs (only change these)
EVENTS=100000
SOURCE="CH3T"
#SOURCE="DD"
EMIN=0
EMAX=100
FIELD=-1
POS=-1
DRANGE=-1

echo "*** WARNING: This script will overwrite detector.hh file, and recompile testNEST!!! ***"
echo "Proceed? [y/N] "
read ANSWER

if [[ $ANSWER != "y" && $ANSWER != "Y" && $ANSWER != "yes" ]]; then
	echo "Exiting."
	exit
fi

cd ..

DATE=`date +%Y-%m-%d`
OUTPUT_DIR="./Tools/Run3_${SOURCE}_Output_$DATE"

if [[ ! -d ${OUTPUT_DIR} ]]; then
	mkdir ${OUTPUT_DIR}
else
	echo "*** WARNING: Output directory ${OUTPUT_DIR} already exists! ***"
	echo "Overwrite data? [y/N] "
	read ANSWER
	if [[ $ANSWER != "y" && $ANSWER != "Y" && $ANSWER != "yes" ]]; then
		echo "Exiting."
		exit
	fi
fi

RUN3_SETTINGS="LUX_Detector_Settings/LUXRun3/LUX_Run3_${SOURCE}.hh"
echo "Copying settings file $RUN3_SETTINGS to detector.hh and recompiling..."
cp $RUN3_SETTINGS ./detector.hh
make clean
make
sleep 3
echo "Generating NEST output for $EVENTS events of $SOURCE between $EMIN and $EMAX keV..."
sleep 3
./testNEST $EVENTS $SOURCE $EMIN $EMAX $FIELD $POS $DRANGE > ${OUTPUT_DIR}/$(basename "$RUN3_SETTINGS" .hh).txt
sleep 5
