# source : http://marianlonga.com/simulate-parallel-connections-to-a-website/

initial_time=$(date +%s)

for ((i=1; i<=$2; i++))
do
  curl -s -o /dev/null -H "Connection: close"  $1 && echo "done $i" &
done

wait

echo " === done ===="

final_time=$(date +%s)
time_elapsed=$(($final_time-$initial_time))

echo $2 "requests processed in" $time_elapsed "s"