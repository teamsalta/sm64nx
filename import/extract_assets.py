#!/usr/bin/env python3
import sys
import os
import json
import hashlib
import xxhash
import platform

tools_dir = '../tools/'

tools_dir = os.path.abspath(tools_dir)

def executablePath(file):
	path = os.path.join(tools_dir, file)
	if '.' not in file and platform.system() == 'Windows':
		return path + '.exe'
	return path
	
def localFile(path):
	return os.path.abspath(os.path.join('./', path))

def read_asset_map():
	with open("assets.json") as f:
		ret = json.load(f)
	return ret


def read_local_asset_list(f):
	if f is None:
		return []
	ret = []
	for line in f:
		ret.append(line.strip())
	return ret


def asset_needs_update(asset, version):
	if version <= 4 and asset in ["textures/mountain/ttm_textures.01800.rgba16.png", "textures/mountain/ttm_textures.05800.rgba16.png"]:
		return True
	if version <= 3 and asset == "textures/cave/hmc_textures.01800.rgba16.png":
		return True
	if version <= 2 and asset == "textures/inside/inside_castle_textures.09000.rgba16.png":
		return True
	if version <= 1 and asset.endswith(".m64"):
		return True
	if version <= 0 and asset.endswith(".aiff"):
		return True
	return False


def remove_file(fname):
	os.remove(fname)
	print("deleting", fname)
	try:
		os.removedirs(os.path.dirname(fname))
	except OSError:
		pass


def clean_assets(local_asset_file):
	assets = set(read_asset_map().keys())
	assets.update(read_local_asset_list(local_asset_file))
	for fname in list(assets) + [".assets-local.txt"]:
		if fname.startswith("@"):
			continue
		try:
			remove_file(fname)
		except FileNotFoundError:
			pass

def findRom(lang = None):
	expected_hash = '9bef1128717f958171a4afac3ed78ee2bb4e86ce'
	dirs = ['./', '../']
	for dir in dirs:
		for fname in os.listdir(dir):
			if fname.lower().endswith('.z64'):
				path = os.path.join(dir, fname)
				
				with open(path, "rb") as f:
					sha1 = hashlib.sha1(f.read()).hexdigest().lower()				
				
				if sha1 == expected_hash:
					return path
				else:
					print('%s has wrong hash! %s vs %s' % (path, sha1, expected_hash))
	return None

def main():
	# In case we ever need to change formats of generated files, we keep a
	# revision ID in the local asset file.
	new_version = 5

	try:
		local_asset_file = open(".assets-local.txt")
		local_asset_file.readline()
		local_version = int(local_asset_file.readline().strip())
	except Exception:
		local_asset_file = None
		local_version = -1

	langs = ['us'] #sys.argv[1:]
	if langs == ["--clean"]:
		clean_assets(local_asset_file)
		sys.exit(0)

	all_langs = ["jp", "us", "eu"]
	if not langs or not all(a in all_langs for a in langs):
		langs_str = " ".join("[" + lang + "]" for lang in all_langs)
		print("Usage: " + sys.argv[0] + " " + langs_str)
		print("For each version, baserom.<version>.z64 must exist")
		sys.exit(1)

	asset_map = read_asset_map()
	all_assets = []
	any_missing_assets = False
	for asset, data in asset_map.items():
		if asset.startswith("@"):
			continue
		if os.path.isfile(asset):
			all_assets.append((asset, data, True))
		else:
			all_assets.append((asset, data, False))
			if not any_missing_assets and any(lang in data[-1] for lang in langs):
				any_missing_assets = True

	if not any_missing_assets and local_version == new_version:
		# Nothing to do, no need to read a ROM. For efficiency we don't check
		# the list of old assets either.
		return

	# Late imports (to optimize startup perf)
	import subprocess
	import tempfile
	from collections import defaultdict

	new_assets = {a[0] for a in all_assets}

	previous_assets = read_local_asset_list(local_asset_file)
	if local_version == -1:
		# If we have no local asset file, we assume that files are version
		# controlled and thus up to date.
		local_version = new_version

	# Create work list
	todo = defaultdict(lambda: [])
	for (asset, data, exists) in all_assets:
		# Leave existing assets alone if they have a compatible version.
		if exists and not asset_needs_update(asset, local_version):
			continue

		meta = data[:-2]
		size, positions = data[-2:]
		for lang, pos in positions.items():
			mio0 = None if len(pos) == 1 else pos[0]
			pos = pos[-1]
			if lang in langs:
				todo[(lang, mio0)].append((asset, pos, size, meta))
				break

	# Load ROMs
	roms = {}
	for lang in langs:
		fname = findRom(lang)
		try:
			with open(fname, "rb") as f:
				roms[lang] = f.read()
		except:
			print("Failed to open " + fname + ". Please ensure it exists!")
			sys.exit(1)

	'''
	# Make sure tools exist
	subprocess.check_call(
		["make", "-s", "-C", tools_dir, "n64graphics", "skyconv", "mio0", "aifc_decode"]
	)
	'''

	# Go through the assets in roughly alphabetical order (but assets in the same
	# mio0 file still go together).
	keys = sorted(list(todo.keys()), key=lambda k: todo[k][0][0])

	# Import new assets
	for key in keys:
		assets = todo[key]
		lang, mio0 = key
		if mio0 == "@sound":
			with tempfile.NamedTemporaryFile(prefix="ctl", delete=False) as ctl_file:
				with tempfile.NamedTemporaryFile(prefix="tbl", delete=False) as tbl_file:
					rom = roms[lang]
					size, locs = asset_map["@sound ctl " + lang]
					offset = locs[lang][0]
					ctl_file.write(rom[offset : offset + size])
					ctl_file.close()
					size, locs = asset_map["@sound tbl " + lang]
					offset = locs[lang][0]
					tbl_file.write(rom[offset : offset + size])
					tbl_file.close()
					args = [
						"python3",
						executablePath("disassemble_sound.py"),
						ctl_file.name,
						tbl_file.name,
						"--only-samples",
					]
					for (asset, pos, size, meta) in assets:
						print("extracting", asset)
						args.append(asset + ":" + str(pos))
					try:
						subprocess.run(args, check=True)
					except BaseException as e:
						print(str(e))
					finally:
						os.unlink(ctl_file.name)
						os.unlink(tbl_file.name)
			continue

		if mio0 is not None:
			image = subprocess.run(
				[
					executablePath("mio0"),
					"-d",
					"-o",
					str(mio0),
					findRom(),
					"-",
				],
				check=True,
				stdout=subprocess.PIPE,
			).stdout
		else:
			image = roms[lang]

		for (asset, pos, size, meta) in assets:
			print("extracting", asset)
			input = image[pos : pos + size]
			os.makedirs(os.path.dirname(asset), exist_ok=True)
			
			'''
			if asset.startswith("textures/skyboxes/") or asset.startswith("levels/ending/cake"):
				continue
				
			if asset.endswith(".png"):
				w, h = meta
				asset = '%s.%dx%d.raw' % (asset[0:-4], w, h)
			'''

			if asset.endswith(".png"):
				
				if asset.startswith("textures/skyboxes/") or asset.startswith("levels/ending/cake"):
					with tempfile.NamedTemporaryFile(prefix="asset") as png_file:
						png_file.write(input)
						png_file.flush()
						if asset.startswith("textures/skyboxes/"):
							imagetype = "sky"
						else:
							imagetype =  "cake" + ("-eu" if "eu" in asset else "")
						subprocess.run(
							[
								executablePath("skyconv"),
								"--type",
								imagetype,
								"--combine",
								png_file.name,
								asset,
							],
							check=True,
						)
				else:
					w, h = meta
					with tempfile.NamedTemporaryFile(prefix="asset", delete=False) as png_file:
						png_file.write(input)
						png_file.flush()
						png_file.close()
						fmt = asset.split(".")[-2]
						subprocess.run(
							[
								executablePath("n64graphics"),
								"-e",
								png_file.name,
								"-g",
								'%s[%s].png' % (asset[0:-4], xxhash.xxh64(input).hexdigest()),
								"-f",
								fmt,
								"-w",
								str(w),
								"-h",
								str(h),
							],
							check=True,
						)
						
					asset = '%s.%dx%d.raw' % (asset[0:-4], w, h)
					with open(asset, "wb") as f:
						f.write(input)
						
			else:
				with open(asset, "wb") as f:
					f.write(input)

	# Remove old assets
	for asset in previous_assets:
		if asset not in new_assets:
			try:
				remove_file(asset)
			except FileNotFoundError:
				pass

	# Replace the asset list
	output = "\n".join(
		[
			"# This file tracks the assets currently extracted by extract_assets.py.",
			str(new_version),
			*sorted(list(new_assets)),
			"",
		]
	)
	with open(".assets-local.txt", "w") as f:
		f.write(output)


main()
