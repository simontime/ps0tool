ps0tool: ps0tool.c cmp.c crc.c util.c
	cc -Wno-unused-result -O2 -o ps0tool ps0tool.c cmp.c crc.c util.c
clean:
	rm -f ps0tool