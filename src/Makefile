include ../Makefile_h

GPP 	= g++
UIC 	= uic
CGLAG 	= -c -fPIC
TARGET 	= iAloy_Desktop_App

DEST_BIN_LOC 	= /usr/bin/

QTLIB 	= 	-isystem /usr/include/$(SYS_ARC)/qt5 \
			-isystem /usr/include/$(SYS_ARC)/qt5/QtWidgets \
			-isystem /usr/include/$(SYS_ARC)/qt5/QtGui \
			-isystem /usr/include/$(SYS_ARC)/qt5/QtCore

SHLIB	=	/usr/lib/$(SYS_ARC)/libQt5Widgets.so \
			/usr/lib/$(SYS_ARC)/libQt5Gui.so \
			/usr/lib/$(SYS_ARC)/libQt5Core.so \
			/usr/local/lib/libPocoNet.so\
			/usr/local/lib/libPocoFoundationd.so\
			/usr/lib/$(SYS_ARC)/libGL.so -lpthread

CPPFILE	= main.cpp mainwindow.cpp
OBJFILE	= main.o mainwindow.o connect_init.o hardware.o

all: hardware connect_init main mainwindow moc_predefs moc_mainwindow_cpp moc_mainwindow_o main_bin

moc_predefs:
	g++ -E -o moc_predefs.h /usr/lib/$(SYS_ARC)/qt5/mkspecs/features/data/dummy.cpp

moc_mainwindow_cpp:moc_predefs.h
	moc --include moc_predefs.h mainwindow.h -o moc_mainwindow.cpp

main: main.cpp
	$(GPP) $(CGLAG) $(QTLIB) -o main.o main.cpp

mainwindow: mainwindow.cpp
	$(GPP) $(CGLAG) $(QTLIB) -o mainwindow.o mainwindow.cpp

connect_init: connect_init.cpp
	$(GPP) $(CGLAG) -o connect_init.o connect_init.cpp -lPocoNet -lPocoFoundation

hardware:hardware.cpp
	$(GPP) $(CGLAG) -o hardware.o hardware.cpp

moc_mainwindow_o: moc_mainwindow.cpp
	$(GPP) $(CGLAG) $(QTLIB) -o moc_mainwindow.o moc_mainwindow.cpp

main_bin: $(OBJFILE) moc_mainwindow.o
	$(GPP) -o $(TARGET) $(OBJFILE) moc_mainwindow.o $(SHLIB)

install:
	sudo cp $(TARGET) $(DEST_BIN_LOC)

uninstall:
	sudo rm $(DEST_BIN_LOC) $(TARGET)

clean:
	rm -f moc_predefs.h
	rm -f moc_mainwindow.cpp
	rm -f *.o
	rm -f *~ core *.core
	rm -f $(TARGET)
