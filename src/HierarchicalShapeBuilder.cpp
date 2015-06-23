/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "HierarchicalShapeBuilder.h"
#include "HierarchicalBoolean.h"

#include <algorithm>
#include <iostream>

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

		std::cout << "New node declared: " << name << std::endl;

		return nextID - 1;
    }

    // Node is registered
    else {
		std::cout << "Node access: " << location->second << std::endl;
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
		std::cout << "Data added: " << id << std::endl;
		vec.push_back(id);
    }
}


void HierarchicalShapeBuilder::declareRelationship(NameType parent, NameType child) {
	std::cout << "New relationship: " << parent << " is parent of " << child << std::endl;

	IDType parentID = declareNode(parent);
	IDType childID = declareNode(child);

	Node& parentNode = getNodeFromID(parentID);
	Node& childNode = getNodeFromID(childID);

/*
	easyAddID(parentNode.children, childID);
	easyAddID(childNode.parents, parentID);
	*/

	parentNode.children.push_back(childID);
	childNode.parents.push_back(parentID);

	std::cout << child << " now has " << getNodeFromID(childID).parents.size() << " parent(s)" << std::endl;
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
		for(std::vector<IDType>::iterator childIt = focus.children.begin(); childIt != focus.children.end(); ++ childIt) {
			IDType childID = *childIt;

			// If this parent is an ancestor of child in a way other than being an immediate parent
			if(isAncestor(childID, focus.ID, false)) {
				removeUs.push_back(childID);
			}
		}

		for(std::vector<IDType>::iterator childIt = removeUs.begin(); childIt != removeUs.end(); ++ childIt) {
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
		for(std::vector<IDType>::iterator childIt = focus.children.begin(); childIt != focus.children.end(); ++ childIt) {
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
		std::cout << "Creating affects/affector grids for: " << focus.ID << std::endl;
		retVal->affects[focus.ID] = std::vector<IDType>();
		retVal->affectedBy[focus.ID] = std::vector<IDType>();
	}

	// For every node
	for(std::map<IDType, Node>::iterator nodeIt = idToNode.begin(); nodeIt != idToNode.end(); ++ nodeIt) {
		Node& focus = nodeIt->second;
		std::cout << "Populating grids for: " << focus.ID << std::endl;

		// Nodes affect every ancestor
		recursiveAffectsParents(retVal, focus.ID, focus.ID);

		// Nodes affect every descendant
		recursiveAffectsChildren(retVal, focus.ID, focus.ID);
	}

	//
	retVal->nameToID = nameToID;

    return retVal;
}

void HierarchicalShapeBuilder::recursiveAffectsParents(HierarchicalBooleanShape* retVal, IDType receiver, IDType focusID) {

	Node& focus = getNodeFromID(focusID);
	std::cout << "Element " << focus.ID << " has " << focus.parents.size() << " parent(s)" << std::endl;
	for(std::vector<IDType>::iterator parentIt = focus.parents.begin(); parentIt != focus.parents.end(); ++ parentIt) {
		IDType parentID = *parentIt;

		std::cout << receiver << " affects " << parentID << std::endl;

		retVal->affects[receiver].push_back(parentID);
		retVal->affectedBy[parentID].push_back(receiver);

		recursiveAffectsParents(retVal, receiver, parentID);
	}
}

void HierarchicalShapeBuilder::recursiveAffectsChildren(HierarchicalBooleanShape* retVal, IDType receiver, IDType focusID) {

	Node& focus = getNodeFromID(focusID);
	std::cout << "Element " << focus.ID << " has " << focus.children.size() << " child(ren)" << std::endl;
	for(std::vector<IDType>::iterator childIt = focus.children.begin(); childIt != focus.children.end(); ++ childIt) {
		IDType childID = *childIt;

		std::cout << receiver << " affects " << childID << std::endl;

		retVal->affects[receiver].push_back(childID);
		retVal->affectedBy[childID].push_back(receiver);

		recursiveAffectsChildren(retVal, receiver, childID);
	}
}
