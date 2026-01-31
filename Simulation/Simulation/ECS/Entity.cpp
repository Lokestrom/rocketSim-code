#include "Entity.hpp"

#include "Simulation.hpp"
#include "types.hpp"

Entity::Entity(Database::String name, ID::ID_T localID)
	: _id(ID::createID(name, localID))
{
}