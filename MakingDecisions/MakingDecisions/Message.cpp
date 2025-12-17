#include "Message.h"
#include "Buildings.h"
#include "NPCPerson.h"

InviteMessage::InviteMessage()
{
	this->content = "Wanna meet in " + this->place->name + " at " + std::to_string(this->hour) + "?";
}

void InviteMessage::respond(bool positive)
{

}