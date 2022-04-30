#pragma once
#include <vector>
#include <fstream>
#include "Structs.h"
#include "Graphics/Texture/stb_image.h"
#include "stb_image_write.h"

std::vector<uint8_t> readFile(const char* path);

vec3f HSVtoRGB(float H, float S, float V);

std::string numToString(int n);
std::string numToString(float n);
std::string numToString(double n);

std::string numToStringPrecision(float n, int precision);
std::string numToStringPrecision(double n, int precision);
std::string timeAsString(int minutes, float seconds);
std::string timeAsString(float seconds);

float distance(vec2f v1, vec2f v2);
float distanceNRoot(vec2f v1, vec2f v2);

