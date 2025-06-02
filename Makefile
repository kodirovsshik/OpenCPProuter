
override eregex := -regextype egrep -regex
override debugprint = @echo Making $@ with $^

define find_files
	$(shell find "./$(1)" $(eregex) ".*\.$(2)")
endef

override WARN := -Wall -Wextra -Werror



.PHONY: all
all: all-with-known-dependencies

override ALL_DEPENDENCIES :=
include $(wildcard */Makefile.inc)

all-with-known-dependencies: $(ALL_DEPENDENCIES)

.PHONY: clear clean
clear clean:
	find . $(eregex) "(.*\/)?(~.+|.+~|#.+#|.*\.(obj|o|d|elf|efi|bin|iso))" -exec rm {} \;
	rm -rf ESP





# Development QoL

override QEMU_EFI_x86_64 := qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive if=virtio,format=raw,file=fat:rw:./ESP

ESP:
	mkdir -p ESP/EFI/BOOT

.PHONY: ESP-update
ESP-update: all | ESP
	cp $(KERNEL_EFI) ESP/EFI/BOOT/BOOTX64.efi

.PNONY: run
run: ESP-update
	$(QEMU_EFI_x86_64)

.PHONY: debug
debug: ESP-update
	$(QEMU_EFI_X86_64) -s -S >qemu_out.log 2>qemu_err.log &
#debugger invocation here
