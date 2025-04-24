#pragma once
#include "data/MUIManRobotRuleData.h"
#include <QObject>

class YL_ManRobotRuleLeafNodeItemData : public MUIManRobotRuleData
{
    Q_OBJECT
public:
    YL_ManRobotRuleLeafNodeItemData(void);
    YL_ManRobotRuleLeafNodeItemData(const YL_ManRobotRuleLeafNodeItemData& o);
    ~YL_ManRobotRuleLeafNodeItemData(void);

public:
    void InitData(MUIManRobotRuleData& obj);
};
