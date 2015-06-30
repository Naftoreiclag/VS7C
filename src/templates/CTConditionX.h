/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CTCONDITIONX_H
#define CTCONDITIONX_H

#include "CharacterTaskCondition.h"

class CTConditionX : public CharacterTaskCondition {
public:
	CTConditionX();
	virtual ~CTConditionX();

	virtual bool isFulfilled(const CharacterState& state) const;
};

#endif // CTCONDITIONX_H
