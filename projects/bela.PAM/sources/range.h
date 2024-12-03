#pragma once

template<class T>
class Range{
    public:
        Range(T t_min, T t_max):m_min(t_min),m_max(t_max){}
        inline T getMin(){return m_min;}
        inline T getMax(){return m_max;}
    private:
        T m_min, m_max;
};

template<class T>
T mapValue(T t_value, Range<T> t_inputRange, Range<T> t_outputRange){
    return ((t_value-t_inputRange.getMin())/(t_inputRange.getMax()-t_inputRange.getMin()))*(t_outputRange.getMax()-t_outputRange.getMin())+t_outputRange.getMin();
}

template<class T>
T mapFromMIDI(T t_value, Range<T> t_destRange){
    return mapValue(t_value, Range<T>(0,127), t_destRange);
}

template<class T>
T mapToMIDI(T t_value, Range<T> t_inputRange){
    return mapValue(t_value, t_inputRange, Range<T>(0,127));
}