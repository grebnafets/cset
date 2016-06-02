#ifndef CSET_GATE_CROSS_PLATFORM_PRE
#define CSET_GATE_CROSS_PLATFORM_PRE 1

// Possible hooks for every function and do while macro.
// You can test this with #define CSET_GATE_PRE printf("%s\n", __func__);
// Add it _before_ you add test.h
#ifndef CSET_GATE_PRE
	#define CSET_GATE_PRE
#endif
#ifndef CSET_GATE_POST
	#define CSET_GATE_POST
#endif

// {{{

#define cset_gate_Gate volatile int
#define cset_gate_Pass volatile int

cset_gate_Pass cset_gate_Mem;

extern inline void
cset_gate_Enter
(cset_gate_Gate *gate, cset_gate_Pass *pass)
{CSET_GATE_PRE
	asm volatile (
		"jmp cset_gate_Enter_check\n"
		"cset_gate_Enter_wait:\n"
		"pause\n"
		"cset_gate_Enter_check:\n"
		"cmp %[lock], %[checkin]\n"   // Skip if pass >= lock
		"jge cset_gate_Enter_skip\n"
		"mov %[lock], %%eax\n"        // Spinlock start
		"cmp %[gate], %%eax\n"        // Check if locked
		"je cset_gate_Lock_wait\n"    // If locked, wait.
		"lock xchg %%eax, %[gate]\n"
		"test %%eax, %%eax\n"
		"jnz cset_gate_Enter_wait\n"  // Spinlock end
		"cset_gate_Enter_skip:\n"
		"add %[lock], %[checkin]\n"   // Checkin pass
		: [gate] "=m" (*gate), [checkin] "=m" (*pass)
		: [lock] "r" (1)
		: "eax"
	);
CSET_GATE_POST
}

extern inline void
cset_gate_Lock
(cset_gate_Gate *gate)
{CSET_GATE_PRE
	asm volatile (
		"jmp cset_gate_Lock_check\n" // Renegade selected, lets skip the line!
		"cset_gate_Lock_wait:\n"     // Honest citizens wait in line.
		"pause\n"                    // Stroke beard, check phone/watch.
		"cset_gate_Lock_check:\n"    // Ok, lets do this...
		"mov %[lock], %%eax\n"       // eax = 1
		"cmp %[gate], %%eax\n"       // Check if locked
		"je cset_gate_Lock_wait\n"   // If locked, wait.
		"lock xchg %%eax, %[gate]\n" // Exhange eax with gate value.
		"test %%eax, %%eax\n"        // 1 = closed, 0 = open.
		"jnz cset_gate_Lock_wait\n"  // Ohhh man, here I go again...
		: [gate] "=m" (*gate)
		: [lock] "r" (1)
		: "eax"                      // Inform compiler you want eax.
	);
CSET_GATE_POST
}

extern inline void
cset_gate_Unlock
(cset_gate_Gate *gate)
{CSET_GATE_PRE
	asm volatile (
		"mov %[unlock], %[gate]\n"
		: [gate] "=m" (*gate)
		: [unlock] "r" (0)
	);
CSET_GATE_POST
}

// TODO: if isLast: unlock gate; else: only checkout pass;
extern inline void
cset_gate_Leave
(cset_gate_Gate *gate, cset_gate_Pass *pass)
{CSET_GATE_PRE
	asm volatile (
		"cmp %[pass], %[isLast]\n"
		"jg cset_gate_Leave_skip\n"
		"mov %[unlock], %[gate]\n"
		"cset_gate_Leave_skip:\n"
		"add %[checkout], %[pass]\n"
		: [gate] "=m" (*gate), [pass] "=m" (*pass)
		: [unlock] "r" (0), [checkout] "r" (-1), [isLast] "r" (1)
	);
CSET_GATE_POST
}
// }}}

#endif // CSET_GATE_CROSS_PLATFORM_PRE
