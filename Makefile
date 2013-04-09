TARGET   = libloax.a
LOAX     = loax_client loax_client_gl2 loax_log loax_serialize loax_listener
SOURCE   = $(LOAX:%=%.c)
OBJECTS  = $(SOURCE:.c=.o)
HFILES   = $(LOAX:%=%.h)
OPT      = -O2 -Wall
CFLAGS   = $(OPT) -I. -I..
LDFLAGS  = -lm -L/usr/lib -L../net -lnet
AR       = ar

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

clean:
	rm -f $(OBJECTS) *~ \#*\# $(TARGET)

$(OBJECTS): $(HFILES)
