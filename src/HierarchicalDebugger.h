/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef HIERARCHICALDEBUGGER_H
#define HIERARCHICALDEBUGGER_H

#include <iostream>
#include <string>

#include "HierarchicalBoolean.h"
#include "HierarchicalShapeBuilder.h"

class HierarchicalDebugger
{
	public:
		/*
		HierarchicalDebugger(){};

		RID getName(const HierarchicalBooleanShape* shape, const irr::u32 id) {
			for(auto it = shape->nameToID.begin(); it != shape->nameToID.end(); ++ it) {

				if(it->second == id) {
					return it->first;
				}
			}
		}

		void debug(HierarchicalBooleanShape* shape) {
			for(auto it = shape->affects.begin(); it != shape->affects.end(); ++ it) {

				std::cout << getName(shape, it->first);

				std::vector<irr::u32> affects = it->second;
				std::cout << " :";

				for(auto itt = affects.begin(); itt != affects.end(); ++ itt) {


					std::cout << " ";

					std::cout << getName(shape, *itt);
				}

				std::cout  << std::endl;
			}
		}

		void debug(HierarchicalBoolean& boolean) {
			for(int i = 0; i < boolean.shape.nameToID.size(); ++ i) {
				std::cout << getName(&boolean.shape, i) << ": ";
				//std::cout << ((int) *(boolean.bits + i)) << std::endl;


			}


		}
*/
	protected:
	private:
};

#endif // HIERARCHICALDEBUGGER_H
