TOPDIR	=	.
include config.mk

SUBDIRS		= src

.PHONY: all clean install
.PHONY: $(SUBDIRS)
.SUFFIXES:

all: $(HEXFILE) $(BINFILE)
	@echo "Build complete."
	@echo 

install:
	stty -F $(TTYDEV) $(TTYBAUD)
	cat $(NAME).hex > $(TTYDEV)

clean: $(SUBDIRS)
	@echo " [ RM ] $(BINFILE)"
	@$(RM) $(BINFILE)
	
	@echo
	@echo "Source tree cleaned."
	@echo

$(ELFFILE): $(SUBDIRS)
	@echo " [ LD ] $@"
	@$(CC) -o $@ $(CFLAGS) -Wl,--whole-archive $(addsuffix /out.a,$(SUBDIRS)) -Wl,--no-whole-archive $(LDFLAGS)

$(HEXFILE): $(ELFFILE)
	@echo " [OCPY] $@"
	@$(TARGET)objcopy -O ihex $< $@
	@$(SREC_CAT) -O $@ -Intel $@ -Intel

$(BINFILE): $(ELFFILE)
	@echo " [OCPY] $@"
	@$(TARGET)objcopy -O binary $< $@

$(SUBDIRS):
	@echo " [ CD ] $(CURRENTPATH)$@/"
	@+make -C "$@" "CURRENTPATH=$(CURRENTPATH)$@/" $(MAKECMDGOALS)
