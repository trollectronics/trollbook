# Sub directories to build
SUBDIRS		=	$(patsubst %/,%,$(foreach subdir,$(wildcard */Makefile),$(dir $(subdir))))
LIBFILES		=	$(addsuffix .a,$(SUBDIRS))

.PHONY: all clean
.PHONY: $(SUBDIRS)

all: $(LIBFILES)
	@echo "Plugins build complete."
	@echo 

clean: $(SUBDIRS)
	@echo " [ RM ] $(LIBFILES)"
	@$(RM) $(LIBFILES)
	
	@echo
	@echo "Plugins source tree cleaned."
	@echo

$(SUBDIRS):
	@echo " [ CD ] $(CURRENTPATH)$@/"
	@+make -C "$@" $(MAKECMDGOALS)

%.a: $(SUBDIRS)
	@cp $*/$@ $@