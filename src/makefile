NOME_SAIDA=../sim_mips

IDIR =../include
CC=gcc
EXT = c
CFLAGS=-Wall -g -I $(IDIR)

ODIR=obj

LIBS=-lm

_DEPS = memory.h run.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = memory.o sim_mips.o run.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.$(EXT) $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(NOME_SAIDA): $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(NOME_SAIDA)   
