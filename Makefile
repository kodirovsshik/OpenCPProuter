
override eregex := -regextype egrep -regex
override debugprint = @echo Making $@ with $^

define find_files
	$(shell find "./$(1)" $(eregex) ".*\.$(2)")
endef

override WARN := -Wall -Wextra -Werror



.PHONY: all
all: all-with-known-dependencies

override ALL_DEPENDENCIES :=
include $(wildcard */Makefile.inc)

all-with-known-dependencies: $(ALL_DEPENDENCIES)

.PHONY: clean
clean:
	find . $(eregex) "(.*\/)?(~.+|.+~|#.+#|.*\.(obj|o|d|elf|efi|bin|iso))" -exec rm {} \;
	rm -rf ESP 
