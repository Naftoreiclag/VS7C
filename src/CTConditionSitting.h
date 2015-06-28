/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CTCONDITIONSITTING_H
#define CTCONDITIONSITTING_H

#include "CharacterTaskCondition.h"

class CTConditionSitting : public CharacterTaskCondition {
public:
	CTConditionSitting(bool sitting);
	virtual ~CTConditionSitting();

	const bool sitting;

	virtual bool isFulfilled(CharacterState state) const;
};

#endif // CTCONDITIONSITTING_H
