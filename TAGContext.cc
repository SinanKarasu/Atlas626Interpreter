#include	"Types.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"Vertex.h"
#include	"TAGContext.h"

TAGContext::TAGContext(Vertex * v,int hop)
		:m_UutConnectorNode		(c_Und)
		,m_InterfaceConnectorNode	(c_Und)
		,m_SourceDevicePortNode		(c_Und)
		,m_SensorDevicePortNode		(c_Und)
		,m_LoadDevicePortNode		(c_Und)
		,m_PathNode			(c_Und)
		,m_SwitchContactNode		(c_Und)
		,m_PointSourceNode		(c_Und)
		{
			if(v)tagUpdate(v,hop);
		}


int TAGContext::sawUutConnectorNode		(const int hop)	{ return update(m_UutConnectorNode	,hop);}
int TAGContext::sawInterfaceConnectorNode	(const int hop)	{ return update(m_InterfaceConnectorNode,hop);}
int TAGContext::sawSourceDevicePortNode		(const int hop)	{ return update(m_SourceDevicePortNode	,hop);}
int TAGContext::sawSensorDevicePortNode		(const int hop)	{ return update(m_SensorDevicePortNode	,hop);}
int TAGContext::sawLoadDevicePortNode		(const int hop)	{ return update(m_LoadDevicePortNode	,hop);}
int TAGContext::sawPathNode			(const int hop)	{ return update(m_PathNode		,hop);}
int TAGContext::sawSwitchContactNode		(const int hop)	{ return update(m_SwitchContactNode	,hop);}
int TAGContext::sawPointSourceNode		(const int hop)	{ return update(m_PointSourceNode	,hop);}

int TAGContext::tagUpdate		(Vertex * v,const int hop)
	{
		switch (v->getNodeType()){
		case	UndefinedNodeType		:assert(0);return 0;
		case	UutConnectorNodeType		:return update(m_UutConnectorNode	,hop);
		case	InterfaceConnectorNodeType	:return update(m_InterfaceConnectorNode	,hop);
		case	SourceDevicePortNodeType	:return update(m_SourceDevicePortNode	,hop);
		case	SensorDevicePortNodeType	:return update(m_SensorDevicePortNode	,hop);
		case	LoadDevicePortNodeType		:return update(m_LoadDevicePortNode	,hop);
		case	PathNodeType			:return update(m_PathNode		,hop);
		case	SwitchContactNodeType		:return update(m_SwitchContactNode	,hop);
		case	PointSourceNodeType		:return update(m_PointSourceNode	,hop);
		default					:return 0;
		}
	}

void TAGContext::tagUpdate		(TAGContext & other)
	{
		sawUutConnectorNode		(other.sawUutConnectorNode	());
		sawInterfaceConnectorNode	(other.sawInterfaceConnectorNode());
		sawSourceDevicePortNode		(other.sawSourceDevicePortNode	());
		sawSensorDevicePortNode		(other.sawSensorDevicePortNode	());
		sawLoadDevicePortNode		(other.sawLoadDevicePortNode	());
		sawPathNode			(other.sawPathNode		());
		sawSwitchContactNode		(other.sawSwitchContactNode	());
		sawPointSourceNode		(other.sawPointSourceNode	());
	}


int TAGContext::update(int & saw, const int hop)
	{
		if(hop!=c_Und){
			if(saw==c_Und){
				saw=hop;
			} else if(hop<saw){
				saw=hop;
			}
		}
		return saw;
	}
