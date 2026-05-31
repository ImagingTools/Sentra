// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <cassert>
#include <iostream>
#include <QtCore/QString>
#include <imtsentra/CPixelDiffComparatorComp.h>
#include <iimg/IBitmap.h>

using namespace imtsentra;

void testComparisonAlgorithm() {
    CPixelDiffComparatorComp comparator;
    assert(comparator.GetAlgorithm() == CA_PIXEL_DIFF);
    std::cout << "  [PASS] testComparisonAlgorithm" << std::endl;
}

void testIgnoreRegions() {
    ComparisonConfig config;
    config.threshold = 0.05f;
    config.ignoreRegions.push_back(IgnoreRegion{
        .x = 0, .y = 0, .width = 100, .height = 50, .reason = "Dynamic header"
    });

    assert(config.ignoreRegions.size() == 1);
    assert(config.ignoreRegions[0].width == 100);

    std::cout << "  [PASS] testIgnoreRegions" << std::endl;
}

void testComparisonConfig() {
    ComparisonConfig config;
    assert(config.algorithm == CA_PIXEL_DIFF);
    assert(config.threshold > 0.0f);
    assert(config.antiAliasingTolerance == true);
    assert(config.ignoreRegions.empty());

    std::cout << "  [PASS] testComparisonConfig" << std::endl;
}

int main() {
    std::cout << "=== ImageComparison Tests ===" << std::endl;

    testComparisonAlgorithm();
    testIgnoreRegions();
    testComparisonConfig();

    std::cout << "\nAll tests passed!" << std::endl;
    return 0;
}
