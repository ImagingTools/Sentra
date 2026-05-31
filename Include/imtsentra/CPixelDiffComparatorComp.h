// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <icomp/CComponentBase.h>

// ImtSentra includes
#include <imtsentra/IImageComparator.h>

// Qt includes
#include <QtCore/QList>

namespace imtsentra
{

/**
 * \brief Pixel-by-pixel image comparison component
 *
 * Compares images pixel by pixel with configurable threshold
 * and anti-aliasing tolerance. Uses iimg::IBitmap for image
 * representation and iipr bitmap operations for diff calculation.
 *
 * \ingroup imtsentra
 */
class CPixelDiffComparatorComp:
        public icomp::CComponentBase,
        virtual public IImageComparator
{
public:
    typedef icomp::CComponentBase BaseClass;

    I_BEGIN_COMPONENT(CPixelDiffComparatorComp)
        I_REGISTER_INTERFACE(IImageComparator);
    I_END_COMPONENT

    // reimplemented (imtsentra::IImageComparator)
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
    bool IsInIgnoreRegion(int x, int y, const QList<IgnoreRegion>& regions) const;
};

} // namespace imtsentra
