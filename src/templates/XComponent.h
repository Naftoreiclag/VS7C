/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef XCOMPONENT_H
#define XCOMPONENT_H

#include "NREntitySystem.h"

class XComponent : public nres::ComponentData {
public:
	XComponent();
	virtual XComponent* clone() const;
	virtual ~XComponent();
protected:
private:
};

#endif // XCOMPONENT_H
