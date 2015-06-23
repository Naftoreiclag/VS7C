/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef HIERARCHICALSHAPEBUILDER_H
#define HIERARCHICALSHAPEBUILDER_H

#include "irrlicht.h"

#include "RID.h"

#include <vector>
#include <map>

class HierarchicalBooleanShape;
class HierarchicalShapeBuilder {
public:
	typedef irr::u32 IDType;
	typedef RID NameType;
private:
	struct Node {
		IDType ID;
		std::vector<IDType> parents;
		std::vector<IDType> children;
	};
public:
	HierarchicalShapeBuilder();
	virtual ~HierarchicalShapeBuilder();

	void cleanRedundantData();
	HierarchicalBooleanShape* makeNewBooleanShape();

	IDType declareNode(NameType name);
	void declareRelationship(NameType parent, NameType child);

	bool isAncestor(IDType child, IDType parent, bool checkImmediate); // Recursive search, checks if parent is an ancestor to child, optionally ignoring immediate parents
	void recursiveAffectsParents(HierarchicalBooleanShape* shape, IDType receiver, IDType focus);
	void recursiveAffectsChildren(HierarchicalBooleanShape* shape, IDType receiver, IDType focus);
private:
	std::map<NameType, IDType> nameToID;
	std::map<IDType, Node> idToNode;
	IDType nextID;
private:
	Node& getNodeFromID(IDType id);
	void easyAddID(std::vector<IDType>& vec, IDType id);
};

#endif // HIERARCHICALSHAPEBUILDER_H
