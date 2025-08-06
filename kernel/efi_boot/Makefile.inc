
# Customization point: "make EFI_CXX=x86_64-efi-g++" to override C++ compiler for EFI applications
EFI_CXX := clang++
# Customization point: "make 'EFI_CXX_ARGS=...'" to override arguments for C++ compiler for EFI applications
EFI_CXX_ARGS := --target=x86_64-unknown-windows-efi -I/usr/include/efi -mno-red-zone \
-fuse-ld=lld-link -nostdlib -Wl,-entry:efi_main -Wl,-subsystem:efi_application -Wl,-largeaddressaware -mgeneral-regs-only -Ikernel/include -Wno-c99-designator -std=c++26 -Wno-c23-extensions -g

override EFI_CXX := "$(EFI_CXX)" $(WARN) $(BAREMETAL_ARGS_COMPILE) $(BAREMETAL_ARGS_LINK) $(EFI_CXX_ARGS)
override EFI_CPP_SRC := $(call find_files,kernel/efi_boot,cpp)



override KERNEL_BIN := $(ARCHDIR)/kernel.bin
override KERNEL_EFI := $(ARCHDIR)/kernel.efi

$(KERNEL_BIN): $(KERNEL_ELF)
	objcopy -O binary $^ $@

$(KERNEL_EFI): $(KERNEL_BIN) $(EFI_CPP_SRC)
	$(EFI_CXX) -o $@ $(EFI_CPP_SRC) "-DKERNEL_BIN=$(KERNEL_BIN:kernel/%=../%)"

override ALL_DEPENDENCIES += kernel_efi
.PHONY: kernel_efi
kernel_efi: $(KERNEL_EFI)
