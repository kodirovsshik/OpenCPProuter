
override KERNEL_ARCH_NASM_SRC := $(call find_files,$(ARCHDIR),asm)
override KERNEL_ARCH_OTHER_OBJ := $(KERNEL_ARCH_NASM_SRC:.asm=.asm.obj)

# Customization point: "make NASM=my-custom-nasm" to override default nasm
NASM := nasm
# Customization point: "make NASM_ARGS='...'" to append custom args to nasm
NASM_ARGS :=

$(ARCHDIR)/%.asm.obj: $(ARCHDIR)/%.asm
	"$(NASM)" $(NASM_ARGS) -Wall -Werror -f elf64 $^ -o $@

override ARCH_LINK_ARGS := -T $(ARCHDIR)/link.ld