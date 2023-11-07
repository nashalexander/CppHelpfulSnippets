#include <string>
#include <memory>
#include <functional>
#include <iostream>
#include <dlfcn.h>

//
// Load a dynamic library using modern C++
// * Use std::function instead of C-style function pointers
// * Utilize RAII principles by encapsulating the dynamic library handle in a std::unique_ptr 
//

class DynamicLib
{
public:
    DynamicLib(std::string pathToLib);

    const std::function<std::string()> getName;
    const std::function<int(int, int)> add;
    const std::function<int(int, int)> sub;

private:
    std::unique_ptr<void, decltype(&dlclose)> handle;
};

DynamicLib::DynamicLib(std::string pathToLib) :
    handle(dlopen(pathToLib.c_str(), RTLD_LAZY), dlclose),
    getName(*reinterpret_cast<std::string(*)()>(dlsym(handle.get(), "getName"))),
    add(*reinterpret_cast<int(*)(int, int)>(dlsym(handle.get(), "add"))),
    sub(*reinterpret_cast<int(*)(int, int)>(dlsym(handle.get(), "sub")))
{}

// Example usage, functions are seamlessly usable just like any other class
int main()
{
    DynamicLib lib("path/to/dynamic/lib");

    std::cout << lib.getName();
    std::cout << lib.add(2, 3);
    std::cout << lib.sub(6, 2);

    return 0;
}