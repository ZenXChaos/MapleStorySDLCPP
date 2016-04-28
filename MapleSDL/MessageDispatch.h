
#ifndef MESSAGEDISPATCH_H
#define  MESSAGEDISPATCH_H

#include <functional>
using namespace std::placeholders;

class Entity;

class MD_Time {
public:
	Uint32 birth;

	Uint32 GetAge(Uint32 tick);
};

class DispatchedMessage {
public:
	Entity* e;
	void(*callback)(Entity *e);
};

extern void IsHit(Entity *e);


class MessageDispatch{
public:

	std::vector<DispatchedMessage> messages;

	void RegisterMessage(std::string msg, void(*callback)(Entity *e), Entity *context);

	MessageDispatch() {
		
	}
};
#endif