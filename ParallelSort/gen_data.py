import os
import sys
import numpy as np


ii32 = np.iinfo(np.uint32)

def generate_test_set(nsize, p):
    data = np.random.randint(ii32.max, dtype=np.uint32, size=nsize)
    infile = f'input_{nsize}.txt'

    with open(infile, 'w') as fp:
        lines = [f"{nsize} {p}\n"]
        lines.extend([f"{x}\n" for x in data])
        fp.writelines(lines)

    print(f'Sample data saved at {infile}')


if __name__ == '__main__':
    nsize = int(sys.argv[1])
    p = int(sys.argv[2])
    generate_test_set(nsize, p)
