/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "ComponentSerializer.h"

#include <map>

ComponentSerializer::ComponentSerializer()
{
	//ctor
}

ComponentSerializer::~ComponentSerializer()
{
	//dtor
}

namespace ComponentSerializerRegistry {

	std::map<RID, ComponentSerializer*> serializers;

	void setSerializer(RID, ComponentSerializer* serializer) {
		serializers[RID] = serializer;
	}

	nres::ComponentData* deserialize(RID id, Json::Value data) {
		std::map<RID, ComponentSerializer*>::iterator loc = serializers.find(id);

		if(loc == serializers.end()) {
			return 0;
		}

		ComponentSerializer* serializer = loc->second;
		return serializer.read(data);
	}
}

