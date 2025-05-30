
override ALL_DEPENDENCIES += arch_help
override ARCH_LIST := $(shell find kernel/arch/* -maxdepth 0 -type d ! -name "help" -printf "%f\n")

.PHONY: arch_help
arch_help:
	@echo ARCH must be defined one of: $(ARCH_LIST)
	@echo "	Example: make ARCH=x86-64"
	@echo "	Example: ARCH=x86-64 make"
	@echo "	Example: export ARCH=x86-64; make"
	@false