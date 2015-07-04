/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef NRSALLIST_H
#define NRSALLIST_H

#include "nrsalValue.h"

namespace nrsal {
	class List {
	public:
		List();
		virtual ~List();

		void add(Value value);
	};
}
#endif // NRSALLIST_H
