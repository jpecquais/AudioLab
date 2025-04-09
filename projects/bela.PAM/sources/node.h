template <class T>
class IProcessNode
{
public:
    virtual void init(int new_block_size, int new_input_size, int new_output_size) = 0;
    virtual void bypass(int state) = 0;
    virtual void process(T** input_buffer, T** output_buffer) = 0;
    virtual void reset() = 0;
};

template<class T, class Processor>
class ProcessNode : public IProcessNode<T>
{
public:
    using InitCallback = std::function<void(int,int,int)>;
    using ResetCallback = std::function<void(void)>;
    using ProcessCallback = std::function<void(T**,T**)>;

    ProcessNode(Processor proc,
                InitCallback init_callback,
                ResetCallback reset_callback,
                ProcessCallback process_callback):m_processor(std::move(proc)),
                                                  m_init_callback(std::function<InitCallback>(init_callback)),
                                                  m_reset_callback(std::function<ResetCallback>(reset_callback)),
                                                  m_process_callback(std::function<ProcessCallback>(process_callback))
    {
    }
    ~ProcessNode()
    {
        deallocate_internal_buffer();
    } 

    void init(int new_block_size, int new_input_size, int new_output_size)
    {
        m_block_size = new_block_size;
        m_input_size = new_input_size; 
        m_output_size = new_output_size;
        m_max_size = std::max<int>(m_input_size,m_output_size);

        allocate_internal_buffer();
    }

    void attach_buffer(T** target_input_buffer, T** target_output_buffer)
    {
        m_input_path = &target_input_buffer;
        m_output_path = &target_output_buffer;
        m_actual_output_buffer = *m_output_path;
    }

    void process()
    {
        m_process_callback(*m_input_path,m_internal_buffer);
        copy_buffer(*m_actual_output_buffer,*m_output_path);
    }

    void bypass(int state)
    {
        if (state)
        {
            m_actual_output_buffer = m_input_path;
        }
        else
        {
            m_actual_output_buffer = &m_internal_buffer;
        }
    }

    void reset()
    {
        deallocate_internal_buffer();
        allocate_internal_buffer();
    }

private:

    Processor m_processor;
    InitCallback m_init_callback;
    ResetCallback m_reset_callback;
    ProcessCallback m_process_callback;

    int m_block_size, m_input_size, m_output_size, m_max_size;
    T** m_internal_buffer;
    T*** m_input_path, m_output_path, m_actual_output_buffer;

    void copy_buffer(T** input, T** output)
    {
        for (int ch = 0; ch < m_max_size; ch++)
        {
            for (int i = 0; i < m_block_size; i++)
            {
                output[ch][i] = input[ch][i];
            }
        }
    }

    void allocate_internal_buffer()
    {
        m_internal_buffer = new T*[max_size]
        for (int ch = 0; ch < m_max_size; ch++)
        {
            m_internal_buffer[ch] = new T[m_block_size];
            for (int i = 0; i < m_block_size; i++)
            {
                m_internal_buffer[ch][i] = 0;
            }
        }
    }

    void deallocate_internal_buffer()
    {
        for (int i = 0; i < m_max_size; i++)
        {
            delete[] m_internal_buffer[i];
        }
        delete[] m_internal_buffer;
    }

    void clear_internal_buffer()
    {
        for (int ch = 0; ch < m_max_size; ch++)
        {
            for (int i = 0; i < m_block_size; i++)
            {
                m_internal_buffer[ch][i] = 0;
            }
        }
    }

};