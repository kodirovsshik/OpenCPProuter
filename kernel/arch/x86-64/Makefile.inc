
override KERNEL_ARCH_NASM_SRC := $(call find_files,$(ARCHDIR),asm)
override KERNEL_ARCH_OTHER_OBJ := $(KERNEL_ARCH_NASM_SRC:.asm=.asm.elf)
# ^^^ I wanted to name asm object files .asm.o but it messes with make's internal rules

$(ARCHDIR)/%.asm.elf: $(ARCHDIR)/%.asm
	nasm -f elf64 $^ -o $@
