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

#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include "railnet-filter_options.h"

bool cargo_ltr(char letter)
{
	return isupper(letter) || letter == '_';
}

options::options(int argc, char** argv)
{
	int c;

	while (true) {
		int option_index = 0;
		static struct option long_options[] = {
			{"help",		no_argument,		0, 'h'},
			{"version",		no_argument,		0, 'v'},
			{"list-cargo",		no_argument,		0, 'l'},
			{"cargo",		required_argument,	0, 'c'},
			{"no-short-trains",	no_argument,		0, 's'},
			{"no-express-trains",	no_argument,		0, 'e'},
			{0,			0,			0,  0 }
		};

		c = getopt_long(argc, argv, "hvlc:se",
			long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
		//	std::cerr << "option " << long_options[option_index].name;
		//	if (optarg)
		//		printf(" with arg %s", optarg);
		//	printf("\n");
			break;
		case 'l':
			command = cmd_list_cargo;
			break;
		case 'h':
			command = cmd_print_help;
			break;
		case 'v':
			command = cmd_print_version;
			break;
		case 'e':
			hide_express = true;
			break;
		case 's':
			hide_short_trains = true;
			break;
		case 'c':
		//	if(strlen(optarg) % 5 != 4)
		//	 throw "invalid cargo string. expected: AAAA,BBBB,...";
			if(!optarg)
			 throw "expecting argument";
			cargo = optarg;
			for(const char* ptr = cargo.c_str(); *ptr; )
			{
				if(!cargo_ltr(*ptr)|| !cargo_ltr(*++ptr)
					|| !cargo_ltr(*++ptr) || !cargo_ltr(*++ptr))
					throw "expected uppercase or underscore here";
				if(*++ptr==',')
				 ++ptr;
				else if(*ptr)
				 throw "expected comma here";
			}
			break;
		case '?':
			std::cerr << "unrecognized option found" << std::endl;
			exit(0);
			break;
		default:
			std::cerr << "?? getopt returned character code 0%o ??" << std::endl;
		}
	}

	// FEATURE: allow of forbid this
/*	if (optind < argc) {
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}*/
}

void options::usage()
{
	std::cerr << "usage: railnet-filter <options>" << std::endl;
	std::cerr << "options:" << std::endl;
	std::cerr << "\t-h, --help\t\tprint help and exit" << std::endl;
	std::cerr << "\t-v, --version\t\tprint version and exit" << std::endl;
	std::cerr << "\t-l, --list-cargo\tprint all cargo types from file and exit" << std::endl;
	std::cerr << "\t-c, --cargo=C1,C2,...\tshow only lines with cargo C1 or C2 or ..." << std::endl;
	std::cerr << "\t-s, --no-short-trains\ttrains that don't run the full order list are" << std::endl;
	std::cerr << "\t\t\t\tnot printed as extra railyway lines" << std::endl;
	std::cerr << "\t-e, --no-express-trains\ttrains that stop less often are" << std::endl;
	std::cerr << "\t\t\t\tnot printed as extra railyway lines" << std::endl;
}

