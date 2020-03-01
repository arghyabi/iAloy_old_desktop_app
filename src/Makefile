include ../Makefile_h
include ../filelist

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

all: moc_predefs $(UIMOCCPP) $(UIMOCOBJ) $(CPPOBJFILE) $(UIOBJFILE) main_bin

%.o: %.cpp
	$(GPP) $(CGLAG) $(QTLIB) $< -o $@

moc_predefs:
	g++ -E -o moc_predefs.h /usr/lib/$(SYS_ARC)/qt5/mkspecs/features/data/dummy.cpp

moc_%.cpp: %.h
	moc --include moc_predefs.h $< -o $@

moc_%.o:moc_%.cpp
	$(GPP) $(CGLAG) $(QTLIB) $< -o $@

main_bin: $(CPPOBJFILE) $(UIOBJFILE) $(UIMOCOBJ)
	$(GPP) -o $(TARGET) $(CPPOBJFILE) $(UIOBJFILE) $(UIMOCOBJ) $(SHLIB)

install:
	sudo cp $(TARGET) $(DEST_BIN_LOC)

uninstall:
	sudo rm $(DEST_BIN_LOC) $(TARGET)

clean:
	rm -f moc_predefs.h
	rm -f $(UIMOCCPP)
	rm -f *.o
	rm -f $(TARGET)
