#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# class particle_io
# {
# private:
#     int pid;
#     int Q;
#     int Z;
#     int A;
#     double x;
#     double xp;
#     double y;
#     double yp;
#     double w;
#
from struct import unpack, pack

HEADER_SIZE = 1024
STRUCTURE = 'iiiiddddd'
SIZE_OF_INT = 4
SIZE_OF_DOUBLE = 8
SIZE_OF_STRUCTURE = SIZE_OF_INT*4 + SIZE_OF_DOUBLE*5



def read_particles( filepath ):
    particles = list()
    with open( filepath, 'rb' ) as fi:
        header = fi.read( HEADER_SIZE )
        while True:
            try:
                pset = unpack( STRUCTURE, fi.read( SIZE_OF_STRUCTURE ) )
                p = particle_io( *pset )
                particles.append( p )
                #p.printinfo()
            except:
                break
    print("{:d}-particle were loaded".format(len(particles)))
    return particles


def write_particles( filepath, particle_io_list ):
    counter = 0
    with open( filepath, 'wb' ) as fo:
        # header first
        NI__ = int(HEADER_SIZE/SIZE_OF_INT)
        PRM__ = [0]*NI__
        fo.write( pack('i'*NI__, *PRM__ ) )
        for p in particle_io_list:
            try:
                pset = p.get_paramlist()
                #print(pset)
                fo.write( pack( STRUCTURE, *pset ) )
                counter += 1
            except:
                print('exception was occurred')
                break
    print(f'{counter}-particles were successfully wrote')

def append_particles( filepath, particle_io_list ):
    try:
        existing_particles = read_particles( filepath )
    except:
        print('file reading error')
        return
    
    counter = 0
    with open( filepath, 'wb+' ) as fo:
        # header first
        NI__ = int(HEADER_SIZE/SIZE_OF_INT)
        PRM__ = [0]*NI__
        fo.write( pack('i'*NI__, *PRM__ ) )
        
        # body later: existing one : quick and dirty way
        for p in existing_particles:
            try:
                pset = p.get_paramlist()
                #print(pset)
                fo.write( pack( STRUCTURE, *pset ) )
            except:
                print('exception was occurred')
                break
        # body : appending one
        for p in particle_io_list:
            try:
                pset = p.get_paramlist()
                #print(pset)
                fo.write( pack( STRUCTURE, *pset ) )
                counter += 1
            except:
                print('exception was occurred')
                break
        print(f'{counter}-particles successfully appended')

class particle_io:
    def __init__(self, 
                 PDGpid = 0, 
                 charge = 0,
                 atomic_number = 0,
                 atomic_mass = 0,
                 phase_x = 0.,
                 phase_xp = 0.,
                 phase_y = 0.,
                 phase_yp = 0.,
                 kinetic_energy = 1.):
        self.pid = PDGpid
        self.Q = charge
        self.Z = atomic_number
        self.A = atomic_mass
        self.x = phase_x
        self.xp = phase_xp
        self.y = phase_y
        self.yp = phase_yp
        self.w = kinetic_energy
    
    def get_paramlist(self):
        return (int(self.pid),
                int(self.Q),
                int(self.Z),
                int(self.A),
                self.x,
                self.xp,
                self.y,
                self.yp,
                self.w)
    
    def printinfo(self):
        if self.pid == 0:
            pid = 'ion'
        elif self.pid == 11:
            pid = 'e-'
        elif self.pid == 22:
            pid = 'photon'
        else:
            pid = str(self.pid)
        msg = ""
        msg += f"pid={pid:s}, Q={self.Q:d}, Z={self.Z:d}, A={self.A:d}, "
        msg += f"x={self.x:.3f}, xp={self.xp:.3f}, y={self.y:.3f}, yp={self.yp:.3f}, "
        msg += f"w={self.w:.6f}"
        print(msg)
        

def select_particle(plist, pdgpid):
    selected_particles = list()
    for p in plist:
        if p.pid == pdgpid:
            selected_particles.append(p)
    return selected_particles




