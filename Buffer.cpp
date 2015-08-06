#include "Buffer.h"
#include "InBuffer.h"
#include "OutBuffer.h"

std::string CBuffer::in_port1 = "g_in" ;
std::string CBuffer::in_port2 = "p_in" ;
std::string CBuffer::out_port = "out" ;

CBuffer::CBuffer()
{
	SetName("buffer");
	
	// Port 
	AddInPorts(1, in_port1.c_str());
	AddInPorts(1, in_port2.c_str());
	AddOutPorts(1, out_port.c_str());

	// Add Components
	item_list = new int;
	*(item_list) = 0;
	CModel* in_buffer = new CInBuffer(item_list);
	CModel* out_buffer = new COutBuffer(item_list); 

	AddComponent(1, in_buffer);
	AddComponent(1, out_buffer);
	
	// Add Coupling
	AddCoupling(this, in_port1.c_str(), in_buffer, CInBuffer::in_port.c_str());
	AddCoupling(this, in_port2.c_str(), out_buffer, COutBuffer::proc_port.c_str());
	AddCoupling(in_buffer, CInBuffer::out_port.c_str(), out_buffer, COutBuffer::inbuf_port.c_str());
	AddCoupling(out_buffer, COutBuffer::out_port.c_str(), this, out_port.c_str());
}


CBuffer::~CBuffer()
{
	delete item_list;
}