
build_dir := build
kernel_elf_name := kernel.elf
override kernel_elf := $(build_dir)/$(kernel_elf_name)

kernel_sources := $(shell find src -name "*.cpp")
kernel_objects := $(patsubst %.cpp,%.cpp.o,$(kernel_sources))
kernel_headers := $(shell find src/include -name "*.hpp")

target := x86_64-unknown-none-elf
#Configuring include and library directories is up to the compiler config
override target_flag := --target=$(target)

override Cflags :=
override Cflags += $(target_flag)
override Cflags += -ffreestanding
override Cflags += -mno-red-zone
override Cflags += -Wall -Wextra -Werror

override link_script := src/link.ld
override Lflags :=
override Lflags += $(target_flag)
override Lflags += -fuse-ld=lld
override Lflags += -T $(link_script)
override Lflags += -static


CXX := clang++

.PHONY: all kernel
all kernel: $(kernel_elf)

$(build_dir):
	mkdir -p $@



%.cpp.o: %.cpp
	"$(CXX)" $(CXX_FLAGS) $(Cflags) -Isrc/include -c "$<" -o "$@"

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
