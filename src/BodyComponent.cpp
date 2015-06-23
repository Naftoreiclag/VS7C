#include "BodyComponent.h"

BodyComponent::BodyComponent(const HierarchicalBooleanShape* bodyShape)
: bodyShape(bodyShape),
occupation(HierarchicalBoolean(*bodyShape)) {
	//ctor
}

BodyComponent* BodyComponent::clone() const {
	return new BodyComponent(bodyShape);
}

BodyComponent::~BodyComponent()
{
	//dtor
}
