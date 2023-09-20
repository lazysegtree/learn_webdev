# source : http://marianlonga.com/simulate-parallel-connections-to-a-website/

initial_time=$(date +%s)

LOG_FILE="/tmp/conreq5.log"
SLEEP_TIME="3"
CNT_REQ=$2

echo "" > $LOG_FILE

for ((i=1; i<=$2; i++))
do
  # append both stderr and stdout to a file
  netcat -v 127.0.0.1 $1 >> $LOG_FILE 2>&1 &
done

sleep $SLEEP_TIME

#pkill -f netcat

CNT_SUCCESS=$(wc -l < $LOG_FILE)

echo "=== Done with ${CNT_REQ} requests===="
echo "Succeded" 

final_time=$(date +%s)
time_elapsed=$(($final_time-$initial_time))

echo $2 "requests processed in" $time_elapsed "s"