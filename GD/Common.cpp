#include "Common.h"
#include <sys/types.h>
#include "zlib/zlib.h"
#include <charconv>

#define BUFFER_INC_FACTOR (2)

int64_t Common::inflateMemory(unsigned char* in, int64_t inLength, unsigned char** out)
{
    return inflateMemoryWithHint(in, inLength, out, (size_t)256 * (size_t)1024);
}

int Common::inflateMemoryWithHint(unsigned char* in,
    int64_t inLength,
    unsigned char** out,
    int64_t* outLength,
    int64_t outLengthHint)
{
    /* ret value */
    int err = Z_OK;

    int64_t bufferSize = outLengthHint;
    *out = (unsigned char*)malloc(bufferSize);

    z_stream d_stream; /* decompression stream */
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;

    d_stream.next_in = in;
    d_stream.avail_in = static_cast<unsigned int>(inLength);
    d_stream.next_out = *out;
    d_stream.avail_out = static_cast<unsigned int>(bufferSize);

    /* window size to hold 256k */
    if ((err = inflateInit2(&d_stream, 15 + 32)) != Z_OK)
        return err;

    for (;;)
    {
        err = inflate(&d_stream, Z_NO_FLUSH);

        if (err == Z_STREAM_END)
        {
            break;
        }

        switch (err)
        {
        case Z_NEED_DICT:
            err = Z_DATA_ERROR;
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            inflateEnd(&d_stream);
            return err;
        }

        // not enough memory ?
        if (err != Z_STREAM_END)
        {
            *out = (unsigned char*)realloc(*out, bufferSize * BUFFER_INC_FACTOR);

            /* not enough memory, ouch */
            if (!*out)
            {
                std::cout << "axmol: ZipUtils: realloc failed";
                inflateEnd(&d_stream);
                return Z_MEM_ERROR;
            }

            d_stream.next_out = *out + bufferSize;
            d_stream.avail_out = static_cast<unsigned int>(bufferSize);
            bufferSize *= BUFFER_INC_FACTOR;
        }
    }

    *outLength = bufferSize - d_stream.avail_out;
    err = inflateEnd(&d_stream);
    return err;
}

int64_t Common::inflateMemoryWithHint(unsigned char* in, int64_t inLength, unsigned char** out, int64_t outLengthHint)
{
    int64_t outLength = 0;
    int err = inflateMemoryWithHint(in, inLength, out, &outLength, outLengthHint);

    if (err != Z_OK || *out == nullptr)
    {
        if (err == Z_MEM_ERROR)
        {
            std::cout << "axmol: ZipUtils: Out of memory while decompressing map data!";
        }
        else if (err == Z_VERSION_ERROR)
        {
            std::cout << "axmol: ZipUtils: Incompatible zlib version!";
        }
        else if (err == Z_DATA_ERROR)
        {
            std::cout << "axmol: ZipUtils: Incorrect zlib compressed data!";
        }
        else
        {
            std::cout << "axmol: ZipUtils: Unknown error while decompressing map data! " << err;
        }

        if (*out)
        {
            free(*out);
            *out = nullptr;
        }
        outLength = 0;
    }

    return outLength;
}

std::vector<std::string> Common::splitByDelim(const std::string& str, char delim)
{
    std::vector<std::string> tokens;
    size_t pos = 0;
    size_t len = str.length();
    tokens.reserve(len / 2);

    while (pos < len)
    {
        size_t end = str.find_first_of(delim, pos);
        if (end == std::string::npos)
        {
            tokens.emplace_back(str.substr(pos));
            break;
        }
        tokens.emplace_back(str.substr(pos, end - pos));
        pos = end + 1;
    }

    return tokens;
}

std::vector<std::string_view> Common::splitByDelimStringView(std::string_view str, char delim)
{
    std::vector<std::string_view> tokens;
    size_t pos = 0;
    size_t len = str.length();

    while (pos < len)
    {
        size_t end = str.find(delim, pos);
        if (end == std::string_view::npos)
        {
            tokens.emplace_back(str.substr(pos));
            break;
        }
        tokens.emplace_back(str.substr(pos, end - pos));
        pos = end + 1;
    }

    return tokens;
}

int Common::stoi(std::string_view s)
{
    int ret = 0.0f;
    std::from_chars(s.data(), s.data() + s.size(), ret);
    return ret;
}

float Common::stof(std::string_view s)
{
    float ret = 0.0f;
    std::from_chars(s.data(), s.data() + s.size(), ret);
    return ret;
}

int Common::sectionForPos(float x)
{
    int section = x / 100;
    if (section < 0)
        section = 0;
    return section;
}