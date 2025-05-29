
include kernel/arch/Makefile.inc

define find_sources
	$(shell find "kernel/$(1)" -regextype egrep -regex ".*\.$(2)")
endef

override KERNEL_COMMON_SRC := $(call find_sources,src,c(pp)?)

override ALL_DEPENDENCIES += kernel

.PHONY: kernel
kernel: