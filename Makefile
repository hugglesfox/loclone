CFLAGS = -g
DEPS = lo.h subvol.h
OBJ = xfsub-snapshot.o lo.o subvol.o

xfsub-snapshot: $(OBJ)

%.o: %.c $(DEPS)

.PHONY: clean
clean:
	rm $(OBJ)

.PHONY: install
install:
	cp xfsub-snapshot /usr/local/bin
