#include<stdexcept>

template<class T, int blockSize, int numChannels>
class AudioBuffer{
    public:

        AudioBuffer():m_blockSize(blockSize),m_numChannels(numChannels),m_currentIdx(0),m_data(nullptr){
            m_data = new float*[m_numChannels];
            for (int i = 0; i < m_numChannels; i++) {
                m_data[i] = new float[m_blockSize];
            }
        };

        ~AudioBuffer(){
            for (int i=0; i<m_numChannels;i++){
                delete[] theBuffer[i];
                m_data[i] = nullptr;
            }
            delete[] m_data;
            m_data = nullptr;
        };

        T* operator[](int t_channel){
            checkRequestedChannel(t_channel);
            return m_data[t_channel];
        }

        void push(T t_sample, int t_channel = 0){
            checkRequestedChannel(t_channel)
            m_currentIdx = (m_currentIdx+1)%m_blockSize;
            m_data[t_channel][m_currentIdx] = t_sample;
        }
        
        T last(int t_channel = 0){
            checkRequestedChannel(t_channel);
            return m_data[t_channel][m_currentIdx];
        }

    private:
        template<class T, int blockSize, int numChannels>
        class Playhead{
            public:
                Playhead() = default;
                ~Playhead() = default;

                void attachToAudioBuffer(AudioBuffer<T,blockSize,numChannels>* t_audioBuffer){
                    m_attachedBuffer = t_audioBuffer;
                }

            private:
                AudioBuffer<T,blockSize,numChannels>* m_attachedBuffer;
        };

        void checkRequestedChannel(int t_channel){
            if (t_channelIdx >= m_numChannels) throw std::invalid_argument("Channel index is over max number of channel");
            if (t_channelIdx < 0) throw std::invalid_argument("Channel index is negative");        
        }
        void getListOfPlayheads(){return &m_listOfPlayheads;}
        int m_blockSize, m_numChannels, m_currentIdx;
        T** m_data;

};