#include "lfo.h"

void audiolab::Lfo::Init(float sr)
{
    _samplerate = sr;
    _samplePeriode = 1 / _samplerate;
    _throbble = 1;
    _w = _t = 0;
    _freq = 1;
    _phi = 0;
}

void audiolab::Lfo::Reset()
{
    _t = _phi/_freq;
}

void audiolab::Lfo::Clock()
{
    _t += _samplePeriode;
    if (_t > 1) _t -= 1;
    _w = 2*PI*_freq*pow(_t,_throbble);
}

float audiolab::Lfo::Sine()
{
    return sin(_w);
}

float audiolab::Lfo::Triangle()
{
    float tri = _t*4; // TODO : FINISH!!!!!!!
    if (tri>1)
    {
        tri=2-tri;
    }
    else if (tri>2)
    {
        tri=tri-2;
    }
    else if (tri>3)
    {
        tri = 4-tri;
    }
    return tri;
}

float audiolab::Lfo::Square()
{
    return sin(_w)>0?1:0;
}