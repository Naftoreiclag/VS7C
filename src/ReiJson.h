/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef REIJSON_H
#define REIJSON_H

#include "json/json.h"
#include "NREntitySystem.h"

namespace reij {

	Json::Value entityToValue(const nres::Entity& entity);

};

#endif // REIJSON_H
