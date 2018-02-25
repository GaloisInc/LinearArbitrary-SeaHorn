#!/bin/sh
build/run/bin/clang -c -emit-llvm -D__SEAHORN__ -fgnu89-inline -m32 -g -Ibuild/run/include/ -o test.bc $2;
build/run/bin/seapp -o test.pp.bc --horn-inline-all --strip-extern=true --devirt-functions --externalize-addr-taken-funcs --kill-vaarg=true test.bc;
build/run/bin/seapp -o test.pp.ms.bc --horn-mixed-sem --ms-reduce-main test.pp.bc;
build/run/bin/seaopt -f -funit-at-a-time -o test.pp.ms.o.bc -O3 --disable-loop-vectorization=true --disable-slp-vectorization=true --vectorize-slp-aggressive=false test.pp.ms.bc;
build/run/bin/seahorn --keep-shadows=true --horn-solve -horn-inter-proc -horn-sem-lvl=ptr --horn-step=large -log none test.pp.ms.o.bc --horn-global-constraints=true --horn-stats --horn-singleton-aliases=true --horn-pdr-contexts=600 --horn-ignore-calloc=false --horn-make-undef-warning-error=false --horn-answer $1;
