#include "seahorn/seahorn.h"

/*
dyn-140-169:seahorn hezhu$ ./build/run/bin/sea pf --horn-answer --horn-stats --log=none --horn-ice --horn-ice-svm-c-paramter=10 --horn-ice-c5-exec-path=../ICE-C5/C50/c5.0.dt_entropy --horn-ice-svm-exec-path=../libsvm/svm-invgen --horn-ice-svm-coeff-bound=10 --horn-rule-sample-length=1 --horn-ice-svm-call-freq-pos=0 --horn-ice-svm-call-freq-neg=30 --horn-ice-local-strengthening=1 test/depthk/mergesort.c
/Users/hezhu/Documents/Research/seahorn/build/run/bin/clang -c -emit-llvm -D__SEAHORN__ -fgnu89-inline -m32 -I./build/run/include -o /var/folders/5t/f5ghv51132gbf1sgmwb7n5zw0000gp/T/sea-49RvKi/mergesort.bc test/depthk/mergesort.c
test/depthk/mergesort.c:15:1: warning: return type of 'main' is not 'int' [-Wmain-return-type]
void main()
^
test/depthk/mergesort.c:15:1: note: change return type to 'int'
void main()
^~~~
int
test/depthk/mergesort.c:21:7: warning: implicit declaration of function 'unknown' is invalid in C99 [-Wimplicit-function-declaration]
  n = unknown();
      ^
2 warnings generated.
/Users/hezhu/Documents/Research/seahorn/build/run/bin/seapp -o /var/folders/5t/f5ghv51132gbf1sgmwb7n5zw0000gp/T/sea-49RvKi/mergesort.pp.bc --strip-extern=false --kill-vaarg=true -log none /var/folders/5t/f5ghv51132gbf1sgmwb7n5zw0000gp/T/sea-49RvKi/mergesort.bc
/Users/hezhu/Documents/Research/seahorn/build/run/bin/seapp -o /var/folders/5t/f5ghv51132gbf1sgmwb7n5zw0000gp/T/sea-49RvKi/mergesort.pp.ms.bc --horn-mixed-sem --ms-reduce-main /var/folders/5t/f5ghv51132gbf1sgmwb7n5zw0000gp/T/sea-49RvKi/mergesort.pp.bc
/Users/hezhu/Documents/Research/seahorn/build/run/bin/seaopt -f -funit-at-a-time -o /var/folders/5t/f5ghv51132gbf1sgmwb7n5zw0000gp/T/sea-49RvKi/mergesort.pp.ms.o.bc -O3 --enable-indvar=false --enable-loop-idiom=false --enable-nondet-init=false --unroll-threshold=150 --disable-loop-vectorization=true --disable-slp-vectorization=true --vectorize-slp-aggressive=false /var/folders/5t/f5ghv51132gbf1sgmwb7n5zw0000gp/T/sea-49RvKi/mergesort.pp.ms.bc
/Users/hezhu/Documents/Research/seahorn/build/run/bin/seahorn --keep-shadows=true --horn-solve -horn-inter-proc -horn-sem-lvl=mem --horn-step=large -log none /var/folders/5t/f5ghv51132gbf1sgmwb7n5zw0000gp/T/sea-49RvKi/mergesort.pp.ms.o.bc --horn-answer --horn-stats --horn-ice --horn-ice-svm-c-paramter=10 --horn-ice-c5-exec-path=../ICE-C5/C50/c5.0.dt_entropy --horn-ice-svm-exec-path=../libsvm/svm-invgen --horn-ice-svm-coeff-bound=10 --horn-rule-sample-length=1 --horn-ice-svm-call-freq-pos=0 --horn-ice-svm-call-freq-neg=30 --horn-ice-local-strengthening=1
************** CHCs Solved in 2.537780e+03 (secs) **************

************** Program is correct **************
REL: (verifier.error V_0 V_1 V_2) -- invariant size: 1, 2, 
REL: (main@entry V_0) -- invariant size: 1, 
REL: (main@.loopexit3 V_0 V_1 V_2 V_3) -- invariant size: 2, 
REL: (main@_bb V_0 V_1 V_2 V_3 V_4 V_5 V_6) -- invariant size: 2, 
REL: (main@.lr.ph V_0 V_1 V_2 V_3 V_4 V_5 V_6 V_7 V_8 V_9 V_10) -- invariant size: 2, 
REL: main@verifier.error.split -- invariant size: 1, 
************** Program Correctness End **************

************** Learning Statistics **************:
Total CHC size: 16
Total Relation size: 6
Total Var size: 66
Neg sample size: 372
Pos sample size: 99
Total sample size: 471
Iteration number: 35769
************** Learning Statistics End **************

unsat
Function: main
main@entry: true
main@.loopexit3:
    (!(main@%x.0.i<=0))
  (!(((1*main@%_1)+(-1*main@%_2))<=-2))
  (((1*main@%_1)+(-1*main@%_2))<=-1)
main@_bb:
    ([&&
  true
  ((1*main@%x.0.i)+(1*main@%_2))<=5
  !(main@%x.0.i<=0)
  main@%x.0.i<=1
  ((1*main@%_4)+(-1*main@%z.0.i11))<=1
  !(main@%z.0.i11<=0)
  ((1*main@%_1)+(-1*main@%_2))<=0
]||[&&
  true
  !(((1*main@%x.0.i)+(1*main@%_2))<=5)
  !(main@%z.0.i11<=0)
  !(((1*main@%_4)+(-1*main@%x.0.i))<=-1)
])
  (!(((1*main@%_1)+(-1*main@%_7))<=-1))
  ([&&
  true
  ((1*main@%x.0.i)+(1*main@%_2))<=5
  !(main@%x.0.i<=0)
  main@%x.0.i<=1
  ((1*main@%_7)+(-1*main@%z.0.i11))<=1
  ((1*main@%_1)+(-1*main@%_2))<=-1
  !(((1*main@%z.0.i11)+(1*main@%_2))<=3)
]||[&&
  true
  !(((1*main@%x.0.i)+(1*main@%_2))<=5)
  !(((1*main@%_4)+(-1*main@%x.0.i))<=0)
  ((1*main@%_1)+(-1*main@%_2))<=-1
  ((1*main@%x.0.i)+(-1*main@%_7))<=-1
])
  (!(((1*main@%_1)+(-1*main@%_2))<=-2))
  ([&&
  true
  ((1*main@%x.0.i)+(1*main@%_2))<=5
  !(main@%x.0.i<=0)
  !(((1*main@%_4)+(1*main@%_2))<=4)
]||[&&
  true
  !(((1*main@%x.0.i)+(1*main@%_2))<=5)
  ((1*main@%_1)+(-1*main@%_2))<=-1
  !(((1*main@%_4)+(-1*main@%x.0.i))<=-1)
])
  (!(main@%x.0.i<=0))
  (!(((1*main@%_1)+(-1*main@%z.0.i11))<=-1))
main@.lr.ph:
    ([&&
  true
  ((1*main@%_1)+(-1*main@%k.0.i7))<=1
  ((1*main@%_7)+(-1*main@%i.0.i8))<=1
  !(((1*main@%j.0.i6)+(-1*main@%..i))<=-2)
  ((1*main@%_1)+(-1*main@%_2))<=0
]||(true&&(!(((1*main@%_1)+(-1*main@%k.0.i7))<=1))))
  (!(((1*main@%_1)+(-1*main@%k.0.i7))<=0))
  (((1*main@%_4)+(-1*main@%_8))<=0)
  (!([+
    1*main@%_1
    -1*main@%_7
    1*main@%j.0.i6
    1*main@%i.0.i8
    -1*main@%k.0.i7
    -1*main@%..i
  ]<=-2))
  ([&&
  true
  ((1*main@%..i)+(1*main@%_2))<=11
  !(main@%..i<=2)
  !(((1*main@%i.0.i8)+(1*main@%k.0.i7))<=1)
  !([+
      4*main@%_4
      1*main@%_1
      -1*main@%_2
    ]<=6)
  ((1*main@%_4)+(-1*main@%_8))<=-1
]||[&&
  true
  !(((1*main@%..i)+(1*main@%_2))<=11)
  !(main@%_4<=1)
  ((1*main@%j.0.i6)+(-1*main@%..i))<=-1
  ((1*main@%_1)+(-1*main@%_2))<=0
])
  (!(((1*main@%x.0.i)+(1*main@%i.0.i8))<=1))
  ([&&
  true
  ((1*main@%..i)+(1*main@%_2))<=11
  !(main@%..i<=2)
  !(((1*main@%i.0.i8)+(1*main@%k.0.i7))<=1)
  !(((1*main@%_8)+(-1*main@%_7))<=0)
  !(((1*main@%_1)+(-1*main@%_2))<=-2)
  !(main@%x.0.i<=0)
]||[&&
  true
  !(((1*main@%..i)+(1*main@%_2))<=11)
  !(main@%x.0.i<=0)
  !(((1*main@%_1)+(-1*main@%_2))<=-2)
])
  (((1*main@%_1)+(-1*main@%_2))<=-1)
  (((1*main@%x.0.i)+(-1*main@%..i))<=-2)
  (!(((1*main@%_4)+(-1*main@%x.0.i))<=0))
main@verifier.error.split: false


************** BRUNCH STATS ***************** 
BRUNCH_STAT Result TRUE
BRUNCH_STAT Horn 0.02
BRUNCH_STAT HornClauseDB::loadZFixedPoint 0.00
BRUNCH_STAT HornifyModule 0.00
BRUNCH_STAT ICE inv 750.44
BRUNCH_STAT LargeHornifyFunction 0.00
BRUNCH_STAT seahorn_total 750.60
************** BRUNCH STATS END ***************** 
*/

// This is an iterative version of merge sort.
// It merges pairs of two consecutive lists one after another.
// After scanning the whole array to do the above job,
// it goes to the next stage. Variable k controls the size
// of lists to be merged. k doubles each time the main loop
// is executed.

//#include <stdio.h>
//#include <math.h>

//int a[100000],b[100000];

void main()
{ 
  int i,n,t,k;
  int l,r,u,j;
  int x,y,z;

  n = unknown();

  x=1;
  while (x<n) {
    z=1;
    while (z+x<=n) {
      y=z+x*2;
      if (y>n) y=n+1;
      //      merge(z,z+x,y);
      l = z; r = z+x; u = y;
      i=l; j=r; k=l;
      while (i<r && j<u) { 
	//	assert(0<=i);assert(i<=n);
	//assert(0<=j);assert(j<=n);
	if(unknown()) {
	//if (a[i]<=a[j]) {
	  //assert(0<=i);assert(i<=n);
	  //assert(0<=k);assert(k<=n);
	  //b[k]=a[i]; 
	  i++;
	} 
	else {
	  //assert(0<=j);assert(j<=n);
	  //assert(0<=k);assert(k<=n);	  
	  //b[k]=a[j]; 
	  j++;
	}
	k++;
      }
      //assert(0<=r);assert(r<=n);
      
      sassert(k<=n);
      
      while (i<r) {
	//assert(0<=i);assert(i<=n);
	//assert(0<=k);assert(k<=n);
	//b[k]=a[i]; 
	i++; 
	k++;
      }
      while (j<u) { 
	//assert(0<=j);assert(j<=n);
	//assert(0<=k);assert(k<=n);
	//b[k]=a[j]; 
	j++; k++;
      }
      for (k=l; k<u; k++) { 
	//assert(0<=k);assert(k<=n);
	//a[k]=b[k]; 
      }
      
      z=z+x*2;
    }
    x=x*2;
  }
}

/*
main()
{ printf("input size \n");
  scanf("%d",&n); 
  for (i=1;i<=n;i++) a[i]=random()%1000;
  t=clock();
  sort1();
  for (i=1;i<=10;i++) printf("%d ",a[i]);
  printf("\n");
  printf("time= %d millisec\n",(clock()-t)/1000);
}
*/