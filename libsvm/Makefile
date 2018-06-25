CXX ?= g++
CFLAGS = -Wall -O3 -fPIC -Wno-c++11-extensions
SHVER = 2
OS = $(shell uname)

all: svm-invgen svm-train

lib: svm.o
	if [ "$(OS)" = "Darwin" ]; then \
		SHARED_LIB_FLAG="-dynamiclib -Wl,-install_name,libsvm.so.$(SHVER)"; \
	else \
		SHARED_LIB_FLAG="-shared -Wl,-soname,libsvm.so.$(SHVER)"; \
	fi; \
	$(CXX) $${SHARED_LIB_FLAG} svm.o -o libsvm.so.$(SHVER)

#svm-predict: svm-predict.c svm.o
#	$(CXX) $(CFLAGS) svm-predict.c svm.o -o svm-predict -lm
svm-train: svm-train.c svm.o
	$(CXX) $(CFLAGS) svm-train.c svm.o -o svm-train -lm
#svm-scale: svm-scale.c
#	$(CXX) $(CFLAGS) svm-scale.c -o svm-scale
svm.o: svm.cpp svm.h
	$(CXX) $(CFLAGS) -c svm.cpp

svm-invgen: invgen/conjunctive_learner.cpp include/conjunctive_learner.h svm.o config.o classifier.o polynomial.o state.o
	$(CXX) $(CFLAGS) -I./ -Iinclude/ invgen/conjunctive_learner.cpp svm.o config.o classifier.o polynomial.o state.o -o svm-invgen -lm

classifier.o: invgen/classifier.cpp include/classifier.h polynomial.o
	$(CXX) $(CFLAGS) -I./ -Iinclude/ -c invgen/classifier.cpp

polynomial.o: invgen/polynomial.cpp include/polynomial.h
	$(CXX) $(CFLAGS) -I./ -Iinclude/ -c invgen/polynomial.cpp

config.o: invgen/config.cpp include/config.h
	$(CXX) $(CFLAGS) -Iinclude/ -c invgen/config.cpp

state.o: invgen/state.cpp include/states.h
	$(CXX) $(CFLAGS) -Iinclude/ -c invgen/state.cpp

#svm.o: svm.cpp svm.h
#	$(CXX) $(CFLAGS) -c svm.cpp 

clean:
	rm -f *~ config.o state.o polynomial.o classifier.o svm.o svm-train svm-predict svm-scale svm-invgen libsvm.so.$(SHVER)
