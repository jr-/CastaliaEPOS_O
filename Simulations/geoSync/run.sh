#!/bin/bash

ARGS="$*"
MY_NAME="$0"
INI="omnetpp.ini"
BIN="../../castalia"
if [ ! -x "$BIN" ]; then
    BIN="../../CastaliaBin"
    if [ ! -x "$BIN" ]; then
        BIN="../../../castalia"
        if [ ! -x "$BIN" ]; then
            BIN="../../../CastaliaBin"
        fi
    fi
fi
LABEL="(not set)"
CONFIGS="TSTP"
IN_LABEL=0
IN_CONFIGS=0

while [[ $# -gt 0 ]]; do
    key="$1"

    case $key in
        -h|--help)
            echo "Usage: $MY_NAME [-i/--ini <.ini file>] [-b/--bin <Omnet/Castalia binary>] [-l/--label <Run label>] [-c/--configs <Configs to run>] [-h/--help]"
            exit 0
            ;;
        -i|--ini)
            INI="$2"
            shift
            ;;
        -b|--bin)
            BIN="$2"
            shift
            ;;
        -l|--label)
            LABEL="$2"
            IN_LABEL=1
            IN_CONFIGS=0
            shift
            ;;
        -c|--configs)
            CONFIGS="$2"
            IN_LABEL=0
            IN_CONFIGS=1
            shift
            ;;
        *)
            if [ $IN_LABEL == 1 ]; then
                LABEL="$LABEL $1"
            elif [ $IN_CONFIGS == 1 ]; then
                CONFIGS="$CONFIGS $1"
            fi
            ;;
    esac
    shift
done

if [ ! -d ../../../Castalia/ ]; then
    echo "Error! You should run this script from Castalia/Simulations/<simulation>/"
    exit 1
fi

echo "Run label: $LABEL"
echo ".ini file = $INI"
echo "Omnet/Castalia binary = $BIN"
echo "configs = $CONFIGS"

if [ ! -f "$INI" ]; then
    echo "Error! .ini file not found: $INI"
    exit 2
fi

if [ ! -x "$BIN" ]; then
    echo "Error! Castalia binary $BIN not executable!"
    exit 3
fi

RUN="run-$(date +%Y%m%d%H%M%S)"

echo "================================"
echo "= Run name: $RUN ="
echo "================================"


echo "================================"
echo "= Creating directory structure ="
echo "================================"
DIR="$HOME/simulation_results"
if [ ! -d "$DIR" ]; then
    mkdir "$DIR"
fi

DIR="$DIR"/$RUN
mkdir -v "$DIR"

SETUP="$DIR/setup"
mkdir -v "$SETUP"

PROCESSED_DATA="$DIR/processed_data"
mkdir -v "$PROCESSED_DATA"

mkdir -v "$DIR/results"

OUTPUTS="$DIR/outputs"
mkdir -v "$OUTPUTS"

echo "$LABEL" > "$DIR/label.txt"

TMP_RESULTS_DIR="results-pre-$RUN"

if [ -d results ]; then
    mv -v results $TMP_RESULTS_DIR
fi

TMP_TRACES_DIR="traces-pre-$RUN"
mkdir "$TMP_TRACES_DIR"
if [ -f Castalia-Trace.txt ]; then
    mv -v Castalia-Trace.txt "$TMP_TRACES_DIR/"
fi

find . -maxdepth 1 -name 'trace-*.pcap' -exec mv -v {} "$TMP_TRACES_DIR/" \;
rmdir "$TMP_TRACES_DIR" 2>/dev/null

echo "==========================="
echo "= Saving simulation setup ="
echo "==========================="
echo "$PWD/$0 $ARGS" > "$SETUP/invocation.txt"
cp -v $0 "$SETUP/"
cp -v "$INI" "$SETUP/"

echo "Saving Castalia source (../../../Castalia/) to $SETUP/Castalia-$RUN.tgz"
tar -czf "$SETUP/Castalia-$RUN.tgz" ../../../Castalia/

if [ -f "process.sh" ]; then
    cp -v "process.sh" "$PROCESSED_DATA"
fi
if [ -f "process.py" ]; then
    cp -v "process.py" "$PROCESSED_DATA"
fi
if [ -f "plot.r" ]; then
    cp -v "plot.r" "$PROCESSED_DATA"
fi

echo "======================="
echo "= Running simulations ="
echo "======================="

echo "runName,runTime(s)" >> "$OUTPUTS/time.csv"

for config in $CONFIGS; do
    RUNS=$($BIN -f $INI -a | grep "Config $config:" | sed 's/.*: \([0-9*]\)/\1/g')
    for run in $(seq 0 $((RUNS-1))); do
        seed=$RANDOM
        echo "Running config $config, run number $run of $((RUNS-1))"
        sed 's:[0-9][0-9]*}#SEEDTOREPLACE:'$seed'}#SEEDTOREPLACE:g' -i "$INI"
        echo "$BIN -u Cmdenv -c $config -r $run -n ..:../../src $INI | tee $OUTPUTS/$config-$run-omnet-output.txt"
        SECONDS=0
        $BIN -u Cmdenv -c $config -r $run -n ..:../../src $INI | tee -a $OUTPUTS/$config-$run-omnet-output.txt
        echo "$config-$run,$SECONDS" >> "$OUTPUTS/time.csv"
        mv -v results/* "$DIR/results/"
        if [ -f Castalia-Trace.txt ]; then
            mv -v Castalia-Trace.txt "$OUTPUTS/$config-$run-Castalia-Trace.txt"
        fi
        find . -maxdepth 1 -name 'trace-*.pcap' -exec mv -v {} "$OUTPUTS/" \;

        # You can change the line below to get the temperature in your machine, then uncomment the if statement
        TEMPERATURE=$(sensors | grep 'Physical id 0' | awk '{print $4}' | sed -e 's:[^0-9\.]::g' -e 's:\..*$::g')
        if [[ $TEMPERATURE > 65 ]]; then
            COOLDOWN=$(($SECONDS / 50))
            echo "Cooling down for $COOLDOWN seconds"
            sleep $COOLDOWN

            # You can change the line below to get the temperature in your machine, then uncomment the while loop
            TEMPERATURE=$(sensors | grep 'Physical id 0' | awk '{print $4}' | sed -e 's:[^0-9\.]::g' -e 's:\..*$::g')
            while [[ $TEMPERATURE > 51 ]]; do
                echo "Cooling down for 60 more seconds (temperature = $TEMPERATURE)"
                sleep 60
                # Change the line below to get the temperature in your machine
                TEMPERATURE=$(sensors | grep 'Physical id 0' | awk '{print $4}' | sed -e 's:[^0-9\.]::g' -e 's:\..*$::g')
            done
        fi
    done
done

rmdir results

if [ -d $TMP_RESULTS_DIR ]; then
    if [ ! -d results ]; then
        mv -v $TMP_RESULTS_DIR results
    fi
fi

if [ -d $TMP_TRACES_DIR ]; then
    find "$TMP_TRACES_DIR" -maxdepth 1 -exec mv -v {} . \;
    rmdir "$TMP_TRACES_DIR"
fi

echo "==========================="
echo "= Running data processing ="
echo "==========================="
cd $PROCESSED_DATA

if [ -f "process.sh" ]; then
    bash process.sh
fi
if [ -f "process.py" ]; then
    python3 process.py
fi
if [ -f "plot.r" ]; then
    Rscript plot.r
fi

pdfunite *.pdf plots.pdf
#if [ -f "plots.pdf" ]; then
#    evince plots.pdf
#fi

cd -

echo "Run name: $RUN"
echo "Run label: $LABEL"
echo "Results directory: $DIR"
echo "========="
echo "= Done! ="
echo "========="
