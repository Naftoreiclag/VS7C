/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "RID.h"

RID::RID(RIDValue value)
: value(value) {

}

RID::RID(std::string humanReadableID) {
	value = 0;
}

RID::RID(const RID& arg) {
	value = arg.value;
}

RID& RID::operator=(const RID& arg) {
	value = arg.value;
	return *this;
}

bool RID::operator==(const RID& arg) {
	return arg.value == value;
}

bool RID::operator<(const RID& arg) {
	return arg.value < value;
}

RID::operator RIDValue() const {
	return value;
}

