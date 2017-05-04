# Project: muil
# Makefile configurations

VERSION		=	0.1

#DBGFLAGS	=	-O0 -g -D__DEBUG__
DBGFLAGS	=	-O0 -g
CFLAGS		+=	-fPIC -shared -Wall $(INCS) $(DBGFLAGS) -I../include
LDFLAGS		+=	-shared -lm -ldarnit -Wl,-soname,muil
RM		=	rm -f
