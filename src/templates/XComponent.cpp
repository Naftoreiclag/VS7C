/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "XComponent.h"

XComponent::XComponent()
{
	//ctor
}
XComponent* XComponent::clone() const {
	return new XComponent();
}

XComponent::~XComponent()
{
	//dtor
}
