
#Guards against overwriting ARCH in case it is defined as an environment variable
ifeq "$(ARCH)" ""
#Customization point: "make ARCH=arch_name" to speciy kernel architecture. When ARCH is ommited, a complete list of supported architectures is printed
ARCH := help
endif

override ARCHDIR := kernel/arch/$(ARCH)

override KERNEL_ARCH_ELF := $(ARCHDIR)/kernel_arch.elf
override KERNEL_ARCH_CPP_SRC := $(call find_files,$(ARCHDIR),cpp)
override KERNEL_ARCH_CPP_OBJ := $(KERNEL_ARCH_CPP_SRC:.cpp=.cpp.o)

include $(ARCHDIR)/Makefile.inc

$(KERNEL_ARCH_ELF): $(KENREL_ARCH_CPP_OBJ) $(KERNEL_ARCH_OTHER_OBJ)
	$(debugprint)