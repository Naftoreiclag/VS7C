/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "ReiJson.h"

#include "BaseComponent.h"
#include "ComponentSerializer.h"

namespace reij {

	Json::Value entityToValue(const nres::Entity& entity) {

		Json::Value jsonRet;

		const std::map<nres::ComponentID, nres::ComponentData*>& compData = entity.getComponentDataMap();

		for(std::map<nres::ComponentID, nres::ComponentData*>::const_iterator it = compData.begin(); it != compData.end(); ++ it) {
			const nres::ComponentID& compID = it->first;
			const nres::ComponentData* rawData = it->second;

			//const BaseComponent* data = static_cast<const BaseComponent*>(rawData);

			jsonRet[compID.getHumanReadableID()] = ComponentSerializerRegistry::serialize(compID, rawData);
		}

		return jsonRet;
	}
};
