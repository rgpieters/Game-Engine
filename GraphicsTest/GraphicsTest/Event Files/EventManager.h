#ifndef _EVENT_MANAGER_H_
#define _EVENT_MANAGER_H_

#include <string>
#include <list>
#include <map>
using std::multimap;
using std::pair;
using std::list;
using std::string;

#include "Event.h"
#include "IListener.h"

class EventManager
{
private:
	// Client database
	multimap<EVENTID, IListener*>	m_ClientDatabase;

	// Events waiting to be processed
	list<Event>						m_CurrentEvents;

	void DispatchEvent(Event* pEvent);
	bool AlreadyRegistered(EVENTID eventID, IListener* pClient);

public:
	EventManager();
	EventManager(const EventManager&);
	~EventManager();

	void Initialize();
	void RegisterClient(EVENTID eventID, IListener* pClient);
	void UnregisterClient(EVENTID eventID, IListener* pClient);
	void UnregisterClientAll(IListener* pClient);
	void SendEvent(EVENTID eventID, void* pData = nullptr);
	void ProcessEvents();
	void ClearEvents();
	void ShutdownEventSystem();
};

#endif