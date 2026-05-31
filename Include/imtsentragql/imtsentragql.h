// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

/**
\file imtsentragql.h
\brief Main header file for the imtsentragql module - Sentra GraphQL API Layer
*/

/**
\defgroup imtsentragql imtsentragql
\brief Sentra GraphQL API Layer Module

The \b imtsentragql module bridges the imtsentra domain model with the
GraphQL API. It provides service controller components that translate
between C++ domain objects and GraphQL representation types.

\section imtsentragql_overview Overview
- \b CScenarioServiceControllerComp — scenario CRUD, duplicate, import/export
- \b CExecutionServiceControllerComp — execution start/stop/retry and progress subscriptions

\section imtsentragql_related Related Modules
- \b imtsentra — domain model and execution engine
*/

#include <imtsentragql/CScenarioServiceControllerComp.h>
#include <imtsentragql/CExecutionServiceControllerComp.h>

namespace imtsentragql {}
