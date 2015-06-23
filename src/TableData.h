#ifndef TABLEDATA_H
#define TABLEDATA_H

#include "irrlicht.h"

class TableData {
public:
	TableData(const irr::u32 numColumns);
	const irr::u32 numColumns;
	virtual ~TableData();
protected:
private:
};

#endif // TABLEDATA_H
