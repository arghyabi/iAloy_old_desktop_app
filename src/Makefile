include ../Makefile_h

GPP 	= g++
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

CPPFILE	= main.cpp mainwindow.cpp dashboard.cpp connect_init.cpp hardware.cpp
OBJFILE	= main.o mainwindow.o dashboard.o connect_init.o hardware.o

all: moc_predefs moc_mainwindow_cpp moc_mainwindow_o $(OBJFILE) main_bin

%.o: %.cpp
	$(GPP) $(CGLAG) $(QTLIB) $< -o $@

moc_predefs:
	g++ -E -o moc_predefs.h /usr/lib/$(SYS_ARC)/qt5/mkspecs/features/data/dummy.cpp

moc_mainwindow_cpp:moc_predefs.h
	moc --include moc_predefs.h mainwindow.h -o moc_mainwindow.cpp
	moc --include moc_predefs.h dashboard.h -o moc_dashboard.cpp

moc_mainwindow_o: moc_mainwindow.cpp moc_dashboard.cpp
	$(GPP) $(CGLAG) $(QTLIB) -o moc_mainwindow.o moc_mainwindow.cpp
	$(GPP) $(CGLAG) $(QTLIB) -o moc_dashboard.o moc_dashboard.cpp

main_bin: $(OBJFILE) moc_mainwindow.o moc_dashboard.o
	$(GPP) -o $(TARGET) $(OBJFILE) moc_mainwindow.o moc_dashboard.o $(SHLIB)

install:
	sudo cp $(TARGET) $(DEST_BIN_LOC)

uninstall:
	sudo rm $(DEST_BIN_LOC) $(TARGET)

clean:
	rm -f moc_predefs.h
	rm -f moc_mainwindow.cpp moc_dashboard.cpp
	rm -f *.o
	rm -f *~ core *.core
	rm -f $(TARGET)
