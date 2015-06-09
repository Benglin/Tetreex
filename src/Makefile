# Build script for Tetreex
# 
# RGB_INCDIR=include
# RGB_LIBDIR=lib
# RGB_LIBRARY_NAME=rgbmatrix
# RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
# LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

# all : $(BINARIES)

# $(RGB_LIBRARY):
#         $(MAKE) -C $(RGB_LIBDIR)

# led-matrix : demo-main.o $(RGB_LIBRARY)
#         $(CXX) $(CXXFLAGS) demo-main.o -o $@ $(LDFLAGS)

# %.o : %.cc
#         $(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -DADAFRUIT_RGBMATRIX_HAT -c -o $@ $<

clean:
        rm -f *.o $(OBJECTS) $(BINARIES)
