#!/usr/bin/env python
# -*- coding: utf-8 -*-

# gen random string with given length, used as pattern.
# see @ https://stackoverflow.com/questions/18319101/whats-the-best-way-to-generate-random-strings-of-a-specific-length-in-python

from random import choice
from string import ascii_uppercase
from subprocess import call

WORKDIR="./input/"

def gen_random_str(len):
	return ''.join(choice(ascii_uppercase) for i in range(len))

if __name__ == "__main__":
	pattern = gen_random_str(100)
	with open(WORKDIR + "pattern_100", "w") as f:
		f.write(pattern)

	for i in range(1000, 11000, 1000):
		input = gen_random_str(i-100) 
		# gen input that does not contain pattern
		while pattern in input:
			input = gen_random_str(i-100)

		with open(WORKDIR + "input_%d_100" % i, "w") as f:
			f.write(input)
