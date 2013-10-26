#!/usr/bin/python

import sys

header = """POST /?edu=colorado.edu HTTP/1.1
Host: almamater.xkcd.com
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:20.0) Gecko/20100101 Firefox/20.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Referer: http://almamater.xkcd.com/?edu=colorado.edu
Connection: keep-alive
Content-Type: application/x-www-form-urlencoded
Content-Length: %s

hashable="""

pop = lambda x: (x[:2], x[2:])
hash_bytes = []
message = sys.argv[1]
while message != "":
	abyte, message = pop(message)
	hash_bytes.append( chr(int(abyte, 16)) )

con_len = str(len(header)+1 + len(hash_bytes))

open("header.dat", "wb").write( header%con_len + "".join(hash_bytes) )
open("raw.dat", "wb").write( "".join(hash_bytes))
