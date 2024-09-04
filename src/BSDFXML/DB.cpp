#include <fileParse/FileDataHandler.hxx>

#include "DB.hpp"
#include "Serializers.hpp"

namespace BSDFXML
{
    const std::string topNodeName = "WindowElement";

    WindowElement loadWindowElementFromFile(std::string_view fileName)
    {
        return Common::loadFromXMLFile<WindowElement>(fileName, topNodeName);
    }

    int saveToFile(const WindowElement & model, std::string_view fileName)
    {
        return Common::saveToXMLFile(model, fileName, topNodeName);
    }
}   // namespace BSDFXML