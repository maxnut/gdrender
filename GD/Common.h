#pragma once

#include <iostream>
#include <vector>

namespace Common
{
    int inflateMemoryWithHint(unsigned char* in,
        int64_t inLength,
        unsigned char** out,
        int64_t* outLength,
        int64_t outLengthHint);

    int64_t inflateMemoryWithHint(unsigned char* in, int64_t inLength, unsigned char** out, int64_t outLengthHint);

    int64_t inflateMemory(unsigned char* in, int64_t inLength, unsigned char** out);

    std::vector<std::string> splitByDelim(const std::string& str, char delim);

    std::vector<std::string_view> splitByDelimStringView(std::string_view str, char delim);

    int stoi(std::string_view s);

    float stof(std::string_view s);

    int sectionForPos(float x);
};