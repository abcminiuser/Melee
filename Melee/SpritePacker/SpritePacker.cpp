#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <optional>
#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>

namespace
{
    struct ImageInfo
    {
        std::filesystem::path          path;
        std::string                    name;
        sf::Image                      image;
        sf::Vector2u                   size;

        std::optional<sf::Vector2u>    packPosition;
    };

    std::list<ImageInfo> GatherImages(const std::filesystem::path& path)
    {
        std::list<ImageInfo> images;

        for (const auto& asset : std::filesystem::directory_iterator(path))
        {
            ImageInfo imageInfo = {};
            imageInfo.path      = asset.path();
            imageInfo.name      = imageInfo.path.filename().stem().string();

            if (imageInfo.image.loadFromFile(imageInfo.path.string()))
            {
                std::cout << "Loaded asset '" << imageInfo.name << "'.\n";
            }
            else
            {
                std::cerr << "Failed to load asset '" << imageInfo.name << "!\n";
                exit(1);
            }

            imageInfo.size = imageInfo.image.getSize();

            images.push_back(std::move(imageInfo));
        }

        return images;
    }

    sf::Vector2u PackImages(std::list<ImageInfo>& images, unsigned int maxImageDimension)
    {
        sf::Vector2u outputSize = { 0, 0 };

        // Simple X-only packing for now.
        for (auto& imageInfo : images)
        {
            if (std::max(imageInfo.size.x, imageInfo.size.y) > maxImageDimension)
            {
                std::cout << "Not packing asset '" << imageInfo.name << "' due to large size.\n";
                continue;
            }

            imageInfo.packPosition = sf::Vector2u(outputSize.x, 0);
            outputSize.x += imageInfo.size.x;
            outputSize.y = std::max(outputSize.y, imageInfo.size.y);
        }

        return outputSize;
    }

    void WritePackedSprites(const std::list<ImageInfo>& images, const std::filesystem::path& outputFolder, const sf::Vector2u& outputImageSize)
    {
        const auto outputImagePath = outputFolder / "SpriteSheet.png";
        const auto outputMetadataPath = outputFolder / "SpriteSheet.dat";

        std::ofstream packedImageMetadata(outputMetadataPath);

        sf::Image packedImage;
        packedImage.create(outputImageSize.x, outputImageSize.y, sf::Color::Transparent);

        unsigned int currentXPos = 0;
        for (const auto& imageInfo : images)
        {
            if (!imageInfo.packPosition.has_value())
                continue;

            packedImage.copy(imageInfo.image, imageInfo.packPosition->x, imageInfo.packPosition->y, {}, true);

            packedImageMetadata << imageInfo.name << "\t";
            packedImageMetadata << imageInfo.packPosition->x << "\t";
            packedImageMetadata << imageInfo.packPosition->y << "\t";
            packedImageMetadata << imageInfo.size.x << "\t";
            packedImageMetadata << imageInfo.size.y << "\n";
        }

        if (!packedImage.saveToFile(outputImagePath.string()))
        {
            std::cerr << "Failed to save asset '" << outputImagePath.filename().stem() << "!\n";
            exit(1);
        }
        else
        {
            std::cout << "Saved asset '" << outputImagePath.filename().stem() << "'.\n";
        }
    }

    void WriteRejectedSprites(const std::list<ImageInfo>& images, const std::filesystem::path& outputFolder)
    {
        for (const auto& imageInfo : images)
        {
            if (imageInfo.packPosition.has_value())
                continue;

            const auto outputPath = outputFolder / (imageInfo.name + ".png");

            if (!imageInfo.image.saveToFile(outputPath.string()))
            {
                std::cerr << "Failed to save asset '" << imageInfo.name << "!\n";
                exit(1);
            }
            else
            {
                std::cout << "Saved asset '" << imageInfo.name << "'.\n";
            }
        }
    }
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

    // Load all source images from the input directory, and compute their basic info.
    auto images = GatherImages(inputFolder);

    // Pack the images into a sprite sheet, returning the final sheet size. Some sprites
    // may be rejected if either of their size dimensions exceeeds the limit.
    auto outputImageSize = PackImages(images, 512);

    // Write the final sprite sheet out to the output folder.
    if (outputImageSize.x && outputImageSize.y)
        WritePackedSprites(images, outputFolder, outputImageSize);

    // Any images rejected for packing should just be saved as-is to the output folder.
    WriteRejectedSprites(images, outputFolder);

    return 0;
}
