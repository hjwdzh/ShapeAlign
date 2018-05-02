#include "persistent.hpp"

std::map<std::string, objl::Loader> OBJData::objdata;

double rayIntersectsTriangle(Eigen::Vector3f& tri0, Eigen::Vector3f& tri1, Eigen::Vector3f& tri2,
                            Eigen::Vector3f& p, Eigen::Vector3f& d) {
    Eigen::Vector3f e1, e2, h, s, q;
    
    double a,f,u,v;
    e1 = tri1 - tri0;
    e2 = tri2 - tri0;
    
    h = d.cross(e2);
    a = e1.dot(h);
    
    if (a > -0.00001 && a < 0.00001)
        return 1e30;
    
    f = 1/a;
    s = p - tri0;
    u = f * s.dot(h);
    
    if (u < 0.0 || u > 1.0)
        return 1e30;
    
    q = s.cross(e1);
    v = f * d.dot(q);
    
    if (v < 0.0 || u + v > 1.0)
        return 1e30;

    // at this stage we can compute t to find out where
    // the intersection point is on the line
    double t = f * e2.dot(q);
    
    if (t > 0.00001) // ray intersection
        return t;
    
    else // this means that there is a line intersection
        // but not a ray intersection
        return 1e30;
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
