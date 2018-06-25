#*************************************************************************#
#*									 *#
#*		Makefile for the C5.0 system				 *#
#*		----------------------------				 *#
#*									 *#
#*************************************************************************#

.PHONY: clean


CC	= gcc -g -ffloat-store
CFLAGS = -g -Wall -DVerbOpt -O0
LFLAGS = $(S) -static-libstdc++
#-static-libgcc -static-libstdc++
SHELL  = /bin/bash

# Debug (uncomment if desired)
CFLAGS += -DDEBUG


#	Definitions of file sets
#	New file ordering suggested by gprof

src =\
	global.c\
	c50.c\
	construct.c\
	formtree.c\
	info.c\
	discr.c\
	contin.c\
	subset.c\
	prune.c\
	p-thresh.c\
	trees.c\
	siftrules.c\
	ruletree.c\
	rules.c\
	getdata.c\
	implicitatt.c\
	mcost.c\
	confmat.c\
	sort.c\
	update.c\
	attwinnow.c\
	classify.c\
	formrules.c\
	getnames.c\
	modelfiles.c\
	utility.c\
	xval.c\
	debug.c

obj =\
	 c50.o global.o\
	 construct.o formtree.o info.o discr.o contin.o subset.o prune.o\
	 p-thresh.o trees.o\
	 formrules.o siftrules.o ruletree.o rules.o\
	 xval.o\
	 getnames.o getdata.o implicitatt.o\
	 mcost.o classify.o confmat.o sort.o\
	 update.o utility.o\
	 modelfiles.o\
	 attwinnow.o\
	 debug.o\

all:
	echo $(CC)
	make -C cpp_wrapper
	make c5.0dbg c5.0.dt_penalty c5.0.dt_entropy
	$(CC) $(LFLAGS) -o report report.c -lm


# debug version (including verbosity option)

c5.0dbg:\
	$(obj) defns.i extern.i text.i Makefile
	$(CC) $(CFLAGS) -g -o c5.0dbg $(obj) cpp_wrapper/cpp_wrapper.a -lm -lstdc++


# production version

c5.0.dt_penalty:\
	$(src) defns.i extern.i text.i Makefile
	$(CC) $(LFLAGS) -DPENALTY -O3 -o c5.0.dt_penalty $(src) cpp_wrapper/cpp_wrapper.a -lm -lstdc++
	strip c5.0.dt_penalty



c5.0.dt_entropy:\
	$(src) defns.i extern.i text.i Makefile
	$(CC) $(LFLAGS) -O3 -o c5.0.dt_entropy $(src) cpp_wrapper/cpp_wrapper.a -lm -lstdc++
	strip c5.0.dt_entropy

$(obj):		Makefile defns.i extern.i text.i


.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	make -C cpp_wrapper clean
	-rm -rf *.o c5.0.dt_penalty c5.0.dt_entropy c5.0dbg  report
