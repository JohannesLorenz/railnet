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
#include "railnet2dot_options.h"

options::options(int argc, char** argv)
{
	int c;

	while (true) {
		int option_index = 0;
		static struct option long_options[] = {
			{"help",		no_argument,		0, 'h'},
			{"version",		no_argument,		0, 'v'},
			{"stretch",		optional_argument,	0, 's'},
			{0,			0,			0,  0 }
		};

		c = getopt_long(argc, argv, "hvs:",
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
		case 'h':
			command = cmd_print_help;
			break;
		case 'v':
			command = cmd_print_version;
			break;
		case 's':
			stretch = atof(optarg);
			if(stretch < 0.01 || stretch > 100)
			 throw "stretch factor should be in range [0.01,100].";
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
	std::cerr << "usage: railnet <options>" << std::endl;
	std::cerr << "options:" << std::endl;
	std::cerr << "\t-h, --help\t\tprint help and exit" << std::endl;
	std::cerr << "\t-v, --version\t\tprint version and exit" << std::endl;
	std::cerr << "\t-s, --stretch\t\tstretch factor; default: 1.0" << std::endl;
}


