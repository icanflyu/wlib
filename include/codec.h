#ifndef _CODEC_H
#define _CODEC_H



#include "wlib.h"
#include <string>

namespace wlib 
{
    namespace codec
    {
        char lower(char c);
		char upper(char c);
		wchar_t lowerw(wchar_t wc);
		wchar_t upperw(wchar_t wc);
        namespace url
        {

        }
           
    }
}

#endif