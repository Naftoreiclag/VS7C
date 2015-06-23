/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "RID.h"

RID::RID(RIDValue value)
: value(value) { }

RID::RID(std::string humanReadableID) {
	value = RIDDatabase::getValueID(humanReadableID);
}

RID::RID(const RID& arg) {
	value = arg.value;
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

RIDDatabase::RIDMetadata::RIDMetadata(RIDValue valueID, std::string humanReadableID, std::string humanDesc)
:valueID(valueID),
humanReadableID(humanReadableID),
humanDesc(humanDesc) { }

void RIDDatabase::addRID(const RIDValue& value, const std::string& humanReadableID, const std::string& humanDesc) {
	metadataTable.insert(RIDMetadata(value, humanReadableID, humanDesc));
}

const RIDDatabase::RIDMetadata& RIDDatabase::getMetadata(const RIDValue& value) {
	RIDMetadataTable::nth_index<0>::type::iterator it = metadataTable.get<0>().find(value);
	return *it;
}

const RIDDatabase::RIDMetadata& RIDDatabase::getMetadata(const std::string& humanReadableID) {
	RIDMetadataTable::nth_index<1>::type::iterator it = metadataTable.get<1>().find(humanReadableID);
	return *it;
}

const std::string& RIDDatabase::getHumanReadableID(const RIDValue& value) {
	RIDMetadataTable::nth_index<0>::type::iterator it = metadataTable.get<0>().find(value);
	return it->humanDesc;
}
const RIDDatabase::RIDValue& RIDDatabase::getValueID(const std::string& humanReadableID) {
	RIDMetadataTable::nth_index<1>::type::iterator it = metadataTable.get<1>().find(humanReadableID);
	return it->valueID;
}
