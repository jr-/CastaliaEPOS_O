#!/bin/bash

for i in TSTP AODV AODVT LEACH REL LABILE; do echo $i >> pacotes.txt; cat ../results/$i-0.sca | grep Pacotes >> pacotes.txt; done

for i in TSTP AODV AODVT LEACH REL LABILE; do echo $i; grep Recebidos ../results/$i-0.sca | grep '\[0\]'; grep Global ../results/$i-0.sca | head -n 1; done

for i in TSTP AODV AODVT LEACH REL LABILE; do cat ../results/$i-0.sca | grep "TXed pkts" | awk '{print $5}' > $i-txed.txt; done
