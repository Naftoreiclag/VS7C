/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef INTERACTIONCOMPONENT_H
#define INTERACTIONCOMPONENT_H

#include "NREntitySystem.h"
#include "ComponentIDs.h"

/*
Defines entities that can be interacted with
*/
class InteractionComponent : public nres::ComponentData {
public:
	InteractionComponent();
};

#endif // INTERACTIONCOMPONENT_H
