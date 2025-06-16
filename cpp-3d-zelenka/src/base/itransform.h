#pragma once

#include "other_vectors.h"

namespace ag {

inline bool limitAngle(float& value, const float minLimit, const float maxLimit) {
    float checkValue = value;
    if(value < minLimit) value = minLimit;
    if(value > maxLimit) value = maxLimit;
    return value == checkValue;
}

inline float modulusAngle(float Angle) {
    while (Angle > 360.0f) Angle -= 360.0f;
    while (Angle < 0.0f) Angle += 360.0f;
    return Angle;
}

class iTransform {
public:
    virtual ~iTransform() {}
    
    virtual void TranslateX(const float iDistance) = 0;
    virtual void TranslateY(const float iDistance) = 0;
    virtual void TranslateZ(const float iDistance) = 0;

    virtual void Pitch(const float iAngle) = 0;
    virtual void Yaw(const float iAngle) = 0;
    virtual void Roll(const float iAngle) = 0;

    virtual void SetScale(const float iScale) = 0;
};

class iReadWriteXML {
public:
    virtual ~iReadWriteXML() {};
    
    virtual bool Read(TiXmlElement* model) = 0;
    virtual void Write(TiXmlElement* models) const = 0;

    bool ReadString(TiXmlElement* model, std::string& out_str) {
        if (!model) return false;
        out_str = model->GetText();
        return true;
    }

    bool ReadVec3(TiXmlElement* model, ag::vec3& out_vec) {
        if (!model) return false;
        float x = 0, y = 0, z = 0;
        if (model->QueryFloatAttribute("x", &x) != TIXML_SUCCESS)
            return false;
        if (model->QueryFloatAttribute("y", &y) != TIXML_SUCCESS)
            return false;
        if (model->QueryFloatAttribute("z", &z) != TIXML_SUCCESS)
            return false;

        out_vec.set(x, y, z);
        return true;
    }

    bool ReadColor(TiXmlElement* model, ag::color& out_color) {
        if (!model) return false;
        float r = 0, g = 0, b = 0, a = 0;        
        if (model->QueryFloatAttribute("red",   &r) != TIXML_SUCCESS)
            return false;
        if (model->QueryFloatAttribute("green", &g) != TIXML_SUCCESS)
            return false;
        if (model->QueryFloatAttribute("blue",  &b) != TIXML_SUCCESS)
            return false;
        if (model->QueryFloatAttribute("alpha", &a) != TIXML_SUCCESS)
            return false;

        out_color.set(r, g, b, a);
        return true;
    }
};

} //end namespace ag
