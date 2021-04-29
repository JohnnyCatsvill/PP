#include <windows.h>
#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <ctime>
#include "BitMapReader.h"
#include <ctime>
#include <iostream>
#include <vector>

const int RULE[3][3] = { 0, 0, 0,
                         0, 1, 0,
                         0, 0, 1 };

const int RULE_ROUNDUP = 0.5;
const int AMOUNT_OF_BLUR = 1;

uint16_t time_start;
const std::string FILE_OUT_NAME = "fout";
int THREAD_PRIORITY_0 = THREAD_PRIORITY_ABOVE_NORMAL;
int THREAD_PRIORITY_1 = THREAD_PRIORITY_NORMAL;
int THREAD_PRIORITY_2 = THREAD_PRIORITY_BELOW_NORMAL;

struct ThreadData
{
    bitmap& image_in;
    bitmap& image_out;
    std::ofstream& file_out;
    int line_start;
    int line_end;
    int thread_count;
};

DWORD WINAPI ThreadProc(LPVOID data)
{
    ThreadData* refined_data = static_cast<ThreadData*>(data);

    bitmap& in = refined_data->image_in;
    bitmap& out = refined_data->image_out;
    std::ofstream& fout = refined_data->file_out;
    int line_start = refined_data->line_start;
    int line_end = refined_data->line_end;
    int thread_count = refined_data->thread_count;

    int image_width = in.getWidth();
    int image_height = in.getHeight();

    for (int y = line_start; y < line_end; y++)
    {
        for (int x = 0; x < image_width; x++)
        {
            int avg_red = 0;
            int avg_green = 0;
            int avg_blue = 0;

            int avg_sum = 0;

            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    int current_rule_pos = RULE[dy + 1][dx + 1];
                    int current_x_pos = x + dx;
                    int current_y_pos = y + dy;

                    if (!(current_rule_pos == 0 or current_x_pos >= image_width or
                        current_y_pos >= image_height or current_x_pos < 0 or current_y_pos < 0))
                    {
                        rgb32 current_color = *in.getPixel(current_x_pos, current_y_pos);
                        avg_red += current_color.r * current_rule_pos;
                        avg_green += current_color.g * current_rule_pos;
                        avg_blue += current_color.b * current_rule_pos;
                        avg_sum += current_rule_pos;
                    }
                }
            }

            avg_red /= avg_sum;
            avg_green /= avg_sum;
            avg_blue /= avg_sum;

            if (RULE_ROUNDUP != 0)
            {
                if (avg_blue % avg_sum > RULE_ROUNDUP * avg_sum)
                {
                    avg_blue++;
                }
                if (avg_red % avg_sum > RULE_ROUNDUP * avg_sum)
                {
                    avg_red++;
                }
                if (avg_green % avg_sum > RULE_ROUNDUP * avg_sum)
                {
                    avg_green++;
                }
            }

            *out.getPixel(x, y) = rgb32{ (uint8_t)avg_red, (uint8_t)avg_green, (uint8_t)avg_blue };
            
        }
        uint16_t time_end = clock();
        fout << std::to_string(time_end - time_start) + "\n";
    }

    ExitThread(0); // функция устанавливает код завершения потока в 0
}

int main(int argc, CHAR* argv[])
{
    unsigned int start_time = clock();

    int num_of_threads = atoi(argv[1]);
    int num_of_processors = atoi(argv[2]);
    char* image_in = argv[3];
    char* image_out = argv[4];

    DWORD processor_affinitiy_mask = 0x0;
    for (int i = 0; i < num_of_processors; i++)
    {
        processor_affinitiy_mask += pow(2, i);
    }

    bitmap image(image_in);
    bitmap processed_image(image);

    std::vector<std::ofstream> fout(num_of_threads);
    
    for (int iteration_of_blur = 0; iteration_of_blur < AMOUNT_OF_BLUR; iteration_of_blur++)
    {
        HANDLE* handles = new HANDLE[num_of_threads];
        SetThreadPriority(handles[0], THREAD_PRIORITY_0);
        SetThreadPriority(handles[1], THREAD_PRIORITY_1);
        SetThreadPriority(handles[2], THREAD_PRIORITY_2);
        for (int i = 0; i < num_of_threads; i++)
        {
            fout[i].open(FILE_OUT_NAME + std::to_string(i) + ".csv");
            int line_start = image.getHeight() * i / num_of_threads;
            int line_end = image.getHeight() * (i + 1) / num_of_threads;

            LPVOID data = new ThreadData{ image, processed_image, fout[i], line_start, line_end, i};

            handles[i] = CreateThread(NULL, 0, &ThreadProc, data, CREATE_SUSPENDED, NULL);
            SetProcessAffinityMask(GetCurrentProcess(), processor_affinitiy_mask);
        }

        start_time = clock();
        for (int i = 0; i < num_of_threads; i++)
        {
            ResumeThread(handles[i]);
        }

        // ожидание окончания работы двух потоков
        WaitForMultipleObjects(num_of_threads, handles, true, INFINITE);

        for (int i = 0; i < num_of_threads; i++)
        {
            fout[i].close();
        }

        image = processed_image;
    }

    processed_image.save(image_out);

    unsigned int end_time = clock();
    std::cout << "Work time = " << end_time - start_time << "\n";

    return 0;
}