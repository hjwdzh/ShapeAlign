#include "persistent.hpp"

std::map<std::string, objl::Loader> OBJData::objdata;

double rayIntersectsTriangle(const Eigen::Vector3f& v0, const Eigen::Vector3f& v1, const Eigen::Vector3f& v2,
                            const Eigen::Vector3f &orig, const Eigen::Vector3f &dir) {
    float u, v, t;
    Eigen::Vector3f v0v1 = v1 - v0;
    Eigen::Vector3f v0v2 = v2 - v0;
    Eigen::Vector3f pvec = dir.cross(v0v2);
    float det = v0v1.dot(pvec);
    // if the determinant is negative the triangle is backfacing
    // if the determinant is close to 0, the ray misses the triangle
    if (std::abs(det) < 1e-6f) return 1e30;
    
    float invDet = 1 / det;
    
    Eigen::Vector3f tvec = orig - v0;
    u = tvec.dot(pvec) * invDet;
    if (u < 0 || u > 1) return 1e30;
    
    Eigen::Vector3f qvec = tvec.cross(v0v1);
    v = dir.dot(qvec) * invDet;
    if (v < 0 || u + v > 1) return 1e30;
    
    t = v0v2.dot(qvec) * invDet;
    return t;
}

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
    auto it = objdata.find(filename);
    if (it == objdata.end())
        return 0;
	return &it->second;
}

void OBJData::RemoveElement(const std::string& filename) {
    objdata.erase(filename);
}

Eigen::Vector3f OBJData::Intersect(Eigen::Vector4f d, Eigen::Vector4f t, std::string& filename) {
    Eigen::Vector3f pts(1e30, 1e30, 1e30);
    double dis = 1e30;
    for (auto& obj : objdata) {
        if (strcmp(obj.first.c_str(), "sens") == 0) {
            continue;
        }
        Eigen::Matrix4f obj_mat = obj.second.model.inverse();
        Eigen::Vector4f d4 = obj_mat * d;
        Eigen::Vector4f t4 = obj_mat * t;
        Eigen::Vector3f d3(d4[0], d4[1], d4[2]);
        Eigen::Vector3f t3(t4[0], t4[1], t4[2]);
        for (auto& m : obj.second.LoadedMeshes) {
            for (int i = 0; i < m.Indices.size(); i += 3) {
                auto& p1 = m.Vertices[m.Indices[i]].Position;
                auto& p2 = m.Vertices[m.Indices[i + 1]].Position;
                auto& p3 = m.Vertices[m.Indices[i + 2]].Position;
                Eigen::Vector3f pp1(p1.X, p1.Y, p1.Z);
                Eigen::Vector3f pp2(p2.X, p2.Y, p2.Z);
                Eigen::Vector3f pp3(p3.X, p3.Y, p3.Z);
                
                float dt = rayIntersectsTriangle(pp1, pp2, pp3, d3, t3);
                if (dt < dis) {
                    dis = dt;
                    pts = d3 + t3 * dt;
                    filename = obj.first;
                }
            }
        }
        if (pts[0] > 1e20) {
            {
                std::ofstream os("/Users/jingwei/Desktop/model1.obj");
                for (auto& v: obj.second.LoadedMeshes.begin()->Vertices) {
                    os << "v " << v.Position.X << " " << v.Position.Y << " " << v.Position.Z << "\n";
                }
                for (int i = 0; i < obj.second.LoadedMeshes.begin()->Vertices.size(); i += 3) {
                    os << "f " << i + 1 << " " << i + 2 << " " << i + 3 << "\n";
                }
                os.close();
            }
            {
                std::ofstream os("/Users/jingwei/Desktop/model2.obj");
                for (int i = 0; i < 100; ++i) {
                    os << "v " << d3[0] + t3[0] * i * 0.01 << " " << d3[1] + t3[1] * i * 0.01 << " " << d3[2] + t3[2] * i * 0.01 << "\n";
                }
            }
        }
    }
    return pts;
}

void OBJData::SaveModelToFile(const std::string& filename)
{
    std::ofstream os(filename);
    for (auto& obj : objdata) {
        if (strcmp(obj.first.c_str(), "sens") == 0)
            continue;
        os << "# " << obj.first << "\n";
        for (int i = 0; i < sens_data.cam2world.size(); ++i) {
            cv::Mat world2cam = sens_data.cam2world[i].inv();
            Eigen::Matrix4f model = obj.second.model;
            Eigen::Matrix4f world;
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    world(j, k) = world2cam.at<float>(j, k);
                }
            }
            world = world * model;
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    os << world(j, k) << " ";
                }
            }
            os << "\n";
        }
    }
    os.close();
}


SensData sens_data;
