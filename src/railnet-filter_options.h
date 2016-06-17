/*************************************************************************/
/* railnet - utilities to export OpenTTD savegames to railroad networks  */
/* Copyright (C) 2016                                                    */
/* Johannes Lorenz (jlsf2013 at sourceforge)                             */
/*                                                                       */
/* This program is free software; you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation; either version 3 of the License, or (at */
/* your option) any later version.                                       */
/* This program is distributed in the hope that it will be useful, but   */
/* WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      */
/* General Public License for more details.                              */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program; if not, write to the Free Software           */
/* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA  */
/*************************************************************************/

/** @file railnet_options.h Struct defining the @a options class */

#ifndef RAILNET_OPTIONS_H
#define RAILNET_OPTIONS_H

#include <string>

//! struct holding options (e.g. commandline parameters) for the
//! railnet converter
struct options
{
	enum command_t
	{
		cmd_print_version,
		cmd_print_help,
		cmd_list_cargo,
		cmd_filter
	} command = cmd_filter;

	std::string cargo; //! csv-string for cargo filter
	bool hide_express = false,
		hide_short_trains = false;

	options(int arg, char** argv);
	static void usage();
};

#endif // RAILNET_OPTIONS_H

