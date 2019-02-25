
#ifndef PDM_LOCALE
#define PDM_LOCALE

#include <libintl.h>
#include <locale.h>
#include <QString>


namespace lliurex
{
    namespace pdm
    {
        static QString T(const char* s)
        {
            return QString::fromUtf8(gettext(s));
        }
    }
}

#endif
