/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "HierarchicalShapeBuilder.h"
#include "HierarchicalBooleanShape.h"

#include <algorithm>

HierarchicalShapeBuilder::HierarchicalShapeBuilder()
: nextID(0) {
}

HierarchicalShapeBuilder::~HierarchicalShapeBuilder() {

}

HierarchicalShapeBuilder::IDType HierarchicalShapeBuilder::declareNode(NameType name) {
    std::map<NameType, IDType>::iterator location = nameToID.find(name);

    // Node is not already registered
    if(location == nameToID.end()) {
		nameToID[name] = nextID;
		Node newNode;
		newNode.ID = nextID;
		idToNode[nextID] = newNode;
		++ nextID;

		return nextID - 1;
    }

    // Node is registered
    else {
		return location->second;
    }
}

HierarchicalShapeBuilder::Node& HierarchicalShapeBuilder::getNodeFromID(IDType id) {
	return idToNode.find(id)->second;
}

void HierarchicalShapeBuilder::easyAddID(std::vector<IDType>& vec, IDType id) {
	bool found = false;
    for(std::vector<IDType>::iterator it = vec.begin(); it != vec.end(); ++ it) {
		if(*it == id) {
			found = true;
			break;
		}
    }

    if(!found) {
		vec.push_back(id);
    }
}


void HierarchicalShapeBuilder::declareRelationship(NameType parent, NameType child) {
	IDType parentID = declareNode(parent);
	IDType childID = declareNode(child);

	Node& parentNode = getNodeFromID(parentID);
	Node& childNode = getNodeFromID(childID);

	easyAddID(parentNode.children, childID);
	easyAddID(childNode.parents, parentID);
}

/*
Removes all redundant data.
For each node, remove from the child list any children who are not immediate.
For example:
	A is parent to B and C.
	B is parent to C.
In this case, A's knowledge of C will be erased since it knows through B that C is a grandchild.

Parent knowledge is modified afterward
*/
void HierarchicalShapeBuilder::cleanRedundantData() {
	// Clean child data
	for(std::map<IDType, Node>::iterator nodeIt = idToNode.begin(); nodeIt != idToNode.end(); ++ nodeIt) {
		Node& focus = nodeIt->second;

		// Remember which children to remove from the list
		std::vector<IDType> removeUs;
		for(std::vector<IDType>::iterator childIt = focus.children.begin(); childIt != focus.children.begin(); ++ childIt) {
			IDType childID = *childIt;

			// If this parent is an ancestor of child in a way other than being an immediate parent
			if(isAncestor(childID, focus.ID, false)) {
				removeUs.push_back(childID);
			}
		}

		for(std::vector<IDType>::iterator childIt = removeUs.begin(); childIt != removeUs.begin(); ++ childIt) {
			// erase-remove idiom
            focus.children.erase(std::remove(focus.children.begin(), focus.children.end(), *childIt), focus.children.end());
		}
	}

	// Delete all parent data
	for(std::map<IDType, Node>::iterator nodeIt = idToNode.begin(); nodeIt != idToNode.end(); ++ nodeIt) {
		Node& focus = nodeIt->second;
		focus.parents.clear();
	}

	// Update parent data
	for(std::map<IDType, Node>::iterator nodeIt = idToNode.begin(); nodeIt != idToNode.end(); ++ nodeIt) {
		Node& focus = nodeIt->second;

		// For each node, tell its childen (which should now only be immediate children) that this node is their parent
		for(std::vector<IDType>::iterator childIt = focus.children.begin(); childIt != focus.children.begin(); ++ childIt) {
			IDType childID = *childIt;
			Node& childNode = getNodeFromID(childID);

            easyAddID(childNode.parents, focus.ID);
		}
	}
}

bool HierarchicalShapeBuilder::isAncestor(IDType child, IDType search, bool checkImmediate) {

	Node childNode = getNodeFromID(child);

	for(std::vector<IDType>::iterator parentIt = childNode.parents.begin(); parentIt != childNode.parents.end(); ++ parentIt) {
		IDType parentID = *parentIt;

		if(checkImmediate) {
			if(parentID == search) {
				return true;
			}
		}

		bool recurse = isAncestor(parentID, search, true);
		if(recurse) {
			return true;
		}
	}
	return false;
}

HierarchicalBooleanShape* HierarchicalShapeBuilder::makeNewBooleanShape() {
	cleanRedundantData();

    HierarchicalBooleanShape* retVal = new HierarchicalBooleanShape();

    // Register every node in advance
	for(std::map<IDType, Node>::iterator nodeIt = idToNode.begin(); nodeIt != idToNode.end(); ++ nodeIt) {
		Node& focus = nodeIt->second;
		retVal->affects[focus.ID] = std::vector<IDType>();
		retVal->affectedBy[focus.ID] = std::vector<IDType>();
	}

	// For every node
	for(std::map<IDType, Node>::iterator nodeIt = idToNode.begin(); nodeIt != idToNode.end(); ++ nodeIt) {
		Node& focus = nodeIt->second;

		// Nodes affect every ancestor
		recursiveAffectsParents(retVal, focus.ID);

		// Nodes affect every descendant
		recursiveAffectsChildren(retVal, focus.ID);


	}

    return retVal;
}

void HierarchicalShapeBuilder::recursiveAffectsParents(HierarchicalBooleanShape* retVal, IDType focusID) {

	Node& focus = getNodeFromID(focusID);
	std::vector<IDType>& affects = retVal->affects[focusID];
	for(std::vector<IDType>::iterator parentIt = focus.parents.begin(); parentIt != focus.parents.begin(); ++ parentIt) {
		IDType parentID = *parentIt;
		std::vector<IDType>& affectedBy = retVal->affectedBy[parentID];

		affects.push_back(parentID);
		affectedBy.push_back(focusID);

		recursiveAffectsChildren(retVal, parentID);
	}
}

void HierarchicalShapeBuilder::recursiveAffectsChildren(HierarchicalBooleanShape* retVal, IDType focusID) {

	Node& focus = getNodeFromID(focusID);
	std::vector<IDType>& affects = retVal->affects[focusID];
	for(std::vector<IDType>::iterator childIt = focus.children.begin(); childIt != focus.children.begin(); ++ childIt) {
		IDType childID = *childIt;
		std::vector<IDType>& affectedBy = retVal->affectedBy[childID];

		affects.push_back(childID);
		affectedBy.push_back(focusID);

		recursiveAffectsChildren(retVal, childID);
	}
}
