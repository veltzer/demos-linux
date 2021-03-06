#!/usr/bin/python3

'''
This script moves over all source file and switches their license. It
only does so if they carry the old license
'''

import sys # for argv
import os # for walk

if len(sys.argv)>1:
    raise ValueError('this script does not accept parameters on the cmd line')

spdx="// SPDX-License-Identifier: GPL-2.0\n"
with open('support/old_license.txt') as f:
    old_lic=f.read()
with open('support/license.txt') as f:
    new_lic=f.read()

suffixes=[
    '.c',
    '.cc',
    '.tt',
    '.h',
    '.hh',
    '.S',
    '.S.dont_work',
    '.c.moved',
    '.cc.moved',
    '.cc.dont_work',
    '.h.moved',
    '.hh.moved',
    '.S.moved',
]
bad_suffixes=['.mod.c']
bad_prefixes=['./.venv']
root_folder='.'
for root,dirs,files in os.walk(root_folder):
    for file in files:
        current_file=os.path.join(root,file)
        doit=False
        for suf in suffixes:
            if current_file.endswith(suf):
                doit=True
        for suf in bad_suffixes:
            if current_file.endswith(suf):
                doit=False
        for pref in bad_prefixes:
            if current_file.startswith(pref):
                doit=False
        if doit:
            # print(f"examining [{current_file}]")
            in_f=open(current_file,'r')
            f=in_f.read()
            in_f.close()
            if f.startswith(old_lic):
                f=new_lic+f[len(old_lic):]
                with open(current_file,'wb') as out_f:
                    out_f.write(f.encode('utf-8'))
                    print(f"file [{current_file}] got its license replaced...")
                    next
            if f.startswith(spdx+old_lic):
                f=spdx+new_lic+f[len(spdx+old_lic):]
                with open(current_file,'wb') as out_f:
                    out_f.write(f.encode('utf-8'))
                    print(f"file [{current_file}] got its license replaced...")
                    next
            # print(f"file [{current_file}] was not replaced")
