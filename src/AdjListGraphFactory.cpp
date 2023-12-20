#include <AdjList.hpp>
#include <filesystem>

namespace fs = std::filesystem;

namespace
{
    std::filesystem::path getFileExtension(std::string_view fileName)
    {
        return fs::path(fileName).extension();
    }
} // namespace

namespace graph
{
template<>
class GraphFactory<AdjList>
{
public:
    std::optional<AdjList> createFromFile(std::string_view fileName)
    {
        auto extension = getFileExtension(fileName);

        if (not validateFileExtension(extension))
        {
            return std::nullopt;
        }

        return parseFile(fileName);
    }

private:
    bool validateFileExtension(fs::path extension)
    {
        return extension == ".graphml" or ".lst";
    }

    bool parseFile(std::string_view fileName, fs::path extension)
    {
        return std::nullopt;
    }
};
} // namespace graph