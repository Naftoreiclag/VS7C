/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "ComponentSerializer.h"

#include <map>

ComponentSerializer::ComponentSerializer() {}
ComponentSerializer::~ComponentSerializer() {}

namespace ComponentSerializerRegistry {

	std::map<RID, ComponentSerializer*> serializers;

	void setSerializer(RID id, ComponentSerializer* serializer) {
		serializers[id] = serializer;
	}

	nres::ComponentData* deserialize(RID id, const Json::Value& data) {
		std::map<RID, ComponentSerializer*>::iterator loc = serializers.find(id);

		if(loc == serializers.end()) {
			return 0;
		}

		ComponentSerializer* serializer = loc->second;
		return serializer->read(data);
	}
	Json::Value serialize(RID id, const nres::ComponentData* data) {
		std::map<RID, ComponentSerializer*>::iterator loc = serializers.find(id);

		if(loc == serializers.end()) {
			return Json::objectValue;
		}

		ComponentSerializer* serializer = loc->second;
		return serializer->write(data);
	}
}

