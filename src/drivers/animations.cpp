#include "animations.hpp"


using namespace animations;

double animations::linear(double x)
{
    return x;
}


double Fade::InOut(double x)
{
    return sin(PI*x - HALF_PI) / 2 + .5;
}

double Fade::In(double x)
{
    return sin(HALF_PI*x - HALF_PI) + 1;
}

double Fade::Out(double x)
{
    return sin(HALF_PI*x);
}
