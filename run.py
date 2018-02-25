import os, sys, subprocess, os.path, multiprocessing, time

from threading import Timer

#path   = os.path.dirname(sys.argv[0])
solve  = "./build/run/bin/sea" #% (path)
solveSVComp = "./svcomp.command"

def setupExperiments (path, flags):
  arguments = flags[:]
  arguments.extend(["--horn-answer", "--horn-stats", "--log=none", "--horn-ice", "--horn-ice-svm-c-paramter=10"])
  # Call the solvers on the PLDI benchmarks
  if (path.find("hola") != -1):
    return set_hola_Experiments(path, arguments)
  elif (path.find ("dillig_tacas13") != -1):
    return set_dillig_TACAS13_Experiments(path, arguments)
  elif (path.find("ICE") != -1):
    return set_ICE_Experiments(path, arguments)
  elif (path.find("invgen") != -1):
    return set_invgen_Experiments(path, arguments)
  elif (path.find("recursive") != -1):
    return set_recursvie_Experiments(path, arguments)
  elif (path.find("sharma_splitter") != -1):
    return set_sharma_splitter_Experiments(path, arguments)
  elif (path.find("sv-benchmarks") != -1):
    del arguments[:]
    arguments.extend(["--horn-ice", "--horn-ice-svm-c-paramter=10"])
    return set_SVComp_Experiments(path, arguments)
  else:
    return set_default_Experiments(arguments)

def set_hola_Experiments(path, arguments):
  arguments.extend(["--horn-ice-c5-exec-path=../ICE-C5/C50/c5.0.dt_entropy", "--horn-ice-svm-exec-path=../libsvm/svm-invgen"])
  arguments.extend(["--horn-ice-svm-coeff-bound=10"])
  arguments.extend(["--horn-ice-svm-call-freq-pos=0", "--horn-ice-svm-call-freq-neg=30"])
  # Benchmarks need invariants involving mod (%) operation 
  if (path.find("02") != -1 or path.find("36") != -1 or path.find("40") != -1 or path.find("42") != -1):
    arguments.extend(["--horn-ice-mod=1"])
  # Optimization in the implementation: Sample more positive data each time by unrolling CHCs.
  if (path.find("11") != -1 or path.find("18") != -1 or path.find("32") != -1):
    # for loops with constant loop bound, best performance achieved when unrolling CHCs
    # the number of times equal to the constant bounds.
    arguments.extend(["--horn-rule-sample-length=100"])  
  else:
    # for infinite loops, unroll CHCs with just a tiny step is totoally OK.
    arguments.extend(["--horn-rule-sample-length=1"])
  return arguments

def set_dillig_TACAS13_Experiments(path, arguments):
  arguments.extend(["--horn-ice-c5-exec-path=../ICE-C5/C50/c5.0.dt_entropy", "--horn-ice-svm-exec-path=../libsvm/svm-invgen"])
  arguments.extend(["--horn-ice-svm-coeff-bound=5", "--horn-rule-sample-length=1"])
  arguments.extend(["--horn-ice-svm-call-freq-pos=0", "--horn-ice-svm-call-freq-neg=10"])
  arguments.extend(["--horn-ice-local-strengthening=1"])
  # benchmarks need invariants involving mod (%) operation 
  if (path.find("benchmark2") != -1 or path.find("benchmark5") != -1 or path.find("benchmark6") != -1 or path.find("benchmark7") != -1):
    arguments.extend(["--horn-ice-mod=1"])
  return arguments

def set_ICE_Experiments(path, arguments):
  arguments.extend(["--horn-ice-c5-exec-path=../ICE-C5/C50/c5.0.dt_entropy", "--horn-ice-svm-exec-path=../libsvm/svm-invgen"])
  arguments.extend(["--horn-ice-svm-coeff-bound=5", "--horn-rule-sample-length=1"])
  arguments.extend(["--horn-ice-svm-call-freq-pos=0", "--horn-ice-svm-call-freq-neg=10"])
  return arguments

def set_invgen_Experiments(path, arguments):
  arguments.extend(["--horn-ice-c5-exec-path=../ICE-C5/C50/c5.0.dt_entropy", "--horn-ice-svm-exec-path=../libsvm/svm-invgen"])
  arguments.extend(["--horn-ice-svm-coeff-bound=5"])
  # For programs with multiple loops, it is a good idea to unroll CHCs a small nubmer of steps to accerlate
  # sampling positive data. For instance, the mergesort aglorithm.
  if (path.find("nested") != -1):
    arguments.extend(["--horn-rule-sample-length=10"])
  elif (path.find("mergesort") != -1): 
    arguments.extend(["--horn-rule-sample-length=10", "--horn-ice-local-strengthening=1"])
  elif (path.find("seq-len") != -1):
    arguments.extend(["--horn-rule-sample-length=10", "--horn-ice-svm-call-freq-pos=0", "--horn-ice-svm-call-freq-neg=30"])
  elif (path.find("split") != -1):
    arguments.extend(["--horn-rule-sample-length=100", "--horn-ice-svm-call-freq-pos=0", "--horn-ice-svm-call-freq-neg=30"])
  else:
    arguments.extend(["--horn-rule-sample-length=1", "--horn-ice-svm-call-freq-pos=0", "--horn-ice-svm-call-freq-neg=30"])
  return arguments

def set_recursvie_Experiments(path, arguments):
  arguments.extend(["--horn-ice-c5-exec-path=../ICE-C5/C50/c5.0.dt_entropy"])
  arguments.extend(["--horn-ice-svm-coeff-bound=5"])
  arguments.extend(["--horn-ice-svm-call-freq-pos=0", "--horn-ice-svm-call-freq-neg=10"])
  if (path.find("Add") != -1 or path.find("sum") != -1):
    arguments.extend(["--horn-ice-svm-exec-path=../libsvm/svm-invgen"])
  if (path.find("EvenOdd") != -1):
    arguments.extend(["--horn-ice-mod=1"])
  if ((path.find("fibo") != -1 or path.find("Fibo") != -1) and path.find("false") == -1):
    arguments.extend(["--horn-ice-local-strengthening=1"])
  if (path.find("100") != -1):
    arguments.extend(["--horn-rule-sample-length=100"])
  elif (path.find("200") != -1):
    arguments.extend(["--horn-rule-sample-length=200"])
  else:
    arguments.extend(["--horn-rule-sample-length=1"])
  return arguments

def set_sharma_splitter_Experiments(path, arguments):
  arguments.extend(["--horn-ice-c5-exec-path=../ICE-C5/C50/c5.0.dt_entropy", "--horn-ice-svm-exec-path=../libsvm/svm-invgen"])
  arguments.extend(["--horn-ice-svm-coeff-bound=5", "--horn-rule-sample-length=100"])
  arguments.extend(["--horn-ice-svm-call-freq-pos=0", "--horn-ice-svm-call-freq-neg=30"])
  return arguments 

def set_SVComp_Experiments(path, arguments):
  arguments.extend(["--horn-ice-c5-exec-path=../ICE-C5/C50/c5.0.dt_entropy", "--horn-ice-svm-exec-path=../libsvm/svm-invgen"])
  arguments.extend(["--horn-ice-svm-coeff-bound=5", "--horn-rule-sample-length=10"])
  arguments.extend(["--horn-ice-svm-call-freq-pos=0", "--horn-ice-svm-call-freq-neg=20"])
  if (path.find("email_") != -1): arguments.extend(["-horn-rule-sample-width=2"])
  return arguments

def set_default_Experiments(arguments):
  arguments.extend(["--horn-ice-c5-exec-path=../ICE-C5/C50/c5.0.dt_entropy", "--horn-ice-svm-exec-path=../libsvm/svm-invgen"])
  arguments.extend(["--horn-ice-svm-coeff-bound=5", "--horn-rule-sample-length=1"])
  arguments.extend(["--horn-ice-svm-call-freq-pos=0", "--horn-ice-svm-call-freq-neg=30"])
  return arguments

#def setupExperiments (path, arguments):
#  arguments.extend (["--horn-answer", "--horn-stats"])
#  return arguments

def logged_sys_call(args, quiet):
  print "exec: " + " ".join(args)
  if quiet:
    out = open("result.log", "w")
  else:
    out = None
  kill = lambda process: process.kill()  
  seahorn = subprocess.Popen(args, stdout=out, stderr=None)
  timer = Timer(150, kill, [seahorn])

  try:
    timer.start()
    seahorn.communicate()    
  finally:
    timer.cancel()
    poll = seahorn.poll()
    if poll < 0:
      print("------------ Time-outs! ------------ ")
      os.system("killall -9 seahorn");
      child = subprocess.Popen(["pgrep seahorn"], stdout=subprocess.PIPE, shell=True)
      while True:
        result = child.communicate()[0]
        if result == "":
          break
  #return subprocess.call(args, stdout=out, stderr=None, timeout=5)


def solve_horn(file,quiet,flags):
  if (file.find ("sv-benchmarks") == -1):
    return logged_sys_call([solve, "pf"] + flags + [("%s" % file)], quiet)
  else:
    return logged_sys_call([solveSVComp] + [" ".join(flags)] + [("%s" % file)], quiet)

def run(quiet, flags):
  if len(flags) == 0:
    print ("Usage: %s [flags] [sourcefile]" % sys.argv[0])
    sys.exit(0)
  if "-help" in flags or "--help" in flags:
    os.system("%s -help" % (solve))
    return 0
  src   = flags[len(flags) - 1]
  flags = flags[:-1]
  for (root, dirs, files) in os.walk (src):
    for file in files:
      filename = os.path.join(root, file)
      if (not filename.endswith(".c")):
        continue
      print ("************************************************************************************")
      print ("Verifying benchmark: %s" % filename)
      solve_horn(filename, quiet, setupExperiments(filename, flags))
      print ("************************************************************************************\n")
  return 0

if __name__ == "__main__":
  print "LinearArbitrary: Copyright (c) 2018 Galois, Inc"
  sys.exit(run(False, sys.argv[1:]))