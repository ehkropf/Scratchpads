import cProfile
import pstats
import sudoku as su

def repeat(n):
    """Repeat creating solutions n times."""
    for _ in range(n):
        su.Solution()

cProfile.run('repeat(100)', 'supstats')
p = pstats.Stats('supstats')

p.strip_dirs().sort_stats('time').print_stats()
