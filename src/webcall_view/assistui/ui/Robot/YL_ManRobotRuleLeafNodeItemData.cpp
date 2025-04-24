#include "YL_ManRobotRuleLeafNodeItemData.h"
#include "service/DbUiWebcallService.h"
#include "utils/utilshelper.h"

YL_ManRobotRuleLeafNodeItemData::YL_ManRobotRuleLeafNodeItemData(void)
{
}

YL_ManRobotRuleLeafNodeItemData::YL_ManRobotRuleLeafNodeItemData(const YL_ManRobotRuleLeafNodeItemData& o)
{
    Name(o.Name());
    Id(o.Id());
    Rank(o.Rank());
    State(o.State());
}

YL_ManRobotRuleLeafNodeItemData::~YL_ManRobotRuleLeafNodeItemData(void)
{

}

void YL_ManRobotRuleLeafNodeItemData::InitData(MUIManRobotRuleData& obj)
{
    Name(obj.Name());
    Rank(obj.Rank());
    Id(obj.Id());
    State(obj.State());
}
