
include kernel/arch/Makefile.inc
include kernel/src/Makefile.inc


override ALL_DEPENDENCIES += kernel
override KERNEL_ELF := $(ARCHDIR)/kernel.elf

.PHONY: kernel
kernel: $(KERNEL_ELF)

$(KERNEL_ELF): $(KERNEL_COMMON_ELF) $(KERNEL_ARCH_ELF)
