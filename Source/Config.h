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
    unsigned    simWidth,
                simHeight,
                maxGenerations;
    State initialState;
};

#endif // CONFIG_H_INCLUDED
