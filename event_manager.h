#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <stdlib.h>
#include <string>
#include <functional>
#include <unordered_map>
#include <list>

template<typename T>
struct FunctionNode{
    unsigned long int callbackId;
    std::function<T> callback;
};

template <typename T>
using EventMap = std::unordered_map<std::string, std::list<FunctionNode<T>>>;

using cbId = std::size_t; // CallBackId

class EventManager 
{
    private:
        static std::unordered_map<std::string, std::list<FunctionNode<void()>>> callbacks_void;
        static std::unordered_map<std::string, std::list<FunctionNode<void(int,int)>>> callbacks_int_int;
        static std::unordered_map<std::string, std::list<FunctionNode<void(int)>>>callbacks_int;
        static std::unordered_map<std::string, std::list<FunctionNode<void(float,float)>>> callbacks_float_float;
        static std::unordered_map<std::string, std::list<FunctionNode<void(float)>>> callbacks_float;

    public:
        static void cleanup();

        static void Invoke(const std::string& event_name);
        static void RemoveListener(const std::string&  event_name, std::function<void()>, cbId callbackId);
        static cbId AddListener(const std::string&  event_name, std::function<void()> callback);

        static void Invoke(const std::string& event_name, int x, int y);
        static void RemoveListener(const std::string&  event_name, std::function<void(int,int)>, cbId callbackId);
        static cbId AddListener(const std::string&  event_name, std::function<void(int,int)> callback);

        static void Invoke(const std::string&  event_name, int x);
        static void RemoveListener(const std::string&  event_name, std::function<void(int)>, cbId callbackId);
        static cbId AddListener(const std::string&  event_name, std::function<void(int)> callback);
        
        static void Invoke(const std::string&  event_name, float x, float y);
        static void RemoveListener(const std::string&  event_name,  std::function<void(float, float)>, cbId callbackId); 
        static cbId AddListener(const std::string&  event_name, std::function<void(float,float)> callback); 

        static void Invoke(const std::string&  event_name, float x);
        static void RemoveListener(const std::string&  event_name,  std::function<void(float)>, cbId callbackId); 
        static cbId AddListener(const std::string&  event_name, std::function<void(float)> callback); 
    
};

#endif