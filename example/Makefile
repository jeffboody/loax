TARGET   = client
CLASSES  = 
SOURCE   = $(TARGET).c $(CLASSES:%=%.c)
OBJECTS  = $(TARGET).o $(CLASSES:%=%.o)
HFILES   = $(CLASSES:%=%.h)
OPT      = -O2 -Wall
CFLAGS   = $(OPT) -I.
LDFLAGS  = -L/usr/lib -Lloax -lloax -Lnet -lnet
CCC      = gcc

all: $(TARGET)

$(TARGET): $(OBJECTS) net loax
	$(CCC) $(OPT) $(OBJECTS) -o $@ $(LDFLAGS)

.PHONY: net loax

net:
	$(MAKE) -C net

loax:
	$(MAKE) -C loax

clean:
	rm -f $(OBJECTS) *~ \#*\# $(TARGET)
	$(MAKE) -C net clean
	$(MAKE) -C loax clean
	rm net loax

$(OBJECTS): $(HFILES)
