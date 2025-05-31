
ifeq "$(ARCH)" ""

# Customization point: "make ARCH=arch_name" to speciy kernel architecture. When ARCH is ommited, a complete list of supported architectures is printed
ARCH := help

# Wrapped in ifeq because assignment would overwrite ARCH is defined via environment
endif


override ARCHDIR := kernel/arch/$(ARCH)

override KERNEL_ARCH_ELF := $(ARCHDIR)/kernel_arch.elf
override KERNEL_ARCH_CPP_SRC := $(call find_files,$(ARCHDIR),cpp)
override KERNEL_ARCH_CPP_OBJ := $(KERNEL_ARCH_CPP_SRC:.cpp=.cpp.o)

include $(ARCHDIR)/Makefile.inc

$(KERNEL_ARCH_ELF): $(KERNEL_ARCH_CPP_OBJ) $(KERNEL_ARCH_OTHER_OBJ)
