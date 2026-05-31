#pragma once

#include "IImageComparator.h"

namespace imtsentra {

/**
 * @brief Pixel-by-pixel image comparison component
 *
 * Compares images pixel by pixel with configurable threshold
 * and anti-aliasing tolerance.
 */
class CPixelDiffComparatorComp : public IImageComparator {
public:
    CPixelDiffComparatorComp();
    ~CPixelDiffComparatorComp() override;

    ComparisonResult compare(
        const std::string& baselinePath,
        const std::string& actualPath,
        const ComparisonConfig& config
    ) override;

    bool generateDiffImage(
        const std::string& baselinePath,
        const std::string& actualPath,
        const std::string& outputPath,
        const ComparisonConfig& config
    ) override;

    ComparisonAlgorithm getAlgorithm() const override;
};

} // namespace imtsentra
