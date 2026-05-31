// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include "ImtSentraPck.h"


// ACF includes
#include <icomp/export.h>


namespace ImtSentraPck
{


I_EXPORT_PACKAGE(
			"ImtSentraPck",
			"Sentra core component package",
			IM_PROJECT("\"ImagingTools Sentra\"") IM_COMPANY("ImagingTools"));

I_EXPORT_COMPONENT(
			ScenarioGraph,
			"Scenario graph component (DAG, topological sort, cycle detection)",
			"Scenario Graph");

I_EXPORT_COMPONENT(
			ScenarioExecutor,
			"Scenario executor component",
			"Scenario Executor");

I_EXPORT_COMPONENT(
			PixelDiffComparator,
			"Pixel difference image comparator component",
			"Pixel Diff Comparator");

I_EXPORT_COMPONENT(
			BaselineManager,
			"Baseline management component",
			"Baseline Manager");

I_EXPORT_COMPONENT(
			ScreenshotStorage,
			"Screenshot storage service component",
			"Screenshot Storage");

I_EXPORT_COMPONENT(
			AppMapBuilder,
			"Application map builder component",
			"App Map Builder");

I_EXPORT_COMPONENT(
			ExecutionEngine,
			"Execution engine component (parallel runs, retry, timeouts)",
			"Execution Engine");


} // namespace ImtSentraPck
