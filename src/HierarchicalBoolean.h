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
	std::map<HierarchicalShapeBuilder::NameType, HierarchicalShapeBuilder::IDType> nameToID;
    std::map<HierarchicalShapeBuilder::IDType, std::vector<HierarchicalShapeBuilder::IDType> > affects;
    std::map<HierarchicalShapeBuilder::IDType, std::vector<HierarchicalShapeBuilder::IDType> > affectedBy;
};

class HierarchicalBoolean {
public:
	typedef HierarchicalShapeBuilder::NameType NameType;
	typedef HierarchicalShapeBuilder::IDType IDType;
public:
	HierarchicalBoolean(const HierarchicalBooleanShape shape);
	const HierarchicalBooleanShape shape;
	virtual ~HierarchicalBoolean();
public:
	void setBoolean(HierarchicalShapeBuilder::NameType name, bool boolean);
	void setWeakBoolean(HierarchicalShapeBuilder::NameType name);

	bool getBoolean(HierarchicalShapeBuilder::NameType name);
private:
	void setValue(HierarchicalShapeBuilder::NameType name, irr::u8 value);
	void update(IDType boolID);

	static const irr::u8 vFalse = 0;
	static const irr::u8 vInfTrue = 1;
	static const irr::u8 vWeakTrue = 2;
	static cosnt irr::u8 vTrue = 3;

	const std::size_t shapeSize;
	irr::u8* bits;

};

#endif // HIERARCHICALBOOLEAN_H
