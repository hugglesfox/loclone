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
	cp loclone /usr/local/bin
