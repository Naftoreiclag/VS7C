/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "ConversationComponent.h"

ConversationComponent::ConversationComponent() {
}

ConversationComponent* ConversationComponent::clone() const {
	return new ConversationComponent();
}

ConversationComponent::~ConversationComponent() {
}
