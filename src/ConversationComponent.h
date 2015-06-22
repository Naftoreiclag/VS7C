#ifndef CONVERSATIONCOMPONENT_H
#define CONVERSATIONCOMPONENT_H

#include "NREntitySystem.h"

// 8
class ConversationComponent : public nres::ComponentData {
public:
	ConversationComponent();
	virtual ConversationComponent* clone() const;
	virtual ~ConversationComponent();

};

#endif // CONVERSATIONCOMPONENT_H
