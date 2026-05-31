#pragma once

#include <string>
#include <vector>
#include <optional>

namespace imtsentra {

/**
 * @brief Comparison algorithm type
 */
enum class ComparisonAlgorithm {
    PixelDiff,
    PerceptualDiff,
    StructuralSimilarity,  // SSIM
    LayoutShift
};

/**
 * @brief Region to ignore during comparison
 */
struct IgnoreRegion {
    int x;
    int y;
    int width;
    int height;
    std::optional<std::string> reason;
};

/**
 * @brief Configuration for image comparison
 */
struct ComparisonConfig {
    ComparisonAlgorithm algorithm = ComparisonAlgorithm::PixelDiff;
    float threshold = 0.01f;  // 1% difference threshold
    std::vector<IgnoreRegion> ignoreRegions;
    bool antiAliasingTolerance = true;
};

/**
 * @brief Result of image comparison
 */
struct ComparisonResult {
    bool matches;
    float diffPercentage;
    std::string diffImagePath;
    int changedPixels;
    int totalPixels;
    std::vector<IgnoreRegion> detectedChanges;
};

/**
 * @brief Interface for image comparison algorithms
 *
 * Supports multiple comparison strategies including pixel-by-pixel,
 * perceptual diff, SSIM, and layout shift detection.
 */
class IImageComparator {
public:
    virtual ~IImageComparator() = default;

    /**
     * @brief Compare two images
     * @param baselinePath Path to the baseline image
     * @param actualPath Path to the actual (test) image
     * @param config Comparison configuration
     * @return Comparison result with diff details
     */
    virtual ComparisonResult compare(
        const std::string& baselinePath,
        const std::string& actualPath,
        const ComparisonConfig& config
    ) = 0;

    /**
     * @brief Generate a visual diff image highlighting differences
     * @param baselinePath Path to baseline image
     * @param actualPath Path to actual image
     * @param outputPath Path to save diff image
     * @param config Comparison configuration
     * @return true if diff image was generated successfully
     */
    virtual bool generateDiffImage(
        const std::string& baselinePath,
        const std::string& actualPath,
        const std::string& outputPath,
        const ComparisonConfig& config
    ) = 0;

    /**
     * @brief Get the algorithm type this comparator implements
     */
    virtual ComparisonAlgorithm getAlgorithm() const = 0;
};

} // namespace imtsentra
