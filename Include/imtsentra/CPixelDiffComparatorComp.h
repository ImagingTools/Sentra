// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

#include <imtsentra/IImageComparator.h>

namespace imtsentra
{

/**
 * \brief Pixel-by-pixel image comparison component
 *
 * Compares images pixel by pixel with configurable threshold
 * and anti-aliasing tolerance. Uses iimg::IBitmap for image
 * representation and iipr bitmap operations for diff calculation.
 */
class CPixelDiffComparatorComp : public IImageComparator {
public:
    CPixelDiffComparatorComp();
    ~CPixelDiffComparatorComp() override;

    ComparisonResult Compare(
        const iimg::IBitmap& baseline,
        const iimg::IBitmap& actual,
        const ComparisonConfig& config
    ) override;

    bool GenerateDiffImage(
        const iimg::IBitmap& baseline,
        const iimg::IBitmap& actual,
        iimg::IBitmap& diffOutput,
        const ComparisonConfig& config
    ) override;

    ComparisonAlgorithm GetAlgorithm() const override;

private:
    /**
     * \brief Check whether a pixel coordinate falls within any ignore region
     */
    bool IsInIgnoreRegion(int x, int y, const std::vector<IgnoreRegion>& regions) const;
};

} // namespace imtsentra
