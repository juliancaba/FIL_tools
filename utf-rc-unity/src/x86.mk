CFLAGS=-Wall -ggdb -fPIC
LDFLAGS=-fPIC -shared
VERSION=1.0.0


all: librcunity.so.$(VERSION) librcunity++.so.$(VERSION) 

%.o: %.c
	$(CC) -fPIC -c -o $@ $^

%.o: %.cc
	$(CC) -fPIC -c -o $@ $^



librcunity.so.$(VERSION): unity.o rc-unity.o
	$(CC) $(LDFLAGS) -o $@ $^

librcunity++.so.$(VERSION): unity.o rc-unity++.o
	$(CC) $(LDFLAGS) -o $@ $^

librcunity_remote.so.$(VERSION): unity.o rc-unity_remote.o
	$(CC) $(LDFLAGS) -o $@ $^


clean:
	$(RM) *~ *.o *.so.$(VERSION)
