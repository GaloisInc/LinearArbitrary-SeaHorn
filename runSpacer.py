import os, sys, subprocess, os.path, multiprocessing, time

from threading import Timer

#path   = os.path.dirname(sys.argv[0])
solve  = "./build/run/bin/sea" #% (path)
solveSVComp = "./svcomp.command"
total = 0
success = 0

def setupExperiments (path, flags):
  arguments = flags[:]
  if (path.find("sv-benchmarks") != -1):
    return arguments
  else:
    arguments.extend(["--horn-answer", "--horn-stats", "--log=none", "--horn-pdr-engine=spacer"])
    return arguments

def logged_sys_call(args, quiet):
  print "exec: " + " ".join(args)
  if quiet:
    out = open("result.log", "a")
  else:
    out = None
  kill = lambda process: process.kill()  
  seahorn = subprocess.Popen(args, stdout=out, stderr=None)
  timer = Timer(150, kill, [seahorn])

  global total
  total = total + 1
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
    else:
      global success
      success = success + 1
      print("Success!")
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
  # Clear previous result file.
  open('result.log', 'w').close()
  # Clear previous implication files in case the user called Garg's ICE approach in the last run.
  open('FromCmd.implications', 'w').close()
  src   = flags[len(flags) - 1]
  flags = flags[:-1]
  if (src.endswith(".c")):
    header = open ('result.log', 'a')
    print ("************************************************************************************")
    print ("Verifying benchmark: %s" % src)
    header.write ("************************************************************************************\n")
    header.write ("Verifying benchmark: %s\n" % src)
    header.close()
    solve_horn(src, quiet, setupExperiments(src, flags))
    print ("************************************************************************************\n")
  else:
    for (root, dirs, files) in os.walk (src):
      for file in files:
        filename = os.path.join(root, file)
        if (not filename.endswith(".c")):
          continue
        header = open ('result.log', 'a')
        print ("************************************************************************************")
        print ("Verifying benchmark: %s" % filename)
        header.write ("************************************************************************************\n")
        header.write ("Verifying benchmark: %s\n" % filename)
        header.close()
        solve_horn(filename, quiet, setupExperiments(filename, flags))
        print ("************************************************************************************\n")
  print "Among the total %d benchmarks LinearArbitrary successfully verified %d.\n" % (total, success)
  print "Check result.log for analysis result.\n"
  return 0

if __name__ == "__main__":
  print "LinearArbitrary: Copyright (c) 2017-2018 Galois Inc. All rights reserved."
  if (len(sys.argv) <= 1):
    print ("Usage: %s [flags] [sourcefile]" % sys.argv[0])
    sys.exit(0)
  if sys.argv[1] == "screen":
    sys.exit(run(False, sys.argv[2:]))
  else:
    sys.exit(run(True, sys.argv[1:]))