for i in ../results/*.sca; do
    /home/davi/masters/dissertation/data/parse_sca.sh "$i" >> /home/davi/masters/dissertation/data/pigtailing.csv
    echo "$i" >> pigtailing.txt
    ~/masters/dissertation/process_pigtailing.sh "$i" >> pigtailing.txt
    echo "" >> pigtailing.txt
done
