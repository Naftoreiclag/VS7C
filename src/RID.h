/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef UIDMANAGER_H
#define UIDMANAGER_H

#include <string>
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/ordered_index.hpp"
#include "boost/multi_index/identity.hpp"
#include "irrlicht.h"

class RID {
public:
    typedef irr::u64 RIDValue;
	RID(RIDValue value = 0);
	RID(const RID& arg);
	RID(std::string humanReadableID);
	~RID();

	RID& operator=(const RID& arg);
	bool operator==(const RID& arg);
	bool operator<(const RID& arg);
	operator RIDValue() const;
private:
    RIDValue value;
};

namespace RIDDatabase {
    typedef RID::RIDValue RIDValue;

	void addRID(RIDValue value, std::string humanReadableID);
}


#endif // UIDMANAGER_H
