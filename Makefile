TARGET = spnav
LIBS = -Llib -lhidapi-libusb
CC = gcc
CFLAGS = -Wall -Wextra -pedantic

.PHONY: mrproper all default

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o

mrproper: clean
	-rm -f $(TARGET)