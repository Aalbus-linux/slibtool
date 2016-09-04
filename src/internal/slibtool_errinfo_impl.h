/*******************************************************************/
/*  slibtool: a skinny libtool implementation, written in C        */
/*  Copyright (C) 2016  Z. Gilboa                                  */
/*  Released under the Standard MIT License; see COPYING.SLIBTOOL. */
/*******************************************************************/

#include <errno.h>
#include <slibtool/slibtool.h>

int slbt_record_error(
	const struct slbt_driver_ctx *,
	int		syserror,
	int		liberror,
	const char *	function,
	int		line,
	unsigned	flags,
	void *		ctx);

#define SLBT_SYSTEM_ERROR(dctx)           \
	slbt_record_error(                \
		dctx,                     \
		errno,                    \
		0,                        \
		__func__,                 \
		__LINE__,                 \
		SLBT_ERROR_TOP_LEVEL,     \
		0)

#define SLBT_BUFFER_ERROR(dctx)           \
	slbt_record_error(                \
		dctx,                     \
		ENOBUFS,                  \
		0,                        \
		__func__,                 \
		__LINE__,                 \
		SLBT_ERROR_TOP_LEVEL,     \
		0)

#define SLBT_SPAWN_ERROR(dctx)            \
	slbt_record_error(                \
		dctx,                     \
		errno,                    \
		0,                        \
		__func__,                 \
		__LINE__,                 \
		SLBT_ERROR_TOP_LEVEL      \
		| (errno ? 0              \
		   : SLBT_ERROR_CHILD),   \
		0)

#define SLBT_FILE_ERROR(dctx)             \
	slbt_record_error(                \
		dctx,                     \
		EIO,                      \
		0,                        \
		__func__,                 \
		__LINE__,                 \
		SLBT_ERROR_TOP_LEVEL,     \
		0)

#define SLBT_CUSTOM_ERROR(dctx,liberror)  \
	slbt_record_error(                \
		dctx,                     \
		0,                        \
		liberror,                 \
		__func__,                 \
		__LINE__,                 \
		SLBT_ERROR_TOP_LEVEL      \
		| SLBT_ERROR_CUSTOM,      \
		0)

#define SLBT_NESTED_ERROR(dctx)           \
	slbt_record_error(                \
		dctx,                     \
		0,                        \
		0,                        \
		__func__,                 \
		__LINE__,                 \
		SLBT_ERROR_NESTED,        \
		0)
