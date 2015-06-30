CC=g++-4.9

# Directories...
TETREEX_INCDIR=inc
TETREEX_SRCDIR=src
TETREEX_OUTDIR=obj

RGB_INCDIR=matrix/include
RGB_LIBDIR=matrix/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

# CFLAGS=-I$(TETREEX_INCDIR) -DUSE_SDL_RENDERER -std=gnu++11 `sdl2-config --cflags --libs` -lSDL2_mixer
CFLAGS=-I$(TETREEX_INCDIR) -std=gnu++11 `sdl2-config --cflags --libs` -lSDL2_mixer

_DEPS = Tetreex.h
DEPS = $(patsubst %,$(TETREEX_INCDIR)/%,$(_DEPS))

_OBJS = main.o Application.o AudioDevice.o Board.o Game.o PixelBuffer.o Tetromino.o
OBJS = $(patsubst %,$(TETREEX_OUTDIR)/%,$(_OBJS))

$(TETREEX_OUTDIR)/%.o: $(TETREEX_SRCDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tetreex-game: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TETREEX_OUTDIR)/*.o *- core $(TETREEX_INCDIR)/*-
	rm -f tetreex-game
