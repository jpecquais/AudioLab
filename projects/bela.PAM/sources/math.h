#include<cmath>

template<class T>
T db2linear(T value){
    return std::pow<T,T>(10,value/20);
}