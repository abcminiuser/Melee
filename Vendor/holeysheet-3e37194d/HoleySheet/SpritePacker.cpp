#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <optional>

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
            if (asset.path().filename().extension() != ".png")
                continue;

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

        // We want to pack the larger images first, as we'll then have a higher chance of getting smaller images into the resulting free space.
        images.sort([](const auto& image1, const auto& image2) -> bool { return std::max(image1.size.x, image1.size.y) > std::max(image2.size.x, image2.size.y); });

        std::list<sf::Rect<unsigned int>> freeSpaces;

        for (auto& imageInfo : images)
        {
            if (std::max(imageInfo.size.x, imageInfo.size.y) > maxAssetDimension)
            {
                std::cout << "Not packing asset '" << imageInfo.name << "' due to large size.\n";
                continue;
            }

            if (imageInfo.name.rfind("-nopack") != imageInfo.name.npos)
            {
                std::cout << "Not packing asset '" << imageInfo.name << "' due to exclusion suffix.\n";
                continue;
            }

            // Sort the free spaces smallest first, so we always try to pick the smallest free space that will accommodate our new image.
            freeSpaces.remove_if([](const auto& space) { return !space.width || !space.height; });
            freeSpaces.sort([](const auto& space1, const auto& space2) -> bool { return std::min(space1.width, space1.height) < std::min(space2.width, space2.height); });

            const auto foundSpace = std::find_if(freeSpaces.begin(), freeSpaces.end(), [&](const auto& space) { return (space.width >= imageInfo.size.x) && (space.height >= imageInfo.size.y); });
            if (foundSpace == freeSpaces.end())
            {
                // No free space that can accommodate this sprite: we need to increase out output dimensions, so we should expand the sprite sheet
                // to accommodate it. We want to grow the smallest dimension first, to try to keep the final output size mostly square.

                if (outputSize.y < outputSize.x)
                {
                    imageInfo.packPosition = { 0, outputSize.y };

                    // If the new image is wider than the current total size, we need to add a space rect above it. If it's smaller, the new space
                    // rect goes to the right of it.
                    if (imageInfo.size.x > outputSize.x)
                        freeSpaces.push_back({ outputSize.x, 0, imageInfo.size.x - outputSize.x, outputSize.y });
                    else if ((imageInfo.size.x < outputSize.x))
                        freeSpaces.push_back({ imageInfo.size.x, outputSize.y, outputSize.x - imageInfo.size.x, imageInfo.size.y });
                }
                else
                {
                    imageInfo.packPosition = { outputSize.x, 0 };

                    // If the new image is taller than the current total size, we need to add a space rect to the left of it. If it's smaller, the new space
                    // rect goes below it.
                    if (imageInfo.size.y > outputSize.y)
                        freeSpaces.push_back({ 0, outputSize.y, outputSize.x, imageInfo.size.y - outputSize.y });
                    else if ((imageInfo.size.y < outputSize.y))
                        freeSpaces.push_back({ outputSize.x, imageInfo.size.y, imageInfo.size.x, outputSize.y - imageInfo.size.y });
                }
            }
            else
            {
                // Found a space: pack at this position, then split the remaining space into new rectangles.

                imageInfo.packPosition = sf::Vector2u(foundSpace->left, foundSpace->top);

                const auto insertionSpace = *foundSpace;
                freeSpaces.erase(foundSpace);

                // We need to split the remaining free space (if any) after inserting this image. We want to split the remaining space
                // to give the largest free area rects post-split.

                const size_t freeAreaRight = (insertionSpace.width <= imageInfo.size.x) ? 0 : (insertionSpace.width - imageInfo.size.x) * insertionSpace.height;
                const size_t freeAreaBelow = (insertionSpace.height <= imageInfo.size.y) ? 0 : insertionSpace.width * (insertionSpace.height - imageInfo.size.y);

                if (freeAreaRight > freeAreaBelow)
                {
                    freeSpaces.push_back({ insertionSpace.left + imageInfo.size.x, insertionSpace.top, insertionSpace.width - imageInfo.size.x, insertionSpace.height });
                    freeSpaces.push_back({ insertionSpace.left, insertionSpace.top + imageInfo.size.y, imageInfo.size.x, insertionSpace.height - imageInfo.size.y });
                }
                else
                {
                    freeSpaces.push_back({ insertionSpace.left, insertionSpace.top + imageInfo.size.y, insertionSpace.width, insertionSpace.height - imageInfo.size.y });
                    freeSpaces.push_back({ insertionSpace.left + imageInfo.size.x, insertionSpace.top, insertionSpace.width - imageInfo.size.x, imageInfo.size.y });
                }
            }

            // Bump output sheet size if this asset's pack position exceeds the current bounds.
            outputSize.x = std::max(outputSize.x, imageInfo.packPosition->x + imageInfo.size.x);
            outputSize.y = std::max(outputSize.y, imageInfo.packPosition->y + imageInfo.size.y);

            std::cout << "Packed asset '" << imageInfo.name << "' at (" << imageInfo.packPosition->x << "," << imageInfo.packPosition->y << ").\n";
        }

        std::cout << "Final packed sheet size: " << outputSize.x << " x " << outputSize.y << ".\n";

        return outputSize;
    }

    void WritePackedSprites(const std::list<ImageInfo>& images, const std::filesystem::path& outputFolder, const std::string& spriteSheetName, const sf::Vector2u& outputImageSize)
    {
        const auto outputImagePath = outputFolder / (spriteSheetName + ".png");
        const auto outputMetadataPath = outputFolder / (spriteSheetName + ".dat");

        std::ofstream packedImageMetadata(outputMetadataPath);

        sf::Image packedImage;
        packedImage.create(outputImageSize.x, outputImageSize.y, sf::Color::Transparent);

        for (const auto& imageInfo : images)
        {
            if (!imageInfo.packPosition.has_value())
                continue;

            packedImage.copy(imageInfo.image, imageInfo.packPosition->x, imageInfo.packPosition->y, {}, true);

            packedImageMetadata << imageInfo.name << ",";
            packedImageMetadata << imageInfo.packPosition->x << ",";
            packedImageMetadata << imageInfo.packPosition->y << ",";
            packedImageMetadata << imageInfo.size.x << ",";
            packedImageMetadata << imageInfo.size.y << "\n";
        }

        if (!packedImage.saveToFile(outputImagePath.string()))
        {
            std::cerr << "Failed to save asset '" << outputImagePath.filename().stem() << "'!\n";
            exit(1);
        }
        else
        {
            std::cout << "Saved asset '" << outputImagePath.filename().stem().string() << "'.\n";
        }
    }

    void WriteRejectedSprites(const std::list<ImageInfo>& images, const std::filesystem::path& outputFolder)
    {
        for (const auto& imageInfo : images)
        {
            if (imageInfo.packPosition.has_value())
                continue;

            auto outputName = imageInfo.name;
            outputName = outputName.substr(0, outputName.find("-nopack"));

            const auto outputPath = outputFolder / (outputName + ".png");

            if (!imageInfo.image.saveToFile(outputPath.string()))
            {
                std::cerr << "Failed to save asset '" << outputName << "!\n";
                exit(1);
            }
            else
            {
                std::cout << "Saved asset '" << outputName << "'.\n";
            }
        }
    }
}

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cout << "Holey Sheet: Sprite sheet packer, by Dean Camera.\n";
        std::cout << "\tUsage: " << argv[0] << " {Input Images Path} {Output Images Path} {Sprite Sheet Name}\n";
        exit(1);
    }

    const auto inputFolder = std::filesystem::path(argv[1]);
    const auto outputFolder = std::filesystem::path(argv[2]);
    const auto spriteSheetName = std::string(argv[3]);

    std::filesystem::create_directories(outputFolder);

    // Load all source images from the input directory, and compute their basic info.
    auto images = GatherImages(inputFolder);

    // Pack the images into a sprite sheet, returning the final sheet size. Some sprites
    // may be rejected if either of their size dimensions exceeds the limit.
    auto outputImageSize = PackImages(images, 512);

    // Write the final sprite sheet out to the output folder.
    if (outputImageSize.x && outputImageSize.y)
        WritePackedSprites(images, outputFolder, spriteSheetName, outputImageSize);

    // Any images rejected for packing should just be saved as-is to the output folder.
    WriteRejectedSprites(images, outputFolder);

    return 0;
}
