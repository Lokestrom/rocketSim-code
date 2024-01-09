#pragma once

#include "../App.hpp"

void openMainWindow(WindowInfo& window);
void openStartWindow(WindowInfo& window);
void openNewSimulationWindow(WindowInfo& window);
void openLoadInConfigWindow(WindowInfo& window);
void openLoadInRunWindow(WindowInfo& window);
void openOptionsWindow(WindowInfo& window);
void openTelemetryWindow(WindowInfo& window);
void openInstructionsWindow(WindowInfo& window);
void openMapViewWindow(WindowInfo& window);
void openFreeCamWindow(WindowInfo& window);
void openAlarmsWindow(WindowInfo& window);
void openTimeWindow(WindowInfo& window);

void swapToNewSimulationWindow(WindowInfo& window);
void swapToLoadInConfingWindow(WindowInfo& window);
void swapToLoadInRunWindow(WindowInfo& window);
void swapToMainMenuWindow(WindowInfo& window);
void swapToStartMenuWindow(WindowInfo& window);

WindowInfo createWindowInfo(WindowType type, void* info = nullptr);
void swapWindowType(WindowInfo& window, WindowType newType, void* info = nullptr);
void createWindow(WindowType type, void* info = nullptr);