/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERIDENTITYCOMPONENT_H
#define CHARACTERIDENTITYCOMPONENT_H

#include <string>
#include "NREntitySystem.h"

class CharacterIdentityComponent : public nres::ComponentData {
public:
	std::string name;

	CharacterIdentityComponent();
	virtual CharacterIdentityComponent* clone() const;
	virtual ~CharacterIdentityComponent();
protected:
private:
};

#endif // CHARACTERIDENTITYCOMPONENT_H
