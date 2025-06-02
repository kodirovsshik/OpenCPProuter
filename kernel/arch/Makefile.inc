
ifeq "$(ARCH)" ""

# Customization point: "make ARCH=arch_name" to speciy kernel architecture. When ARCH is ommited, a complete list of supported architectures is printed

# I love makefiles. Here's how to print a tab in $(info):
override NULL :=
override TAB := $(NULL)	$(NULL)

override ARCH_LIST := $(shell find kernel/arch/* -maxdepth 0 -type d -printf "%f\n")

$(info ARCH must be defined one of: $(ARCH_LIST))
$(info $(TAB)Example: make ARCH=x86-64)
$(info $(TAB)Example: ARCH=x86-64 make)
$(info $(TAB)Example: export ARCH=x86-64; bash -c make)
$(error Please specify the ARCH variable)

endif



override ARCHDIR := kernel/arch/$(ARCH)

override KERNEL_ARCH_ELF := $(ARCHDIR)/kernel_arch.elf
override KERNEL_ARCH_CPP_SRC := $(call find_files,$(ARCHDIR),cpp)
override KERNEL_ARCH_CPP_OBJ := $(KERNEL_ARCH_CPP_SRC:.cpp=.cpp.obj)

include $(ARCHDIR)/Makefile.inc

$(KERNEL_ARCH_ELF): $(KERNEL_ARCH_CPP_OBJ) $(KERNEL_ARCH_OTHER_OBJ)
	$(KERNEL_CXX) $^ -o $@ -r

kernel/arch/$(ARCH)/%.cpp.obj: kernel/arch/$(ARCH)/%.cpp
	$(KERNEL_CXX) $^ -o $@ -c $(BAREMETAL_ARGS_COMPILE)