
RESULT_FILE="/tmp/conreq3.log"

echo "" > ${RESULT_FILE}

for ((i=1; i<=$2; i++))
do
  xargs -I $i -P $2 curl $1 >> ${RESULT_FILE}    
  echo "done $i"
done

# xargs 2 -P 500 curl "http://localhost:8080/"