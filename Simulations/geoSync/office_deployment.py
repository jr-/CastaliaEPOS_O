# This script is called by office_deployment.sh

import sys
import math
import re
import random

if len(sys.argv) < 1:
    print("ERROR! Usage:", sys.argv[0], "number_of_nodes")
    quit()

nodes = int(sys.argv[1]) - 1

l0 = "300ms"
l1 = "1s"
l2 = "10s"
l3 = "1min"

n0 = int(0.1 * nodes)
n2 = int(0.6 * nodes)
n3 = int(0.1 * nodes)

n1 = nodes - (n0+n2+n3)

period = []
for i in range(n0):
    period.append(l0)
for i in range(n1):
    period.append(l1)
for i in range(n2):
    period.append(l2)
for i in range(n3):
    period.append(l3)

random.shuffle(period)

print("SN.node[0].Application.dataPeriod = 0s")

for i in range(len(period)):
    print("SN.node[",i+1,"].Application.dataPeriod = ", period[i] , sep='')
    if period[i] == l0 or period[i] == l3:
        expiry = l0
    else:
        expiry = period[i]
    print("SN.node[",i+1,"].Communication.Routing.expiry = ", expiry, sep='')

period2 = []
for i in range(len(period)):
    if period[i] == l3:
        print("SN.node[",i+1,"].ResourceManager.initialEnergy = 18720", sep='')
    elif period[i] == l2:
        period2.append(i)

random.shuffle(period2)
for i in range(len(period2) // 2):
    print("SN.node[",period2[i]+1,"].ResourceManager.initialEnergy = 18720", sep='')

print("SN.node[*].ResourceManager.initialEnergy = 999999999")
