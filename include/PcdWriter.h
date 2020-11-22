#include <string>
#include "Map.h"

namespace ORB_SLAM2
{
class Map;

class PcdWriter
{
    public:
    static bool save(const std::string pcdFile, Map* map);

    private:
    static std::string pcdHeader(const uint16_t width, const uint16_t height = 1);
};
}