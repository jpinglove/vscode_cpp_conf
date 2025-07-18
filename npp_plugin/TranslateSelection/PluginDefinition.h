#pragma once

#include "menuCmdID.h"

void pluginInit(const NppData& nppData);
void pluginCleanUp();
void beNotified(SCNotification* pSCNotification);
LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam);

void translateSelection();