CC = g++
CFLAGS = -std=c++11
LIB = -L lib
INC = -I include
SRCEXT = cpp

SRCDIR = src
LIBDIR = lib
INCDIR = include
BUILDDIR = build
TARGETDIR = app
TARGET = runner

all:
	@make install

install:
	@echo " Linking..."
	$(CC) $(CFLAGS) -c $(LIBDIR)/*.$(SRCEXT) -I $(INCDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/*.$(SRCEXT) -I $(INCDIR) -L $(LIBDIR)
	@mkdir -p $(BUILDDIR)
	@mv *.o $(BUILDDIR)

	@echo " Building..."
	$(CC) $(CFLAGS) $(BUILDDIR)/*.o -o $(TARGET)
	@mv $(TARGET) $(TARGETDIR)
