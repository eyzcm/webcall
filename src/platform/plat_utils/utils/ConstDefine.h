#pragma once

// nickName  : 昵称 headImgUrl : 头像 telePhone :  联系方式  gender : 性别
#define THIRD_NODE_NAME "thirdProp"
#define DOUYI_NICKNAME "thirdProp_nickName"
#define DOUYI_HEADIMGURL "thirdProp_headImgUrl"
#define DOUYI_TELEPHONE "thirdProp_telePhone"
#define DOUYI_GENDER "thirdProp_gender"
#define DOUYI_CHILD_THIRDID "thirdProp_thirdId"
#define DOUYI_LOCHEADIMG "locHeadImg"
#define DOUYI_CHILD_NICKNAME "nickName"
#define DOUYI_CHILD_HEADIMGURL "headImgUrl"
#define DOUYI_CHILD_TELEPHONE "telePhone"
#define DOUYI_CHILD_GENDER "gender"

#define DEFAULT_PROMOTIONID "promotionId"     //自定义渠道ID
#define DEFAULT_PROMOTIONNAME "promotionName" //自定义渠道名称
#define DEFAULT_SKINNAME "skinName"           //网页样式名称

//增加名片 不自动进行保存
#define CARDSAVE_NO_UPLOADOCPC "card_savetype" // 0自动保存 1，不自动保存，默认自动保存
#define BCP_BIDDING_WORD "bcp_bidding_word"    //关键词
#define EXTEND_KEYWORD "extend_keyword"        //关键词

#define DATAADDEXTENDDATA(x, y)                                                         \
    x->AddExtendData(DOUYI_NICKNAME, y->GetValueByKey(DOUYI_NICKNAME));                 \
    x->AddExtendData(DOUYI_HEADIMGURL, y->GetValueByKey(DOUYI_HEADIMGURL));             \
    x->AddExtendData(DOUYI_TELEPHONE, y->GetValueByKey(DOUYI_TELEPHONE));               \
    x->AddExtendData(DOUYI_GENDER, y->GetValueByKey(DOUYI_GENDER));                     \
    x->AddExtendData(DOUYI_LOCHEADIMG, y->GetValueByKey(DOUYI_LOCHEADIMG));             \
    x->AddExtendData(DOUYI_CHILD_THIRDID, y->GetValueByKey(DOUYI_CHILD_THIRDID));       \
    x->AddExtendData(CARDSAVE_NO_UPLOADOCPC, y->GetValueByKey(CARDSAVE_NO_UPLOADOCPC)); \
    x->AddExtendData(BCP_BIDDING_WORD, y->GetValueByKey(BCP_BIDDING_WORD));             \
    x->AddExtendData(DEFAULT_PROMOTIONID, y->GetValueByKey(DEFAULT_PROMOTIONID));       \
    x->AddExtendData(DEFAULT_PROMOTIONNAME, y->GetValueByKey(DEFAULT_PROMOTIONNAME));   \
    x->AddExtendData(DEFAULT_SKINNAME, y->GetValueByKey(DEFAULT_SKINNAME));

#define UPDATADEXTENDDATA(x, y)                                                         \
    x->AddExtendData(DOUYI_NICKNAME, y->GetExtendData(DOUYI_NICKNAME));                 \
    x->AddExtendData(DOUYI_HEADIMGURL, y->GetExtendData(DOUYI_HEADIMGURL));             \
    x->AddExtendData(DOUYI_TELEPHONE, y->GetExtendData(DOUYI_TELEPHONE));               \
    x->AddExtendData(DOUYI_GENDER, y->GetExtendData(DOUYI_GENDER));                     \
    x->AddExtendData(DOUYI_LOCHEADIMG, y->GetExtendData(DOUYI_LOCHEADIMG));             \
    x->AddExtendData(DOUYI_CHILD_THIRDID, y->GetExtendData(DOUYI_CHILD_THIRDID));       \
    x->AddExtendData(CARDSAVE_NO_UPLOADOCPC, y->GetExtendData(CARDSAVE_NO_UPLOADOCPC)); \
    x->AddExtendData(BCP_BIDDING_WORD, y->GetExtendData(BCP_BIDDING_WORD));             \
    x->AddExtendData(DEFAULT_PROMOTIONID, y->GetExtendData(DEFAULT_PROMOTIONID));       \
    x->AddExtendData(DEFAULT_PROMOTIONNAME, y->GetExtendData(DEFAULT_PROMOTIONNAME));   \
    x->AddExtendData(DEFAULT_SKINNAME, y->GetExtendData(DEFAULT_SKINNAME));

#define UPDATADEXTENDDATA_1(x, y)                                                           \
    if (x->GetExtendData(DOUYI_NICKNAME).isEmpty())                                         \
        x->AddExtendData(DOUYI_NICKNAME, y->GetExtendData(DOUYI_NICKNAME));                 \
    if (x->GetExtendData(DOUYI_HEADIMGURL).isEmpty())                                       \
        x->AddExtendData(DOUYI_HEADIMGURL, y->GetExtendData(DOUYI_HEADIMGURL));             \
    if (x->GetExtendData(DOUYI_TELEPHONE).isEmpty())                                        \
        x->AddExtendData(DOUYI_TELEPHONE, y->GetExtendData(DOUYI_TELEPHONE));               \
    if (x->GetExtendData(DOUYI_GENDER).isEmpty())                                           \
        x->AddExtendData(DOUYI_GENDER, y->GetExtendData(DOUYI_GENDER));                     \
    if (x->GetExtendData(DOUYI_LOCHEADIMG).isEmpty())                                       \
        x->AddExtendData(DOUYI_LOCHEADIMG, y->GetExtendData(DOUYI_LOCHEADIMG));             \
    if (x->GetExtendData(DOUYI_CHILD_THIRDID).isEmpty())                                    \
        x->AddExtendData(DOUYI_CHILD_THIRDID, y->GetExtendData(DOUYI_CHILD_THIRDID));       \
    if (x->GetExtendData(CARDSAVE_NO_UPLOADOCPC).isEmpty())                                 \
        x->AddExtendData(CARDSAVE_NO_UPLOADOCPC, y->GetExtendData(CARDSAVE_NO_UPLOADOCPC)); \
    if (x->GetExtendData(BCP_BIDDING_WORD).isEmpty())                                       \
        x->AddExtendData(BCP_BIDDING_WORD, y->GetExtendData(BCP_BIDDING_WORD));             \
    if (x->GetExtendData(DEFAULT_PROMOTIONID).isEmpty())                                    \
        x->AddExtendData(DEFAULT_PROMOTIONID, y->GetExtendData(DEFAULT_PROMOTIONID));       \
    if (x->GetExtendData(DEFAULT_PROMOTIONNAME).isEmpty())                                  \
        x->AddExtendData(DEFAULT_PROMOTIONNAME, y->GetExtendData(DEFAULT_PROMOTIONNAME));   \
    if (x->GetExtendData(DEFAULT_SKINNAME).isEmpty())                                       \
        x->AddExtendData(DEFAULT_SKINNAME, y->GetExtendData(DEFAULT_SKINNAME));
