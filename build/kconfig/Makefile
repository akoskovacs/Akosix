RM = rm -rf
OBJECTS := mconf.o zconf.tab.o lxdialog/checklist.o lxdialog/util.o lxdialog/inputbox.o lxdialog/textbox.o lxdialog/yesno.o lxdialog/menubox.o
TARGET = mconf
CFLAGS = -DCURSES_LOC="<ncurses.h>" -DKBUILD_NO_NLS

$(TARGET): $(OBJECTS)
	@echo "LINK $(TARGET)"
	$(CC) $(OBJECTS) -o $(TARGET) -lncurses

%.o : %.c 
	@echo "CC $*.c"
	$(CC) $(CFLAGS) -c -o $*.o $*.c 

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(TARGET)	
