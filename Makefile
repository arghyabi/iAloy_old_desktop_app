all:
	(cd resource && ${MAKE} all)
	(cd src && ${MAKE} all)

install:
	(cd resource && ${MAKE} install)
	(cd src && ${MAKE} install)

clean:
	(cd resource && ${MAKE} clean)
	(cd src && ${MAKE} clean)

uninstall:
	(cd resource && ${MAKE} uninstall)
	(cd src && ${MAKE} uninstall)