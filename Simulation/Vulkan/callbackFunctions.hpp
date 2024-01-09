#pragma once

#include "App.hpp"

/*Free cam window functions*/
void changeView(WindowInfo& window);

/*Options window function*/
void changeMaxFPS(int newFPS);
void maxInstructionRevertsStored(int newMaxReverts);

/*Pause window functions*/
void addPause(WindowInfo& window);

/*Instructions window functions*/


/*Time window functions*/
void setSimulationTime(WindowInfo& window);
void changeSimulationDeltaTime(WindowInfo& window);
void changePlaybackSpeed(WindowInfo& window);

/*Map view window functions*/
void changeMapView(WindowInfo& window);
void ChangeFocus(WindowInfo& window);




