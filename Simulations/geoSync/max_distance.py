# Reads an Omnet++ deployment map and prints the distance between the most far away node and its closest neighbor

import sys
import math
import re

if len(sys.argv) < 1:
    print("ERROR! Usage:", sys.argv[0], "ini_file")
    quit()

coords = [[False, 0, 0, 0] for i in range(1000)]
fn = sys.argv[1]
with open(fn, 'r') as f:
    for line in f:
        if line[0] != '#':
            if 'xCoor' in line:
                node = int(re.findall(r'[-+]?\d+', line.split()[0])[0])
                coords[node][0] = True
                coords[node][1] = float(re.findall(r"[-+]?\d*\.\d+|\d+", line.split()[2])[0])
            elif 'yCoor' in line:
                node = int(re.findall(r'[-+]?\d+', line.split()[0])[0])
                coords[node][0] = True
                coords[node][2] = float(re.findall(r"[-+]?\d*\.\d+|\d+", line.split()[2])[0])
            elif 'zCoor' in line:
                node = int(re.findall(r'[-+]?\d+', line.split()[0])[0])
                coords[node][0] = True
                coords[node][3] = float(re.findall(r"[-+]?\d*\.\d+|\d+", line.split()[2])[0])

def dist(x,y,z,x2,y2,z2):
    return math.sqrt((x-x2)**2 + (y-y2)**2 + (z-z2)**2)

maxdist = 0
for c in coords:
    if c[0]:
        mindist = 1000000000000
        for c2 in coords:
            if c2[0]:
                d = dist(c[1], c[2], c[3], c2[1], c2[2], c2[3])
                if d > 0 and d < mindist:
                    mindist = d
        if mindist > maxdist:
            maxdist = mindist

print(maxdist)
