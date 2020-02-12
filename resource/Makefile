DEST_RESOURCE_LOC 	= /usr/share/iAloy/

all: ui/mainwindow.ui
	uic ui/mainwindow.ui -o ../src/ui_mainwindow.h

install:
	sudo mkdir -p $(DEST_RESOURCE_LOC)
	sudo cp photos/back.jpg $(DEST_RESOURCE_LOC)

clean:
	
uninstall:
	sudo rm -r $(DEST_RESOURCE_LOC)
