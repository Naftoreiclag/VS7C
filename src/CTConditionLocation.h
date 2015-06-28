/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CTCONDITIONLOCATION_H
#define CTCONDITIONLOCATION_H

#include "CharacterTaskCondition.h"

class CTConditionLocation : public CharacterTaskCondition {
public:
	CTConditionLocation();
	virtual ~CTConditionLocation();
	virtual bool isFulfilled(CharacterState state) const;
};

#endif // CTCONDITIONLOCATION_H
