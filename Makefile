all: terminal

terminal: src/terminal.c
	gcc `pkg-config --cflags --libs gtk+-3.0 vte-2.91` -o terminal $^

install:
	mv -f terminal /usr/bin
	desktop-file-install terminal.desktop

uninstall:
	rm -f /usr/bin/terminal
	rm -f /usr/share/applications/terminal.desktop

.PHONY: clean

clean:
	rm -f terminal
