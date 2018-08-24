#!/usr/bin/env python
############################################################################
#
# Author: Edgar Carrera
# March, 2017
############################################################################

"""
   usage: %prog [options] <directory path>
   -b, --bz2 : conversion of dat files bz2-compressed (default is True)
   -d, --dat : conversion of dat files, with no bz2-compression (default is False) Its usage overrides -b
"""

import os,sys
import subprocess
#flag to debug
DEBUG = True
printConfig = True
# _____________________OPTIONS_______________________________________________

############################################################################
# Code taken from http://code.activestate.com/recipes/278844/
############################################################################
import re, optparse
USAGE = re.compile(r'(?s)\s*usage: (.*?)(\n[ \t]*\n|$)')
def nonzero(self): # will become the nonzero method of optparse.Values
    "True if options were given"
    for v in self.__dict__.itervalues():
        if v is not None: return True
    return False

optparse.Values.__nonzero__ = nonzero # dynamically fix optparse.Values

class ParsingError(Exception): pass

optionstring=""

def exit(msg=""):
    raise SystemExit(msg or optionstring.replace("%prog",sys.argv[0]))

def parse(docstring, arglist=None):
    global optionstring
    optionstring = docstring
    match = USAGE.search(optionstring)
    if not match: raise ParsingError("Cannot find the option string")
    optlines = match.group(1).splitlines()
    try:
        p = optparse.OptionParser(optlines[0])
        for line in optlines[1:]:
            opt, help=line.split(':')[:2]
            short,long=opt.split(',')[:2]
            if '=' in opt:
                action='store'
                long=long.split('=')[0]
            else:
                action='store_true'
            p.add_option(short.strip(),long.strip(),
                         action = action, help = help.strip())
    except (IndexError,ValueError):
        raise ParsingError("Cannot parse the option string correctly")
    return p.parse_args(arglist)


#_________________________________________________________________________


#######################################################
def do_mass_conversion_dat2root(pathname,dobz2):
#######################################################
    #do bz2 uncompression if needed
    if(dobz2):
        str_ls = "ls -1 "+pathname+"/*.dat.bz2"
    else:
        str_ls = "ls -1 "+pathname+"/*.dat"

    #make a list with the full path filenames
    mypipe = subprocess.Popen(str_ls,shell=True,stdout=subprocess.PIPE)
    files = mypipe.communicate()[0].split()

    #Loop over all files
    for thefile in files:
        thedatfile = thefile
        #uncompress and keep original if needed
        if(dobz2):
            str_bz2 = "bzip2 -dk "+thefile
            os.system(str_bz2)
            #print "os.system("+str_bz2+")"
            thedatfile = thefile.strip(".bz2")
        #convert the datfile to root
        str_convert = "./dat2root "+thedatfile
        os.system(str_convert)
        #print "os.system("+str_convert+")"

    return 0
    

    
#######################################################
def get_default_options(option):
#######################################################
    dicOpt = {}

    # default is assuming that data files are bz2-compressed
    if not option.bz2:
        dicOpt['bz2'] = True
        dicOpt['dat'] = False
    else:
        dicOpt['bz2'] = True
        dicOpt['dat'] = False

    # default is assuming that they are not only dat
    if not option.dat:
        dicOpt['dat'] = False
        dicOpt['bz2'] = True
    else:
        dicOpt['dat'] = True
        dicOpt['bz2'] = False

    
    return dicOpt


#######################################################
if __name__ =='__main__':
#######################################################

    #import optionparser
    option,args = parse(__doc__)
    if not args and not option:
        exit()


    print args[0]

    #set default options
    dicOpt = get_default_options(option)
    if (printConfig):
        for k in dicOpt:
            print str(k)+" = "+str(dicOpt[k])

    #get options
    dobz2 = dicOpt['bz2']
    dojustdat = dicOpt['dat']
    
    #if download is needed, run the routine
    if(dobz2):
        do_mass_conversion_dat2root(str(args[0]),True)
    if(dojustdat):
        do_mass_conversion_dat2root(str(args[0]),False)
