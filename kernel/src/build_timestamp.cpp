
#define PERSISTENT_STRING(section_name, var_name, value) extern "C" volatile const char var_name[] __attribute__((section(section_name))) = value

PERSISTENT_STRING(".note.build-date", kernel_build_date, __DATE__);
PERSISTENT_STRING(".note.build-time", kernel_build_time, __TIME__);
