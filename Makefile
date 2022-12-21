CFLAGS = -g
DEPS = lo.h img.h
OBJ = loclone.o lo.o img.o

loclone: $(OBJ)

%.o: %.c $(DEPS)

.PHONY: clean
clean:
	rm $(OBJ)

.PHONY: install
install:
	install loclone /usr/local/bin
	install loclone.1 /usr/local/share/man/man1
