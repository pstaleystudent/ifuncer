//This is required for operation!
# include <stddef.h>
void foo(unsigned *data, size_t len);

extern void _IFUNCER_foo_SVE2(unsigned *data, size_t len);
extern void _IFUNCER_foo_SVE(unsigned *data, size_t len);
extern void _IFUNCER_foo_NEON(unsigned *data, size_t len);
extern void _IFUNCER_foo_NONE(unsigned *data, size_t len);

void foo(unsigned *data, size_t len) __attribute__((ifunc ("resolve_foo")));

static void *resolve_foo(void)
{
    if (__builtin_cpu_supports("sve2"))
        return _IFUNCER_foo_SVE2;
    else if (__builtin_cpu_supports("sve"))
        return _IFUNCER_foo_SVE;
    else if (__builtin_cpu_supports("simd"))
        return _IFUNCER_foo_NEON;
    else
        return _IFUNCER_foo_NONE;
}