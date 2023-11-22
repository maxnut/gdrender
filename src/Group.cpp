#include "Group.h"
#include <deque>

std::shared_ptr<Group> Group::create()
{
	return std::shared_ptr<Group>(new Group);
}