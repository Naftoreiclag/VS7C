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

	virtual nres::ComponentData* read(const Json::Value& data) = 0;
	virtual Json::Value write(const nres::ComponentData* data) = 0;
};

namespace ComponentSerializerRegistry {
	void setSerializer(RID id, ComponentSerializer* serializer);
	nres::ComponentData* deserialize(RID id, const Json::Value& data);
	Json::Value serialize(RID id, const nres::ComponentData* data);
}

#endif // COMPONENTSERIALIZER_H
