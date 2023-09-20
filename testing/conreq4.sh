#!/bin/bash
(for x in {1..$1}; do xargs -I  % -P $2 curl "http://localhost:8080/" < <(printf '%s\n' {1..$2}); echo "\n=====Total  $(($x*$2)) requests were made======\n"; done ) >> temp.txt
