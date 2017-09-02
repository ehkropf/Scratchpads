#!/opt/local/bin/python

from random import randint

class Entry:
    """Sudoku grid entry."""
    def __init__(self, index, value):
        self.value = value
        self.column = (index % 9) + 1
        self.row = (index // 9) + 1
        self.region = 3*((self.row - 1) // 3) + (self.column - 1) // 3 + 1

    def __repr__(self):
        return '<Entry value:' + str(self.value) + ' row:' + str(self.row) \
                + ' column: ' + str(self.column) + ' region:' + str(self.region) \
                + '>'

    def __str__(self):
        return self.value

class Solution:
    """Sudoku solution."""

    def __init__(self, observer=None):
        """Create soluiton with backtracking algorithm."""
        self.entries = []
        self._observer = observer

        available = [[i+1 for i in range(9)] for _ in range(81)]
        c = 0

        while c < 81:
            if len(available[c]) > 0:
                i = randint(0, len(available[c])-1)
                test = Entry(c, available[c][i])
                del available[c][i]

                if not self._conflicts(test):
                    self.entries.append(test)
                    c += 1
                else:
                    self._callObserver(c, test)
                    continue
            else:
                # Backtrack
                available[c] = [i+1 for i in range(9)]
                c -= 1
                del self.entries[c]
            self._callObserver(c)

    def _callObserver(self, c, test=[]):
        if self._observer:
            self._observer(c, self.entries, test)

    def __str__(self):
        """Convert to grid as string."""
        grid = ''
        row = 0
        while row < 9:
            for j in range(3):
                for k in range(3):
                    i = 9*row + 3*j + k
                    grid += ' ' + str(self.entries[i].value)
                grid += ' '
                if j < 2:
                    grid += '|'
            grid += '\n'
            row += 1
            if row == 3 or row ==6:
                grid += self._separatorRow() + '\n'
            elif row < 9:
                grid += self._blankRow() + '\n'
        return grid

    def __repr__(self):
        """Just a wrapper for __str__, really."""
        return '<sudoku.Solution\n' + str(self) + '>'

    @staticmethod
    def _blankRow():
        """Grid blank row."""
        return '       |       |      '

    @staticmethod
    def _separatorRow():
        """Grid separator row."""
        return '-------+-------+-------'

    def _conflicts(self, test):
        """Does test entry conflict with existing entries?"""

        for s in self.entries:
            # Short-circuiting logicl operators for the win.
            if s.value == test.value \
                    and (s.row == test.row or s.column == test.column or s.region == test.region):
                return True
        return False

################################################################################
if __name__ == "__main__":
    sol = Solution()
    print("Solution:\n")
    print(sol)
