#include <wx/log.h>
#include "Scanner.hpp"

bool Scanner::Next(int32_t& var, int base)
{
    if (!HasMoreTokens()) return false;

    wxString token = GetNextToken();
    wxLogVerbose("Read int %s base %d", token, base);
    long ret;
    if (!token.ToLong(&ret, base)) return false;

    var = (int32_t) ret;
    return true;
}

bool Scanner::Next(uint32_t& var, int base)
{
    if (!HasMoreTokens()) return false;

    wxString token = GetNextToken();
    wxLogVerbose("Read unsigned int %s base %d", token, base);
    unsigned long ret;
    if (!token.ToULong(&ret, base)) return false;

    var = (uint32_t) ret;
    return true;
}

bool Scanner::Next(float& var)
{
    if (!HasMoreTokens()) return false;

    wxString token = GetNextToken();
    wxLogVerbose("Read float %s", token);
    double ret;
    if (!token.ToDouble(&ret)) return false;

    var = (float) ret;

    return true;
}

bool Scanner::Next(std::string& var)
{
    if (!HasMoreTokens()) return false;

    var = GetNextToken();
    wxLogVerbose("Read string %s", var);
    return true;
}
