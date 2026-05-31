// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

#include <imtsentra/IImageComparator.h>

namespace imtsentra
{

/**
 * \brief Pixel-by-pixel image comparison component
 *
 * Compares images pixel by pixel with configurable threshold
 * and anti-aliasing tolerance.
 */
class CPixelDiffComparatorComp : public IImageComparator {
public:
    CPixelDiffComparatorComp();
    ~CPixelDiffComparatorComp() override;

    ComparisonResult Compare(
        const std::string& baselinePath,
        const std::string& actualPath,
        const ComparisonConfig& config
    ) override;

    bool GenerateDiffImage(
        const std::string& baselinePath,
        const std::string& actualPath,
        const std::string& outputPath,
        const ComparisonConfig& config
    ) override;

    ComparisonAlgorithm GetAlgorithm() const override;
};

} // namespace imtsentra
