#include "Message.h"
#include "Buildings.h"
#include "NPCPerson.h"

InviteMessage::InviteMessage()
{
	
}

void InviteMessage::respond(bool positive)
{
	if (positive)
	{
		// Store into schedule
		this->sender->meeting = this;
		this->sender->schedule[this->hour].action = NPCAction::Hangout;
		this->sender->schedule[this->hour].place = this->place;
	}
}