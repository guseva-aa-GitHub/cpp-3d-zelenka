#ifndef IMODEL_H
#define IMODEL_H

#include "math/vector3.h"

// class ShaderProgram;

class iModel {
    public:
    virtual ~iModel(){}
    virtual bool Init()=0;
    virtual void Draw(const unsigned int program_id) const = 0;

    virtual void SetCenterAndScope() = 0;
    virtual bool IsReady(void) const = 0;
};

#endif // IMODEL_H
