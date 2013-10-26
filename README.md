##XKCD Hasher

Simple XKCD hasher I cobbled together for XKCD's hash breaking competition. It is some rather horrible code,
and relies upon the NIST implementation of Skein for the actual hashing.

Just build it with 'make'. Also, it initializes everything with 0x2a. Because I didn't want to actually start at 00,
and 42 just seemed like a good answer.

Invoke it with

	./hash_checker_systematic <prefix1> <prefix2> ...
	
You can specify any n prefixes, (n>=0) and it will set the first n bytes of the iterator array. (So multiple instances with
different prefixes don't overlap)

There's also a simple script for generating a valid binary HTTP request given the hex output from a hasher.

	./genHeader.py 2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2ed07f

Outputs a header.dat (complete request) and raw.dat (Just the hash binary). Header can be directly piped to an HTTP server.
