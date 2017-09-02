#!/opt/local/bin/python

from sys import stdout
from time import sleep
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
CSI = '\033['

class Placer:
    """Curser placement and printing thingy.

       Uses ANSI cursor codes to move around.
    """

    def __init__(self):
        self._relative_row = 0

    @staticmethod
    def print(str):
        """No newline print."""
        print(str, end='')

    def _csiPrint(self, code):
        """Prefix code with CSI and print."""
        self.print(CSI + code)

    def _nCodePrint(self, n, code):
        """Print ANSI movement code with argument n.

           It is assumed the code format is ESC[nX where X is the code.
        """
        self._csiPrint(str(n) + code)

    # Postion reporting
    def curRelLine(self):
        """Returns current relative line number."""
        return self._relative_row

    # Movement
    def BOL(self):
        """Goto beginning of line."""
        self._csiPrint('G')

    def nUp(self, n):
        """Move to beginning of line n lines up."""
        self._nCodePrint(n, 'F')
        self._relative_row += n

    def nDown(self, n):
        """Move to beginning of line n lines down."""
        self._nCodePrint(n, 'E')
        self._relative_row -= n

    def nRight(self, n):
        """Move right n cells."""
        self._nCodePrint(n, 'C')

    def nLeft(self, n):
        """Move left n cells."""
        self._nCodePrint(n, 'D')

    def nColumn(self, n):
        """Move to column n."""
        self._nCodePrint(n, 'G')

#===============================================================================
def gridRowCol(entry):
    """Return cursor relative row and column given entry.

       Assume entry is in range [1,81].
       Bottom row of grid is 1, top row is 17.
    """

    row = 2*((entry - 1)//9)

    column = (entry - 1)%9
    column = 2*(column + 1) + 2*(column//3)

    return 17 - row, column

class SolutionObserver:
    """Observer for solution entries."""

    def __init__(self):
        self._last = 0
        self._pos = Placer()
        self._pos.nUp(17)

    def _rowPosition(self, row):
        """Make sure we're on the right row."""
        s = self._pos.curRelLine() - row
        if s > 0:
            self._pos.nDown(s)
        elif s < 0:
            self._pos.nUp(-s)

    def _position(self, c):
        """Go to required grid position."""
        row, col = gridRowCol(c)
        self._rowPosition(row)
        self._pos.nColumn(col)

    def observer(self, c, entries, test):
        if len(entries) < 1:
            return

        # If test is not empty, then c+1 is current entry being tested.
        # If nothing in test, then print entry. Note test is empty on a backtrack.

        if test:
            self._position(c+1)
            self._pos.print(test.value)
        else:
            if c < self._last:
                l = self._last
                while l > c:
                    self._position(l+1)
                    self._pos.print(' ')
                    l -= 1

            self._position(c)
            self._pos.print(entries[c-1].value)

            self._last = c

        stdout.flush()
        sleep(0.005)

#===============================================================================
if __name__ == "__main__":

    for r in range(3):
        for _ in range(5):
            print(blankRow())
        if r < 2:
            print(separatorRow())

    sol = su.Solution(SolutionObserver().observer)
    print('')
