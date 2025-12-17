#pragma once
#include <string>

class NPCPerson;
class Bar;

class Message
{
public:
	bool inPerson = true;
	NPCPerson* sender;
	std::string content = "Hej!";

	Message() {}
	virtual ~Message() {}
};

class InviteMessage : public Message
{
public:
	Bar* place;
	int hour;

	InviteMessage();
	void respond(bool positive);
};
