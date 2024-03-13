## Sample commands

Run `make` to create client binary.

- Send Request with default IP Options 
`client.out -s 127.0.0.1 -p 9121 -v 3 -i`

- Send Request to IPv6 without IP Options
`client.out -s 127.0.0.1 -p 9121 -v 3`

- Send Request with 4 threads, and 4 request per thread
`client.out -s 127.0.0.1 -p 9121 -v 1 -t 4 -r 4`