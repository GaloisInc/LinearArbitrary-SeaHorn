
<table>
  <tr>
    <th>Windows</th><th>Ubuntu</th><th>OS X</th>
  </tr>
    <td>TBD</td>
    <td>Build</td>
    <td>Build</td>
  </tr>
</table>

# About #

<a href="">LinearArbitrary-SeaHorn</a> is a CHC solver for LLVM-based languages.

# License #

<a href="">LinearArbitrary-SeaHorn</a> is distributed under [license.txt](license.txt).

# LinearArbitrary-SeaHorn Dependencies #

* LLVM 3.6 (the build script can automatically install it)
* Z3 4.4.0 (the build script can automatically install it)
* Boost 1.63 (brew installation available on Mac os x)
* libncurses (brew installation available on Mac os x)
* libgmp (brew installation available on Mac os x)


# LinearArbitrary-SeaHorn Installation #

* `cd seahorn ; mkdir build ; cd build`
* `cmake -DCMAKE_INSTALL_PREFIX=run ../ `
* `cmake --build .` to build dependencies (Z3 and LLVM)
* `cmake --build . --target extra` to download extra packages
* `cd ../llvm-seahorn/ && git reset --hard 39aa187 && cd ../llvm-dsa/ && git reset --hard fedb3e3 && cd ../sea-dsa/ && git reset --hard 246f0f5 && cd ../crab-llvm/ && git reset --hard e2fac87 && cd ../build/ && make ..` to configure extra packages
* `cmake --build . --target crab && cmake ..` to configure crab-llvm
* `cmake --build . --target install` to build LinearArbitrary-SeaHorn and install everything in `run` directory

LinearArbitrary-SeaHorn and dependencies are installed in `build/run`

# Learning Libraries Installation #

LinearArbitrary-SeaHorn does not explicitly link to the SVM and DT learning libraries.
The interaction is currently handled by IO. This is a severe performance burden and will be
improved in the future.

LinearArbitrary-SeaHorn assumes that the learning libraries, libsvm and ICE-C5, are in 
parallel to the `seahorn` directory. Installation steps are:

* `cd libsvm; make clean; make`
* `cd ICE-C5/C50/; make clean; make` 

# Usage #

LinearArbitrary-SeaHorn provides a python script called `run.py` to interact with
users. Given a C program annotated with assertions, users just need to
type: `python run.py file.c` Given a directory of C programs, users just need to
type: `python run.py dir`

To get detailed results, users can type: `python run.py screen file.c`

For each program, LinearArbitrary-SeaHorn outputs invariants if all assertions hold or 
otherwise a counterexample if any of the assertions is violated. 

LinearArbitrary-SeaHorn translates `file.c` into LLVM bitcode,
generates a set of verification conditions (VCs), and
finally, solve them. The main back-end solver
is Z3 + libsvm + C5.0_decision_tree.

`run.py` is essentially a wrapper to `seahorn/build/run/bin/sea` which calls the
seahorn verification tool on `file.c`. The `sea` verification engine accepts
several options to configure verification. Tracking how to use these options
is interesting but `run.py` comes with a default configuration and also automates
the steps to reproduce the experimental result of the paper.

- `PIE benchmarks (Fig 8(a))`

  python run.py test/c/pie

    (Our result: Among the total 117 benchmarks LinearArbitrary successfully verified 117.)

- `DIG benchmarks (Fig 8(b))`

  python run.py test/c/pie/hola

    (Our result: Among the total 46 benchmarks LinearArbitrary successfully verified 46.)

- `Recursive benchmarks (Fig 8(c))`

  python run.py test/c/recursions

    (Our result: Among the total 101 benchmarks LinearArbitrary successfully verified 91.)

- `C Recursive+Loop benchmarks (Fig 8(d))`

  python run.py test/c/

    (Our result: Among the total 357 benchmarks LinearArbitrary successfully verified 345.)

- `SVComp benchmarks (Fig 9)` 

  python run.py test/sv-benchmarks/

    (Our result: Among the total 795 benchmarks LinearArbitrary successfully verified 765.)

- `Other interesting benchmarks`

  python run.py test/demo

    (Our result: Among the total 26 benchmarks LinearArbitrary successfully verified 26.)

  python run.py test/dagger

    (Our result: Among the total 25 benchmarks LinearArbitrary successfully verified 25.)

- Users are recommended to access to `test_logs` directory to find the details of our
own experimental results.  

The above arguments to `run.py` refer to directories. All .c files under the given directories
are verified. The result of whether a verification task is successful is displayed in the
command-lines window. Additionally, invariants learned to prove a program or a counterexample 
generated to disprove a program, for each verification task, are available in the log file 
`./result.log`. Users can access to this file to get more insight of LinearArbitrary-SeaHorn.
Alternatively one can use `python run.py screen test/demo` to get the tool's output directly
displayed in the command-lines window.

In case users are interested in knowing what options are passed to LinearArbitrary-SeaHorn
for verification in `run.py`, `result.log` is a good place for such information. We explain 
these options in `run.py` and below.

`--horn-answer`
  show learned invariants in the output

`--horn-stats`
  show some stats about learning and verification

`--horn-ice`
  use inductive counterexample guided learning to solve a verification task

`--horn-ice-svm-c-paramter={int}`
  adjust the C parameter of SVM learning (see sec 3.1 and sec 3.2 of the paper)

`--horn-ice-c5-exec-path={str}`
  specify where to find the decision tree learning library

`--horn-ice-svm-exec-path={str}`
  specify where to find the libsvm library

`--horn-ice-mod={1,0}`
  allow learning to use mod(%) operation of a numeric variable against a constant as a 
  feature (see line 857-871 of the paper).

`--horn-ice-svm-coeff-bound={int}` 
  Z3 sometimes times-out on a formula with very large coefficients. Setting this parameter can
  avoid considering such candidate invariants.

`--horn-rule-sample-length={int}`
  When the verifier finds a positive sample, using this parameter allows the system to use 
  bounded DFS on CHCs to generate more related positive samples to accelerate verification.

`--horn-rule-sample-width={int}`
  When the verifier finds a positive sample, using this parameter allows the system to use 
  bounded BFS on CHCs to generate more related positive samples to accelerate verification. 

`--horn-ice-svm-call-freq-pos={int}`
`--horn-ice-svm-call-freq-neg={int}`
  This is an optimization implemented in the tool. The learning algorithm is based on the
  interaction between svm and decision tree (DT) learning. Since svm provides attributes 
  to construct DT, if svm is called everytime a counterexample comes, DT construction 
  may diverge since attributes change too fast. If this parameter is set to `n`, svm is 
  called every `n` samples. The idea is like using target/current networks used in DQN learning. 

`--horn-ice-local-strengthening={1,0}`
  This is an optimization implemented in the tool. It can greatly improve the tool's
  performance on some benchmarks. For a CHC, p1(x) /\ ... -> p2(x) where p1 = p2, this
  optimization only updates the solution of p1 during CEGAR iterations and p2 is only
  updated to the solution of p1 when the new p1 solution can imply the old p2 solution.


## Example ##

This is an example of a C program annotated with a safety property:
``` c
    /* verification command: python run.py test.c */
    #include "seahorn/seahorn.h"

    int main(void){
      int k=1;
      int i=1;
      int j=0;
      int n = unknown();
      while(i<n) {
        j=0;
        while(j<i) {
          k += (i-j);
          j++;
        }
        i++;
      }
      sassert(k>=n);
    }

```
LinearArbitrary-SeaHorn assumes all variables are initialized. Nondeterministic variables
can be initialized with an unknown function.
LinearArbitrary-SeaHorn follows [SV-COMP](http://sv-comp.sosy-lab.org) convention of
encoding error locations by a call to the designated error function
`__VERIFIER_error()`. `sassert()` method is defined in `seahorn/seahorn.h`.

The solver returns invariants when `__VERIFIER_error()` is unreachable, and the program 
is considered safe. It returns counterexamples when `__VERIFIER_error()` is reachable 
and the program is unsafe. Such information can be found in `result.log`. 

Alternatively, if running using `python run.py screen test.c`, the following output
can be directly found in the command-line window:

```
************** CHCs Solved in 4.492560e-01 (secs) **************

************** Program is correct **************
REL: (verifier.error V_0 V_1 V_2) -- invariant size: 1, 2, 
REL: (main@entry V_0) -- invariant size: 1, 
REL: (main@.preheader V_0 V_1 V_2) -- invariant size: 2, 
REL: (main@.lr.ph V_0 V_1 V_2 V_3) -- invariant size: 3, 
REL: main@verifier.error.split -- invariant size: 1, 
************** Program Correctness End **************

************** Learning Statistics **************:
Total CHC size: 13
Total Relation size: 5
Total Var size: 44
Neg sample size: 11
Pos sample size: 2
Total sample size: 13
Iteration number: 15
************** Learning Statistics End **************

unsat
Function: main
main@entry: true
main@.preheader: (!(main@%k.0.i5<=0))
main@.lr.ph:
    (!(main@%i.0.i3<=0))
  (!(((1*main@%k.1.i1)+(-1*main@%j.0.i2))<=0))
main@verifier.error.split: false


************** BRUNCH STATS ***************** 
BRUNCH_STAT Result TRUE
BRUNCH_STAT Horn 0.01
BRUNCH_STAT HornClauseDB::loadZFixedPoint 0.00
BRUNCH_STAT HornifyModule 0.00
BRUNCH_STAT ICE inv 0.35
BRUNCH_STAT LargeHornifyFunction 0.00
BRUNCH_STAT seahorn_total 0.37
************** BRUNCH STATS END ***************** 
Success!
************************************************************************************

Among the total 1 benchmarks LinearArbitrary successfully verified 1.
```

The first line of the output gives the CHC solving time while the last line of the
output only estimates the time spent within seahorn not counting learning time.
If a program is correct, the tool first outputs the size of each invariant learned
for a CHC unknown (see line 1114 of the paper). It then outputs some information
about CHC system size and number of positive and negative samples and CEGAR iterations
used to derive the solution. After that, the tool prints the learned invariants
as shown above followed with some brunch stats.


# Appreciation #

To people who developed the SeaHorn verification framework:
* [Arie Gurfinkel](arieg.bitbucket.org)
* [Jorge Navas](http://jorgenavas.github.io/)
* [Temesghen Kahsai](http://www.lememta.info/)

To people who developed an initial implementation of the ICE algorithm inside SeaHorn:
* [Chenguang Zhu](http://cozy.ece.utexas.edu/~cgzhu/)
