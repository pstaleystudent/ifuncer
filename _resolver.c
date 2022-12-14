//This is required for operation!
//Dollar signs will be replaced with function names during compilation
#include <stddef.h>
#include <stdio.h>
#include <sys/auxv.h>
//extern void $();
extern void _IFUNCER_$_SVE2();
extern void _IFUNCER_$_SVE();
extern void _IFUNCER_$_SIMD();
extern void _IFUNCER_$_NONE();
extern void $() __attribute__((ifunc ("resolve_$")));
static void *resolve_$(void) {
    printf("_RESOLVER: Starting Resolve\n");
    unsigned long cap1 = getauxval(AT_HWCAP);
    unsigned long cap2 = getauxval(AT_HWCAP2);
    if(cap2 & HWCAP2_SVE2){
        printf("_RESOLVER: Resolved SVE2\n");
        return _IFUNCER_$_SVE2;
    }
    if(cap1 & HWCAP_SVE){
        printf("_RESOLVER: Resolved SVE\n");
        return _IFUNCER_$_SVE;
    }
    if(cap1 & HWCAP_ASIMD){
        printf("_RESOLVER: Resolved ASIMD\n");
        return _IFUNCER_$_SIMD;
    }
    printf("_RESOLVER: Resolved NONE\n");
    return _IFUNCER_$_NONE;
}