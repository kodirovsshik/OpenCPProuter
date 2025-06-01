
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

clean:
	find . $(eregex) "(.*\/)?(~.+|.+~|#.+#|.*\.(obj|o|d|elf|bin|iso))" -exec rm {} \;
	rm -r ESP

ESP:
	mkdir -p ESP/EFI/BOOT
run: all | ESP
	cp $(KERNEL_EFI) ESP/EFI/BOOT/BOOTX64.efi
	qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive if=virtio,format=raw,file=fat:rw:./ESP
