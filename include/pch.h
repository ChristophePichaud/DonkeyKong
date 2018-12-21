// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H


// TODO: add headers that you want to pre-compile here
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>

#define SFML_STATIC

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/OpenGL.hpp>

#ifdef SFML_STATIC
#pragma comment(lib, "sfml-audio-s-d.lib")
#pragma comment(lib, "sfml-graphics-s-d.lib")
#pragma comment(lib, "sfml-network-s-d.lib")
#pragma comment(lib, "sfml-system-s-d.lib")
#pragma comment(lib, "sfml-window-s-d.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "openal32.lib")
#pragma comment(lib, "flac.lib")
#pragma comment(lib, "ogg.lib")
#pragma comment(lib, "vorbis.lib")
#pragma comment(lib, "vorbisenc.lib")
#pragma comment(lib, "vorbisfile.lib")
#else
#pragma comment(lib, "sfml-audio-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-network-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#endif

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "opengl32.lib")

#endif //PCH_H
