#!/usr/bin/env python
# -*- coding: utf-8 -*-

#===============================================================================
#
#  Transverse Twiss Beam Viewer ( a part of optics expert )
#    
#                                                 2013. 8. 2. by Garam Hahn
#
#===============================================================================

import matplotlib.pyplot as plt
from ParticleInterface import *
from sys import argv
from numpy import linspace
# global units, accelerator basics and linear optics was included automatically
# random was imported automatically

# Generation of 80 keV photons 1,000,000



# Reading
photons = read_particles('blockphoton.d2')
print('reading complete')

# Selection
photons1 = list()
for p in photons:
    if p.x * p.x > 100.:
        continue
    photons1.append(p)
photons = photons1

write_particles('photons1.d2', photons)


ws = [p.w * 1e3 for p in photons]
plt.hist(ws, weights=ws, bins=1000, range=(0.01, 200.), histtype='step')
plt.yscale('log')
plt.xscale('log')
plt.xlim(0.01, 200)
plt.xlabel('Photon Energy [keV]')
plt.ylabel('Power of Photon [Arb. Unit]')

plt.figure()
plt.hist(ws, bins=1000, range=(0.01, 200.), histtype='step')
plt.yscale('log')
plt.xscale('log')
plt.xlim(0.01, 200)
plt.xlabel('Photon Energy [keV]')
plt.ylabel('Number of Photon [Arb. Unit]')

plt.show()

