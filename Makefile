TARGET = spnav
TARGET_LIB = libspnavhdi.so # target lib

CC = gcc
#CFLAGS = -Wall -Wextra -fPIC -pedantic -O2 # C flags for building library
CFLAGS = -Wall -Wextra -pedantic -g # C flags for developpement

LDFLAGS = -Wall -Wextra -O2

LIBS = -Llib -lhidapi-libusb
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
	$(CC) $(OBJECTS) -Wall -o $@ $(LIBS)

# Shared Libs
$(TARGET_LIB): $(OBJECTS)
	$(CC) $(LDFLAGS) -shared -fpic -Wl,--no-undefined -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	-${RM} ${TARGET}
	-${RM} ${TARGET_LIB} ${OBJECTS} $(SRCS:.c=.d)
