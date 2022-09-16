#include <stdlib.h>
#include "../ThreeStruct.h"

#ifndef _DATAUTILS_H
#define _DATAUTILS_H

extern unsigned int util_hashCodeFromAbonentStruct(const abonent_t*);
extern unsigned int util_hashCodeFromFio(const fio_t*);
extern unsigned int util_hashCodeFromString(const char*);

#endif //! _DATAUTILS_H