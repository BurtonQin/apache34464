#!/usr/bin/env python
# -*- coding: utf-8 -*-

# draw pictures for each function to find their complexity category

import pandas as pd
import numpy as np

import matplotlib.pyplot as plt

from scipy.optimize import curve_fit

df = pd.read_csv('/home/boqin/Projects/bugs/apache/34464/apache34464_all_result.csv')
grouped = df.groupby(['run_id', 'func_id'])
rms_cost_df = df
distinct_func_ids =  rms_cost_df['func_id'].unique()
func_name_id_dict = dict()

with open('/home/boqin/Projects/bugs/apache/34464/build/apache34464_func_name_id.txt', 'r') as func_name_id_f:
    lines = func_name_id_f.readlines()
    for line in lines:
        items = line.strip('\n').split(':')
        func_name_id_dict[items[1]] = items[0]

for _id in distinct_func_ids:
    rms_cost_df_func_id_ = rms_cost_df.loc[df['func_id'] == _id]
    if _id == 0:
        continue
    func_name = func_name_id_dict[str(_id)]
    rms_cost_df_func_id_.plot.scatter(x='rms', y='cost', label='id: {0}, function: {1}'.format(_id, func_name));
