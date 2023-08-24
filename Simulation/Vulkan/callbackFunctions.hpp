#pragma once

#include "App.hpp"

/*Load window functions*/
void loadMainWindow(WindowInfo& window);
void loadFreeCamWindow(WindowInfo& window);
void loadTelemetryWindow(WindowInfo& window);
void loadInstructionsWindow(WindowInfo& window);
void loadMapViewWindow(WindowInfo& window);
void loadOptionsWindow(WindowInfo& window);
void loadPauseWindow(WindowInfo& window);
void loadTimeWindow(WindowInfo& window);

/*Main window functions*/
void openOptionsWindow(WindowInfo& window);
void openTelemetryWindow(WindowInfo& window);
void openInstructionsWindow(WindowInfo& window);
void openMapViewWindow(WindowInfo& window);
void openFreeCamWindow(WindowInfo& window);
void openAlarmsWindow(WindowInfo& window);
void openTimeWindow(WindowInfo& window);

/*Free cam window functions*/
void changeView(WindowInfo& window);

/*Options window function*/
void changeMaxFPS(int newFPS);
void maxInstructionRevertsStored(int newMaxReverts);

/*Pause window functions*/
void addPause(WindowInfo& window);

/*Instructions window functions*/
void edit(WindowInfo& window);
void submitChanges(WindowInfo& window);
void RevertChanges(WindowInfo& window);

/*Time window functions*/
void setSimulationTime(ld newTime);
void changeSimulationDeltaTime(ld newDT);
void changePlaybackSpeed(ld multyplyer);

/*Map view window functions*/
void changeMapView(WindowInfo& window);
void ChangeFocus(WindowInfo& window);




