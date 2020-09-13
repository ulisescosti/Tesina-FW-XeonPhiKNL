#!/bin/bash
readonly EXEC_DIR="/bin"
readonly OUT_FILE="output/result.csv"
readonly TIME_FILE="output/script_time.txt"
readonly SUMMARY_FILE="output/summary.csv"
readonly OUTPUT_PROCCESOR="output/proc_output"
readonly OPTIONS="-PC"

MCDRAM="numactl -p 1 "
#MCDRAM=""

#VERSION="extra_exp"
VERSION="opt_4"
VOLLEY_SIZE=3
N=4096


function runTestVolley(){
	BS=$1
	T=$2
#	GD=$3
	ARGS="$N $T $OPTIONS"
	echo "Version;N;BS;T" >> $OUT_FILE
	echo $VERSION";"$N";"$BS";"$T >> $OUT_FILE
	echo "Exec time;GFlops" >> $OUT_FILE
	for((i=1;i<=$VOLLEY_SIZE;i++)); do
		$MCDRAM.$EXEC_DIR"/BS-"$BS/$VERSION $ARGS >> $OUT_FILE 2>&1
	done
	echo "#" >> $OUT_FILE
}

function setDatetimeFile(){
	if [ $1 == "START" ]
	then
		echo "Hora de comienzo de script:" >> $TIME_FILE
	else
		echo "Hora de fin de script:" >> $TIME_FILE
	fi
	date +"%d/%m/%y %H:%M" >> $TIME_FILE
}

function createOrOverwriteOutputFiles(){
	echo "" > $OUT_FILE
	echo "" > $TIME_FILE
}

function testAllParams(){
	runTestVolley 32 64
	runTestVolley 64 64
	runTestVolley 128 64
	runTestVolley 256 64
	runTestVolley 32 128
	runTestVolley 64 128
	runTestVolley 128 128
	runTestVolley 256 128
	runTestVolley 32 256
	runTestVolley 64 256
	runTestVolley 128 256
	runTestVolley 256 256
}

function run_tests(){
#	export KMP_AFFINITY=granularity=fine,balanced

	testAllParams
	
: <<'END'
	N=8192
	VOLLEY_SIZE=10
	testAllParams
	
	N=16384
	VOLLEY_SIZE=5
        testAllParams

	N=32768
	VOLLEY_SIZE=3
    testAllParams

	N=65536
	VOLLEY_SIZE=3
	runTestVolley 32 64
	runTestVolley 64 64
	runTestVolley 128 64
	runTestVolley 32 128
	runTestVolley 64 128
	runTestVolley 128 128
	VOLLEY_SIZE=1
	runTestVolley 256 128
	runTestVolley 256 64
END
}

createOrOverwriteOutputFiles
setDatetimeFile "START"
run_tests
echo "" >> $OUT_FILE
setDatetimeFile "END"
./$OUTPUT_PROCCESOR < $OUT_FILE > $SUMMARY_FILE 2>&1

