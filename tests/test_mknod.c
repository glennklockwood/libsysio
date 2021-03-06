/*
 *    This Cplant(TM) source code is the property of Sandia National
 *    Laboratories.
 *
 *    This Cplant(TM) source code is copyrighted by Sandia National
 *    Laboratories.
 *
 *    The redistribution of this Cplant(TM) source code is subject to the
 *    terms of the GNU Lesser General Public License
 *    (see cit/LGPL or http://www.gnu.org/licenses/lgpl.html)
 *
 *    Cplant(TM) Copyright 1998-2006 Sandia Corporation. 
 *    Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 *    license for use of this work by or on behalf of the US Government.
 *    Export of this program may require a license from the United States
 *    Government.
 */

/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Questions or comments about this library should be sent to:
 *
 * Lee Ward
 * Sandia National Laboratories, New Mexico
 * P.O. Box 5800
 * Albuquerque, NM 87185-1110
 *
 * lee@sandia.gov
 */

/* 
 * Can't provoke a definition of the S_IFMT macros without a little extra work.
 */
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>

#if defined(SYSIO_LABEL_NAMES)
#include "sysio.h"
#endif
#include "xtio.h"
#include "test.h"

/*
 * Create a node.
 *
 * Usage: mknod path {f|b|c} [dev]
 *
 * The dev argument should not be present for regular file and FIFO object
 * creation.
 */

static int do_mknod(const char *path, mode_t mode, dev_t dev);
static void usage(void);

int
main(int argc, char *const argv[])
{
	int	i;
	int	err;
	mode_t	mode;
	dev_t	dev;
	extern int _test_sysio_startup(void);

	/*
	 * Parse command line arguments.
	 */
	while ((i = getopt(argc, argv, "")) != -1)
		switch (i) {

		default:
			usage();
		}

	/*
	 * Init sysio lib.
	 */
	err = _test_sysio_startup();
	if (err) {
		errno = -err;
		perror("sysio startup");
		exit(1);
	}

	if (argc - optind < 2)
		usage();
	if (strlen(argv[optind + 1]) != 1)
		usage();
	mode = 0666;
	switch (*argv[optind + 1]) {
	
	case 'f':
		mode |= S_IFREG;
		break;
	case 'b':
		mode |= S_IFBLK;
		break;
	case 'c':
		mode |= S_IFCHR;
		break;
	case 'p':
		mode |= S_IFIFO;
		break;
	default:
		usage();
	}
	dev = 0;
	if (!(S_ISREG(mode) || S_ISFIFO(mode)))
		dev = atoi(argv[optind + 2]);
	else if (argc - optind != 2) {
		(void )fprintf(stderr, "Too many arguments\n");
		usage();
	}
	(void )do_mknod(argv[optind + 0], mode, dev);

	/*
	 * Clean up.
	 */
	_test_sysio_shutdown();

	return 0;
}

static int
do_mknod(const char *path, mode_t mode, dev_t dev)
{

	if (SYSIO_INTERFACE_NAME(mknod)(path, mode, dev) != 0) {
		perror(path);
		return -1;
	}

	return 0;
}

static void
usage()
{

	(void )fprintf(stderr, "Usage: mknod path {f|b|c|p} dev\n");
	exit(1);
}
