/*
 * Copyright (C) 2010-2016 Alexander Saprykin <xelfium@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
 */

#include "pfile.h"
#include "plib-private.h"

#ifdef P_OS_WIN
#include <windows.h>
#else
#include <unistd.h>
#endif

P_LIB_API pboolean
p_file_is_exists (const pchar *file)
{
#ifdef P_OS_WIN
	DWORD attrs;
#endif

	if (file == NULL)
		return FALSE;

#ifdef P_OS_WIN
	attrs = GetFileAttributes ((LPCTSTR) file);

	return (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
	return access (file, F_OK) == 0;
#endif
}

P_LIB_API pboolean
p_file_remove (const pchar	*file,
	       PError		**error)
{
	pboolean result;

	if (file == NULL) {
		p_error_set_error_p (error,
				     (pint) P_ERROR_IO_INVALID_ARGUMENT,
				     0,
				     "Invalid input argument");
		return FALSE;
	}

#ifdef P_OS_WIN
	result = (DeleteFile ((LPCTSTR) file) != 0);
#else
	result = (unlink (file) == 0);
#endif

	if (!result)
		p_error_set_error_p (error,
				     (pint) __p_error_get_last_io (),
				     __p_error_get_last_error (),
				     "Failed to remove file");

	return result;
}
