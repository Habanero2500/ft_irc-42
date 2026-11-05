#include "Headers.hpp"

std::string formatDebugBuffer(const std::string& buffer)
{
    std::string debugStr = "";
    
    for (size_t i = 0; i < buffer.length(); i++)
    {
        if (buffer[i] == '\r')
            debugStr += RED "\\r" RESET;
        else if (buffer[i] == '\n')
            debugStr += RED "\\n" RESET "\n"; // Ajoute le texte "\n" visuel + un vrai saut de ligne
        else if (buffer[i] == '\0')
            debugStr += RED "\\0" RESET;
        else if (buffer[i] < 32 || buffer[i] > 126)
            debugStr += RED "?" RESET; // Remplace tout autre caractère non imprimable par un ?
        else
            debugStr += buffer[i];
    }
    return debugStr;
}