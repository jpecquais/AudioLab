template<class T>
void bela_uninterleaved_input_buffer(const T* const input_buffer, T** output_buffer, int max_ch, int max_smpl)
{
    for (int ch = 0; ch < max_ch; ch++)
    {
        for (int smpl = 0; smpl < max_smpl; smpl++)
        {
            output_buffer[ch][smpl] = input_buffer[smpl*2+ch];
        }
    }
}