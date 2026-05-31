// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once


// Sentra includes
#include <sentra/CScenarioGraphComp.h>
#include <sentra/CScenarioExecutorComp.h>
#include <sentra/CPixelDiffComparatorComp.h>
#include <sentra/CBaselineManagerComp.h>
#include <sentra/CScreenshotStorageComp.h>
#include <sentra/CAppMapBuilderComp.h>
#include <sentra/CExecutionEngineComp.h>


/**
	SentraPck package
*/
namespace SentraPck
{


typedef sentra::CScenarioGraphComp ScenarioGraph;
typedef sentra::CScenarioExecutorComp ScenarioExecutor;
typedef sentra::CPixelDiffComparatorComp PixelDiffComparator;
typedef sentra::CBaselineManagerComp BaselineManager;
typedef sentra::CScreenshotStorageComp ScreenshotStorage;
typedef sentra::CAppMapBuilderComp AppMapBuilder;
typedef sentra::CExecutionEngineComp ExecutionEngine;


} // namespace SentraPck
