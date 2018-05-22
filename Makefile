TARGET = spnav
TARGET_LIB = libspnavhdi.so # target lib
LIBS = -Llib -lhidapi-libusb
CC = gcc
CFLAGS = -Wall -Wextra -fPIC -pedantic -O2 # C flags for building library
#CFLAGS = -Wall -Wextra -pedantic -g # C flags for developpement
LDFLAGS = $(LIBS) -shared # linking flags
RM = rm -f # rm command

.PHONY: all default

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

$(TARGET_LIB): $(OBJECTS)
	$(CC) ${LDFLAGS} -o $@ $^

.PHONY: clean
clean:
	-${RM} ${TARGET}
	-${RM} ${TARGET_LIB} ${OBJECTS} $(SRCS:.c=.d)
