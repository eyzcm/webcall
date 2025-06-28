#pragma once
#include "utils/DefineHelper.h"


class  LicenceSetting
{
public:
    LicenceSetting(void);
    ~LicenceSetting(void);

public:
    Property_QString(Expired, expired);
    Property_QString(LimitOnlineCount, limitOnlineCount);
};

//"licence": {
//		"expired": 1577808000000,
//		"limitOnlineCount": 200
//},
