#!/bin/bash

# For EPOS TSTP:
#find . -name \*.o -exec arm-none-eabi-nm --size-sort -S -t d -C \{} \; | grep '\<T\>' | grep -v 'Security' | grep 'TSTP' > code_size_tstpe.txt

find . -name \*.o -exec nm --size-sort -S -t d -C \{} \; | grep '\<T\>' | grep -v 'finishSpecific()' | grep -v 'TSTPMAC' | grep -v 'TSTPPacket' | grep -v 'GeoSync::' | grep 'TSTP' > code_size_tstpc.txt

find . -name \*.o -exec nm --size-sort -S -t d -C \{} \; | grep '\<T\>' | grep -v 'finish()'| grep 'AodvRouting' > code_size_aodv.txt

find . -name \*.o -exec nm --size-sort -S -t d -C \{} \; | grep '\<T\>' | grep -v 'finish()'| grep "AodvTestRouting" > code_size_aodvt.txt

find . -name \*.o -exec nm --size-sort -S -t d -C \{} \; | grep '\<T\>' | grep -v 'finish()' | grep 'RELRouting' > code_size_rel.txt

find . -name \*.o -exec nm --size-sort -S -t d -C \{} \; | grep '\<T\>' | grep -v 'finish()' | grep 'LabileRouting' > code_size_labile.txt

find . -name \*.o -exec nm --size-sort -S -t d -C \{} \; | grep '\<T\>' | grep -v 'finish()' | grep 'BypassRouting::' > code_size_bypass.txt

if [ -f code_size_total.txt ]; then
    rm code_size_total.txt
fi

for i in tstpe tstpc aodv aodvt rel labile bypass; do
    echo -ne "$i\t" >> code_size_total.txt
    cat code_size_$i.txt | awk '{sum += $2} END {print sum}' >> code_size_total.txt
done

sort -k2 -n code_size_total.txt -o code_size_total.txt
cat code_size_total.txt
