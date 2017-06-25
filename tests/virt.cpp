#include "common/time.hpp"
#include "common/log.hpp"
#include <string>
#include <cstdio>

class animal
{
public:
    virtual std::string get_sound() = 0;
};

class cat : public animal
{
public:
    std::string get_sound() override
        { return "meow"; }
};

class dog : public animal
{
public:
    std::string get_sound() override
        { return "woof"; }
};

std::string g_meow()
    { return "meow"; }

std::string g_bark()
    { return "woof"; }

int main(int argc, char ** argv)
{
    if(argc != 2)
        fatal("usage: %s call-count", argv[0]);
    
    size_t count = atoll(argv[1]);
    
    double t = 0.0;
    time_point start;
    animal * a = new cat;
    for(size_t i = 0; i < count; ++i) {
        start = now();
        a->get_sound();
        t += elapsed(start, now());
    }
    fprintf(stderr, "Virtual: %f\n", t);

    t = 0.0;
    for(size_t i = 0; i < count; ++i) {
        start = now();
        g_meow();
        t += elapsed(start, now());
    }
    fprintf(stderr, "Non-virtual: %f\n", t);

    return 0;
}