import os
from itertools import chain 

buffer = 'sm64.py '
actors = []

groups = [['star', 'unagi'], ['bobomb', 'bowling_ball'], ['koopa_without_shell', 'koopa'], ['pokey_body_part', 'pokey'], ['mario_cap', 'star', 'transparent_star', 'klepto', 'ukiki']]
flatten_group = list(chain.from_iterable(groups))

for fn in os.listdir('actors'):
	path = os.path.join('actors', fn)
	if not os.path.isdir(path):
		continue

	if fn not in flatten_group:
		actors.append(fn)
		
for fn in actors:
	path = os.path.join('actors', fn)
	print(path)
	
	model = os.path.join(path, 'model.inc.c')
	geo = os.path.join(path, 'geo.inc.c')
	
	tmp = ''
	if os.path.isfile(model):
		print(model)
		tmp += ' ' + model
		
	if os.path.isfile(geo):
		print(geo)
		tmp += ' ' + geo

	buffer += tmp
	
	if len(tmp) > 0:
		with open(fn + '.bat', 'w') as f:
			f.write('sm64.py ' + tmp + ' > ' + fn + '.py')

def isListValid(lst):
	for i in lst:
		if not os.path.isdir(os.path.join('actors', i)):
			return False
	return True
	
for lst in groups:
	if not isListValid(lst):
		continue
	print('group: ' + ', '.join(lst))
	
	tmp = ''
	for i in lst:
		path = os.path.join('actors', i)
		model = os.path.join(path, 'model.inc.c')
		if os.path.isfile(model):
			print(model)
			tmp += ' ' + model
			
	for i in lst:
		path = os.path.join('actors', i)
		geo = os.path.join(path, 'geo.inc.c')
		if os.path.isfile(geo):
			print(geo)
			tmp += ' ' + geo
			
	if len(tmp) > 0:
		actors.append('_'.join(lst))
		with open('_'.join(lst) + '.bat', 'w') as f:
			f.write('sm64.py ' + tmp + ' > ' + '_'.join(lst) + '.py')
	else:
		print('could not get any data for ' + ', '.join(lst))
			
	buffer += tmp
		
	
		
with open('all_actors.bat', 'w') as f:
	f.write(buffer + ' > all_actors.py')
	
with open('build_seq.bat', 'w') as f:
	for actor in actors:
		f.write('call ' + actor + '.bat' + '\n')
		
with open('run_seq.bat', 'w') as f:
	for actor in actors:
		f.write(actor + '.py' + '\n')