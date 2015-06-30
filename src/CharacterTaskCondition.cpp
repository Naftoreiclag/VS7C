/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CharacterTaskCondition.h"

CharacterTaskCondition::CharacterTaskCondition() {}
CharacterTaskCondition::~CharacterTaskCondition() {}
bool CharacterTaskCondition::isValid(const CharacterState& state) const { return true; }

CharacterState::CharacterState(CharacterPerformerComponent* perf, CharacterBodyComponent* body, CharacterPhysicsComponent* charPhys, PhysicsComponent* phys)
: perf(perf),
body(body),
charPhys(charPhys),
phys(phys) {
    valid = perf != 0 && body != 0 && charPhys != 0 && phys != 0;
}

CharacterState::CharacterState(nres::Entity& entity) {
    perf = (CharacterPerformerComponent*) entity.getComponentData(RID("comp character performer"));
	body = (CharacterBodyComponent*) entity.getComponentData(RID("comp character body"));
	charPhys = (CharacterPhysicsComponent*) entity.getComponentData(RID("comp character physics"));
	phys = (PhysicsComponent*) entity.getComponentData(RID("comp physics"));

    valid = perf != 0 && body != 0 && charPhys != 0 && phys != 0;
}
