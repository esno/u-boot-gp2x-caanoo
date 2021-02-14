/*
 * This was automagically generated from arch/arm/tools/mach-types!
 * Do NOT edit
 */

//#define MACH_TYPE_POLLUX               2007
#define MACH_TYPE_POLLUX               1950
#ifdef CONFIG_MACH_POLLUX
# ifdef machine_arch_type
#  undef machine_arch_type
#  define machine_arch_type	__machine_arch_type
# else
#  define machine_arch_type	MACH_TYPE_POLLUX
# endif
# define machine_is_pollux()	(machine_arch_type == MACH_TYPE_POLLUX)
#else
# define machine_is_pollux()	(0)
#endif
