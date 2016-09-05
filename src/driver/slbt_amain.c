/*******************************************************************/
/*  slibtool: a skinny libtool implementation, written in C        */
/*  Copyright (C) 2016  Z. Gilboa                                  */
/*  Released under the Standard MIT License; see COPYING.SLIBTOOL. */
/*******************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <slibtool/slibtool.h>
#include "slibtool_driver_impl.h"

#ifndef SLBT_DRIVER_FLAGS
#define SLBT_DRIVER_FLAGS	SLBT_DRIVER_VERBOSITY_ERRORS \
				| SLBT_DRIVER_VERBOSITY_USAGE
#endif

static const char vermsg[] = "%s%s%s (git://midipix.org/slibtool): "
			     "version %s%d.%d.%d%s.\n"
			     "%s%s%s%s%s\n";

static const char * const slbt_ver_color[6] = {
		"\x1b[1m\x1b[35m","\x1b[0m",
		"\x1b[1m\x1b[32m","\x1b[0m",
		"\x1b[1m\x1b[34m","\x1b[0m"
};

static const char * const slbt_ver_plain[6] = {
		"","",
		"","",
		"",""
};

static ssize_t slbt_version(struct slbt_driver_ctx * dctx)
{
	const struct slbt_source_version * verinfo;
	const char * const * verclr;
	bool gitver;

	verinfo = slbt_source_version();
	verclr  = isatty(STDOUT_FILENO) ? slbt_ver_color : slbt_ver_plain;
	gitver  = strcmp(verinfo->commit,"unknown");

	return fprintf(stdout,vermsg,
			verclr[0],dctx->program,verclr[1],
			verclr[2],verinfo->major,verinfo->minor,
			verinfo->revision,verclr[3],
			gitver ? "[commit reference: " : "",
			verclr[4],gitver ? verinfo->commit : "",
			verclr[5],gitver ? "]" : "");
}

static void slbt_perform_driver_actions(struct slbt_driver_ctx * dctx)
{
	if (dctx->cctx->drvflags & SLBT_DRIVER_CONFIG)
		slbt_output_config(dctx);

	if (dctx->cctx->mode == SLBT_MODE_COMPILE)
		slbt_exec_compile(dctx,0);

	if (dctx->cctx->mode == SLBT_MODE_EXECUTE)
		slbt_exec_execute(dctx,0);

	if (dctx->cctx->mode == SLBT_MODE_INSTALL)
		slbt_exec_install(dctx,0);

	if (dctx->cctx->mode == SLBT_MODE_LINK)
		slbt_exec_link(dctx,0);
}

static void slbt_perform_unit_actions(struct slbt_unit_ctx * uctx)
{
	(void)uctx;
}

static int slbt_exit(struct slbt_driver_ctx * dctx, int ret)
{
	slbt_output_error_vector(dctx);
	slbt_free_driver_ctx(dctx);
	return ret;
}

int slbt_main(int argc, char ** argv, char ** envp)
{
	int				ret;
	uint64_t			flags;
	struct slbt_driver_ctx *	dctx;
	struct slbt_unit_ctx *		uctx;
	const char **			unit;
	char *				program;
	char *				dash;
	char *				sargv[5];

	/* --version only? */
	if ((argc == 2) && (!strcmp(argv[1],"--version")
				|| !strcmp(argv[1],"--help-all")
				|| !strcmp(argv[1],"--help")
				|| !strcmp(argv[1],"-h"))) {
		sargv[0] = argv[0];
		sargv[1] = argv[1];
		sargv[2] = "--mode=compile";
		sargv[3] = "<compiler>";
		sargv[4] = 0;

		return (slbt_get_driver_ctx(sargv,envp,SLBT_DRIVER_FLAGS,&dctx))
			? 2 : (slbt_version(dctx) < 0)
				? slbt_exit(dctx,2)
				: slbt_exit(dctx,0);
	}

	/* program */
	if ((program = strrchr(argv[0],'/')))
		program++;
	else
		program = argv[0];

	/* dash */
	if ((dash = strrchr(program,'-')))
		dash++;

	/* flags */
	if (dash == 0)
		flags = SLBT_DRIVER_FLAGS;

	else if (!(strcmp(dash,"shared")))
		flags = SLBT_DRIVER_FLAGS | SLBT_DRIVER_DISABLE_STATIC;

	else if (!(strcmp(dash,"static")))
		flags = SLBT_DRIVER_FLAGS | SLBT_DRIVER_DISABLE_SHARED;

	else
		flags = SLBT_DRIVER_FLAGS;

	/* debug */
	if (!(strcmp(program,"dlibtool")))
		flags |= SLBT_DRIVER_DEBUG;

	else if (!(strncmp(program,"dlibtool",8)))
		if ((program[8] == '-') || (program[8] == '.'))
			flags |= SLBT_DRIVER_DEBUG;

	/* legabits */
	if (!(strcmp(program,"clibtool")))
		flags |= SLBT_DRIVER_LEGABITS;

	else if (!(strncmp(program,"clibtool",8)))
		if ((program[8] == '-') || (program[8] == '.'))
			flags |= SLBT_DRIVER_LEGABITS;

	/* driver context */
	if ((ret = slbt_get_driver_ctx(argv,envp,flags,&dctx)))
		return (ret == SLBT_USAGE) ? !--argc : 2;

	if (dctx->cctx->drvflags & SLBT_DRIVER_VERSION)
		if ((slbt_version(dctx)) < 0)
			return slbt_exit(dctx,2);

	slbt_perform_driver_actions(dctx);

	for (unit=dctx->units; *unit; unit++) {
		if (!(slbt_get_unit_ctx(dctx,*unit,&uctx))) {
			slbt_perform_unit_actions(uctx);
			slbt_free_unit_ctx(uctx);
		}
	}

	return slbt_exit(dctx,dctx->errv[0] ? 2 : 0);
}
