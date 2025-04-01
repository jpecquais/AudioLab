template<class T>
void uninterleaved(T** input_buffer, T** output_buffer, int max_ch, int max_smpl)
{
    for (ch = 0; ch < max_ch; ch++)
    {
        for (smpl = 0; smpl < max_smpl; smpl++)
        {
            output_buffer[ch][smpl] = input_buffer[smpl][ch];
        }
    }
}