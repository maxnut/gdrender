#pragma once

#include "ActionInterval.h"
#include "Group.h"

class RotateAction : public ActionInterval
{
private:
    int groupID = -1;
    std::shared_ptr<Group> targetGroup, centerGroup;

    float rotateTotal = 0, rotateLast = 0;

    bool lockRotation;

    std::shared_ptr<GameObject> rotateAround;
public:
    static std::shared_ptr<RotateAction> create(float duration, int target, int center, float rotation, bool lock);
    bool init(float duration, int target, int center, float rotation, bool lock);

    virtual void update(float time) override;
    virtual void stop() override;
};