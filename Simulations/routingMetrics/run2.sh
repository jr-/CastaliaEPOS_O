#!/bin/bash
while test -d "/proc/8931/"; do
    sleep 300
done

sleep 300

./run.sh ./omnetpp.ini "Routing metrics 1/5 traffic"
