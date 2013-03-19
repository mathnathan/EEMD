DEBUG = -O0 -g -fbacktrace

eemd: cppcall.cpp obj/eemd.o obj/gasdev.o obj/standev.o obj/natural_spline.o obj/min_max.o
	g++ -g -o cppeemd cppcall.cpp obj/eemd.o obj/gasdev.o obj/standev.o obj/natural_spline.o obj/min_max.o
    
obj/standev.o: standev.f90
	gfortran $(DEBUG) -o obj/standev.o -c   standev.f90
obj/natural_spline.o: natural_spline.f90
	gfortran $(DEBUG) -o obj/natural_spline.o -c   natural_spline.f90
obj/min_max.o: min_max.f90
	gfortran $(DEBUG) -o obj/min_max.o -c   min_max.f90
obj/gasdev.o: gasdev.f90
	gfortran $(DEBUG) -o obj/gasdev.o -c   gasdev.f90    
obj/eemd.o: eemd.f90
	gfortran $(DEBUG) -o obj/eemd.o -c   eemd.f90
    
clean:
	rm obj/*
	rm cppeemd
    
