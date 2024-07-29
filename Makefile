all:
	$(MAKE) -C programs/clop/compgcc
	cd programs/clop/compqt/clop-console && qmake
	$(MAKE) -C programs/clop/compqt/clop-console
	cd programs/clop/compqt/clop-gui && qmake
	$(MAKE) -C programs/clop/compqt/clop-gui

clean:
	-$(MAKE) -C programs/clop/compgcc clean
	-$(MAKE) -C programs/clop/compqt/clop-console clean
	-$(MAKE) -C programs/clop/compqt/clop-gui clean
	rm -f programs/clop/compqt/clop-console/{.qmake.stash,Makefile,clop-console}
	rm -f programs/clop/compqt/clop-gui/{.qmake.stash,Makefile,clop-gui}
	rm -f programs/clop/swig/{_clop_swig.so,clop_swig.py,clop_swig_wrap.cxx,clop_swig_wrap.d,i.tmp,py.tmp,clop_swig_wrap.o}
	rm -f programs/rclib/compgcc/bin/*.o
	rm -f programs/plot/compgcc/bin/*.o


PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	install -m 0755 programs/clop/compqt/clop-gui/clop-gui $(DESTDIR)$(BINDIR)
	install -m 0755 programs/clop/compqt/clop-console/clop-console $(DESTDIR)$(BINDIR)
	ln -svf $(DESTDIR)$(BINDIR)/{clop-gui,clop}
	install -m 0755 chess/clop-cutechess.py $(DESTDIR)$(BINDIR)


.PHONY: install all clean
