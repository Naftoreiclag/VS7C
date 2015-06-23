/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "HierarchicalBoolean.h"

HierarchicalBoolean::HierarchicalBoolean(const HierarchicalBooleanShape& shape)
: shape(shape),
shapeSize(shape.nameToID.size()) {
	bits = new irr::u8[shapeSize];
}

HierarchicalBoolean::~HierarchicalBoolean() {
	delete[] bits;
}

void HierarchicalBoolean::setBoolean(NameType name, bool boolean) {
	setValue(name, boolean ? vTrue : vFalse);
}
void HierarchicalBoolean::setWeakTrue(NameType name) {
	setValue(name, vWeakTrue);
}

bool HierarchicalBoolean::getBoolean(NameType name) {
    std::map<NameType, IDType>::const_iterator location = shape.nameToID.find(name);
	if(location == shape.nameToID.end()) {
		return false;
	}
	const IDType& boolID = location->second;

	irr::u8 rawValue = *(bits + boolID);
	return rawValue != vFalse;
}

void HierarchicalBoolean::setValue(NameType name, irr::u8 value) {
	std::map<NameType, IDType>::const_iterator location = shape.nameToID.find(name);
	if(location == shape.nameToID.end()) {
		return;
	}

	const IDType& boolID = location->second;

	irr::u8* valuePtr = bits + boolID;
	irr::u8 prevVal = *valuePtr;

	/*
	   F I W T
	F  r ? y u
	I  y r y u
	W  q ? r u
	T  q ? q r

	F = false
	I = influenced
	W = weak
	T = true

	r = return; there is nothing to do
	? = should not happen
	y = change the value
	u = change the value, update influence
	q = change the value, update all influences

	*/

	// If this method will make no changes, skip any potential update calls
	if(prevVal == value) {
		return;
	}

	/*
	Do nothing since it is already known that this boolean is influenced to be true
	influenced -> false
	*/
	if(prevVal == vInfTrue && value == vFalse) {
		return;
	}

	// Set the variable, every other combination will set the value
	*valuePtr = value;

	/*
	Do nothing, since weak does nothing:
	false -> weak
	influenced -> weak
	*/

	/*
	Undefined (should not happen by calling this method):
	? -> influenced
	*/

	/*
	When reducing influence, recalculate everything
	true -> false
	true -> weak
	weak -> false
	*/
	if((prevVal == vTrue && (value == vFalse || value == vWeakTrue)) || (prevVal == vWeakTrue && value == vFalse)) {
		// Reset all vInfTrue to false (i.e. preserve booleans set by user)
		for(std::size_t i = 0; i < shapeSize; ++ i) {
			if(*(bits + i) == vInfTrue) {
				*(bits + i) = vFalse;
			}
		}

		// Recalculate stuff
		for(std::map<NameType, IDType>::const_iterator it = shape.nameToID.begin(); it != shape.nameToID.end(); ++ it) {
			updateTrueness(it->second);
		}
	}

	/*
	Propagate influnce when making something true
	false -> true
	influenced -> true
	weak -> true
	*/
	if((prevVal == vFalse || prevVal == vInfTrue || prevVal == vWeakTrue) && value == vTrue) {
		updateTrueness(boolID);
	}
}

void HierarchicalBoolean::updateTrueness(IDType boolID) {
	std::vector<IDType> affects = shape.affects.find(boolID)->second;
	for(std::vector<IDType>::iterator it = affects.begin(); it != affects.end(); ++ it) {
        IDType affectedBooleanID = *it;
        irr::u8* valuePtr = bits + affectedBooleanID;
        irr::u8 prevVal = *valuePtr;

        if(prevVal == vFalse) {
			*valuePtr = vInfTrue;
        }
	}
}
