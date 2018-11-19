#!/usr/bin/env bash
# runtime test

WORK=/home/boqin/Projects/bugs/apache/34464/

BUILD=${WORK}build/
EXEC_INST=${BUILD}apache34464.no.opt

BUILD=${WORK}original/
EXEC_BUGGY=${BUILD}apache34464.buggy
EXEC_PATCH1=${BUILD}apache34464.patch1
EXEC_PATCH2=${BUILD}apache34464.patch2

INPUT=${WORK}input/
PATTERN=`cat ${INPUT}pattern_100`

function runtime() {
	EXEC="$1"
	echo "`basename $EXEC` (ns)"
	for filename in ${INPUT}input_*
	do
		bn=`basename $filename`
		if [[ $bn != 'input_10000_100' ]]
		then
			${EXEC} ${PATTERN} < ${filename}
		fi
	done

	${EXEC} ${PATTERN} < ${INPUT}'input_10000_100'
}

runtime $EXEC_BUGGY
runtime $EXEC_PATCH1
runtime $EXEC_PATCH2
runtime $EXEC_INST
