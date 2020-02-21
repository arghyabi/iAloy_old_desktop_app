DEST_RESOURCE_LOC 	= /usr/share/iAloy/

all: ui/mainwindow.ui ui/dashboard.ui
	uic ui/mainwindow.ui -o ../src/ui_mainwindow.h
	uic ui/dashboard.ui -o ../src/ui_dashboard.h

install:
	sudo mkdir -p $(DEST_RESOURCE_LOC)
	sudo cp photos/back.jpg $(DEST_RESOURCE_LOC)

clean:
	rm -f ../src/ui_mainwindow.h
	rm -f ../src/ui_dashboard.h
	
uninstall:
	sudo rm -r $(DEST_RESOURCE_LOC)
