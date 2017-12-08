import subprocess
import shutil
import argparse
import os

SYMBOL = 'CppHint'
CANDIDATE_FILE = '/tmp/candidate.cpp'
VARIABLE_FILE = '/tmp/variable.cpp'
EXECUTABLE = '/tmp/cpphinter'

class NameMap:
    def __init__(self):
        self.map = {}
        self.terminate = False

    def __str__(self):
        return str(self.map) + " " + str(self.terminate)

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('file', help='Source File')
    parser.add_argument('build_dir', help='Build dir')
    return parser.parse_args()

def build_signature_tree(sig_tree, sig):
    if sig[1] not in sig_tree:
        sig_tree[sig[1]] = ({}, [])
    (node, function) = sig_tree[sig[1]]
    for i in range(2, len(sig) - 1):
        if sig[i] not in node:
            node[sig[i]] = ({}, [])
        (node, function) = node[sig[i]]
    function.append(sig[0])

def build_name_map(name_map, sig):
    if sig[0] not in name_map:
        name_map[sig[0]] = NameMap()
    if len(sig) == 1:
        name_map[sig[0]].terminate = True
    else:
        build_name_map(name_map[sig[0]].map, sig[1:])

def parse_signatures(sigs):
    sigs = [sig.split(', ') for sig in sigs.strip().split('\n')]
    sig_tree = {}
    name_map = {}
    for sig in sigs:
        if len(sig) == 2 or sig[-1] == 'void':
            continue
        build_signature_tree(sig_tree, sig)
        build_name_map(name_map, sig[:-1])
    return (sig_tree, name_map)

def generate_signatures(build_dir, source):
    build_dir = os.path.join(build_dir, 'bin')
    return execute(['./FuncSign', source], parse_signatures, build_dir)

def parse_variables(var):
    candidats = {}
    for pair in [v.split(' ') for v in var.split('\n')]:
        if pair[0] not in candidats:
            candidats[pair[0]] = []
        candidats[pair[0]].append(pair[1])
    return candidats

def generate_candidate_variables(build_dir):
    src_path = os.path.abspath(VARIABLE_FILE)
    build_dir = os.path.join(build_dir, 'bin')
    return execute(['./FindVariables', src_path], parse_variables, build_dir)

def execute(command, post_processing, build_dir=None, shell=False):
    if build_dir is None:
        out = subprocess.Popen(command, stdout=subprocess.PIPE, shell=shell)
    else:
        out = subprocess.Popen(command, cwd=build_dir, 
                stdout=subprocess.PIPE, shell=shell)
    (data, _) = out.communicate()
    return post_processing(data.decode('utf-8').strip())

def process_source_file(src):
    var_file = open(VARIABLE_FILE, 'w+')
    with open(src, "r") as f:
        for l in f:
            if SYMBOL in l:
                query = [x.strip() for x in l[l.find(SYMBOL)+8:].split(',')]
                var_file.write('int ' + SYMBOL + ';\n')
            else:
                var_file.write(l)
    return query

def find_candidates_fussy_name(query, sig_tree):
    result = []
    current = []

    def search(query, tree_data):
        if isinstance(tree_data, dict):
            sig_tree = tree_data
        else:
            (sig_tree, functions) = tree_data
        if len(query) == 0:
            for func in functions:
                result.append([func] + current)
        elif query[0] == '?':
            for t in sig_tree:
                current.append(t)
                search(query[1:], sig_tree[t])
                current.pop()
        elif query[0] in sig_tree:
            current.append(query[0])
            search(query[1:], sig_tree[query[0]])
            current.pop()

    search(query, sig_tree)
    return result

def find_candidates_function_name(query, name_map):
    result = []
    current = [query[0]]

    def search(query, name_map):
        if len(query) == 0:
            if name_map.terminate:
                result.append(current[:])
        elif query[0] == '?':
            for t in name_map.map:
                current.append(t)
                search(query[1:], name_map.map[t])
                current.pop()
        elif query[0] in name_map.map:
            current.append(query[0])
            search(query[1:], name_map.map[query[0]])
            current.pop()

    if query[0] in name_map:
        search(query[1:], name_map[query[0]])
    return result


def find_candidates(query, sig_tree, name_map, variables):
    if query[0] == '?':
        funcs = find_candidates_fussy_name(query[1:], sig_tree)
    else:
        funcs = find_candidates_function_name(query, name_map)

    candidates = []
    current = []
    def search(func):
        if len(func) == 0:
            candidates.append(current[0] + '(' + ','.join(current[1:]) + ')')
        elif func[0] in variables:
            for var in variables[func[0]]:
                current.append(var)
                search(func[1:])
                current.pop()
    for func in funcs:
        current = [func[0]]
        search(func[1:])
        current.pop()
    return candidates 

def dumb_processor(out):
    pass

def type_info(out):
    print(out[1:])

def generate_and_build(candidates, src):
    for candidate in candidates:
        out = open(CANDIDATE_FILE, 'w+')
        out.write("#include<iostream>\n")
        out.write("#include<typeinfo>\n")
        with open(src, "r") as f:
            for l in f:
                if SYMBOL in l:
                    out.write("auto hintReturn=" + candidate + ";\n")
                    out.write("std::cout << typeid(*hintReturn).name()"
                            " << std::endl;\n")
                else:
                    out.write(l)
        out.close()
        execute(["clang++", CANDIDATE_FILE, '-std=c++11', '-o', EXECUTABLE], 
                dumb_processor)
        print(candidate + ": ", end='')
        execute([EXECUTABLE], type_info)

def main():
    args = parse_args()
    args.build_dir = os.path.abspath(args.build_dir)
    args.file = os.path.abspath(args.file)
    query = process_source_file(args.file)
    (sig_tree, name_map) = generate_signatures(args.build_dir, args.file)
    variables = generate_candidate_variables(args.build_dir)
    candidates = find_candidates(query, sig_tree, name_map, variables)
    generate_and_build(candidates, args.file)

if __name__ == '__main__':
    main()
