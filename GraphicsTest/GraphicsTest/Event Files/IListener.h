#ifndef _ILISTENER_H_
#define _ILISTENER_H_

#include "Event.h"

class IListener
{
public:
	IListener() {};
	virtual ~IListener() {};

	//	Blue print function - MUST be defined in the derived class.
	//	Mini-proc function for whenever this object receives and event
	virtual void HandleEvent(Event* pEvent) = 0;
};
#endif