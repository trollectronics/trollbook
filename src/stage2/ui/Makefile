# Project: muil
include config.mk

SRCFILES	=	$(wildcard *.c)
OBJFILES	=	$(SRCFILES:.c=.o)
.PHONY: all clean

all: $(OBJFILES)
	@echo " [ LD ] libmuil.so"
	@$(CC) $(CFLAGS) -o ../libmuil.so $(OBJFILES) $(LDFLAGS)
	@echo " [ LD ] libmuil.a"
	@$(AR) -cr ../libmuil.a $(OBJFILES)
	@echo ""

clean:
	@echo " [ RM ]	$(OBJFILES) ../libmuil.a ../libmuil.so"
	@$(RM) $(OBJFILES) ../libmuil.a ../libmuil.so
	@echo ""

%.o: %.c
	@echo " [ CC ]	$<"
	@$(CC) $(CFLAGS) -c -o $@ $< 
