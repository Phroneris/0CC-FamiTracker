/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2014  Jonathan Liss
**
** 0CC-FamiTracker is (C) 2014-2018 HertzDevil
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.  To obtain a
** copy of the GNU Library General Public License, write to the Free
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/


#pragma once

// Application version information

// Define this for beta builds
#define WIP

// Version info
#define VERSION_API  0
#define VERSION_MAJ  3	/// jp
#define VERSION_MIN  15	/// jp
#define VERSION_REV  4	/// jp
#define VERSION_OPT  JPpre001	/// jp

constexpr int Compare0CCFTVersion(int api, int maj, int min, int rev) noexcept {
	if (api > VERSION_API)
		return 1;
	if (api < VERSION_API)
		return -1;
	if (maj > VERSION_MAJ)
		return 1;
	if (maj < VERSION_MAJ)
		return -1;
	if (min > VERSION_MIN)
		return 1;
	if (min < VERSION_MIN)
		return -1;
	if (rev > VERSION_REV)
		return 1;
	if (rev < VERSION_REV)
		return -1;
	return 0;
}

extern const char *Get0CCFTVersionString() noexcept;
extern const char *GetDumpVersionString() noexcept;
