#include "ConversationComponent.h"

ConversationComponent::ConversationComponent() {
}

ConversationComponent* ConversationComponent::clone() const {
	return new ConversationComponent();
}

ConversationComponent::~ConversationComponent() {
}
