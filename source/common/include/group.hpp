#ifndef GROUP_H
#define GROUP_H

#include <vector>
#include <memory>
#include "matrix4.hpp"
#include "model.hpp"

class Group {
public:
    Matrix4 transformations;
    std::vector<Model> models;
    std::vector<std::unique_ptr<Group>> children;
};

#endif