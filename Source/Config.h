#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

enum class State
{
    Sim,
    Creating,
    Done
};

struct Config
{
    unsigned    windowWidth,
                windowHeight,
                simWidth,
                simHeight,
                quadSize;
    State initialState;
};

#endif // CONFIG_H_INCLUDED
