#!/usr/bin/env python

#############################################################################

import time, getpass, commands

#############################################################################

L = [
	'mc11_7TeV.116968.AlpgenHWfZmumubbNp3_4LepM.merge.NTUP_HSG2.e835_s1310_s1300_r3043_r2993_p1054/'
	
]

#############################################################################

VERSION = int(time.time())

#############################################################################

for in_ds in L:

	out_ds = 'user.e%s.%s.%s-%d_%s' % (getpass.getuser(), in_ds.split('.')[+1], in_ds.split('.')[+2], VERSION, in_ds.split('.')[-1])

	print('Running \'%s\' -> \'%s\'...' % (in_ds, out_ds))

	status, output = commands.getstatusoutput('./run_mc.sh %s %s' % (in_ds, out_ds))

	if status == 0:
		print('[ \033[32m Ok. \033[0m ]')
	else:
		print('[ \033[31mError\033[0m ]')

		print('\033[31m%s\033[0m' % output)

#############################################################################

