/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "RID.h"
#include "easylogging++.h"

RID::RID(RIDValue value)
: value(value) { }

RID::RID(std::string humanReadableID)
: value(RIDDatabase::getValueID(humanReadableID)) { }

RID::RID(const RID& arg)
: value(arg.value) { }

RID::~RID() { }

const std::string& RID::getHumanReadableID() const {
	return RIDDatabase::getMetadata(value).humanReadableID;
}
const std::string& RID::getHumanDesc() const {
	return RIDDatabase::getMetadata(value).humanDesc;
}

RID& RID::operator=(const RID& arg) {
	value = arg.value;
	return *this;
}

bool RID::operator==(const RID& arg) {
	return arg.value == value;
}

bool RID::operator<(const RID& arg) {
	return arg.value < value;
}

RID::operator RIDValue() const {
	return value;
}

RID::RIDValue RID::getValue() const {
	return value;
}


RIDDatabase::RIDMetadataTable metadataTable;
RIDDatabase::RIDMetadata* errorData = 0;

RIDDatabase::RIDMetadata::RIDMetadata(RIDValue valueID, std::string humanReadableID, std::string humanDesc)
:valueID(valueID),
humanReadableID(humanReadableID),
humanDesc(humanDesc) { }

void RIDDatabase::addRID(const RIDValue& value, const std::string& humanReadableID, const std::string& humanDesc) {
	LOG(INFO) << "New RID added with humanID = " << humanReadableID;

	if(value == 0) {
		errorData = new RIDMetadata(value, humanReadableID, humanDesc);
		metadataTable.insert(*errorData);

	} else {
		metadataTable.insert(RIDMetadata(value, humanReadableID, humanDesc));
	}
}

const RIDDatabase::RIDMetadata& RIDDatabase::getMetadata(const RIDValue& value) {
	RIDMetadataTable::nth_index<0>::type::iterator it = metadataTable.get<0>().find(value);
	if(it == metadataTable.get<0>().end()) {
		return *errorData;
	} else {
		return *it;
	}
}

const RIDDatabase::RIDMetadata& RIDDatabase::getMetadata(const std::string& humanReadableID) {
	RIDMetadataTable::nth_index<1>::type::iterator it = metadataTable.get<1>().find(humanReadableID);
	if(it == metadataTable.get<1>().end()) {
		return *errorData;
	} else {
		return *it;
	}
}

const std::string& RIDDatabase::getHumanReadableID(const RIDValue& value) {
	RIDMetadataTable::nth_index<0>::type::iterator it = metadataTable.get<0>().find(value);
	if(it == metadataTable.get<0>().end()) {
		return errorData->humanReadableID;
	} else {
		return it->humanReadableID;
	}
}
const RIDDatabase::RIDValue& RIDDatabase::getValueID(const std::string& humanReadableID) {
	RIDMetadataTable::nth_index<1>::type::iterator it = metadataTable.get<1>().find(humanReadableID);
	if(it == metadataTable.get<1>().end()) {
		return errorData->valueID;
	} else {
		return it->valueID;
	}
}

RIDDatabase::RIDMetadata* RIDDatabase::getErrorData() {
	return errorData;
}
