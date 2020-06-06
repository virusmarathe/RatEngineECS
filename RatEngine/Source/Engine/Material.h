#pragma once
#include "Resource.h"

class Material : public Resource
{
public:
    Material(const wchar_t* fullPath);
    ~Material();
};

