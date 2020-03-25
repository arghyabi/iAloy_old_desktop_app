include ../filelist

DEST_RESOURCE_LOC 	= /usr/share/iAloy/photos/
DEST_SCRIPT_LOC 	= /usr/share/iAloy/scripts/

SRC_DIR = ui/
DEST_DIR = ../src/

all: $(UIMHFILE)

ui_%.h: $(SRC_DIR)%.ui
	uic $< -o $(DEST_DIR)$@

install:
	sudo mkdir -p $(DEST_RESOURCE_LOC)
	sudo mkdir -p $(DEST_SCRIPT_LOC)
	sudo cp photos/* $(DEST_RESOURCE_LOC)
	sudo cp scripts/* $(DEST_SCRIPT_LOC)
	sudo chmod 777 $(DEST_SCRIPT_LOC)*

clean: $(addprefix $(DEST_DIR), $(UIMHFILE))
	rm -f $(addprefix $(DEST_DIR), $(UIMHFILE))
	
uninstall:
	sudo rm -r $(DEST_RESOURCE_LOC)
	Sudo rm -r $(DEST_SCRIPT_LOC)
