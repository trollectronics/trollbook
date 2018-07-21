# Project: trollbook-bios
TOPDIR	=	$(shell DIR=.; while [ ! "`readlink -f \"$${DIR}\"`" = "/" -a ! -f "$${DIR}/config.mk" ]; do DIR="../$${DIR}"; done; echo "$${DIR}")
ifeq ($(shell readlink -f "$(TOPDIR)"),/)
	$(error Could not find the project top directory with config.mk)
endif
include $(TOPDIR)/config.mk

ASMFILES	=	$(wildcard *.S)
SRCFILES	=	$(wildcard *.c)
OBJFILES	=	$(SRCFILES:.c=.c.o)
OBJFILES	+=	$(ASMFILES:.S=.S.o)

OUTFILE		=	out.a

# Sub directories to build
SUBDIRS		=	$(foreach dir,$(wildcard */Makefile),$(dir $(dir)))

LIBS		=	$(addsuffix /$(OUTFILE),$(SUBDIRS))

DEPDIR = .deps
df = $(DEPDIR)/$(*F)

.PHONY: all clean
.PHONY: $(SUBDIRS)

all: $(OUTFILE)
	@echo " [DONE] $(CURRENTPATH)"

clean: $(SUBDIRS)
	@echo " [ RM ] $(OBJFILES) $(OUTFILE)"
	@$(RM) $(OBJFILES) $(OUTFILE)
	@$(RM) -R $(DEPDIR)

$(OUTFILE): $(OBJFILES) $(SUBDIRS)
	@echo " [ AR ] $(CURRENTPATH)$(OUTFILE)"
	@$(RM) $(OUTFILE)
	@$(AR) -cmT $(OUTFILE) $(OBJFILES) $(LIBS)

$(SUBDIRS):
	@echo " [ CD ] $(CURRENTPATH)$@"
	@+make -C "$@" "CURRENTPATH=$(CURRENTPATH)$@" $(MAKECMDGOALS)

$(DEPDIR):
	@mkdir -p $@

%.c.o: %.c | $(DEPDIR)
	@echo " [ CC ] $(CURRENTPATH)$<"
	@$(CC) $(CFLAGS) -c -MD -o $@ $<
	@cp $*.c.d $(df).c.P; sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.c.d >> $(df).c.P; $(RM) $*.c.d
	
%.S.o: %.S | $(DEPDIR)
	@echo " [ AS ] $(CURRENTPATH)$<"
	@$(CC) $(CFLAGS) $(ASFLAGS) -c -MD -o $@ $<
	@cp $*.S.d $(df).S.P; sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.S.d >> $(df).S.P; $(RM) $*.S.d

-include $(SRCFILES:%.c=$(DEPDIR)/%.c.P)
-include $(ASMFILES:%.S=$(DEPDIR)/%.S.P)
