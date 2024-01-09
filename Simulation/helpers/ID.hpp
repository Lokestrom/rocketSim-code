#pragma once

#include "String.hpp"

using namespace Database;

struct ID
{
	using ID_T = size_t;
	using GlobaleID_T = size_t;

	static ID createID(String name, ID_T localID) {
		static ID_T currentID = 1;
		return ID{ currentID++, localID, name };
	}

	ID()
		: _name(""), _id(0), _localID(0)
	{}

	ID_T getID() const noexcept { return _id; }
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
	GlobaleID_T _id;
	ID_T _localID;

	friend class IDview;
};

class IDview {
public:
	IDview() = default;
	IDview(const ID& id)
		: _name(&id._name),
		_id(&id._id),
		_localID(&id._localID)
	{}

	ID::GlobaleID_T getID() const noexcept { return *_id; }
	String getName() const noexcept { return *_name; }
	ID::ID_T getLocalID() const noexcept { return *_localID; }

private:
	const String* _name;
	const ID::GlobaleID_T* _id;
	const ID::ID_T* _localID;
};

bool operator==(const ID& l, const ID& r);
bool operator!=(const ID& l, const ID& r);
bool operator==(const IDview& l, const IDview& r);
bool operator!=(const IDview& l, const IDview& r);
bool operator==(const ID& l, const IDview& r);
bool operator!=(const ID& l, const IDview& r);
bool operator==(const IDview& l, const ID& r);
bool operator!=(const IDview& l, const ID& r);