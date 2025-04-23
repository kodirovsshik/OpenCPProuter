
build_dir := build
kernel_elf_name := kernel.elf
override kernel_elf := $(build_dir)/$(kernel_elf_name)

define collect_objects
	$(patsubst %.$(1),%.$(1).obj,$(shell find src -name "*.$(1)"))
endef

kernel_objects := $(call collect_objects,cpp) $(call collect_objects,asm)
kernel_headers := $(shell find src/include -name "*.hpp")

#Proper target paths are assumed to be configured in the compiler target config
target := x86_64-unknown-none-elf
override target_flag := --target=$(target)

override Cflags :=
override Cflags += $(target_flag)
override Cflags += -ffreestanding
override Cflags += -mno-red-zone
override Cflags += -Wall -Wextra -Werror
override Cflags += -masm=intel

override link_script := src/link.ld
override Lflags :=
override Lflags += $(target_flag)
override Lflags += -fuse-ld=lld
override Lflags += -T $(link_script)
override Lflags += -static
override Lflags += -Wl,--fatal-warnings

DEBUG := true
ifeq '$(DEBUG)' 'true'
 override Cflags += -Og -g -D_DEBUG
else
 ifneq '$(DEBUG)' 'false'
  $(error DEBUG must be either true (default) or false)
 endif
 override Cflags += -O3 -DNDEBUG
endif


CXX := clang++
NASM := nasm

.PHONY: all kernel
all kernel: $(kernel_elf)

$(build_dir):
	mkdir -p $@

.PHONY: iso
iso: $(build_dir)/boot.iso

$(build_dir)/boot.iso: $(kernel_elf)
	mkdir -p $(build_dir)/iso_root/boot/grub
	cp $(kernel_elf) $(build_dir)/iso_root

	sed '' <src/grub.cfg >$(build_dir)/iso_root/boot/grub/grub.cfg

	grub-mkrescue -o $@ $(build_dir)/iso_root
	#rm -r $(build_dir)/iso_root


.PHONY: run
run: iso
	qemu-system-x86_64 -cdrom $(build_dir)/boot.iso -no-reboot


%.cpp.obj: %.cpp
	"$(CXX)" $(CXX_FLAGS) $(Cflags) -Isrc/include -c "$<" -o "$@"

%.asm.obj: %.asm
	"$(NASM)" -f elf64 "$<" -o "$@"

$(kernel_elf): $(kernel_objects) $(link_script) | $(build_dir)
	"$(CXX)" $(LINK_FLAGS) $(Lflags) $(kernel_objects) -o "$@"
	grub-file --is-x86-multiboot2 "$@"



.PHONY: clean
clean:
	find src/ \
		-name "*.o" -o \
		-name "*.d" -o \
		-name "*~" -o \
		-name "#*" \
	| xargs rm -f
	rm -rf $(build_dir)
