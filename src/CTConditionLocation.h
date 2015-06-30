/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CTCONDITIONLOCATION_H
#define CTCONDITIONLOCATION_H

#include "CharacterTaskCondition.h"
#include "PhysicsComponent.h"
#include "btBulletDynamicsCommon.h"

#include "irrlicht.h"
#include "nresEntity.h"

class CTConditionLocation : public CharacterTaskCondition {
public:
	enum Type {
		NEXT_TO_ENTITY,
		NEXT_TO_POINT,
		CUBOID
	};
private:
	PhysicsComponent* otherPhys;
public:
	// Be within (dist) units of (entity)
	CTConditionLocation(const btScalar dist, nres::Entity& other);

	// Be within (dist) units of (loc)
	CTConditionLocation(const btScalar dist, const btVector3 loc);

	virtual ~CTConditionLocation();
	virtual bool isFulfilled(const CharacterState& state) const;
	virtual bool isValid(const CharacterState& state) const;

	const btVector3& getClosestPoint(const btVector3& other) const;

	const Type type;

	const btScalar dist;
	nres::Entity* entity;
	const btVector3 loc;
};

#endif // CTCONDITIONLOCATION_H
