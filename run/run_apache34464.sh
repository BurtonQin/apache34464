#!/usr/bin/env bash
# run test

WORK=/home/boqin/Projects/bugs/apache/34464/

BUILD=${WORK}build/
EXEC=${BUILD}apache34464.no.opt
LOGS=${WORK}logs/
LOGGER=/home/tzt77/Develop/ComAirPy/comAir/tools/AprofLogger
PARSER=${WORK}run/parse_logger.py
RESULT=${WORK}apache34464_all_result.csv

INPUT=${WORK}input/
PATTERN=`cat ${INPUT}pattern_100`

function run_logger_collector() {
	${LOGGER}
}

function mv_logger_to_logs() {
	mkdir -p ${LOGS}
	ls ${WORK}run/aprof_logger_* | xargs mv -f -t ${LOGS}
}

function clear_pre_logger() {
	rm -rf ${LOGS}
}

function parse_logger() {
	${PARSER} $RESULT
}

function get_result() {
	for filename in ${INPUT}input_*
	do
		bn=`basename $filename`
		if [[ $bn != 'input_10000_100' ]]
		then
			${EXEC} ${PATTERN} < ${filename}
			run_logger_collector
		fi
	done

	${EXEC} ${PATTERN} < ${INPUT}'input_10000_100'
	run_logger_collector

	mv_logger_to_logs
	
	parse_logger
}

get_result

