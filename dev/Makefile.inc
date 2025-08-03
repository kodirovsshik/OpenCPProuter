# Development QoL things

override QEMU_EFI_x86_64 := qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive if=virtio,format=raw,file=fat:rw:./ESP

ESP:
	mkdir -p ESP/EFI/BOOT

.PHONY: ESP-update
ESP-update: $(KERNEL_EFI) | ESP
	cp $(KERNEL_EFI) ESP/EFI/BOOT/BOOTX64.efi

.PNONY: run
run: ESP-update
	$(QEMU_EFI_x86_64)

.PHONY: debug
debug: ESP-update
	$(QEMU_EFI_X86_64) -s -S >qemu_out.log 2>qemu_err.log &
# TODO: insert debugger invocation here

include dev/Makefile2.inc
