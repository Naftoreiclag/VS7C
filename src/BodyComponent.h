#ifndef BODYCOMPONENT_H
#define BODYCOMPONENT_H

#include "NREntitySystem.h"
#include "HierarchicalBoolean.h"

class BodyComponent {
public:
	BodyComponent(const HierarchicalBooleanShape* bodyShape);
	virtual BodyComponent* clone() const;
	virtual ~BodyComponent();

	const HierarchicalBooleanShape* bodyShape;
	HierarchicalBoolean occupation;
};

#endif // BODYCOMPONENT_H
