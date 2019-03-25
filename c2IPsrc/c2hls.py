#!/usr/bin/python
# -*- coding: utf-8; mode: python -*-

import argparse
from os import listdir, path, system
import sys
from jinja2 import Environment, PackageLoader, FileSystemLoader

TEMPLATES_DIR='/opt/fil_testing/src/c2IPsrc/templates'
STATIC_DIR='/opt/fil_testing/src/c2IPsrc/static'

TCL_TEMPLATE="script.tmpl"
MK_FILE="Makefile"

PART = "xc7z020clg484-1"

VERBOSE = False

class HLSGen():
    def __init__(self, output, sources, part, period, tb, dat, includes, top, directives, csim, cosim):
        global addfiles_line
        global addfiles_basic_line
        global datfiles_line
        global addtbfiles_line
        global HLL_VALID_EXTENSIONS
        global DAT_VALID_EXTENSIONS

        addfiles_line = "add_files {}/{} -cflags \"{}\"\n"
        addfiles_basic_line = "add_files {}/{}\n"
        datfiles_line = "add_files -tb {}/{}\n"
        addtbfiles_line = "add_files -tb {}/{} -cflags \"{}\"\n"
        HLL_VALID_EXTENSIONS={"C", "CC", "CPP", "H", "HPP"}
        DAT_VALID_EXTENSIONS={"DAT"}

        self.env = Environment(loader=FileSystemLoader(TEMPLATES_DIR))
        self.output = output
        self.sources = sources
        self.part = '{'+part+'}'
        self.period = period
        self.tb = tb
        self.dat = dat
        self.includes = includes
        self.topName = top
        self.directives = directives
        self.csim = csim
        self.cosim = cosim
        
        
    def get_files(self, files_path, files_line, valid_extensions, includes=None):
        files = ""

        if includes:
            include_line = "-I {} "
            include = "-I . "
            for itInc in includes:
                include += include_line.format(path.abspath(itInc))
        print(include)
            
        for it in listdir(files_path):
            filename, file_extension = path.splitext(it)
            if file_extension.upper()[1:] in valid_extensions:
                if includes == None:
                    files += files_line.format(path.abspath(files_path), it)
                else:
                    files += files_line.format(path.abspath(files_path), it, include)        
        return files[:-1]

    
    def get_directives_line(self):
        if self.directives != None:
            return 'source ' + path.abspath(self.directives)
        else:
            return ""
    
    def generate_tcl(self):
        if self.includes==None:
            sw_sources=self.get_files(self.sources, addfiles_basic_line, HLL_VALID_EXTENSIONS, self.includes)
        else:
            sw_sources=self.get_files(self.sources, addfiles_line, HLL_VALID_EXTENSIONS, self.includes)
            
        if self.tb != None:
            tb_sources = self.get_files(self.tb, addtbfiles_line, HLL_VALID_EXTENSIONS, self.includes)
        else:
            tb_sources = ""
            
        if self.dat != None:
            dat_sources = self.get_files(self.dat, datfiles_line, DAT_VALID_EXTENSIONS)
        else:
            dat_sources = ""
            
        directives_line = self.get_directives_line()
        script_tcl = self.env.get_template(TCL_TEMPLATE).render(TOP_NAME=self.topName, SRC_FILES=sw_sources, TB_FILES=tb_sources, DAT_FILES=dat_sources, PART=self.part, CLK_PERIOD=self.period, DIRECTIVES_LINE=directives_line, CSIM=self.csim, COSIM=self.cosim)
        f_tcl = open(self.output+"/script.tcl",'w')
        f_tcl.write(script_tcl)
        f_tcl.close()

        if VERBOSE:
            print("[INFO] Generated TCL script (Path: {})".format(self.output))

        
    def copy_mk(self):
        system("cp -f {}/{} {}".format(STATIC_DIR, MK_FILE, self.output))
                
        if VERBOSE:
            print("[INFO] Copied Makefile (Path: {})".format(self.output))

            
    def run(self):
        system("cd {}; make clean all; cd -".format(self.output))
                    
        if VERBOSE:
            print ("[DONE] c2hls")
        

def parse_args_c2hls(args):
    parser = argparse.ArgumentParser(args)
    parser.add_argument('sources', help='Path of source files')
    parser.add_argument('-tb', dest='tb', help='Path of testbench files')
    parser.add_argument('-dat', dest='dat', help='Path of data files for the testbench')
    parser.add_argument('-I', dest='includes', nargs='*', default=None, help='Include files')
    parser.add_argument('-p', '--part', dest='part', default=PART, help='Board part')
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
    args = parse_args_c2hls(sys.argv)
    VERBOSE = args.verbose

    hls_prj = HLSGen(args.output, args.sources, args.part, args.period, args.tb, args.dat, args.includes, args.top, args.directives, args.csim, args.cosim)
    hls_prj.generate_tcl()
    hls_prj.copy_mk()
    hls_prj.run()
