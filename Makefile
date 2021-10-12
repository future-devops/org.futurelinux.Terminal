all: terminal

terminal: src/terminal.c
	gcc `pkg-config --cflags --libs gtk+-3.0 vte-2.91` -o terminal $^

install:
	mv -f terminal /app/usr/bin
	cp -f terminal.desktop /app/usr/share/applications

uninstall:
	rm -f /app/usr/bin/terminal
	rm -f /app/usr/share/applications/terminal.desktop
	
native-install:
	mv -f terminal /usr/bin
	cp -f terminal.desktop /usr/share/applications

native-uninstall:
	rm -f /usr/bin/terminal
	rm -f /usr/share/applications/terminal.desktop

.PHONY: clean

clean:
	rm -f terminal
