#ifndef TAGContext_H
#define TAGContext_H

#include	"GraphContext.h"

class TAGContext {
public:
	TAGContext(Vertex * v=0,int hop=c_Und);

	virtual ~TAGContext() { }

	int sawUutConnectorNode		(const int hop=c_Und);
	int sawInterfaceConnectorNode	(const int hop=c_Und);
	int sawSourceDevicePortNode	(const int hop=c_Und);
	int sawSensorDevicePortNode	(const int hop=c_Und);
	int sawLoadDevicePortNode	(const int hop=c_Und);
	int sawPathNode			(const int hop=c_Und);
	int sawSwitchContactNode	(const int hop=c_Und);
	int sawPointSourceNode		(const int hop=c_Und);

	int tagUpdate		(class Vertex * v,const int hop=c_Und);
	void tagUpdate		(TAGContext & other);

private:
	int update(int & saw, const int hop);

	int m_InterfaceConnectorNode;
	int m_SourceDevicePortNode;
	int m_SensorDevicePortNode;
	int m_LoadDevicePortNode;
	int m_PathNode;
	int m_UutConnectorNode;
	int m_SwitchContactNode;
	int m_PointSourceNode;

};



#endif // TAGContext_H
