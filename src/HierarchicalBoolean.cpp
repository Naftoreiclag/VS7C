/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "HierarchicalBoolean.h"

HierarchicalBoolean::HierarchicalBoolean(const HierarchicalBooleanShape shape)
: shape(shape),
shapeSize(shape.nameToID.size()) {
	bits = new irr::u8[shapeSize];
}

HierarchicalBoolean::~HierarchicalBoolean() {
	delete[] bits;
}

void HierarchicalBoolean::setBoolean(NameType name, bool boolean) {
	if(boolean) {
		setValue(name, vTrue);
	}
}

void HierarchicalBoolean::setValue(NameType name, irr::u8 value) {
	std::map<NameType, IDType>::const_iterator location = shape.nameToID.find(name);
	if(location == shape.nameToID.end()) {
		return;
	}

	const IDType& boolID = location->second;

	irr::u8* valuePtr = bits + boolID
	irr::u8 prevVal = *valuePtr;

	// If this method will make no changes, skip any potential update calls
	if(prevVal == value) {
		return;
	}

	if(value == vWeakTrue) {
		if(prevVal == vFalse) {

		}
		else
	}

	switch(value) {
	case vFalse:
		*valuePtr = value;
		break;

	case vInfTrue:
		*valuePtr = value;
		break;

	case vWeakTrue:
		*valuePtr = value;
		break;

	case vTrue:

		break;

	default:
		break;
	}

	*(bits + boolID) = value;


	if(value == vTrue) {
		update(boolID);
	}
}
