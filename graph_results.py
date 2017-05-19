#
# This script graphs the output of the Brooks-Iyengar algorithm
#

import matplotlib.pyplot as plt
import os
import numpy as np
import re
import sys
import math

estimates = []
averages = []
folder = sys.argv[1]
flag = True
for filename in os.listdir(os.getcwd() + '/' + folder ):
  if re.match('.*nprocess.*',filename):
    continue
  lines = [line.rstrip('\n') for line in open( os.getcwd() + '/' + folder +'/' + filename)]
  for x in range(0, len(lines)):
    split = lines[x].split(',')
    avg = float(split[1])
    est = float(split[0])
    if math.isnan(est):
      est = avg

    if flag:
      estimates.append(est)
      averages.append(avg)
    else:
      estimates[x] += est
      averages[x] += avg

  flag = False

for n in range(0, len(estimates)):
  estimates[n] /= 5

for n in range(0, len(estimates)):
  averages[n] /= 5

t = np.linspace(0, (len(estimates)*.2), len(estimates))
analytical = []
for n in range(0, len(t)):
  analytical.append( 2*math.sin(math.pi*(n*.2)/2))

plt.plot(t, analytical, ':')
plt.plot(t, averages, 'r--')
plt.plot(t, estimates, 'g--')
plt.axhline(linewidth=1, color="k") 
plt.xlabel('t')
plt.ylabel('f(t)')
plt.title('Brooks Iyengar Results')
plt.grid(True)
plt.savefig('results.png')
plt.close()
