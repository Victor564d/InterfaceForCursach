#include "include/data_utils.h"

unsigned int util_hashCodeFromAbonentStruct(const abonent_t* a)
{
	return util_hashCodeFromFio(&a->fio);
}

unsigned int util_hashCodeFromFio(const fio_t* fio)
{
	return util_hashCodeFromString(fio->name) + util_hashCodeFromString(fio->secondname) + util_hashCodeFromString(fio->surname);
}

unsigned int util_hashCodeFromString(const char* string)
{
	unsigned int ret = 0;

	for (int position = 0; string[position] != NULL; ++position)
	{
		ret += string[position];
	}

	return ret;
}
