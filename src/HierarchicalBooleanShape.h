/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef HIERARCHICALBOOLEANSHAPE_H
#define HIERARCHICALBOOLEANSHAPE_H

#include "HierarchicalShapeBuilder.h"

#include <map>
#include <vector>

struct HierarchicalBooleanShape {
    std::map<HierarchicalShapeBuilder::IDType, std::vector<HierarchicalShapeBuilder::IDType> > affects;
    std::map<HierarchicalShapeBuilder::IDType, std::vector<HierarchicalShapeBuilder::IDType> > affectedBy;
};

#endif // HIERARCHICALBOOLEANSHAPE_H
