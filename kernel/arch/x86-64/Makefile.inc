
override KERNEL_ARCH_NASM_SRC := $(call find_files,$(ARCHDIR),asm)
override KERNEL_ARCH_OTHER_OBJ := $(KERNEL_ARCH_NASM_SRC:.asm=.asm.obj)

# Customization point: "make NASM=my-custom-nasm" to override default nasm
NASM := nasm
# Customization point: "make NASM_ARGS='...'" to append custom args to nasm
NASM_ARGS :=

override NASM := "$(NASM)" -Wall -Werror $(NASM_ARGS) -f elf64
OBJCOPY := objcopy
# ^^^ TODO: make it a customization point

$(ARCHDIR)/%.asm.obj: $(ARCHDIR)/%.asm
	$(NASM) $^ -o $@ -g
	$(OBJCOPY) --add-section ".note.GNU-stack=/dev/null" --set-section-flags ".note.GNU-stack=contents,readonly" $@
