
CC       := gcc
LD       := $(CC)

CFLAGS   += -c -Wall -D_DEBUG

OUT      := terra
SOURCES  := $(shell find -type f -name '*.c' -not -iwholename '*tests*')
OBJSDIR  := .objs
OBJECTS  := $(SOURCES:%.c=$(OBJSDIR)/%.o)
PCH_FILE := stdafx.h
OBJSPCH  := $(PCH_FILE:%.h=%.h.gch)
RM       := rm -rf
CMF      := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))

TSRC     := $(shell find -type f -name '*.c' -iwholename '*tests*')
TOBJS    := $(TSRC:%.c=$(OBJSDIR)/%.o)
TLIBS    := $(OUT) pthread m
TOUT     := terra_test

#linkage
$(OUT): $(OBJSPCH) $(OBJECTS)
	ar -cvq lib$@.a $(OBJECTS)

#cleaning
.PHONY: clean
clean:
	$(RM) $(OBJSPCH) $(OBJSDIR) $(TOUT) lib$(OUT).a
	
#tests
.PHONY: test
test: $(TOBJS)
	$(LD) -o $(TOUT) $(TOBJS) -L$(shell pwd) $(TLIBS:%=-l%)

#compiling the precompiled header
%.h.gch:%.h $(CMF)
	@echo "Precompiling header $@..."
	@$(CC) -o $@ -c $< || echo "error. Disabling precompiled header"
	@echo "...Done"

#compiling source files
$(OBJSDIR)/%.o: %.c $(CMF)
	@dirname $@ | xargs mkdir -p 2>/dev/null || echo "$@ already exists" >/dev/null
	$(CC) $(CFLAGS) -I$(shell pwd) $(PCH_FILE:%.h=-include %.h) -c $< -o $@

