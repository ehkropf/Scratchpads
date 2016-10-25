#!/opt/local/bin/python

from __future__ import print_function
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import os

trapdataname = "trapode.data"
trapfigpdf = "trapode.pdf"

if __name__ == "__main__":
    # script bit

    trapdata = np.loadtxt(trapdataname)
    fig = plt.figure()
    ax = fig.gca()
    ax.semilogy(trapdata[:,0], trapdata[:,1], '-', trapdata[:,0], trapdata[:,2], '--')
    fig.savefig(trapfigpdf, format="pdf", bbox_inches="tight")
    os.system("open " + trapfigpdf)
