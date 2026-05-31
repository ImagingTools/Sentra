# Sentra — AI Visual Testing Platform

**Sentra** (от лат. *sentire* — воспринимать, чувствовать + *centra* — центр) — платформа визуального AI-тестирования веб-приложений, построенная на базе ImtCore/ACF.

## Ключевые возможности

- 🎯 **Intent-based тестирование** — описывайте *что* нужно сделать, а не *как* (никаких селекторов, XPath, CSS)
- 📊 **Граф сценариев** — визуальный редактор тестовых сценариев (drag & drop)
- 📸 **Visual comparison** — автоматическое сравнение скриншотов с baseline
- 🤖 **AI Agent** — разрешение намерений, валидация, автономное исследование
- 🗺️ **Application Map** — автоматическое построение карты экранов приложения
- ⚡ **CI/CD интеграция** — CLI, Docker, API, Webhook

## Структура проекта

```
ImtSentra/
├── Include/
│   ├── imtsentra/          # Core interfaces (I*.h) + components (C*Comp.h/.cpp)
│   └── imtsentragql/       # GraphQL API controllers (C*Comp.h/.cpp)
├── Sdl/imtsentra/1.0/      # SDL schemas (Scenario, Execution, Baseline, etc.)
├── Qml/imtsentragui/       # QML UI components
│   ├── GraphEditor/        # Visual graph scenario editor
│   ├── Timeline/           # Execution timeline
│   ├── DiffViewer/         # Screenshot diff viewer
│   └── AppMap/             # Application map viewer
├── Tools/
│   ├── Browser/            # Playwright browser controller (Node.js)
│   ├── AI/                 # AI agent & provider abstraction
│   ├── Semantic/           # Semantic UI model builder
│   ├── Exploration/        # Autonomous exploration agent
│   └── CLI/                # Command-line interface
├── Impl/                   # ACF component packages
│   ├── ImtSentraPck/       # Core package (I_EXPORT_PACKAGE/I_EXPORT_COMPONENT)
│   └── ImtSentraGqlPck/    # GraphQL package
├── Partitura/              # ACF component wiring (SentraCoreVoce.arp/*.acc + *.accl)
├── Tests/                  # Unit & integration tests
├── Config/                 # Configuration files
├── Docs/                   # Documentation
├── CMakeLists.txt          # CMake build system
└── conanfile.py            # Conan package manager
```

## Быстрый старт

### Требования

- C++20 compiler (GCC 12+, Clang 15+, MSVC 2022+)
- Qt 6.5+
- CMake 3.20+
- Conan 2.0+
- Node.js 18+ (для Browser Controller)
- ImtCore/ACF (как зависимость)

### Сборка

```bash
# Установка зависимостей
conan install . --build=missing

# Сборка
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Запуск тестов
cd build && ctest
```

### CLI

```bash
# Запуск сценария
sentra run --project my-app --scenario login-flow --env staging

# Просмотр отчёта
sentra report --execution exec-001 --format html

# Автономное исследование
sentra explore --project my-app --url https://staging.myapp.com
```

### Docker

```bash
docker build -f Tests/Docker/Dockerfile -t sentra .
docker run sentra run --project my-app --scenario login-flow
```

## Технологический стек

| Компонент | Технология |
|-----------|-----------|
| Core Engine | C++ / Qt / ACF |
| Browser Automation | Playwright (Node.js) |
| AI Provider | OpenAI / Claude API |
| GraphQL API | imtgql стек |
| Schema | SDL + кодогенерация |
| UI | QML + imtcontrols |
| Storage | imtmdbx + filesystem |
| Communication | WebSocket (JSON-RPC) |
| CI/CD | Docker |

## Документация

- [Архитектура](Docs/Sentra_Architecture.md) — полное описание архитектуры и концепций

## Лицензия

Proprietary — ImagingTools