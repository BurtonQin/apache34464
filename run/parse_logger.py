#!/usr/bin/env python
# -*-coding: utf-8 -*-

'''
parse logger
'''

import pandas as pd
import sys
import os

WORK = "/home/boqin/Projects/bugs/apache/34464/"
LOGS = WORK + "logs/"

def parse_logger(out_path):
	results = [[0, 0, 0, 0, 1]]
	init_df = pd.DataFrame(data=results, columns=['func_id', 'rms', 'cost', 'chains', 'run_id'])
	for dirname, dirnames, filenames in os.walk(LOGS):
		index = 1
		for filename in filenames:
			if 'aprof_logger' in filename:
				full_path = os.path.join(dirname, filename)
				current_df = pd.read_csv(full_path, header=0)
				current_df['run_id'] = index
				init_df = init_df.append(current_df, ignore_index=True)
				index += 1

	init_df.to_csv(out_path, index=False)


if __name__ == "__main__":
	parse_logger(sys.argv[1])
