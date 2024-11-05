#include "sail.h"
#include "riscv_platform_impl.h"

typedef struct _CacheReplacementPolicy CacheReplacementPolicy;

struct _CacheReplacementPolicy {
	void (*travel)(CacheReplacementPolicy *);
};