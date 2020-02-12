GPP 	= g++
UIC 	= uic
CGLAG 	= -c -fPIC
TARGET 	= iAloy_Desktop_App

DEST_RESOURCE_LOC 	= /usr/share/iAloy/
DEST_BIN_LOC 		= /usr/bin/

QTLIB 	= 	-isystem /usr/include/x86_64-linux-gnu/qt5 \
			-isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets \
			-isystem /usr/include/x86_64-linux-gnu/qt5/QtGui \
			-isystem /usr/include/x86_64-linux-gnu/qt5/QtCore

SHLIB	=	/usr/lib/x86_64-linux-gnu/libQt5Widgets.so \
			/usr/lib/x86_64-linux-gnu/libQt5Gui.so \
			/usr/lib/x86_64-linux-gnu/libQt5Core.so \
			/usr/lib/x86_64-linux-gnu/libGL.so -lpthread

CPPFILE	= main.cpp mainwindow.cpp
OBJFILE	= main.o mainwindow.o

#all: uic_convert $(OBJFILE) moc_predefs moc_mainwindow_cpp moc_mainwindow_o main_bin
all: uic_convert main mainwindow moc_predefs moc_mainwindow_cpp moc_mainwindow_o main_bin

uic_convert: mainwindow.ui
	uic mainwindow.ui -o ui_mainwindow.h

moc_predefs:
	g++ -E -o moc_predefs.h /usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/data/dummy.cpp

moc_mainwindow_cpp:moc_predefs.h
	moc --include moc_predefs.h mainwindow.h -o moc_mainwindow.cpp

$(OBJFILE): $(CPPFILE)
	$(GPP) $(CGLAG) $(QTLIB) $< -o $@

main: main.cpp
	$(GPP) $(CGLAG) $(QTLIB) -o main.o main.cpp

mainwindow: mainwindow.cpp
	$(GPP) $(CGLAG) $(QTLIB) -o mainwindow.o mainwindow.cpp

moc_mainwindow_o: moc_mainwindow.cpp
	$(GPP) $(CGLAG) $(QTLIB) -o moc_mainwindow.o moc_mainwindow.cpp

main_bin: $(OBJFILE) moc_mainwindow.o
	$(GPP) -o $(TARGET) $(OBJFILE) moc_mainwindow.o $(SHLIB)

install:
	sudo mkdir -p $(DEST_RESOURCE_LOC)
	sudo cp back.jpg $(DEST_RESOURCE_LOC)
	sudo cp $(TARGET) $(DEST_BIN_LOC)

uninstall:
	sudo rm $(DEST_BIN_LOC) $(TARGET)
	sudo rm -r $(DEST_RESOURCE_LOC)

clean:
	rm -f moc_predefs.h
	rm -f moc_mainwindow.cpp
	rm -f ui_mainwindow.h
	rm -f main.o mainwindow.o moc_mainwindow.o
	rm -f *~ core *.core
	rm -f $(TARGET)
