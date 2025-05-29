
override ALL_DEPENDENCIES += none_arch_error

.PHONY: none_arch_error
none_arch_error:
	echo ARCH must be one of $(ARCH_LIST)