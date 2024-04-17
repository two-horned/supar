.POSIX:

include config.mk

COM =\
	components/time\
	components/date\
	components/ram\
	components/volume\
	components/updates

all: supar

$(COM:=.o): config.mk supar.h
supar.o: supar.c supar.h config.h config.mk $(REQ:=.h)

.c.o:
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

config.h:
	cp config.def.h $@

supar: supar.o $(COM:=.o) $(REQ:=.o)
	$(CC) -o $@ $(LDFLAGS) $(COM:=.o) $(REQ:=.o) supar.o $(LDLIBS)

clean:
	rm -f supar supar.o $(COM:=.o) $(REQ:=.o) supar-${VERSION}.tar.gz

dist:
	rm -rf "supar-$(VERSION)"
	mkdir -p "supar-$(VERSION)/components"
	cp -R LICENSE Makefile README config.mk config.def.h \
	      arg.h supar.h supar.c $(REQ:=.c) $(REQ:=.h) \
	      supar.1 "supar-$(VERSION)"
	cp -R $(COM:=.c) "supar-$(VERSION)/components"
	tar -cf - "supar-$(VERSION)" | gzip -c > "supar-$(VERSION).tar.gz"
	rm -rf "supar-$(VERSION)"

install: all
	mkdir -p "$(DESTDIR)$(PREFIX)/bin"
	cp -f supar "$(DESTDIR)$(PREFIX)/bin"
	chmod 755 "$(DESTDIR)$(PREFIX)/bin/supar"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)/bin/supar"
