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
#include "boost/multi_index/member.hpp"
#include "irrlicht.h"
#include "json/json.h"

// sizeof RID == sizeof irr::u64
class RID {
public:
    typedef irr::u64 RIDValue;
	RID(RIDValue value = 0);
	RID(const RID& arg);
	RID(std::string humanReadableID);
	~RID();

	const std::string& getHumanReadableID() const;
	const std::string& getHumanDesc() const;

	RID& operator=(const RID& arg);
	bool operator==(const RID& arg);
	bool operator<(const RID& arg);
	operator RIDValue() const;

	RIDValue getValue() const;

private:
	RIDValue value;
};

namespace RIDDatabase {

	struct RIDMetadata {
		typedef RID::RIDValue RIDValue;

		RIDMetadata(RIDValue valueID, std::string humanReadableID, std::string humanDesc);

		RIDValue valueID;
		std::string humanReadableID;
		std::string humanDesc;
	};

    typedef RID::RIDValue RIDValue;
    typedef boost::multi_index_container<
		RIDMetadata,
		boost::multi_index::indexed_by<
			boost::multi_index::ordered_unique<
				boost::multi_index::member<RIDMetadata, RIDValue, &RIDMetadata::valueID> // Order by 64 bit number
			>,
			boost::multi_index::ordered_unique<
				boost::multi_index::member<RIDMetadata, std::string, &RIDMetadata::humanReadableID> // Order by 64 bit number
			>
		>
	> RIDMetadataTable;

	void addRID(const RIDValue& value, const std::string& humanReadableID, const std::string& humanDesc = "");
	const RIDMetadata& getMetadata(const RIDValue& value);
	const RIDMetadata& getMetadata(const std::string& humanReadableID);
	const std::string& getHumanReadableID(const RIDValue& value);
	const RIDValue& getValueID(const std::string& humanReadableID);

	RIDMetadata* getErrorData();
}


#endif // UIDMANAGER_H
