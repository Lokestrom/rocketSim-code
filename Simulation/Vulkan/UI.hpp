#pragma once

#include "String.hpp"
#include "App.hpp"


using namespace Database;

String InputBox(String text);
void loadInputBox(WindowInfo& window);
void errorMsgBox(const String& windowName, const String& errorMsg);
void loadErrorMsgBox(WindowInfo& window);

