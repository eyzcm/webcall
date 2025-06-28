#include "Im_Queue.h"

Im_Queue::Im_Queue(void)
{
    m_imVisitorInfo = NULL;
}

Im_Queue::Im_Queue(const Im_Queue& other)
{
    this->GroupId(other.GroupId());
    this->QueueAction(other.QueueAction());
    this->QueueType(other.QueueType());
    this->From(other.From());
    if (other.GetVisitorInfo())
    {
        m_imVisitorInfo = new Im_VisitorInfo(*other.GetVisitorInfo());
    }
    else
    {
        m_imVisitorInfo = NULL;
    }
}

Im_Queue::~Im_Queue(void)
{
    if (m_imVisitorInfo)
    {
        delete m_imVisitorInfo;
        m_imVisitorInfo = NULL;
    }
}

QString Im_VisitorInfo::GetDisplayName() const
{
    QString ret = "";

    QString position = "";
    position         = GetValueByKey("visitor_location_city");
    if (position.isEmpty())
    {
        position = GetValueByKey("visitor_location_province");
    }
    else
    {
        position += GetValueByKey("Visitor_location_district");
    }

    if (position.isEmpty())
    {
        position = GetValueByKey("visitor_location_country");
    }
    QString userId = GetValueByKey("userId");
    QString format = "%1%2";
    ret            = format.arg(position).arg(userId.right(5));

    if (ret.length() > 7)
    {
        ret = ret.left(7);
    }
    return ret;
}

QString Im_VisitorInfo::GetVisitorLocation() const
{
    QString city     = GetValueByKey("visitor_location_city");
    QString district = GetValueByKey("Visitor_location_district");
    QString province = GetValueByKey("visitor_location_province");
    QString country  = GetValueByKey("visitor_location_country");
    QString ip       = GetValueByKey("visitor_ip");

    QString format = "%1%2%3%4【%5】";
    return format.arg(country).arg(province).arg(city).arg(district).arg(ip);
}
