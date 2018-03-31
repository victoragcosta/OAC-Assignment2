NOME_SAIDA=../sim_mips_test

IDIR =../include
CC=g++
EXT = c
CFLAGS=-Wall -g -I $(IDIR)

ODIR=obj

LIBS=-lm

_DEPS = mips.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = mips.o tests.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.$(EXT) $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(NOME_SAIDA): $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(NOME_SAIDA)   
