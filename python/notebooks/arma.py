# Armadillo utility module.
# TODO: Make it truly compatible with Armadillo instead of not very.

import numpy as np

def complexLine(x):
    '''Parse line with complex numbers.'''
    return [complex(float(a[0]), float(a[1])) for a in [s.strip('()').split(',') for s in x]]

def realLine(x):
    '''Parse line with real numbers.'''
    return [float(a) for a in x]

def matFromFile(fname):
    '''Read matrix from file fname.'''

    with open(fname, 'r') as f:
        armatype = f.readline()
        if armatype.startswith('ARMA_MAT_TXT_FC'):
            datatype = np.complex128
            linefun = complexLine
        elif armatype.startswith('ARMA_MAT_TXT'):
            datatype = np.float64
            linefun = realLine
        else:
            raise Excetption('Not ARMA!')

        line = f.readline()
        rows, cols = line.strip('\n').split(sep=' ')
        (rows, cols) = (int(rows), int(cols))

        L = np.zeros((rows, cols), dtype=datatype)

        i = 0
        for line in f:
            x = line.strip(' \n').split()
            L[i,:] = linefun(x)
            i += 1

    return L
