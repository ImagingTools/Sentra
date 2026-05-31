// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

/**
\file sentragql.h
\brief Main header file for the sentragql module - Sentra GraphQL API Layer
*/

/**
\defgroup sentragql sentragql
\brief Sentra GraphQL API Layer Module

The \b sentragql module bridges the sentra domain model with the
GraphQL API. It provides service controller components that translate
between C++ domain objects and GraphQL representation types.

\section sentragql_overview Overview
- \b CScenarioServiceControllerComp — scenario CRUD, duplicate, import/export
- \b CExecutionServiceControllerComp — execution start/stop/retry and progress subscriptions

\section sentragql_related Related Modules
- \b sentra — domain model and execution engine
*/

#include <sentragql/CScenarioServiceControllerComp.h>
#include <sentragql/CExecutionServiceControllerComp.h>

namespace sentragql {}
