#!/usr/bin/python3
# Copyright 2020
# All rights reserved.

import os.path
import platform

tools_dir = '../tools/'

tools_dir = os.path.abspath(tools_dir)

def executablePath(file):
	path = os.path.join(tools_dir, file)
	if '.' not in file and platform.system() == 'Windows':
		return path + '.exe'
	return path

def process(path):
	for fn in os.listdir('.'):
		if not os.path.isfile(fn) or not fn.endswith('.png'):
			continue
		
		try:
			bits = fn.split('.')
			if len(bits) < 3:
				continue
			format = bits[-2]
			
			os.system('%s -g %s -i %s -f %s' % (executablePath('n64graphics'), fn, fn[0:-4], format))
		except:
			continue
		
		with open(fn[0:-4], 'rb') as i:
			with open(fn[0:-4] + '.inc.c', 'w') as o:
				o.write(', '.join('0x{:02x}'.format(x) for x in i.read()))
				
#process('.')

for (dir, _, files) in os.walk('.'):
	for f in files:
		try:
			if not f.endswith('.png'):
				continue
				
			path = os.path.join(dir, f)
			
			bits = f.split('.')
			if len(bits) < 3:
					continue

			print(path)
			format = bits[-2]
			os.system('%s -g %s -i %s -f %s' % (executablePath('n64graphics'), path, path[0:-4], format))
			with open(path[0:-4], 'rb') as i:
				with open(path[0:-4] + '.inc.c', 'w') as o:
					o.write(', '.join('0x{:02x}'.format(x) for x in i.read()))
		except BaseException as e:
			print(str(e))
