#!/bin/bash

if [ ! -d ../../../Castalia/ ]; then
    echo "Error! You should run this script from Castalia/Simulations/<simulation>/"
    exit 1
fi

if ! which Castalia; then
    export PATH="$PATH:$PWD/../../bin"
fi

echo "Castalia bin: $(which Castalia)"

echo -n "Enter a label for this run: "
read LABEL
echo "Label: $LABEL"

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

TMP_CASTALIA_TRC="Castalia-Trace-pre-$RUN.txt"

if [ -d Castalia-Trace.txt ]; then
    mv -v Castalia-Trace.txt $TMP_CASTALIA_TRC
fi

echo "==========================="
echo "= Saving simulation setup ="
echo "==========================="
echo "$PWD/$0 $*" > "$SETUP/invocation.txt"
cp -v $0 "$SETUP/"
cp -v omnetpp.ini "$SETUP/"

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

echo "# Total time to complete simulation (in seconds)" >> "$OUTPUTS/time.txt"

for config in TSTP AODV AODVT LABILE REL LEACH; do
    echo "Running config $config"
    echo "Castalia --config=$config --input=omnetpp.ini --output=\"$OUTPUTS/$config-castalia-output.txt\""
    SECONDS=0
    Castalia --config=$config --input=omnetpp.ini --output="$OUTPUTS/$config-castalia-output.txt"
    echo "$config: $SECONDS" >> "$OUTPUTS/time.txt"
    mv -v results/* "$DIR/results/"
    mv -v Castalia-Trace.txt "$OUTPUTS/$config-Castalia-Trace.txt"
    COOLDOWN=$(($SECONDS / 4))
    echo "Cooling down for $COOLDOWN seconds"
    sleep $COOLDOWN
done

rmdir results

if [ -d $TMP_RESULTS_DIR ]; then
    mv -v $TMP_RESULTS_DIR results
fi

if [ -d $TMP_CASTALIA_TRC ]; then
    mv -v $TMP_CASTALIA_TRC Castalia-Trace.txt
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

cd -


echo "$RUN"
echo "$LABEL"
echo "========="
echo "= Done! ="
echo "========="
