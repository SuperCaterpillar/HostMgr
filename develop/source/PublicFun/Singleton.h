#ifndef SINGLETON_H
#define SINGLETON_H
#include <boost/noncopyable.hpp>

template <typename T>
class Singleton :public boost::noncopyable
{
public:
    static T* instance()
    {
        return m_instance;
    }


private:
    static T *m_instance;
};

template<typename T>
T* Singleton<T>::m_instance = new T();


#endif // SINGLETON_H

