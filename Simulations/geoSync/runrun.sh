#!/bin/bash

while true; do
    #./run.sh -i ./industry.ini -c ASYNCTSTP -l "Scenario check5: ASYNCTSTP industry.ini"
    #sleep 300
    #./office_deployment.sh TWIST
    #./run.sh -i ./office.ini -c ASYNCTSTP -l "Scenario check5: ASYNCTSTP office.ini TWIST"
    #sleep 300
    ./office_deployment.sh LISHA
    ./run.sh -i ./office.ini -c ASYNCTSTP -l "Scenario check5: ASYNCTSTP office.ini LISHA"
    sleep 300
    ./run.sh -i ./geoMonitoring.ini -c ASYNCTSTP -l "Scenario check5: ASYNCTSTP geoMonitoring.ini"
    sleep 300
    ./office_deployment.sh SSB
    ./run.sh -i ./office.ini -c ASYNCTSTP -l "Scenario check5: ASYNCTSTP office.ini SSB"
    sleep 600
done
