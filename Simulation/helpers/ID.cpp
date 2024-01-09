#include "ID.hpp"

bool operator==(const ID& l, const ID& r)
{
    return l.getID() == r.getID();
}

bool operator!=(const ID& l, const ID& r)
{
    return !(l==r);
}

bool operator==(const IDview& l, const IDview& r)
{
    return l.getID() == r.getID();
}

bool operator!=(const IDview& l, const IDview& r)
{
    return !(l == r);
}

bool operator==(const ID& l, const IDview& r)
{
    return l.getID() == r.getID();
}

bool operator!=(const ID& l, const IDview& r)
{
    return !(l == r);
}

bool operator==(const IDview& l, const ID& r)
{
    return l.getID() == r.getID();
}

bool operator!=(const IDview& l, const ID& r)
{
    return !(l == r);
}
