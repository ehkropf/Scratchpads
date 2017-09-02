#!/opt/local/bin/python

import os
import sudoku as su

def rowcol(pos):
    """Row and column from grid position."""
    row = (pos // 9) + 1
    column = (pos % 9) + 1
    return row, column

def blankRow():
    return '       |       |'

def separatorRow():
    return '-------+-------+-------'

#===============================================================================
class Movement:
    """Base class for movement functions."""
    def __init__(self, n):
        self.distance = n

    def __str__(self):
        return self.descr.format(self.distance)

    def reverse(self):
        return self._reverse(self.distance)

class moveUp(Movement):
    """ansi move up"""
    descr = '\033[{0}A'

    def __init__(self, n):
        Movement.__init__(self, n)
        self._reverse = moveDown

class moveDown(Movement):
    """ansi move down"""
    descr = '\033[{0}B'

    def __init__(self, n):
        Movement.__init__(self, n)
        self._reverse = moveUp

class moveRight(Movement):
    """ansi move right"""
    descr = '\033[{0}C'

    def __init__(self, n):
        Movement.__init__(self, n)
        self._reverse = moveLeft

class moveLeft(Movement):
    """ansi move left"""
    descr = '\033[{0}D'

    def __init__(self, n):
        Movement.__init__(self, n)
        self._reverse = moveRight

#===============================================================================
if __name__ == "__main__":
    sol= su.Solution()

    for r in range(3):
        for _ in range(5):
            print(blankRow())
        if r < 2:
            print(separatorRow())

    moved = []
    moved.append(moveUp(17))
    print(moved[-1], end='')
    for c in range(9):
        if c > 0 and (c % 3) == 0:
            # Jump vertsep.
            moved.append(moveRight(2))
            print(moved[-1], end='')
        moved.append(moveRight(1))
        print(str(moved[-1]) + str(sol.entries[c].value), end='')
        moved.append(moveRight(1))

    moved.append(moveLeft(22))
    print(moved[-1])
    moved.append(moveDown(1))
    print(str(moved[-1]) + 'L', end='')
    moved.append(moveRight(1))

    while len(moved) > 0:
        mv = moved.pop().reverse()
        print(mv, end='')
    print('Done.')
