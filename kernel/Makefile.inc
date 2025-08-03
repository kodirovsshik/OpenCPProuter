
include kernel/arch/Makefile.check_arch.inc

include $(ARCHDIR)/Makefile.arch_vars.inc
# ^^^ this file is expected to provide:
# ARCH_{KERNEL,BAREMETAL}_ARGS_{COMPILE,LINK}
# where BAREMETAL = applicable for both bootloader and kernel

override BAREMETAL_ARGS_COMPILE := -ffreestanding -nostdlibinc -nostdinc++ $(ARCH_BAREMETAL_ARGS_COMPILE)
override BAREMETAL_ARGS_LINK := -nodefaultlibs -nostartfiles -static $(ARCH_BAREMETAL_ARGS_LINK)

override KERNEL_ARGS_COMPILE := $(BAREMETAL_ARGS_COMPILE) $(ARCH_KERNEL_ARGS_COMPILE)
override KERNEL_ARGS_LINK := $(BAREMETAL_ARGS_LINK) $(ARCH_KERNEL_ARGS_LINK)



# Customization point: "make KERNEL_CXX=clang-21" to override C++ compiler used for kernel
KERNEL_CXX := clang++
# Customization point: "make KERNEL_CXX_ARGS='--target=my_baremetal_target'" to specify arguments for a custom C++ compiler for kernel (all invocations)
KERNEL_CXX_ARGS := --target=x86_64-unknown-none-elf

# Customization point: "make KERNEL_CXX_ARGS_COMPILE='...'" to specify C++ compiler args for kernel (compilation only)
KERNEL_CXX_ARGS_COMPILE :=
# Customization point: "make KERNEL_CXX_ARGS_LINK='...'" to specify C++ compiler args for kernel (linking only)
KERNEL_CXX_ARGS_LINK :=
# Customization point: "make KERNEL_CXX_ARGS_PARTIAL_LINK='...'" to specify C++ compiler args for kernel (partial linking only)
KERNEL_CXX_ARGS_PARTIAL_LINK :=

override _KERNEL_CXX_ARGS_COMPILE := -fPIE
override _KERNEL_CXX_ARGS_LINK := -Wl,--orphan-handling=error -Wl,--fatal-warnings -fuse-ld=lld
# ^^^ TODO: come up with a proper names for these

override KERNEL_CXX := "$(KERNEL_CXX)" $(KERNEL_CXX_ARGS)
override KERNEL_CXX_COMPILE := $(KERNEL_CXX) $(WARN) $(_KERNEL_CXX_ARGS_COMPILE) $(KERNEL_CXX_ARGS_COMPILE) $(KERNEL_ARGS_COMPILE) -c
override KERNEL_CXX_LINK := $(KERNEL_CXX) $(_KERNEL_CXX_ARGS_LINK) $(KERNEL_CXX_ARGS_LINK) $(KERNEL_ARGS_LINK)
override KERNEL_CXX_LINK_PARTIAL := $(KERNEL_CXX) $(KERNEL_CXX_ARGS_PARTIAL_LINK) -r



include kernel/arch/Makefile.inc
include kernel/src/Makefile.inc



override KERNEL_ELF := $(ARCHDIR)/kernel.elf

$(KERNEL_ELF): $(KERNEL_COMMON_ELF) $(KERNEL_ARCH_ELF)
	$(KERNEL_CXX_LINK) -Wl,--defsym=BUILD_TIMESTAMP=$(shell date +%s) -T $(ARCHDIR)/link.ld $^ -o $@

override ALL_DEPENDENCIES += kernel
.PHONY: kernel
kernel: $(KERNEL_ELF)

include kernel/efi_boot/Makefile.inc
