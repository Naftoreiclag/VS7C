/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef HIERARCHICALBOOLEAN_H
#define HIERARCHICALBOOLEAN_H

#include "HierarchicalShapeBuilder.h"

#include <map>
#include <vector>

#include "irrlicht.h"

struct HierarchicalBooleanShape {
	typedef HierarchicalShapeBuilder::NameType NameType;
	typedef HierarchicalShapeBuilder::IDType IDType;
	std::map<NameType, IDType> nameToID;
    std::map<IDType, std::vector<IDType> > affects;
    std::map<IDType, std::vector<IDType> > affectedBy;
};

class HierarchicalBoolean {
public:
	typedef HierarchicalShapeBuilder::NameType NameType;
	typedef HierarchicalShapeBuilder::IDType IDType;
public:
	HierarchicalBoolean(const HierarchicalBooleanShape& shape);
	const HierarchicalBooleanShape& shape;
	virtual ~HierarchicalBoolean();
public:
	void setBoolean(NameType name, bool boolean);
	void setWeakTrue(NameType name);

	bool getBoolean(NameType name);
private:
	void setValue(NameType name, irr::u8 value);

	static const irr::u8 vFalse = 0;
	static const irr::u8 vInfTrue = 1;
	static const irr::u8 vWeakTrue = 2;
	static const irr::u8 vTrue = 3;

	const std::size_t shapeSize;
	irr::u8* bits;

	void updateTrueness(IDType boolID);
};

#endif // HIERARCHICALBOOLEAN_H
