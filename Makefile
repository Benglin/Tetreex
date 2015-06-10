CC=g++

# Directories...
TETREEX_INCDIR=inc
TETREEX_SRCDIR=src
TETREEX_OUTDIR=obj

CFLAGS=-I$(TETREEX_INCDIR)

_DEPS = Tetreex.h
DEPS = $(patsubst %,$(TETREEX_INCDIR)/%,$(_DEPS))

_OBJS = main.o Tetreex.o
OBJS = $(patsubst %,$(TETREEX_OUTDIR)/%,$(_OBJS))

$(TETREEX_OUTDIR)/%.o: $(TETREEX_SRCDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tetreex: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(TETREEX_OUTDIR)/*.o *- core $(TETREEX_INCDIR)/*-