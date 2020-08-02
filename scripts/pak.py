#!/usr/bin/python3
# Copyright 2020
# All rights reserved.

import os.path
import sys
import subprocess
from PIL import Image
import os
import zstd
from tqdm import tqdm
import argparse
import json
import re

TEXTURE			= 1
GEO				= 2
DISPLAY_LIST	= 3
BHV_SCRIPT		= 4
LEVEL_SCRIPT	= 5
SOUND_BIN 		= 8
SOUND_BANK		= 9
SOUND_TBL		= 10
SOUND_CTL		= 11

def formatName(fmt):
	return 'rgba32'
	
def formatIndex(fmt):
	return 0
	
def sizeIndex(fmt):
	return 0

textureData = {}	
totalSize = 0

minSize = None
maxSize = None

try:
	with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'assetmap.json'), 'r') as f:
		assetMap = json.load(f)
except:
	assetMap = {}
	print('warning: failed to load asset map')
	raise
	
try:
	with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'gfx_map.json'), 'r') as f:
		idMap = json.load(f)
except:
	idMap = {}
	print('warning: failed to load id map')
	raise
	
def stripBasePath(path, base):
	return path[len(base):].strip('/\\ \t')
	
def getXXHash64(path, base):
	name = os.path.basename(path)
	
	try:
		if len(name) == 20:
			return int(name[0:-4], 16)
			
		m = re.search('\[([A-Za-z0-9]{16})\]', name)
		
		if m:
			return int(m.group(1), 16)
	except:
		pass
	
	try:
		if name in idMap:
			return int(idMap[name], 16)
	except:
		pass
		
	absPath = stripBasePath(os.path.abspath(os.path.join(base, path)).lower(), base)
		
	for originalPath, obj in assetMap.items():
		if 'xxhash64' not in obj:
			continue
		
		if 'riceName' in obj:
			if name == obj['riceName']:
				return int(obj['xxhash64'], 16)

		cmpPath = os.path.abspath(os.path.join(base, originalPath)).lower()
		cmpPath = stripBasePath(cmpPath, base)

		if cmpPath.endswith(absPath):
			return int(obj['xxhash64'], 16)
	
	for originalPath, obj in assetMap.items():
		if 'xxhash64' not in obj:
			continue
		
		if 'riceName' in obj:
			if '_' in name and '_' in obj['riceName']:
				if '_'.join(name.split('_')[0:-1]).upper() == '_'.join(obj['riceName'].split('_')[0:-1]).upper():
					return int(obj['xxhash64'], 16)

		cmpPath = os.path.abspath(os.path.join(base, originalPath)).lower()
		cmpPath = stripBasePath(cmpPath, base)

		if cmpPath.endswith(absPath):
			return int(obj['xxhash64'], 16)
		

	return None
	
def getPath64(xxhash):				
	for originalPath, obj in assetMap.items():
		if 'xxhash64' not in obj:
			continue
			
		if int(obj['xxhash64'], 16) == xxhash:
			return originalPath

	return None

class Asset:
	def __init__(self, buffer, width = 0, height = 0, type = 0):
		self.buffer = buffer
		self.width = width
		self.height = height
		self.type = type
		
def adjustSize(width, height, src):
	ratio = width / height
	owidth = width
	oheight = height
	if minSize is not None and min(width, height) < minSize:
		if width < height:
			width = minSize
			height = width * ratio
		else:
			height = minSize
			width = height / ratio
			
	if maxSize is not None and max(width, height) > maxSize:
		
		if width > height:
			width = maxSize
			height = width / ratio
		else:
			height = maxSize
			width = height * ratio

		width = int(width)
		height = int(height)
		
		if width % 8 != 0 or height % 8 != 0:
			print('warning resizing %d, %d ->  %d, %d: %s' %(owidth, oheight, width, height, src))
			
	return [int(width), int(height)]

def addTexture(src, scale, base):
	global totalSize
	global textureData
	
	if not src.endswith('.png'):
		return
		
	src = src.replace('\\', '/')

	hash = getXXHash64(src, base)
	
	if not hash:
		print('could not get hash for %s' % src)
		return

	
	im = Image.open(src)
	originalWidth, originalHeight = im.size
	
	width, height = adjustSize(originalWidth, originalHeight, src)

	im = im.convert("RGBA")
	
	if scale > 1 or width != originalWidth or height != originalHeight:		
		width, height = adjustSize(width // scale, height // scale, src)
		im = im.resize((width, height))

	pBits = im.tobytes("raw", "RGBA")

	
	totalSize += len(pBits)
	textureData[hash] = Asset(pBits, width, height, type = TEXTURE)
	
def addBinary(src, scale, base, type):
	global totalSize
	global textureData
		
	src = src.replace('\\', '/')

	hash = getXXHash64(src, base)
	
	if not hash:
		print('could not get hash for %s' % src)
		return
	print('adding binary ' + src)
	
	with open(src, 'rb') as f:
		buffer = f.read()
		totalSize += len(buffer)
		textureData[hash] = Asset(buffer, 0, 0, type = type)
	
def addAsset(src, scale, base):
	if src.endswith('.png'):
		return addTexture(src, scale, base)
	elif src.endswith('.geo'):
		return addBinary(src, scale, base, GEO)
	elif src.endswith('.gfx'):
		return addBinary(src, scale, base, DISPLAY_LIST)
	elif src.endswith('.bhv'):
		return addBinary(src, scale, base, BHV_SCRIPT)
	elif src.endswith('.lvl'):
		return addBinary(src, scale, base, LEVEL_SCRIPT)
	elif src.endswith('.bin'):
		return addBinary(src, scale, base, 0)
	elif os.path.basename(src) in idMap:
		return addBinary(src, scale, base, 0)
	
parser = argparse.ArgumentParser(description='Build sm64 mod packs')
parser.add_argument('--output', '-o', required=True, help='output file name or directory')
parser.add_argument('--source', '-s', help='source directory containing files to compare')
parser.add_argument('--basedir', '-b', default='.', help='base directory for searching for files')
parser.add_argument('--extract', '-e', help='file to unpack, use --output to specify directory')
parser.add_argument('--scale', type=int, default=1, help='texture resolution divisor.  2 means half quality, 1 is original quality')
parser.add_argument('--min-size', type=int, help='minimum texture resolution')
parser.add_argument('--max-size', type=int, help='maximum texture resolution')
parser.add_argument('--usepaths', action="store_true", help='export textures using filename paths')
args = parser.parse_args()

magic = (0xE8B675681037DE12).to_bytes(8, byteorder='little')

if args.min_size is not None:
	minSize = args.min_size
	
if args.max_size is not None:
	maxSize = args.max_size

if args.source:
	if args.source.endswith('.json'):
		with open(args.source, 'r') as f:
			entries = json.loads(f.read())
			
			basedir = os.path.basename(args.basedir)
			for path, entry in entries.items():
				try:
					addAsset(path, args.scale, basedir)
				except BaseException as e:
					print(str(e))
	else:
		for (dir, _, files) in os.walk(args.source):
			for f in files:
				path = os.path.join(dir, f)
				try:
					addAsset(path, args.scale, args.source)
				except BaseException as e:
					print(str(e))
			
	def align(offset, alignment):
		return offset
		return (offset // alignment) * alignment + alignment
		
	print('total size: %d bytes' % totalSize)

	header = magic
	body = b''

	header += len(textureData).to_bytes(8, byteorder='little')

	offset = 8 + 8 + len(textureData) * 0x40

	offset = align(offset, 0x4000)

	crcs = list(textureData.keys())
	crcs.sort()

	for crc in tqdm(crcs):
		asset = textureData[crc]
		cctx = zstd.ZstdCompressor(level=22)
		data = cctx.compress(asset.buffer)
		
		header += crc.to_bytes(8, byteorder='little')
		header += len(data).to_bytes(4, byteorder='little')
		header += asset.type.to_bytes(1, byteorder='little')
		header += b'\x00' * 3 # flags
		header += offset.to_bytes(8, byteorder='little')
		header += asset.width.to_bytes(8, byteorder='little')
		header += asset.height.to_bytes(8, byteorder='little')
		header += b'\x00' * 24 # unused

		body += data
		offset = align(offset + len(data), 0x1000)
		
	print('flushing to disk')
		
	with open(args.output, 'wb+') as f:
		f.write(header)
		f.write(body)

elif args.extract:
	os.makedirs(args.output, exist_ok=True)
	with open(args.extract, 'rb') as f:
		f.seek(0)
		if f.read(0x8) != magic:
			raise IOError('invalid magic')
			
		entryCount = int.from_bytes(f.read(0x4), byteorder='little')
		
		for i in tqdm(range(entryCount)):
			f.seek(i * 0x40 + 0x10)
			xxhash64 = int.from_bytes(f.read(0x8), byteorder='little')
			size = int.from_bytes(f.read(0x4), byteorder='little')
			type = f.read(1)
			flags = f.read(3)
			offset = int.from_bytes(f.read(0x8), byteorder='little')
			width = int.from_bytes(f.read(0x8), byteorder='little')
			height = int.from_bytes(f.read(0x8), byteorder='little')
			
			f.seek(offset)
			buffer = f.read(size)
			
			cctx = zstd.ZstdDecompressor()
			buffer = cctx.decompress(buffer)
			
			outputFileName = None
			if args.usepaths:
				outputFileName = os.path.join(args.output, getPath(xxhash64))
				
			if not outputFileName:
				outputFileName = os.path.join(args.output, '%16x.png' % xxhash64)
			
			os.makedirs(os.path.dirname(outputFileName), exist_ok = True)
			im = Image.frombytes("RGBA", (width, height), buffer)
			im.save(outputFileName)
	


