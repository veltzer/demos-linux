/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __err_utils_h
#define __err_utils_h

/*
 * This is a collection of helpers to help you deal with the Linux
 * error API.
 */

/* THIS IS A C FILE, NO C++ here */

#include <firstinclude.h>
#include <us_helper.h>	// for ARRAY_SIZEOF(), CHECK_ASSERT()
#include <stdio.h>	// for printf(3)
#include <string.h>	// for strcmp(3), strerror(3)

/*
 * List of all errors (can be found in 'man errno')
 */

typedef struct _err_val_and_name {
	int val;
	const char* name;
} err_val_and_name;

/*
 * List of all errors and their names. The reason for this is that the standard
 * C library does not provide a translation to/from error name to error value.
 * (it does provide some description of error values via strerror(3)).
 * Note: you cant use stringification macros here because they will substitute
 * the macro. Use only macros that don't substitute.
 */
#define entry(x) { x, # x }
static err_val_and_name err_tbl[]={
	entry(E2BIG),
	entry(EACCES),
	entry(EADDRINUSE),
	entry(EADDRNOTAVAIL),
	entry(EAFNOSUPPORT),
	entry(EAGAIN),
	entry(EALREADY),
	entry(EBADE),
	entry(EBADF),
	entry(EBADFD),
	entry(EBADMSG),
	entry(EBADR),
	entry(EBADRQC),
	entry(EBADSLT),
	entry(EBUSY),
	entry(ECANCELED),
	entry(ECHILD),
	entry(ECHRNG),
	entry(ECOMM),
	entry(ECONNABORTED),
	entry(ECONNREFUSED),
	entry(ECONNRESET),
	entry(EDEADLK),
	entry(EDEADLOCK),
	entry(EDESTADDRREQ),
	entry(EDOM),
	entry(EDQUOT),
	entry(EEXIST),
	entry(EFAULT),
	entry(EFBIG),
	entry(EHOSTDOWN),
	entry(EHOSTUNREACH),
	entry(EIDRM),
	entry(EILSEQ),
	entry(EINPROGRESS),
	entry(EINTR),
	entry(EINVAL),
	entry(EIO),
	entry(EISCONN),
	entry(EISDIR),
	entry(EISNAM),
	entry(EKEYEXPIRED),
	entry(EKEYREJECTED),
	entry(EKEYREVOKED),
	entry(EL2HLT),
	entry(EL2NSYNC),
	entry(EL3HLT),
	entry(EL3RST),
	entry(ELIBACC),
	entry(ELIBBAD),
	entry(ELIBMAX),
	entry(ELIBSCN),
	entry(ELIBEXEC),
	entry(ELOOP),
	entry(EMEDIUMTYPE),
	entry(EMFILE),
	entry(EMLINK),
	entry(EMSGSIZE),
	entry(EMULTIHOP),
	entry(ENAMETOOLONG),
	entry(ENETDOWN),
	entry(ENETRESET),
	entry(ENETUNREACH),
	entry(ENFILE),
	entry(ENOBUFS),
	entry(ENODATA),
	entry(ENODEV),
	entry(ENOENT),
	entry(ENOEXEC),
	entry(ENOKEY),
	entry(ENOLCK),
	entry(ENOLINK),
	entry(ENOMEDIUM),
	entry(ENOMEM),
	entry(ENOMSG),
	entry(ENONET),
	entry(ENOPKG),
	entry(ENOPROTOOPT),
	entry(ENOSPC),
	entry(ENOSR),
	entry(ENOSTR),
	entry(ENOSYS),
	entry(ENOTBLK),
	entry(ENOTCONN),
	entry(ENOTDIR),
	entry(ENOTEMPTY),
	entry(ENOTSOCK),
	entry(ENOTSUP),
	entry(ENOTTY),
	entry(ENOTUNIQ),
	entry(ENXIO),
	entry(EOPNOTSUPP),
	entry(EOVERFLOW),
	entry(EPERM),
	entry(EPFNOSUPPORT),
	entry(EPIPE),
	entry(EPROTO),
	entry(EPROTONOSUPPORT),
	entry(EPROTOTYPE),
	entry(ERANGE),
	entry(EREMCHG),
	entry(EREMOTE),
	entry(EREMOTEIO),
	entry(ERESTART),
	entry(EROFS),
	entry(ESHUTDOWN),
	entry(ESPIPE),
	entry(ESOCKTNOSUPPORT),
	entry(ESRCH),
	entry(ESTALE),
	entry(ESTRPIPE),
	entry(ETIME),
	entry(ETIMEDOUT),
	entry(ETXTBSY),
	entry(EUCLEAN),
	entry(EUNATCH),
	entry(EUSERS),
	entry(EWOULDBLOCK),
	entry(EXDEV),
	entry(EXFULL),
};
#undef entry

/*
 * Translate error name to error value
 */
static inline int error_get_by_name(const char* name) {
	unsigned int i;
	for(i=0; i<ARRAY_SIZEOF(err_tbl); i++) {
		if(strcmp(name, err_tbl[i].name)==0) {
			return err_tbl[i].val;
		}
	}
	CHECK_ASSERT("bad error name"==NULL);
	return -1;
}

/*
 * Translate error values to error names
 */
static inline const char* error_get_by_val(int val) {
	unsigned int i;
	for(i=0; i<ARRAY_SIZEOF(err_tbl); i++) {
		if(err_tbl[i].val==val) {
			return err_tbl[i].name;
		}
	}
	CHECK_ASSERT("bad error value"==NULL);
	return NULL;
}

/*
 * Print out a table of all error values, names and descriptions
 */
static inline void print_error_table() {
	unsigned int i;
	printf("number of error values is %zd\n", ARRAY_SIZEOF(err_tbl));
	for(i=0; i<ARRAY_SIZEOF(err_tbl); i++) {
		int val=err_tbl[i].val;
		const char* name=err_tbl[i].name;
		printf("i=%d, sig=%d, in_code=%s, strerror(%d)=%s\n", i, val, name, val, strerror(val));
	}
}

#endif	/* !__err_utils_h */
