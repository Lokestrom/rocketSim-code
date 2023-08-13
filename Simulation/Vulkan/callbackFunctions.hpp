#pragma once

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
void openOptionsWindow();
void openTelemetryWindow();
void openInstructionsWindow();
void openMapViewWindow();
void openFreeCamWindow();
void openAlarmsWindow();
void openTimeWindow();

/*Free cam window functions*/
void changeView();

/*Options window function*/
void changeMaxFPS(int newFPS);
void maxInstructionRevertsStored(int newMaxReverts);

/*Pause window functions*/
void addPause();

/*Instructions window functions*/
void edit();
void submitChanges();
void RevertChanges();

/*Time window functions*/
void setSimulationTime(ld newTime);
void changeSimulationDeltaTime(ld newDT);
void changePlaybackSpeed(ld multyplyer);

/*Map view window functions*/
void changeMapView();
void ChangeFocus();




