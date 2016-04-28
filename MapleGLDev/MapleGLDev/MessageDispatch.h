
#ifndef MESSAGEDISPATCH_H
#define  MESSAGEDISPATCH_H

#include <functional>
using namespace std::placeholders;

class Entity;

class MD_Time {
public:
	GLint birth;
};

class DispatchedMessage {
public:
	Entity* e;
	void(*callback)(Entity *e);
};

extern void IsHit(Entity *e);


class MessageDispatch {
public:

	std::vector<DispatchedMessage> messages;

	void RegisterMessage(std::string msg, void(*callback)(Entity *e), Entity *context);

	MessageDispatch() {

	}
};
#endif