#pragma once

#include <iostream>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

namespace Common
{
uint32_t parseShader(std::string path, int type);
int inflateMemoryWithHint(unsigned char* in, int64_t inLength, unsigned char** out, int64_t* outLength,
						  int64_t outLengthHint);

int64_t inflateMemoryWithHint(unsigned char* in, int64_t inLength, unsigned char** out, int64_t outLengthHint);

int64_t inflateMemory(unsigned char* in, int64_t inLength, unsigned char** out);

std::vector<std::string> splitByDelim(const std::string& str, char delim);

std::vector<std::string_view> splitByDelimStringView(std::string_view str, char delim);

int stoi(std::string_view s);

float stof(std::string_view s);

int sectionForPos(float x);

void xorString(std::string& str, int key);

glm::mat4 rotateMatrixAroundPoint(const glm::mat4& originalMatrix, float angle, const glm::vec2& center);

glm::vec2 transformPoint(glm::vec2 point, glm::mat4&mat);

}; // namespace Common