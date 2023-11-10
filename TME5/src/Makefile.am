# noinst = no install, on peut utiliser bin_PROGRAMS sinon
noinst_PROGRAMS = tme5

INC_DIR = $(top_builddir)/include
SRC_DIR = $(top_builddir)/src

# Flags to compiler
AM_CPPFLAGS =  -I $(INC_DIR) -g -Wall -std=c++1y

# global LDFLAG  = Linker flags
AM_LDFLAGS = -pthread

# version locale a une cible
#cible_LDFLAGS = -pthread 

tme5_SOURCES = Color.cpp Color.h main.cpp Rayon.h Scene.h Sphere.h Vec3D.cpp Vec3D.h
 







