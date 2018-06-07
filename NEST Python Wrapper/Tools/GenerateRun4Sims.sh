#!/bin/bash

# User inputs (only change these)
EVENTS=100000
SOURCE="CH3T"
#SOURCE="DD"
EMIN=0
EMAX=100
FIELD=-1
POS=-1
DRANGE_MIN=(40 105 170 235)
DRANGE_MAX=(105 170 235 300)

echo "*** WARNING: This script will overwrite detector.hh file, and recompile testNEST!!! ***"
echo "Proceed? [y/N] "
read ANSWER

if [[ $ANSWER != "y" && $ANSWER != "Y" && $ANSWER != "yes" ]]; then
	echo "Exiting."
	exit
fi

cd ..

DATE=`date +%Y-%m-%d`
OUTPUT_DIR="./Tools/Run4_${SOURCE}_Output_$DATE"

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

RUN4_SETTINGS="LUX_Detector_Settings/LUXRun4"
#for SETTINGS_FILE in `ls ${RUN4_SETTINGS}/LUX_Run4_TB*_DT*.hh`; do
for SETTINGS_FILE in `ls ${RUN4_SETTINGS}/LUX_Run4_TB*.hh`; do
	for i in ${!DRANGE_MIN[@]}; do
		DRANGE="${DRANGE_MIN[i]},${DRANGE_MAX[i]}"
		echo "Copying settings file $SETTINGS_FILE to detector.hh and recompiling..."
		cp $SETTINGS_FILE ./detector.hh
		make clean
		make
		sleep 3
		echo "Generating NEST output for $EVENTS events of $SOURCE between $EMIN and $EMAX keV..."
		sleep 3
		./testNEST $EVENTS $SOURCE $EMIN $EMAX $FIELD $POS $DRANGE > ${OUTPUT_DIR}/$(basename "$SETTINGS_FILE" .hh)_DT$(($i+1)).txt
		sleep 5
	done
done
