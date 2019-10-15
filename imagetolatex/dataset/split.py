import re
import os.path
import os

root_path = os.path.join(os.getcwd(), 'labels')

def _list_files(path):
    return [
        os.path.join(path, x)
        for x in filter(lambda x : not x.startswith('.'), os.listdir(path))
        if os.path.isfile(os.path.join(path, x))
    ]

def _load_lines(path):
    with open(path, 'r') as ifs:
        return [x.strip() for x in ifs.readlines()]

def parse(s, _index):
    chars = []
    index = 0
    while index < len(s):
        char = str(s[index])
        if char == '\\':
            chars.append(s[index : index + 5])
            index += 4
        elif char.isalnum() or char in ['+', '-', '/', '{', '}', '^']:
            chars.append(char)
        index += 1
    print(_index)
    return chars

for path in _list_files(root_path):
    lines = _load_lines(path)
    print(path, len(lines))
    result_lines = [
        parse(x.strip(), index) for index, x in enumerate(lines)
    ]
    assert len(result_lines) > 0


    result = "\n".join(["\t".join(x) for x in result_lines])

    with open(path + '.edit', 'w') as ofs:
        ofs.write(result)
        ofs.write('\n')
