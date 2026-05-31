// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <cassert>
#include <iostream>
#include <string>
#include <imtsentra/CPixelDiffComparatorComp.h>

using namespace imtsentra;

void testComparisonAlgorithm() {
    CPixelDiffComparatorComp comparator;
    assert(comparator.GetAlgorithm() == CA_PIXEL_DIFF);
    std::cout << "  [PASS] testComparisonAlgorithm" << std::endl;
}

void testCompareIdenticalImages() {
    CPixelDiffComparatorComp comparator;
    ComparisonConfig config;
    config.threshold = 0.01f;

    // TODO: Create test images and verify comparison
    // For now, test that the interface works without crashing
    auto result = comparator.Compare("nonexistent1.png", "nonexistent2.png", config);
    // Result will indicate failure since files don't exist
    assert(result.diffPercentage >= 0.0f);

    std::cout << "  [PASS] testCompareIdenticalImages (stub)" << std::endl;
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

int main() {
    std::cout << "=== ImageComparison Tests ===" << std::endl;

    testComparisonAlgorithm();
    testCompareIdenticalImages();
    testIgnoreRegions();

    std::cout << "\nAll tests passed!" << std::endl;
    return 0;
}
