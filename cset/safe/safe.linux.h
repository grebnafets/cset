#ifdef _UNISTD_H
#include <cset/gate/gate.h>
pid_t cset_safe_fork()
{
	pid_t pid;
	cset_gate_Lock(&cset_gate_Mem);
	pid = fork();
	cset_gate_Unlock(&cset_gate_Mem);
	return pid;
}
#endif
