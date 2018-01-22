//--------------------------------- datadict.h ---------------------------------

#ifndef DATADICT_H
#define DATADICT_H

#include <fox-1.6/fx.h>

class DataDict : public FXDict
{
    FXDECLARE(DataDict)
    public:

            DataDict(void);
            virtual ~DataDict(void);

            void* createData(const void*);
            void deleteData(void);
};

#endif
