// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <iimg/IBitmap.h>
#include <istd/istd.h>
#include <istd/IPolymorphic.h>
#include <istd/TUniqueInterfacePtr.h>

// Qt includes
#include <QtCore/QString>
#include <QtCore/QList>

// Standard includes
#include <optional>

namespace sentra
{

/**
 * \brief Comparison algorithm type
 *
 * \ingroup sentra
 */
enum ComparisonAlgorithm
{
    CA_PIXEL_DIFF,
    CA_PERCEPTUAL_DIFF,
    CA_STRUCTURAL_SIMILARITY,  // SSIM
    CA_LAYOUT_SHIFT
};
I_DECLARE_ENUM(ComparisonAlgorithm,
            CA_PIXEL_DIFF,
            CA_PERCEPTUAL_DIFF,
            CA_STRUCTURAL_SIMILARITY,
            CA_LAYOUT_SHIFT);

/**
 * \brief Region to ignore during comparison
 */
struct IgnoreRegion {
    int x;
    int y;
    int width;
    int height;
    std::optional<QString> reason;
};

/**
 * \brief Configuration for image comparison
 */
struct ComparisonConfig {
    ComparisonAlgorithm algorithm = CA_PIXEL_DIFF;
    float threshold = 0.01f;  // 1% difference threshold
    QList<IgnoreRegion> ignoreRegions;
    bool antiAliasingTolerance = true;
};

/**
 * \brief Result of image comparison
 */
struct ComparisonResult {
    bool matches;
    float diffPercentage;
    QString diffImagePath;
    int changedPixels;
    int totalPixels;
    QList<IgnoreRegion> detectedChanges;
};

/**
 * \brief Interface for image comparison algorithms
 *
 * Uses iimg::IBitmap from the Acf framework for image representation
 * and supports multiple comparison strategies including pixel-by-pixel,
 * perceptual diff, SSIM, and layout shift detection.
 *
 * \ingroup sentra
 */
class IImageComparator: virtual public istd::IPolymorphic
{
public:
    /**
     * \brief Compare two images provided as bitmaps
     * \param baseline Reference to the baseline bitmap
     * \param actual Reference to the actual (test) bitmap
     * \param config Comparison configuration
     * \return Comparison result with diff details
     */
    virtual ComparisonResult Compare(
        const iimg::IBitmap& baseline,
        const iimg::IBitmap& actual,
        const ComparisonConfig& config
    ) = 0;

    /**
     * \brief Generate a visual diff bitmap highlighting differences
     * \param baseline Reference to the baseline bitmap
     * \param actual Reference to the actual bitmap
     * \param diffOutput Output bitmap to receive the diff visualization
     * \param config Comparison configuration
     * \return true if diff image was generated successfully
     */
    virtual bool GenerateDiffImage(
        const iimg::IBitmap& baseline,
        const iimg::IBitmap& actual,
        iimg::IBitmap& diffOutput,
        const ComparisonConfig& config
    ) = 0;

    /**
     * \brief Get the algorithm type this comparator implements
     */
    virtual ComparisonAlgorithm GetAlgorithm() const = 0;
};

typedef istd::TUniqueInterfacePtr<IImageComparator> IImageComparatorUniquePtr;

} // namespace sentra
