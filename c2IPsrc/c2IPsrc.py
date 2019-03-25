#!/usr/bin/python
# -*- coding: utf-8; mode: python -*-

import os
import sys
import argparse


TEMPLATES_DIR='/opt/fil_testing/src/c2IPsrc/templates'
STATIC_DIR='/opt/fil_testing/src/c2IPsrc/static'

TCL_CREATE_TEMPLATE="create.tmpl"
MK_TEMPLATE="makefile.tmpl"

TCL_TEMPLATE="script.tmpl"
MK_FILE="Makefile"

ZEDBOARD_XILINX="xilinx.com:zc702:part0:0.9"
ZEDBOARD_AVNET="em.avnet.com:zed:part0:1.3"
PART = "xc7z020clg484-1"

HDL = {'VHD':'vhdl', 'VHDL':'vhdl', 'V':'verilog'}

VERBOSE = False

from c2hls import HLSGen
from hls2IPsrc import IPSrcGen

def parse_args_c2ipsrc(args):
    parser = argparse.ArgumentParser(args)
    parser.add_argument('sources', help='Path of source files')
    parser.add_argument('-tb', dest='tb', help='Path of testbench files')
    parser.add_argument('-dat', dest='dat', help='Path of data files for the testbench')
    parser.add_argument('-I', dest='include', nargs='*', help='Include files')
    parser.add_argument('-p', '--part', dest='part', default=PART, help='Board part')
    parser.add_argument('-b', '--board', dest='board', default=ZEDBOARD_AVNET, help='Board')
    parser.add_argument('-l', '--language', dest='language', default='VHDL', required=True, help='Preferred High-Description Language')
    parser.add_argument('-c', '--clock_period', dest='period', default=10, type=int, help='Clock periotd. Bu default 10')
    parser.add_argument('-t', '--top', dest='top', required=True, help='Top function')
    parser.add_argument('-d', '--directives', dest='directives', help='External directives file')
    parser.add_argument('-o', '--output', dest='output', required=True)
    parser.add_argument('--csim', dest='csim', action='store_true')
    parser.add_argument('--cosim', dest='cosim', action='store_true')
    parser.add_argument('-v', '--verbose', dest='verbose', action='store_true')
    parser.add_argument('--version', action='version', version='version 1.0')
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args_c2ipsrc(sys.argv)
    
    VERBOSE = args.verbose

    tmp_output = './tmp_' + str(os.getpid())
    os.system("mkdir -p {}".format(tmp_output))    
    try:
        tmp_sources = tmp_output + '/hls_prj/solution/syn/'+HDL[args.language.upper()]
    except:
        print("[ERROR] Language not found")
        sys.exit(1)

        
    hls_prj = HLSGen(tmp_output, args.sources, args.part, args.period, args.tb, args.dat, args.include, args.top, args.directives, args.csim, args.cosim)
    hls_prj.generate_tcl()
    hls_prj.copy_mk()
    hls_prj.run()

    os.system("mkdir -p {}".format(args.output))
    ipsrc_prj = IPSrcGen(args.output, tmp_sources, HDL[args.language.upper()], args.part, args.board, args.tb)
    ipsrc_prj.generate_tcl()
    ipsrc_prj.generate_mk()
    ipsrc_prj.run()

    os.system("cp -r {}/hls_prj/solution/sim/tv {}/dat".format(tmp_output, args.output))
    os.system("rm -rf {}".format(tmp_output))
    
    
