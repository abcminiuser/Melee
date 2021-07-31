#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>

namespace
{
    struct PackedImageInfo
    {
        std::string         name;
        sf::Rect<size_t>    position;
    };

    constexpr auto kMaxDimensionSize = 512;
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Sprite sheet packer.\n";
        std::cout << "\tUsage: " << argv[0] << " {Input Images Path} {Output Images Path}\n";
        exit(1);
    }

    const auto inputFolder = std::filesystem::path(argv[1]);
    const auto outputFolder = std::filesystem::path(argv[2]);

    std::filesystem::create_directories(outputFolder);

    std::list<std::pair<std::string, sf::Image>> sprites;

    size_t totalWidth = 0;
    size_t maxHeight = 0;

    for (const auto& asset : std::filesystem::directory_iterator(inputFolder))
    {
        std::cout << "Loading asset: " << asset << "\n";

        sf::Image image;
        if (!image.loadFromFile(asset.path().string()))
        {
            std::cerr << "Failed to load source image!\n";
            exit(1);
        }

        const auto imageSize = image.getSize();
        if (imageSize.x > kMaxDimensionSize || imageSize.y > kMaxDimensionSize)
        {
            std::cout << "Ignoring asset due to large size: " << asset << "\n";
            image.saveToFile((outputFolder / asset.path().filename()).string());
            continue;
        }

        totalWidth += imageSize.x;
        maxHeight = std::max<size_t>(maxHeight, imageSize.y);

        sprites.emplace_back(std::make_pair(asset.path().filename().stem().string(), image));
    }

    sf::Image packedImage;
    std::vector<PackedImageInfo> packedImageInfo;

    packedImage.create((unsigned int)totalWidth, (unsigned int)maxHeight, sf::Color::Transparent);

    size_t currentXPos = 0;
    for (const auto& [name, image] : sprites)
    {
        const auto imageSize = image.getSize();

        packedImage.copy(image, (unsigned int)currentXPos, 0, {}, true);

        PackedImageInfo packingInfo = {};
        packingInfo.name = name;
        packingInfo.position = { currentXPos, 0, imageSize.x, imageSize.y };

        packedImageInfo.emplace_back(packingInfo);

        currentXPos += imageSize.x;
    }

    packedImage.saveToFile((outputFolder / "SpriteSheet.png").string());

    std::ofstream infoFile((outputFolder / "SpriteSheet.dat"));
    for (const auto& packingInfo : packedImageInfo)
        infoFile << packingInfo.name << "\t" << packingInfo.position.left << "\t" << packingInfo.position.top << "\t" << packingInfo.position.width << "\t" << packingInfo.position.height << "\n";

    return 0;
}
