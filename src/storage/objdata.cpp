#include "objdata.hpp"

std::map<std::string, objl::Loader> OBJData::objdata;

objl::Loader* OBJData::AddElement(const std::string& filename)
{
    auto it = objdata.find(filename);
    if (it != objdata.end()) {
        return &(it->second);
    } else {
        objdata[filename] = objl::Loader();
        it = objdata.find(filename);
    }
    bool loadout = it->second.LoadFile(filename);
    if (!loadout) {
        objdata.erase(it);
        std::cerr << "Unable to load " << filename << "\n";
        return 0;
    }
    return &it->second;
}

objl::Loader* OBJData::GetElement(const std::string& filename) {
	return &objdata[filename];
}

