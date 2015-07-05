/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef NRSALVALUE_H
#define NRSALVALUE_H


namespace nrsal {
	class Value {
	public:
		enum Type {
			MAP,
			LIST,
			BOOLEAN,
			STRING,
			NUMBER, // long double, use raw for other binary formats
			RAW,
			OTHER
		};
	public:
		Value();
		virtual ~Value();
	};
}

#endif // NRSALVALUE_H
