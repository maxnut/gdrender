#pragma once

#include "ActionInterval.h"
#include "Group.h"

class RotateAction : public ActionInterval
{
private:
    int groupID = -1;
    Group* targetGroup = nullptr, *centerGroup = nullptr;

    float rotateTotal = 0, rotateLast = 0;

    bool lockRotation;

    GameObject* rotateAround = nullptr;
public:
    static std::shared_ptr<RotateAction> create(float duration, int target, int center, float rotation, bool lock);
    bool init(float duration, int target, int center, float rotation, bool lock);

    virtual void update(float time) override;
    void stop() override;
};