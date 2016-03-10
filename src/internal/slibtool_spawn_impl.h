/*******************************************************************/
/*  slibtool: a skinny libtool implementation, written in C        */
/*  Copyright (C) 2016  Z. Gilboa                                  */
/*  Released under the Standard MIT License; see COPYING.SLIBTOOL. */
/*******************************************************************/

#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>

#ifndef SLBT_USE_FORK
#ifndef SLBT_USE_VFORK
#ifndef SLBT_USE_POSIX_SPAWN
#define SLBT_USE_POSIX_SPAWN
#endif
#endif
#endif

#ifdef  SLBT_USE_POSIX_SPAWN
#include <spawn.h>
#endif

extern char ** environ;

static inline int slbt_spawn(
	struct slbt_exec_ctx *	ectx,
	bool			fwait)
{
	pid_t	pid;


#ifdef SLBT_USE_POSIX_SPAWN

	if (posix_spawnp(
			&pid,
			ectx->program,
			0,0,
			ectx->argv,
			ectx->envp ? ectx->envp : environ))
		pid = -1;

#else

#ifdef SLBT_USE_FORK
	pid = fork();
#else
	pid = vfork();
#endif

#endif

	if (pid < 0)
		return -1;

	if (pid == 0)
		return execvp(
			ectx->program,
			ectx->argv);

	ectx->pid = pid;

	if (fwait)
		return waitpid(
			pid,
			&ectx->exitcode,
			0);

	return 0;
}
