//-------------------------------- datadict.cpp --------------------------------

#include "datadict.h"

FXIMPLEMENT(DataDict, FXDict, NULL, 0)

DataDict::DataDict(void) {}

DataDict::~DataDict(void) { clear() }

DataDict::createData(const void *aPtr)
{
  FXint Entries;
  FXdouble *temp= (FXdouble*)aPtr;
  FXdouble *internal;

  Entries= ARRAYNUMBER(temp);
  FXMALLOC(&internal, FXdouble, Entries);
  memcpy(internal, temp, sizeof(FXdouble)*Entries);

  return (void*)internal;
}

DataDict::deleteData(void *aPtr)
{
  if (aPtr) FXFREE(&aPtr);
}

//-------------------------------- datadict.cpp --------------------------------
