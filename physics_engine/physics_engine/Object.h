#pragma once
class Object
{
public:
    Object();
    virtual ~Object() = 0;

    virtual void print() = 0;
};

