SUBDIRS = src 

modesdir=$(prefix)/share/matxin/modes

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = matxin.pc

aclocaldir = $(datadir)/aclocal
aclocal_DATA = matxin.m4


EXTRA_DIST=AUTHORS ChangeLog COPYING INSTALL NEWS README matxin.m4 README.modes

install-data-local:
	mkdir -p $(DESTDIR)$(modesdir)
	$(INSTALL_DATA) README.modes  $(DESTDIR)$(modesdir)/README
