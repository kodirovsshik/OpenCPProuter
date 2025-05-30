
override eregex := -regextype egrep -regex
override debugprint = @echo Making $@ with $^

define find_files
	$(shell find "./$(1)" $(eregex) ".*\.$(2)")
endef



.PHONY: all
all: all-with-known-dependencies

override ALL_DEPENDENCIES :=
include $(wildcard */Makefile.inc)

all-with-known-dependencies: $(ALL_DEPENDENCIES)

clean:
	find . $(eregex) "(\.\/)?(~.+|.+~|#.+#|.*\.(o|d|elf|bin|iso))" -exec rm {} \;
