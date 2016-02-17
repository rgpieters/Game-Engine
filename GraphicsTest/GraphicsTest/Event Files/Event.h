#ifndef _EVENT_H_
#define _EVENT_H_

#include <string>
using std::string;

typedef string EVENTID;

class Event
{
private:
	EVENTID		m_EventID;
	void*		m_pParam;

public:
	Event(EVENTID eventID, void* pParam = nullptr)
	{
		m_EventID	= eventID;
		m_pParam	= pParam;
	}
	~Event() {};

	inline EVENTID	GetEventID()	{ return m_EventID; }
	inline void*	GetParam()		{ return m_pParam;  }
};

#endif