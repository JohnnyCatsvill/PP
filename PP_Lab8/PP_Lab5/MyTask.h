#pragma once
#include <iostream>
#include <string>
#include "ITask.h"
#include "BitMapReader.h"
#include <filesystem>

const int RULE[3][3] = { 0, 0, 5,
                         0, 1, 0,
                         0, 0, 5 };

const int RULE_ROUNDUP = 0.5;
const int AMOUNT_OF_BLUR = 10;

class MyTask: public ITask
{
private:
    bitmap* in;
    bitmap* out;

    int image_width;
    int image_height;

    int line_start;
    int line_end;

    std::string _outPath;
	
public:
	MyTask(const char* in_path, const char* out_path, int curPart, int allPart, bitmap* out):
        _outPath(out_path), out(out)
	{
        in = new bitmap(in_path);

        //using directory_iterator = std::filesystem::directory_iterator;

        //auto it = std::filesystem::path(out_path);
        //if (!it.empty())
        //{
        //    out = in;
        //}
        //else
        //{
        //    out = new bitmap(out_path);
        //}
        

        image_width = in->getWidth();
        image_height = in->getHeight();

        line_start = (curPart - 1) * image_height / allPart;
        line_end = curPart * image_height / allPart;

	}

    void Execute() override
    {
        for (int blurAmount = 0; blurAmount < AMOUNT_OF_BLUR; blurAmount++)
        {
            
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
                                rgb32 current_color = *in->getPixel(current_x_pos, current_y_pos);
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

                    *out->getPixel(x, y) = rgb32{ (uint8_t)avg_red, (uint8_t)avg_green, (uint8_t)avg_blue };

                }
            }
            in = out;
        }
        out->save(_outPath.c_str());
        std::cout  << "in work - " << _outPath.c_str() << "\n";
    }
};

