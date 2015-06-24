/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CharacterBodyComponent.h"

CharacterBodyComponent::CharacterBodyComponent() {

}

CharacterBodyComponent* CharacterBodyComponent::clone() const {
	return new CharacterBodyComponent();
}

CharacterBodyComponent::~CharacterBodyComponent() {

}
