#include	"Types.h"
#include	"Graph.h"
#include	"Circuit.h"

Circuit::~Circuit(){};

Circuit::Circuit(Impedance Inf,Resource * resource)
	: Graph (Inf),m_Resource(resource)
	{};
