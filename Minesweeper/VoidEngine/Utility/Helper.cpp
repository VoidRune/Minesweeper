#include "Helper.h"
#include <iomanip>
#include <sstream>

std::vector<uint8_t> readFile(const char* path)
{
    std::ifstream instream(path, std::ios::in | std::ios::binary);
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
    return data;
}

vec3f HSVtoRGB(float H, float S, float V) {
    if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {
        return { 0, 0, 0 };
    }
    float s = S / 100.0f;
    float v = V / 100.0f;
    float C = s * v;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float r, g, b;
    if (H >= 0 && H < 60) {
        r = C, g = X, b = 0;
    }
    else if (H >= 60 && H < 120) {
        r = X, g = C, b = 0;
    }
    else if (H >= 120 && H < 180) {
        r = 0, g = C, b = X;
    }
    else if (H >= 180 && H < 240) {
        r = 0, g = X, b = C;
    }
    else if (H >= 240 && H < 300) {
        r = X, g = 0, b = C;
    }
    else {
        r = C, g = 0, b = X;
    }
    float R = (r + m);// * 255;
    float G = (g + m);// * 255;
    float B = (b + m);// * 255;
    return vec3f(R, G, B);
}

std::string numToString(int n)
{
    return std::to_string(n);
}
std::string numToString(float n)
{
    std::ostringstream streamObj;
    streamObj << n;
    return streamObj.str();
}
std::string numToString(double n)
{
    std::ostringstream streamObj;
    streamObj << n;
    return streamObj.str();
}

std::string numToStringPrecision(float n, int precision)
{
    std::stringstream streamObj;
    streamObj << std::fixed << std::setprecision(precision) << n;
    return streamObj.str();
}

std::string numToStringPrecision(double n, int precision)
{
    std::stringstream streamObj;
    streamObj << std::fixed << std::setprecision(precision) << n;
    return streamObj.str();
}

std::string timeAsString(int minutes, float seconds)
{
    return numToString(minutes) + ":" + numToStringPrecision(seconds - minutes * 60.0f, 2);
}

std::string timeAsString(float seconds)
{
    int minutes = seconds / 60;
    return numToString(minutes) + ":" + numToStringPrecision(seconds - minutes * 60.0f, 2);
}

float distance(vec2f v1, vec2f v2)
{
    return std::sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

float distanceNRoot(vec2f v1, vec2f v2)
{
    return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
}