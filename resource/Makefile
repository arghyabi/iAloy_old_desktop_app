include ../filelist

DEST_RESOURCE_LOC 	= /usr/share/iAloy/photos/
DEST_SCRIPT_LOC 	= /usr/share/iAloy/scripts/
DEST_FONT_LOC		= /usr/share/iAloy/fonts/
DEST_HEX_LOC		= /usr/share/iAloy/hex/
DEST_CONF_LOC		= /usr/share/iAloy/.conf/
DEST_TEMP_LOC		= /usr/share/iAloy/.temp/

SRC_DIR = ui/
DEST_DIR = ../src/

all: $(UIMHFILE)

ui_%.h: $(SRC_DIR)%.ui
	@uic $< -o $(DEST_DIR)$@
	@echo "	UIC	" $<

install:
	sudo mkdir -p $(DEST_RESOURCE_LOC)
	sudo mkdir -p $(DEST_SCRIPT_LOC)
	sudo mkdir -p $(DEST_FONT_LOC)
	sudo mkdir -p $(DEST_CONF_LOC)
	sudo mkdir -p $(DEST_HEX_LOC)
	sudo mkdir -p $(DEST_TEMP_LOC)
	sudo cp photos/* $(DEST_RESOURCE_LOC)
	sudo cp scripts/* $(DEST_SCRIPT_LOC)
	sudo cp fonts/* $(DEST_FONT_LOC)
	sudo cp hex/* $(DEST_HEX_LOC)
	sudo chmod 777 $(DEST_SCRIPT_LOC)*

clean: $(addprefix $(DEST_DIR), $(UIMHFILE))
	rm -f $(addprefix $(DEST_DIR), $(UIMHFILE))

uninstall:
	sudo rm -r $(DEST_RESOURCE_LOC)
	sudo rm -r $(DEST_SCRIPT_LOC)
	sudo rm -r $(DEST_FONT_LOC)
	sudo rm -r $(DEST_HEX_LOC)
