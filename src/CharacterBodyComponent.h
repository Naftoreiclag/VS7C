/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERBODYCOMPONENT_H
#define CHARACTERBODYCOMPONENT_H

#include "ComponentSerializer.h"
#include "BaseComponent.h"
#include "NREntitySystem.h"
#include "RID.h"

class CharacterBodyComponent : public BaseComponent {
public:
	RID bodyPosition;

	bool sitting = false;

	CharacterBodyComponent();
	virtual CharacterBodyComponent* clone() const;
	virtual ~CharacterBodyComponent();
protected:
private:
};

class CharacterBodySerializer : public ComponentSerializer {
public:
	virtual nres::ComponentData* read(const Json::Value& data);
	virtual Json::Value write(const nres::ComponentData* data);
};

#endif // CHARACTERBODYCOMPONENT_H
