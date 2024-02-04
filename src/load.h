/*
* load.h ~ loads programs from a file into memory
* author: Ben Staehle
* date: 6/20/23
*/

#pragma once
#include<string>

namespace lc3
{
	void load_prog(std::string filename);
	void load_library(std::string filename);
	void load_library_res(int res_id, int res_name);
}
