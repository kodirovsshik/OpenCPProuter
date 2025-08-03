
include $(ARCHDIR)/Makefile.inc
# ^^^ This file is expected to provide KERNEL_ARCH_OTHER_OBJ
# as well as the compilation rules for these object files (ex.: asm obj files)

override KERNEL_ARCH_ELF := $(ARCHDIR)/kernel_arch.elf
override KERNEL_ARCH_CPP_SRC := $(call find_files,$(ARCHDIR),cpp)
override KERNEL_ARCH_CPP_OBJ := $(KERNEL_ARCH_CPP_SRC:.cpp=.cpp.obj)

$(KERNEL_ARCH_ELF): $(KERNEL_ARCH_CPP_OBJ) $(KERNEL_ARCH_OTHER_OBJ)
	$(KERNEL_CXX_LINK_PARTIAL) $^ -o $@

$(ARCHDIR)/%.cpp.obj: $(ARCHDIR)/%.cpp
	$(KERNEL_CXX_COMPILE) $^ -o $@
