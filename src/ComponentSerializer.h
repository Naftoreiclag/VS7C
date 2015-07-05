/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef COMPONENTSERIALIZER_H
#define COMPONENTSERIALIZER_H

#include "json/json.h"
#include "NREntitySystem.h"
#include "RID.h"

class ComponentSerializer {
public:
	ComponentSerializer();
	virtual ~ComponentSerializer();

	virtual ComponentData* read(Json::Value data) = 0;
	virtual Json::Value write(ComponentData* data) = 0;
};

namespace ComponentSerializerRegistry {
	void setSerializer(RID, ComponentSerializer* serializer);
	nres::ComponentData* deserialize(RID id, Json::Value data);
}

#endif // COMPONENTSERIALIZER_H
