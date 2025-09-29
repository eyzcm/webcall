#pragma once


template <class T> class  UISingleton
{
private:
    static T* m_pInstance;
    //禁止拷贝构造和赋值运算符.The only way is getInstance()
    UISingleton(const UISingleton& src)
    {
    }
 /*   UISingleton& operator=(const UISingleton& src)
    {
    }
    */

    class Garbo //它的唯一工作就是在析构函数中析构Singleton的实例，所以private
    {
    public:
        ~Garbo()
        {
            if (UISingleton::m_pInstance)
            {
                delete UISingleton::m_pInstance;
                UISingleton::m_pInstance = nullptr;
            }
        }
    };

    static Garbo garbo; //定义一个静态成员变量，程序结束时，系统会自动调用它的析构函数，我们不需要访问这个变量，所以不需要初始化

protected:
    //使继承者无法 public 构造函数和析构函数
    UISingleton()
    {
    }
    virtual ~UISingleton()
    {
    }

public:
    static  T* GetInstance()
    {
        if (m_pInstance == nullptr) //判断是否第一次调用
            m_pInstance = new T();
        return m_pInstance;
    }
};

template <class T> T* UISingleton<T>::m_pInstance = nullptr; //必须初始化这个静态成员，初始化的过程中会分配内存，否则无法访问
