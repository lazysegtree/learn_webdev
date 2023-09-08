# usage
# conreq_ab.sh localhost  1000      50
#                address  totalreq  concurrent req

# ab -d -n 50 -c 20 http://localhost:8080/

ab -d -n $2 -c $3 http://$1/ &> /tmp/conreq_ab.log

grep /tmp/conreq_ab.log -e 'Failed\|Time taken\|Complete requests'