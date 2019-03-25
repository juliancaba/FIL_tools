#!/usr/bin/python
# -*- coding: utf-8; mode: python -*-

import argparse
from os import listdir, path, system
import sys
from jinja2 import Environment, PackageLoader, FileSystemLoader

TEMPLATES_DIR='/opt/fil_testing/src/c2IPsrc/templates'

TCL_CREATE_TEMPLATE="create.tmpl"
MK_TEMPLATE="makefile.tmpl"

ZEDBOARD_XILINX="xilinx.com:zc702:part0:0.9"
ZEDBOARD_AVNET="em.avnet.com:zed:part0:1.3"
PART = "xc7z020clg484-1"


VERBOSE = False


class IPSrcGen():
    def __init__(self, output, sources, language, part, board, tb):
        global HDL_VALID_EXTENSIONS
        
        HDL_VALID_EXTENSIONS={"VHDL", "VHD", "V"}

        self.env = Environment(loader=FileSystemLoader(TEMPLATES_DIR))
        self.output = output
        self.sources = sources
        self.language = language.upper()
        self.part = part
        self.board = board
        self.tb = tb

        
    def get_sources(self, src_path):
        hw_files = ""
        for it in listdir(src_path):
            filename, file_extension = path.splitext(it)
            if file_extension.upper()[1:] in HDL_VALID_EXTENSIONS:
                hw_files += it + " "        
        return hw_files[:-1]


    def get_ips(self):
        ips_sources = ""
        for it in listdir(self.sources):
            filename, file_extension = path.splitext(it)
            if file_extension.upper()[1:] == "TCL":
                file_tcl = open(self.sources+'/'+it, 'r')
                tcl_lines = file_tcl.readlines()[:-2]
                for itLines in tcl_lines:
                    ips_sources += itLines
                ips_sources += '\n\n'
        return ips_sources
    

    
    def generate_tcl(self):
        hw_sources=self.get_sources(self.sources)
        if self.tb != None:
            tb_sources = self.get_sources(self.tb)
            tb_dir = 'sim'
        else:
            tb_sources = ""
            tb_dir = "src"
            
        ips=self.get_ips()
        script_tcl = self.env.get_template(TCL_CREATE_TEMPLATE).render(SRC_DIR='src', SRC_FILES=hw_sources, TB_DIR=tb_dir, TB_FILES=tb_sources, LANGUAGE=self.language, PART=self.part, BOARD=self.board, IP_CORES=ips)
        f_tcl = open(self.output+"/create.tcl",'w')
        f_tcl.write(script_tcl)
        f_tcl.close()

        if VERBOSE:
            print("[INFO] Generated TCL script (Path: {})".format(self.output))
            
        
    def generate_mk(self):
        if self.tb != None:
            tb_dir = path.abspath(self.tb)
        else:
            tb_dir = None
            
        makefile = self.env.get_template(MK_TEMPLATE).render(SRC_DIR=path.abspath(self.sources), TB_DIR=tb_dir)
        f_mk = open(self.output+"/Makefile",'w')
        f_mk.write(makefile)
        f_mk.close()

        if VERBOSE:
            print("[INFO] Generated Makefile (Path: {})".format(self.output))

            
    def run(self):
        system("cd {}; make clean all; cd -".format(self.output))
        
        if VERBOSE:
            print ("[DONE] hls2IPsrc")

        
        

def parse_args_hls2ipsrc(args):
    parser = argparse.ArgumentParser(args)
    parser.add_argument('sources', help='Path of source files')
    parser.add_argument('-tb', dest='tb', help='Path of testbench files')
    parser.add_argument('-p', '--part', dest='part', default=PART, help='Board part')
    parser.add_argument('-b', '--board', dest='board', default=ZEDBOARD_AVNET, help='Board')
    parser.add_argument('-l', '--language', dest='language', default='VHDL', required=True, help='Preferred High-Description Language')
    parser.add_argument('-o', '--output', dest='output', required=True)
    parser.add_argument('-v', '--verbose', dest='verbose', action='store_true')
    parser.add_argument('--version', action='version', version='version 1.0')
    return parser.parse_args()


        
if __name__ == "__main__":
    args = parse_args_hls2ipsrc(sys.argv)
    VERBOSE = args.verbose

    ipsrc_prj = IPSrcGen(args.output, args.sources, args.language, args.part, args.board, args.tb)
    ipsrc_prj.generate_tcl()
    ipsrc_prj.generate_mk()
    ipsrc_prj.run()
    
