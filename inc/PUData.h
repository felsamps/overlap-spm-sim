#ifndef PUDATA_H
#define	PUDATA_H

#include <list>

#include "TypeDefs.h"
#include "Entry.h"

using namespace std;

class PUData {
private:
    UInt idPart, sizePart, idRefFrame;
    list<Entry*> entries;
public:
    PUData(UInt idPart, UInt sizePart, UInt idRefFrame);
    
    void insertEntry(Entry* e);
    Entry* getEntry();
    
    void setIdRefFrame(UInt idRefFrame);
    UInt getIdRefFrame() const;
    void setSizePart(UInt sizePart);
    UInt getSizePart() const;
    void setIdPart(UInt idPart);
    UInt getIdPart() const;

};

#endif	/* PUDATA_H */

