TOPDIR	=	.
include config.mk

SRCDIR		= src
SUBDIRS		= $(SRCDIR)

OUTFILE		= out.a
LIBS            = $(addsuffix /$(OUTFILE),$(SUBDIRS))


.PHONY: all clean
.PHONY: $(SUBDIRS)
.SUFFIXES:

all: $(AFILE)
	@echo "Build complete."
	@echo 

clean: $(SUBDIRS)
	@echo " [ RM ] $(AFILE)"
	@$(RM) $(AFILE)
	
$(AFILE): $(SUBDIRS)
	@echo " [ AR ] $(CURRENTPATH)$(AFILE)"
	@$(RM) $(AFILE)
	@$(AR) -cm $(AFILE) $(shell $(AR) t $(LIBS))

$(SUBDIRS):
	@echo " [ CD ] $(CURRENTPATH)$@/"
	@+make -C "$@" "CURRENTPATH=$(CURRENTPATH)$@/" $(MAKECMDGOALS)
