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

    sf::Vector2u PackImages(std::list<ImageInfo>& images, unsigned int maxAssetDimension)
    {
        sf::Vector2u outputSize = { 0, 0 };

        images.sort([](const auto& image1, const auto& image2) -> bool { return std::max(image1.size.x, image1.size.y) > std::max(image2.size.x, image2.size.y); });

        std::list<sf::Rect<unsigned int>> freeSpaces;

        for (auto& imageInfo : images)
        {
            if (std::max(imageInfo.size.x, imageInfo.size.y) > maxAssetDimension)
            {
                std::cout << "Not packing asset '" << imageInfo.name << "' due to large size.\n";
                continue;
            }

            // Sort the free spaces smallest first, so we always try to pick the smallest free space that will accomodate our new image.
            freeSpaces.sort([](const auto& space1, const auto& space2) -> bool { return std::min(space1.width, space1.height) < std::min(space2.width, space2.height); });

            const auto foundSpace = std::find_if(freeSpaces.begin(), freeSpaces.end(), [&](const auto& space) { return (space.width >= imageInfo.size.x) && (space.height >= imageInfo.size.y); });
            if (foundSpace == freeSpaces.end())
            {
                // No free space that can accomodate this sprite: we need to increase out output dimensions.

                imageInfo.packPosition = { 0, outputSize.y };

                const auto largerNewDimension = std::max(imageInfo.size.x, imageInfo.size.y);

                // Add new space rect to the right of the image, increasing by the larger of the two new image dimensions.
                freeSpaces.push_back({ outputSize.x, 0, largerNewDimension, outputSize.y });

                // Check if we will have any remaining space rect to right of the new image, if so add a new space rect.
                if ((outputSize.x + largerNewDimension) > imageInfo.size.x)
                    freeSpaces.push_back({ imageInfo.size.x, outputSize.y, outputSize.x + largerNewDimension, imageInfo.size.y });
            }
            else
            {
                // Found a space: pack at this position, then split the remaining space into new rectangles.

                imageInfo.packPosition = sf::Vector2u(foundSpace->left, foundSpace->top);

                // Check if we will have any remaining space rect to right of the new image, if so add a new space rect.
                if (foundSpace->width > imageInfo.size.x)
                    freeSpaces.push_back({ foundSpace->left + imageInfo.size.x, foundSpace->top, foundSpace->width - imageInfo.size.x, foundSpace->height });

                // Check if we will have any remaining space rect below the new image, if so add a new space rect.
                if (foundSpace->height > imageInfo.size.y)
                    freeSpaces.push_back({ foundSpace->left, foundSpace->top + imageInfo.size.y, foundSpace->width, foundSpace->height - imageInfo.size.y });

                freeSpaces.erase(foundSpace);
            }

            // Bump output sheet size if this asset's pack position exceeds the current bounds.
            outputSize.x = std::max(outputSize.x, imageInfo.packPosition->x + imageInfo.size.x);
            outputSize.y = std::max(outputSize.y, imageInfo.packPosition->y + imageInfo.size.y);

            std::cout << "Packed asset '" << imageInfo.name << "' at (" << imageInfo.packPosition->x << "," << imageInfo.packPosition->y << ").\n";
        }

        std::cout << "Final packed sheet size: " << outputSize.x << " x " << outputSize.y << ".\n";

        return outputSize;
    }

    void WritePackedSprites(const std::list<ImageInfo>& images, const std::filesystem::path& outputFolder, const sf::Vector2u& outputImageSize)
    {
        const auto outputImagePath = outputFolder / "SpriteSheet.png";
        const auto outputMetadataPath = outputFolder / "SpriteSheet.dat";

        std::ofstream packedImageMetadata(outputMetadataPath);

        sf::Image packedImage;
        packedImage.create(outputImageSize.x, outputImageSize.y, sf::Color::Transparent);

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
