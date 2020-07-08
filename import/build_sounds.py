#!/usr/bin/python3
import os
import subprocess

tools_dir = '../tools/'

tools_dir = os.path.abspath(tools_dir)

def executablePath(file):
	path = os.path.join(tools_dir, file)
	if '.' not in file:
		return path + '.exe'
	return path
	
def localFile(path):
	return os.path.abspath(os.path.join('./', path))
	
def findFiles(path, ext):
	f = []
	for (dirpath, dirnames, filenames) in os.walk(path):
		f.extend([os.path.join(dirpath, x) for x in filenames if x.endswith(ext)])
	return f
	
bits = [executablePath('assemble_sound.py'), '--sequences', localFile('sound/bank_sets'), localFile('sound/sound_banks/'), localFile('sound/sequences.json')]
bits += findFiles('.', '.m64')
bits += ['--endian', 'little', '--bitwidth', '64']


for aiff in findFiles('.', '.aiff'):
	tbl = aiff[0:-5] + '.table'
	aifc = aiff[0:-5] + '.aifc'
	
	with open(tbl, 'w') as f:
		subprocess.run(
			[
				executablePath("aiff_extract_codebook"),
				aiff
			],
			check=True,
			stdout = f
		)

	subprocess.run(
		[
			executablePath("vadpcm_enc"),
			'-c',
			tbl,
			aiff,
			aifc
		],
		check=True
	)

subprocess.run(
		[
			'python3',
			executablePath("assemble_sound.py"),
			localFile('sound/samples/'),
			localFile('sound_conf/sound_banks'),
			localFile('sound/sound_data.ctl'),
			localFile('sound/sound_data.tbl'),
			'--endian',
			'little',
			'--bitwidth',
			'64'
		],
		check=True
	)