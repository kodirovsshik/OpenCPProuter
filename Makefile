
override ALL_DEPENDENCIES :=

override eregex := -regextype egrep -regex

.PHONY: all
all: all-with-known-dependencies

include kernel/Makefile.inc


all-with-known-dependencies: $(ALL_DEPENDENCIES)

clean:
	find . $(eregex) "(\.\/)?(~.+|.+~|#.+#|.*\.(o|d|elf|bin|iso))" -exec rm {} \;
