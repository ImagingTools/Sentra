// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once


// ImtSentra includes
#include <imtsentra/CScenarioGraphComp.h>
#include <imtsentra/CScenarioExecutorComp.h>
#include <imtsentra/CPixelDiffComparatorComp.h>
#include <imtsentra/CBaselineManagerComp.h>
#include <imtsentra/CScreenshotStorageComp.h>
#include <imtsentra/CAppMapBuilderComp.h>
#include <imtsentra/CExecutionEngineComp.h>


/**
	ImtSentraPck package
*/
namespace ImtSentraPck
{


typedef imtsentra::CScenarioGraphComp ScenarioGraph;
typedef imtsentra::CScenarioExecutorComp ScenarioExecutor;
typedef imtsentra::CPixelDiffComparatorComp PixelDiffComparator;
typedef imtsentra::CBaselineManagerComp BaselineManager;
typedef imtsentra::CScreenshotStorageComp ScreenshotStorage;
typedef imtsentra::CAppMapBuilderComp AppMapBuilder;
typedef imtsentra::CExecutionEngineComp ExecutionEngine;


} // namespace ImtSentraPck
