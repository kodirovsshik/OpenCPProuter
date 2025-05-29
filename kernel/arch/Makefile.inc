
override ARCH_LIST := $(shell find kernel/arch/* -maxdepth 0 -type d ! -name "none" -printf "%f\n")

ARCH := none

include kernel/arch/$(ARCH)/Makefile.inc

