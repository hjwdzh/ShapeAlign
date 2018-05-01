#ifndef PERSISTENT_H_
#define PERSISTENT_H_

#include "objloader.hpp"
#include "sens.h"
#include <map>
class OBJData
{
public:
	static int size() { return objdata.size(); }
    static objl::Loader* AddElement(const std::string& filename);
    static objl::Loader* GetElement(const std::string& filename);
    static void RemoveElement(const std::string& filename);
    static std::map<std::string, objl::Loader> objdata;
};

extern SensData sens_data;
#endif
