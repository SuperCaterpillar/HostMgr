#ifndef PUBLICDEFINE_H
#define PUBLICDEFINE_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <list>

#include <boost/smart_ptr.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/logic/tribool.hpp>

using namespace std;
using namespace boost;

#define HSM_CONFIG_FILE "../config/config.xml"
#define HSM_MODULES_FILE "../config/HsmModules.xml"

#define DECL_SHARED_PTR(className) \
    typedef boost::shared_ptr<className> className##SharedPtr;

#define DECL_CLASS_PRIVATE_DATA(className) \
    struct className##Private; \
    typedef boost::shared_ptr<className##Private> className##PrivatePtr;\


#define DECL_VET(className) \
    typedef vector<className> className##Vet; \
    typedef vector<className>::iterator className##VetIter;

#define DECL_PTR_VET(className) \
    typedef boost::shared_ptr<className> className##SharedPtr; \
    typedef vector<className##SharedPtr> className##PtrVet; \
    typedef vector<className##SharedPtr>::iterator className##PtrVetIter;


#define DECL_LIST(className) \
    typedef list<className> className##List; \
    typedef list<className>::iterator className##ListIter;

#define DECL_MAP(key,val) \
    typedef map<key,val> key##val##Map; \
    typedef map<key,val>::iterator key##val##MapIter;

#define DECL_PTR_MAP(key,val) \
    typedef boost::shared_ptr<val> val##SharedPtr;\
    typedef map<key,val##SharedPtr> key##val##PtrMap; \
    typedef map<key,val##SharedPtr>::iterator key##val##PtrMapIter;


DECL_VET(string);

typedef boost::function< bool (struct inotify_event *) > file_sys_monitor_call;


#endif // PUBLICDEFINE_H
