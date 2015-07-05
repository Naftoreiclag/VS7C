/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include "json/json.h"
#include "NREntitySystem.h"

class BaseComponent : public nres::ComponentData {
public:
	BaseComponent();
	virtual ~BaseComponent();
};

#endif // BASECOMPONENT_H
