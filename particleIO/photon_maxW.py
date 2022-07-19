#!/usr/bin/env python
# -*- coding: utf-8 -*-

#import matplotlib.pyplot as plt
from ParticleInterface import *
#from sys import argv
#from numpy import linspace
from numpy.random import uniform as uniform_dist
from numpy.random import normal as gaussian_dist

keV = 0.001
mm = 1.0
mrad = 1

# Generation of 80 keV photons of 1,000,000
W0 = 80 * keV  # maximum mono energy
dx = 10.0 * mm # uniform dist.
n_photon = 20000
dyp = 0.01 * mrad

photons2 = list()
for i in range(n_photon):
    p = particle_io(PDGpid = 22,
                    charge = 0,
                    atomic_number = 0,
                    atomic_mass = 0,
                    phase_x = uniform_dist(low=-dx, high=dx),
                    phase_xp = 0.,
                    phase_y = gaussian_dist(loc=0.0, scale=dyp),
                    phase_yp = 0.,
                    kinetic_energy = W0)
    photons2.append( p )

write_particles('photo80keV_20k.d2', photons2)


