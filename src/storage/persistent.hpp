#ifndef PERSISTENT_H_
#define PERSISTENT_H_

#include "objloader.hpp"
#include "sens.hpp"
#include <map>
class OBJData
{
public:
	static int size() { return objdata.size(); }
    static objl::Loader* AddElement(const std::string& filename);
    static objl::Loader* GetElement(const std::string& filename);
    static void RemoveElement(const std::string& filename);
    static std::map<std::string, objl::Loader> objdata;
    static Eigen::Vector3f Intersect(Eigen::Vector4f d, Eigen::Vector4f t, std::string& filename);
    static void SaveModelToFile(const std::string& filename);
};

extern SensData sens_data;
double rayIntersectsTriangle(const Eigen::Vector3f& v0, const Eigen::Vector3f& v1, const Eigen::Vector3f& v2,
                             const Eigen::Vector3f &orig, const Eigen::Vector3f &dir);
#endif
