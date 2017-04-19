
#include "ControllerStatus.h"
#include "Common/EventSource.h"

ControllerManager::ControllerManager() {
    JoyConStatus missing;
    JoyConStatus present;
    present.case_color = {0x81, 0x81, 0x81};
    present.button_color = {0x0f, 0x0f, 0x0f};
    present.battery = 8;
    present.player_lights = 0x10;
    ControllerStatePair pair(missing, present);
    m_controllers.push_back(pair);
}

void ControllerManager::Update() {
    empty_struct empty;
    Notify(empty);
}
