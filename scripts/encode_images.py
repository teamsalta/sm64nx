#!/usr/bin/python3
# Copyright 2020
# All rights reserved.

import os.path

def process(path):
	for fn in os.listdir('.'):
		if not os.path.isfile(fn) or not fn.endswith('.png'):
			continue
		
		try:
			bits = fn.split('.')
			if len(bits) < 3:
				continue
			format = bits[-2]
			
			os.system('n64graphics -g %s -i %s -f %s' % (fn, fn[0:-4], format))
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
			os.system('n64graphics -g %s -i %s -f %s' % (path, path[0:-4], format))
			with open(path[0:-4], 'rb') as i:
				with open(path[0:-4] + '.inc.c', 'w') as o:
					o.write(', '.join('0x{:02x}'.format(x) for x in i.read()))
		except BaseException as e:
			print(str(e))
