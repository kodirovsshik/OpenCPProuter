
ifeq "$(ARCH)" ""

# Customization point: "make ARCH=arch_name" to speciy kernel architecture. When ARCH is ommited, a complete list of supported architectures is printed

# I love makefiles. Here's how to print a tab in $(info):
override NULL :=
override TAB := $(NULL)	$(NULL)

override ARCH_LIST := $(shell find kernel/arch/* -maxdepth 0 -type d -printf "%f\n")

$(info ARCH must be defined to be one of: $(ARCH_LIST))
$(info $(TAB)Example: make ARCH=x86-64)
$(info $(TAB)Example: ARCH=x86-64 make)
$(info $(TAB)Example: export ARCH=x86-64; bash -c make)
$(error Please define the ARCH variable)

endif

override ARCHDIR := kernel/arch/$(ARCH)
