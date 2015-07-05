/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CONVERSATIONCOMPONENT_H
#define CONVERSATIONCOMPONENT_H

#include "BaseComponent.h"
#include "NREntitySystem.h"

// 8
class ConversationComponent : public BaseComponent {
public:
	ConversationComponent();
	virtual ConversationComponent* clone() const;
	virtual ~ConversationComponent();

};

#endif // CONVERSATIONCOMPONENT_H
