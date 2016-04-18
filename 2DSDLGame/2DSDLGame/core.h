#pragma once

using namespace std;

#include <SDL.h>
#include <SDL_EVENTS.h>
#include <iostream>
#include <vector>
#include <map>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cvt/wstring>
#include <codecvt>

#include "input.h"


#include "config.h"
#include "sprite_framework.h"
#include "color.h"


#define _StringFromAscIIChars(x) UberSnip::UTILS::STRING::StringFromAscIIChars(x)
#define _String(x) UberSnip::UTILS::STRING::StringFromAscIIChars(x)
#define _StringToAscIIChars(x) UberSnip::UTILS::STRING::StringToAscIIChars(x).c_str()
#define _string(x) UberSnip::UTILS::STRING::StringToAscIIChars(x).c_str()

using namespace System;
using namespace System::Runtime::InteropServices;

class STRING {
	//String^ fromAscII;
public:

	System::String^ StringFromAscIIChars(const char* chars)
	{
		size_t newsize = strlen(chars) + 1;
		wchar_t * wcstring = new wchar_t[newsize];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, chars, _TRUNCATE);
		String^ str = gcnew String(wcstring);
		delete[] wcstring;
		return str;
	}

	static String^ StringFromAscIIChars(std::string str_string)
	{
		const char* chars = str_string.c_str();
		size_t newsize = strlen(chars) + 1;
		wchar_t * wcstring = new wchar_t[newsize];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, chars, _TRUNCATE);
		String^ str = gcnew String(wcstring);
		delete[] wcstring;
		return str;
	}

	static std::string StringToAscIIChars(System::String^ s, string& os) {
		/*

		const char* chars =
			(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();

		Marshal::FreeHGlobal(IntPtr((void*)chars));

		os = chars;

		return chars;
		*/
	}

};

#include "character_framework.h"
#include "mob_framework.h"

void limit_framerate(Uint32 tick) {
	if ((1000 / fps) > SDL_GetTicks() - tick) {
		SDL_Delay(1000 / fps - (SDL_GetTicks() - tick));
	}
}

