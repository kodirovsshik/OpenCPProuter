
override KERNEL_COMMON_HEADER_DEPS := $(call find_files,kernel/src,d)
include $(KERNEL_COMMON_HEADER_DEPS)

override KERNEL_COMMON_ELF := $(ARCHDIR)/kernel_common.elf
override KERNEL_COMMON_CPP_SRC := $(call find_files,kernel/src,cpp)
override KERNEL_COMMON_CPP_OBJ := $(KERNEL_COMMON_CPP_SRC:.cpp=.cpp.obj)

$(KERNEL_COMMON_ELF): $(KERNEL_COMMON_CPP_OBJ)
	$(KERNEL_CXX) $^ -o $@ -r

kernel/src/%.cpp.obj: kernel/src/%.cpp
	$(KERNEL_CXX) $^ -o $@ -c $(BAREMETAL_ARGS_COMPILE)