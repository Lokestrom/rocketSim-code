#pragma once

#include "Database/String.hpp"

using namespace Database;

struct ID
{
	using ID_T = unsigned long long;
	using UUID = unsigned long long;

	static ID createID(String name, ID_T localID) {
		static UUID currentID = 1;
		return ID{ currentID++, localID, name };
	}

	ID()
		: _name(""), _id(0), _localID(0)
	{}

	UUID getUUID() const noexcept { return _id; }
	String getName() const noexcept { return _name; }
	ID_T getLocalID() const noexcept { return _localID; }

	void setName(const String& newName) { _name = newName; }
	void setLocalID(const ID::ID_T& newLocalID) { _localID = newLocalID; }

	ID(const ID&) = delete;
	ID& operator=(const ID&) = delete;
	ID(ID&&) = default;
	ID& operator=(ID&&) = default;

private:
	ID(ID_T id, ID_T localID, String name)
		: _name(name), _id(id), _localID(localID)
	{}
	String _name;
	UUID _id;
	ID_T _localID;

	friend class IDview;
};

class IDview {
public:
	IDview() = default;
	IDview(const ID& id)
		: _id(id)
	{}

	const ID::UUID& getUUID() const noexcept { return _id._id; }
	const String& getName() const noexcept { return _id._name; }
	const ID::ID_T& getLocalID() const noexcept { return _id._localID; }

private:
	const ID& _id;
};

bool operator==(const ID& l, const ID& r);
bool operator!=(const ID& l, const ID& r);
bool operator==(const IDview& l, const IDview& r);
bool operator!=(const IDview& l, const IDview& r);
bool operator==(const ID& l, const IDview& r);
bool operator!=(const ID& l, const IDview& r);
bool operator==(const IDview& l, const ID& r);
bool operator!=(const IDview& l, const ID& r);