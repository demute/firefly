TOPDIR = .
include Makefile.common

.PHONY: all

#PKGS = gsl sdl2 libpng
#PKGS_CFLAGS = $(foreach pkg,$(PKGS),--cflags $(pkg))
#PKGS_LIBS   = $(foreach pkg,$(PKGS),--libs $(pkg))
#CFLAGS     += $(shell $(PKGCONFIG) $(PKGS_CFLAGS))
#LDFLAGS    += $(shell $(PKGCONFIG) $(PKGS_LIBS))

OBJS += firefly.o

TARGET = firefly_test

all:$(TARGET)

test: firefly_test
	./firefly_test

$(TARGET): firefly_test.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -fPIC -o $@ -c $<

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -fPIC -o $@ -c $<

clean:
	rm -f *.o *.elf *.bin *.hex *.size *.dylib lib/*.so lib/*.dylib
