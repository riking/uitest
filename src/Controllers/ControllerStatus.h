//
// Created by Kane York on 4/18/17.
//

#ifndef UITEST_CONTROLLERSTATUS_H
#define UITEST_CONTROLLERSTATUS_H

#include "Common/EventSource.h"

class ControllerManager : public EventSource<struct {}> {
  public:
	explicit ControllerManager();
};

#endif // UITEST_CONTROLLERSTATUS_H
