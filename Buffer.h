#pragma once
#include "DEVSim.h"

class CBuffer : public CCoupled
{
public:
	CBuffer();
	~CBuffer();

//I/O ports
public:
	static std::string in_port1;
	static std::string in_port2;
	static std::string out_port;

private:
	int* item_list;

};

