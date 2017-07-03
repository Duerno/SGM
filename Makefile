CC = g++
CFLAGS = -std=c++11
LIB = -L lib
INC = -I include
SRCEXT = cpp

SRCDIR = src
LIBDIR = lib
INCDIR = include
BUILDDIR = build
TARGET = sgm

compile:
	@echo " Linking..."
	$(CC) $(CFLAGS) -c $(LIBDIR)/*.$(SRCEXT) -I $(INCDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/*.$(SRCEXT) -I $(INCDIR) -L $(LIBDIR)
	@mkdir -p $(BUILDDIR)
	@mv *.o $(BUILDDIR)

	@echo " Building..."
	$(CC) $(CFLAGS) $(BUILDDIR)/*.o -o $(TARGET)

install:
	@echo " Installing..."
	sudo mv $(TARGET) /usr/bin
	mkdir -p ~/.sgm
	cp static ~/.sgm/. -r
