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

/** @file railnet-filter.cpp main file of the railnet filter */

#include <cstring>
#include <map>
#include <iostream>
#include <stdexcept>
#include <getopt.h>
#include <cmath>
#include <cstdlib>

#include "config.h"

#include "common.h"
#include "railnet_node_list.h"
#include "railnet-filter_options.h"

static_assert(comm::RailnetFileInfo::_version == RAILNET_EXPECTED_FILE_VERSION,
	"Version mismatch: Your OpenTTD is too new/too old for this railnet version");

void print_version()
{
	std::cerr << "version: <not specified yet>" << std::endl;
	std::cerr << "railnet file version: "
		<< comm::RailnetFileInfo::_version << std::endl;
}

void print_help()
{
	std::cerr << "railnet filter" << std::endl;
	std::cerr << "filters railnet files" << std::endl;
	options::usage();
}

LblConvT lbl_conv;

//! main routine of the railnet converter
int run(const options& opt)
{
	comm::RailnetFileInfo file;
//	deserialize(file, std::cin);
	comm::RailnetIfile(std::cin) >> file;
//	comm::json_ofile(std::cout) << file; // <- debugging only

	/*
		find out which cargo is used...
	*/
	std::set<int> cargo_ids;

	std::map<unsigned char, CargoLabelT>::iterator itr, nextc;
	for(itr = nextc = file.cargo_names().begin(); itr != file.cargo_names().end(); itr = nextc)
	{
		++nextc;
		const CargoLabelT& clbl = itr->second;
		const char* str = lbl_conv.Convert(clbl);

		if(opt.command == options::cmd_list_cargo)
		 std::cout << str << std::endl;
		else
		{
			if(strstr(opt.cargo.c_str(), str))
			 cargo_ids.insert(clbl);
			else
			 file.cargo_names().erase(itr);
		}
	}
	if(opt.command == options::cmd_list_cargo)
	 return 0;

	if(cargo_ids.size() != (opt.cargo.length()+1)/5)
	 throw "not all of your cargos are known";

	/*
		remove unwanted cargo from list
	*/
	{
	std::list<comm::OrderList>::iterator itr, next;
	for(itr = next = file.order_lists.get().begin();
		itr != file.order_lists.get().end(); itr = next)
	{
		++next;
/*		bool cargo_found = false;
		for(std::map<CargoLabelT, comm::CargoInfo>::const_iterator itr2 = itr->cargo.get().begin();
			!cargo_found && itr2 != itr->cargo.get().end(); ++itr2)
		 cargo_found = (cargo_ids.find(itr2->first) != cargo_ids.end());
		if(!cargo_found)
		 file.order_lists.get().erase(itr);*/
		std::map<CargoLabelT, comm::CargoInfo>::const_iterator itr2,
			next2 = itr->cargo.get().begin();
		for(itr2 = next2;
			itr2 != itr->cargo.get().end(); itr2 = next2)
		{
			++next2;
			if(cargo_ids.find(itr2->first) == cargo_ids.end())
			 itr->cargo().erase(itr2);
		}
		if(itr->cargo().empty())
		 file.order_lists().erase(itr);
	}
	}

	/*
		sort out subset or express trains
	*/
	node_list_t nl;
	for(const comm::OrderList& ol : file.order_lists.get())
	 nl.init(ol);

	auto next = file.order_lists.get().begin();
	for(auto itr = file.order_lists.get().begin(); itr != file.order_lists.get().end(); itr = next)
	{
		++next;
		int type = nl.traverse(*itr, NULL, false, false);

		bool is_express = type & node_list_t::is_express_train,
			is_short = type & node_list_t::is_short_train;

		if((is_express && opt.hide_express)||(is_short && opt.hide_short_trains))
		{
			UnitID erased_no = itr->unit_number;
			std::cerr << "Erasing train: " << erased_no << ", reason: " <<
				((is_express && is_short)
					? "express + short"
					: is_express
						? "express"
						: "short")
				<< std::endl;
			file.order_lists.get().erase(itr);
		}
	}

	std::set<StationID> station_flags;

	/*
		find out which stations are actually being used...
	*/
	// only set flags
	for(std::list<comm::OrderList>::const_iterator itr =
		file.order_lists.get().begin();
		itr != file.order_lists.get().end(); ++itr)
	if(itr->stations.get().size())
	{
		const comm::OrderList& ol = *itr;
		for(std::vector<std::pair<StationID, bool> >::const_iterator
				itr = ol.stations.get().begin();
				itr != ol.stations.get().end(); ++itr)
		{
			station_flags.insert(itr->first);
		}
	}

	auto nextstation = file.stations().begin();
	for(auto itr = file.stations().begin(); itr != file.stations().end(); itr = nextstation)
	{
		++nextstation;
		if(station_flags.find(itr->first) == station_flags.end())
		 file.stations().erase(itr);
	}

	comm::RailnetOfile(std::cout) << file;

	return 0;
}

int main(int argc, char** argv)
{
	try
	{
		setlocale(LC_ALL, "en_US.UTF-8");
		options opt(argc, argv);
		switch(opt.command)
		{
			case options::cmd_print_help:
				print_help();
				break;
			case options::cmd_print_version:
				print_version();
				break;
			default:
				run(opt);
		}
	} catch(const char* s)
	{
		std::cerr << "Error: " << s << std::endl;
		std::cerr << "Rest of file: ";
		char buf[1024];
		int count = 0;
		while(std::cin.getline(buf, 1024) && ++count < 8) {
			std::cerr << "* " << buf << std::endl;
		}
		std::cerr << "Exiting." << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
