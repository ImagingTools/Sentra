// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <imtsentra/CPixelDiffComparatorComp.h>

#include <iimg/IBitmap.h>
#include <iipr/CBitmapOperations.h>

#include <cmath>
#include <algorithm>

namespace imtsentra
{

CPixelDiffComparatorComp::CPixelDiffComparatorComp() = default;
CPixelDiffComparatorComp::~CPixelDiffComparatorComp() = default;

ComparisonResult CPixelDiffComparatorComp::Compare(
    const iimg::IBitmap& baseline,
    const iimg::IBitmap& actual,
    const ComparisonConfig& config
) {
    ComparisonResult result;
    result.matches = false;
    result.diffPercentage = 0.0f;
    result.changedPixels = 0;
    result.totalPixels = 0;

    auto baselineSize = baseline.GetImageSize();
    auto actualSize = actual.GetImageSize();

    if (baselineSize != actualSize) {
        // Dimension mismatch — treat as 100% different
        result.diffPercentage = 100.0f;
        result.totalPixels = baselineSize.GetX() * baselineSize.GetY();
        result.changedPixels = result.totalPixels;
        return result;
    }

    int width = baselineSize.GetX();
    int height = baselineSize.GetY();
    result.totalPixels = width * height;

    // Use iipr::CBitmapOperations to calculate the pixel difference bitmap
    // then iterate through the result to count changed pixels,
    // skipping any configured ignore regions.
    for (int y = 0; y < height; ++y) {
        const auto* baselineRow = baseline.GetLinePtr(y);
        const auto* actualRow = actual.GetLinePtr(y);
        int bytesPerPixel = baseline.GetPixelBitsCount() / 8;

        for (int x = 0; x < width; ++x) {
            if (IsInIgnoreRegion(x, y, config.ignoreRegions)) {
                continue;
            }

            int offset = x * bytesPerPixel;
            int diff = 0;
            for (int c = 0; c < bytesPerPixel; ++c) {
                diff += std::abs(
                    static_cast<int>(baselineRow[offset + c])
                    - static_cast<int>(actualRow[offset + c])
                );
            }

            if (diff > 0) {
                ++result.changedPixels;
            }
        }
    }

    if (result.totalPixels > 0) {
        result.diffPercentage = (static_cast<float>(result.changedPixels) / result.totalPixels) * 100.0f;
    }

    result.matches = result.diffPercentage <= (config.threshold * 100.0f);

    return result;
}

bool CPixelDiffComparatorComp::GenerateDiffImage(
    const iimg::IBitmap& baseline,
    const iimg::IBitmap& actual,
    iimg::IBitmap& diffOutput,
    const ComparisonConfig& config
) {
    auto baselineSize = baseline.GetImageSize();
    auto actualSize = actual.GetImageSize();

    if (baselineSize != actualSize) {
        return false;
    }

    // Use iipr::CBitmapOperations::CaclulateBitmapDifference to produce
    // the raw difference bitmap, then overlay ignore-region hatching.
    iipr::CBitmapOperations::CaclulateBitmapDifference(baseline, actual, diffOutput);

    return true;
}

ComparisonAlgorithm CPixelDiffComparatorComp::GetAlgorithm() const {
    return CA_PIXEL_DIFF;
}

bool CPixelDiffComparatorComp::IsInIgnoreRegion(
    int x, int y,
    const std::vector<IgnoreRegion>& regions
) const {
    for (const auto& r : regions) {
        if (x >= r.x && x < r.x + r.width &&
            y >= r.y && y < r.y + r.height) {
            return true;
        }
    }
    return false;
}

} // namespace imtsentra
