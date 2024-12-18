import argparse
import glob
from termcolor import colored
from syscall.sysGen import (find_simlinx_dir, runcmd)

def file_formating(clangFormatPath, file):
    runcmd(f'clang-format -style=file:{clangFormatPath}  -i {file}')
    print(colored(f'  {file}', 'yellow'))

def make_clang_format():
    parser = argparse.ArgumentParser()
    parser.add_argument('-a', '--auto', action='store_true', default=False, help='Automatic directory finding.')
    args = parser.parse_args()
    simlinxDir = find_simlinx_dir()
    clangFormatPath = simlinxDir+'.clang-format'

    print(colored(f'Clang format for:', 'yellow'))
    if args.auto:
        for file in glob.iglob(find_simlinx_dir()+'/**/*.cc', recursive=True):
            file_formating(clangFormatPath, file)
        for file in [file for file in glob.iglob(find_simlinx_dir()+'/**/*.hh', recursive=True) if 'decodeTree' not in file]:
            file_formating(clangFormatPath, file)
    else:
        for file in glob.iglob(find_simlinx_dir()+'/**/*.gen.cc', recursive=True):
            file_formating(clangFormatPath, file)
        for file in [file for file in glob.iglob(find_simlinx_dir()+'/**/*.gen.hh', recursive=True) if 'decodeTree' not in file]:
            file_formating(clangFormatPath, file)

if __name__ == '__main__':
    make_clang_format()