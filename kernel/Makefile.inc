
override BAREMETAL_ARGS_COMPILE := -ffreestanding -nostdlibinc -nostdinc++
override BAREMETAL_ARGS_LINK := -nodefaultlibs -nostartfiles

override KERNEL_ARGS_COMPILE := $(BAREMETAL_ARGS_COMPILE) -fpie
override KERNEL_ARGS_LINK := $(BAREMETAL_ARGS_LINK) -static-pie -fuse-ld=lld
#TODO: a way for arch-specific makefile to introduce -mno-red-zone

# Customization point: "make KERNEL_CXX=clang-21" to override C++ compiler used for kernel
KERNEL_CXX := clang++
# Customization point: "make KERNEL_CXX_ARGS='--target=my_baremetal_target'" to specify arguments for a custom C++ compiler for kernel
KERNEL_CXX_ARGS := --target=x86_64-unknown-none-elf
override KERNEL_CXX := "$(KERNEL_CXX)" $(WARN) $(KERNEL_CXX_ARGS)

include kernel/arch/Makefile.inc
include kernel/src/Makefile.inc

override KERNEL_ELF := $(ARCHDIR)/kernel.elf

$(KERNEL_ELF): $(KERNEL_COMMON_ELF) $(KERNEL_ARCH_ELF)
	$(KERNEL_CXX) $^ -o $@ $(KERNEL_ARGS_LINK) $(ARCH_ARGS_LINK) -Wl,--defsym=BUILD_TIMESTAMP=$(shell date +%s)

override ALL_DEPENDENCIES += kernel
.PHONY: kernel
kernel: $(KERNEL_ELF)

include kernel/efi_boot/Makefile.inc
