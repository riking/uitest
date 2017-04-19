//
// Created by Kane York on 4/18/17.
//

#ifndef UITEST_CONTROLLERSTATUS_H
#define UITEST_CONTROLLERSTATUS_H

#include "Common/EventSource.h"
#include <array>

enum {
	ControllerTypeEmpty = 1,
	ControllerTypeLeft,
	ControllerTypeRight,
	ControllerTypePair,
	ControllerTypePro,
	ControllerTypeChargeGrip,
};

// TODO replace this with a class in JoyCons/
struct JoyConStatus {
	// 1-8 = discharging
	// TODO is there a charging bit in this
	uint8_t battery;
	uint8_t player_lights;
	std::array<uint8_t, 3> case_color;
	std::array<uint8_t, 3> button_color;

	bool IsMissing() { return case_color[0] == 0; }
};

class ControllerStateVisitor;

struct ControllerState {
	int m_type;
	ControllerState(int t) { m_type = t; };
	virtual void accept(ControllerStateVisitor &visitor) = 0;
	// virtual const char* ui_background() = 0;
};

struct ControllerStateEmpty;
struct ControllerStateLeft;
struct ControllerStateRight;
struct ControllerStatePair;
struct ControllerStateChargeGrip;
struct ControllerStatePro;

class ControllerStateVisitor {
  public:
	virtual void visit(ControllerStateEmpty &cs){};
	virtual void visit(ControllerStateLeft &cs){};
	virtual void visit(ControllerStateRight &cs){};
	virtual void visit(ControllerStatePair &cs){};
	virtual void visit(ControllerStateChargeGrip &cs){};
	virtual void visit(ControllerStatePro &cs){};
};

struct ControllerStateEmpty : ControllerState {
	ControllerStateEmpty() : ControllerState(ControllerTypeEmpty){};
	void accept(ControllerStateVisitor &visitor) { visitor.visit(*this); }
};
struct ControllerStateLeft : ControllerState {
	ControllerStateLeft(JoyConStatus jc_left)
	    : ControllerState(ControllerTypeLeft) {
		m_jc_left = jc_left;
	};
	void accept(ControllerStateVisitor &visitor) { visitor.visit(*this); }
	JoyConStatus m_jc_left;
};
struct ControllerStateRight : ControllerState {
	ControllerStateRight(JoyConStatus jc_right)
	    : ControllerState(ControllerTypeRight) {
		m_jc_right = jc_right;
	};
	void accept(ControllerStateVisitor &visitor) { visitor.visit(*this); }
	JoyConStatus m_jc_right;
};
struct ControllerStatePair : ControllerState {
	ControllerStatePair(JoyConStatus jc_left, JoyConStatus jc_right)
	    : ControllerState(ControllerTypePair) {
		m_jc_left = jc_left;
		m_jc_right = jc_right;
	};
	void accept(ControllerStateVisitor &visitor) { visitor.visit(*this); }
	JoyConStatus m_jc_left;
	JoyConStatus m_jc_right;
};
struct ControllerStateChargeGrip : ControllerState {
	ControllerStateChargeGrip(JoyConStatus jc_left, JoyConStatus jc_right)
	    : ControllerState(ControllerTypeChargeGrip) {
		m_jc_left = jc_left;
		m_jc_right = jc_right;
	};
	void accept(ControllerStateVisitor &visitor) { visitor.visit(*this); }
	JoyConStatus m_jc_left;
	JoyConStatus m_jc_right;
};
struct ControllerStatePro : ControllerState {
	ControllerStatePro()
	    : ControllerState(ControllerTypePro){

	      };
	void accept(ControllerStateVisitor &visitor) { visitor.visit(*this); }
	// TODO
};

// states:
// Slot Empty
// Pro Controller(...)
// Horizontal JoyCon(...)
// Vertical JoyCons(...)

class empty_struct {};

class ControllerManager : public BaseEventSource<empty_struct> {
  public:
	ControllerManager();

	void Update();

	std::vector<ControllerState *> m_controllers;
};

#endif // UITEST_CONTROLLERSTATUS_H
