#!/usr/bin/python3
import json
import xxhash
import os
import sys
import tqdm

MAGIC = b'ASSETLDR'

def listHashes(path):
	hashes = []
	magic = (0xE8B675681037DE12).to_bytes(8, byteorder='little')
	
	with open(path, 'rb') as f:
		f.seek(0)
		if f.read(0x8) != magic:
			raise IOError('invalid magic')
			
		entryCount = int.from_bytes(f.read(0x4), byteorder='little')
		
		for i in tqdm.tqdm(range(entryCount)):
			f.seek(i * 0x40 + 0x10)
			xxhash64 = int.from_bytes(f.read(0x8), byteorder='little')
			
			hashes.append('%16x' % xxhash64)
	return hashes

def formatBuffer(buffer):
	a = []
	for b in buffer:
		a.append('0x%02X' % b)
	return ', '.join(a)

with open('assets.json', 'r') as f:
	entries = json.loads(f.read())
	
hashes = listHashes(sys.argv[1])
existing = []

for path, entry in entries.items():
	try:
		if path[0] == '@' or not path.endswith('.png'):
			continue
			
		cfile = '../' + path[0:-4] + '.inc.c'
		
		with open(cfile, 'r') as f:
			buffer = f.read()
			
		if buffer.startswith(formatBuffer(MAGIC)):
			continue
			
		bits = []
		for bit in buffer.split(','):
			bit = bit.strip()
			if len(bit) == 0:
				continue
				
			if not bit.startswith('0x'):
				raise IOError('unknown token: ' + bit)
				
			bit = bit.replace('0x', '').strip()
			bits.append('%02X' % (int(bit, 16)))
		buffer = ''.join(bits)

		buffer = bytes.fromhex(buffer)
		hashStr = xxhash.xxh64(buffer).hexdigest().lower()
		
		if hashStr not in hashes:
			print('could not find pak asset for %s' % path)
			continue
			
		if hashStr in existing:
			print('hash collision for %s' % path)
			continue
		existing.append(hashStr)
			
		marker = MAGIC + int.from_bytes(xxhash.xxh64(buffer).digest(), byteorder='big').to_bytes(8, byteorder='little')
		
		#with open(cfile, 'w') as f:
		#	f.write(formatBuffer(marker))

	except BaseException as e:
		print(str(e))