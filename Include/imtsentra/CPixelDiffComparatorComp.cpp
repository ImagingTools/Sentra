// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <imtsentra/CPixelDiffComparatorComp.h>

#include <iimg/IBitmap.h>
#include <iipr/CBitmapOperations.h>

#include <cmath>

namespace imtsentra
{

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

    int bitsPerPixel = baseline.GetPixelBitsCount();
    if (bitsPerPixel % 8 != 0 || bitsPerPixel == 0) {
        // Non-byte-aligned or unknown pixel format — cannot compare
        result.diffPercentage = 100.0f;
        result.changedPixels = result.totalPixels;
        return result;
    }
    int bytesPerPixel = bitsPerPixel / 8;

    // Per-pixel color distance threshold derived from config
    // config.threshold is 0–1 representing maximum tolerable fraction of differing pixels,
    // but for per-pixel comparison we use a fixed color distance threshold.
    // Anti-aliasing tolerance raises the per-pixel threshold slightly.
    int pixelThreshold = config.antiAliasingTolerance ? 3 : 1;

    // Use iipr::CBitmapOperations to calculate the pixel difference bitmap
    // then iterate through the result to count changed pixels,
    // skipping any configured ignore regions.
    for (int y = 0; y < height; ++y) {
        const auto* baselineRow = baseline.GetLinePtr(y);
        const auto* actualRow = actual.GetLinePtr(y);

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

            if (diff > pixelThreshold) {
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
    const QList<IgnoreRegion>& regions
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
