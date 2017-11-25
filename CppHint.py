import subprocess
import shutil
import argparse
import os

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('file', help='Source File')
    parser.add_argument('build_dir', help='Build dir')
    return parser.parse_args()

#Build pattern matching tree
def build_function_map(sig_map, sig):
    if sig[-1] not in sig_map:
        sig_map[sig[-1]] = ({}, [])
    (node, function) = sig_map[sig[-1]]
    for i in range(1, len(sig) - 1):
        if sig[i] not in node:
            node[sig[i]] = ({}, [])
        (node, function) = node[sig[i]]
    function.append(sig[0])

def parse_signatures(sigs):
    sigs = [sig.split(', ') for sig in sigs.decode('utf-8').strip().split('\n')]
    sig_map = {}
    for sig in sigs:
        build_function_map(sig_map, sig)
    return sig_map

def generate_signatures(build_dir, source):
    bin_dir = os.path.join(build_dir, 'bin')
    sig_out = subprocess.Popen(['./FuncSign', source], cwd=bin_dir, 
            stdout=subprocess.PIPE)
    (sigs, _) = sig_out.communicate()
    sig_map = parse_signatures(sigs)
    print(sig_map)

def main():
    args = parse_args()
    args.build_dir = os.path.abspath(args.build_dir)
    args.file = os.path.abspath(args.file)
    generate_signatures(args.build_dir, args.file)

if __name__ == '__main__':
    main()
