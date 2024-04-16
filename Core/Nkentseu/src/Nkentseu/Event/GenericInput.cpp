//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 5:13:20 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "GenericInput.h"

#include <cstring>

namespace nkentseu {

	std::string GenericInput::GetButton(Button button) {
#define BUTTON_NAME(button_id) if (button == GenericInput::button_id##_ev) return #button_id

		BUTTON_NAME(Button0);
		BUTTON_NAME(Button1);
		BUTTON_NAME(Button2);
		BUTTON_NAME(Button3);
		BUTTON_NAME(Button4);
		BUTTON_NAME(Button5);
		BUTTON_NAME(Button6);
		BUTTON_NAME(Button7);
		BUTTON_NAME(Button8);
		BUTTON_NAME(Button9);
		BUTTON_NAME(Button10);
		BUTTON_NAME(Button11);
		BUTTON_NAME(Button12);
		BUTTON_NAME(Button13);
		BUTTON_NAME(Button14);
		BUTTON_NAME(Button15);
		BUTTON_NAME(Button16);
		BUTTON_NAME(Button17);
		BUTTON_NAME(Button18);
		BUTTON_NAME(Button19);
		BUTTON_NAME(Button20);
		BUTTON_NAME(Button21);
		BUTTON_NAME(Button22);
		BUTTON_NAME(Button23);
		BUTTON_NAME(Button24);
		BUTTON_NAME(Button25);
		BUTTON_NAME(Button26);
		BUTTON_NAME(Button27);
		BUTTON_NAME(Button28);
		BUTTON_NAME(Button29);
		BUTTON_NAME(Button30);
		BUTTON_NAME(Button31);
		BUTTON_NAME(Button32);
		BUTTON_NAME(Button33);
		BUTTON_NAME(Button34);
		BUTTON_NAME(Button35);
		BUTTON_NAME(Button36);
		BUTTON_NAME(Button37);
		BUTTON_NAME(Button38);
		BUTTON_NAME(Button39);
		BUTTON_NAME(Button40);
		BUTTON_NAME(Button41);
		BUTTON_NAME(Button42);
		BUTTON_NAME(Button43);
		BUTTON_NAME(Button44);
		BUTTON_NAME(Button45);
		BUTTON_NAME(Button46);
		BUTTON_NAME(Button47);
		BUTTON_NAME(Button48);
		BUTTON_NAME(Button49);
		BUTTON_NAME(Button50);
		BUTTON_NAME(Button51);
		BUTTON_NAME(Button52);
		BUTTON_NAME(Button53);
		BUTTON_NAME(Button54);
		BUTTON_NAME(Button55);
		BUTTON_NAME(Button56);
		BUTTON_NAME(Button57);
		BUTTON_NAME(Button58);
		BUTTON_NAME(Button59);
		BUTTON_NAME(Button60);
		BUTTON_NAME(Button61);
		BUTTON_NAME(Button62);
		BUTTON_NAME(Button63);
		BUTTON_NAME(Button64);
		BUTTON_NAME(Button65);
		BUTTON_NAME(Button66);
		BUTTON_NAME(Button67);
		BUTTON_NAME(Button68);
		BUTTON_NAME(Button69);
		BUTTON_NAME(Button70);
		BUTTON_NAME(Button71);
		BUTTON_NAME(Button72);
		BUTTON_NAME(Button73);
		BUTTON_NAME(Button74);
		BUTTON_NAME(Button75);
		BUTTON_NAME(Button76);
		BUTTON_NAME(Button77);
		BUTTON_NAME(Button78);
		BUTTON_NAME(Button79);
		BUTTON_NAME(Button80);
		BUTTON_NAME(Button81);
		BUTTON_NAME(Button82);
		BUTTON_NAME(Button83);
		BUTTON_NAME(Button84);
		BUTTON_NAME(Button85);
		BUTTON_NAME(Button86);
		BUTTON_NAME(Button87);
		BUTTON_NAME(Button88);
		BUTTON_NAME(Button89);
		BUTTON_NAME(Button90);
		BUTTON_NAME(Button91);
		BUTTON_NAME(Button92);
		BUTTON_NAME(Button93);
		BUTTON_NAME(Button94);
		BUTTON_NAME(Button95);
		BUTTON_NAME(Button96);
		BUTTON_NAME(Button97);
		BUTTON_NAME(Button98);
		BUTTON_NAME(Button99);
		BUTTON_NAME(Button100);
		BUTTON_NAME(Button101);
		BUTTON_NAME(Button102);
		BUTTON_NAME(Button103);
		BUTTON_NAME(Button104);
		BUTTON_NAME(Button105);
		BUTTON_NAME(Button106);
		BUTTON_NAME(Button107);
		BUTTON_NAME(Button108);
		BUTTON_NAME(Button109);
		BUTTON_NAME(Button110);
		BUTTON_NAME(Button111);
		BUTTON_NAME(Button112);
		BUTTON_NAME(Button113);
		BUTTON_NAME(Button114);
		BUTTON_NAME(Button115);
		BUTTON_NAME(Button116);
		BUTTON_NAME(Button117);
		BUTTON_NAME(Button118);
		BUTTON_NAME(Button119);
		BUTTON_NAME(Button120);
		BUTTON_NAME(Button121);
		BUTTON_NAME(Button122);
		BUTTON_NAME(Button123);
		BUTTON_NAME(Button124);
		BUTTON_NAME(Button125);
		BUTTON_NAME(Button126);
		BUTTON_NAME(Button127);
		BUTTON_NAME(Button128);

		return "InvalidButton";
	}

	GenericInput::Button GenericInput::GetButton(const std::string& button) {
#define CASE_BUTTON(button_id) if (strcmp(button.c_str(), #button_id) == 0) return GenericInput::button_id##_ev

		CASE_BUTTON(Button0);
		CASE_BUTTON(Button1);
		CASE_BUTTON(Button2);
		CASE_BUTTON(Button3);
		CASE_BUTTON(Button4);
		CASE_BUTTON(Button5);
		CASE_BUTTON(Button6);
		CASE_BUTTON(Button7);
		CASE_BUTTON(Button8);
		CASE_BUTTON(Button9);
		CASE_BUTTON(Button10);
		CASE_BUTTON(Button11);
		CASE_BUTTON(Button12);
		CASE_BUTTON(Button13);
		CASE_BUTTON(Button14);
		CASE_BUTTON(Button15);
		CASE_BUTTON(Button16);
		CASE_BUTTON(Button17);
		CASE_BUTTON(Button18);
		CASE_BUTTON(Button19);
		CASE_BUTTON(Button20);
		CASE_BUTTON(Button21);
		CASE_BUTTON(Button22);
		CASE_BUTTON(Button23);
		CASE_BUTTON(Button24);
		CASE_BUTTON(Button25);
		CASE_BUTTON(Button26);
		CASE_BUTTON(Button27);
		CASE_BUTTON(Button28);
		CASE_BUTTON(Button29);
		CASE_BUTTON(Button30);
		CASE_BUTTON(Button31);
		CASE_BUTTON(Button32);
		CASE_BUTTON(Button33);
		CASE_BUTTON(Button34);
		CASE_BUTTON(Button35);
		CASE_BUTTON(Button36);
		CASE_BUTTON(Button37);
		CASE_BUTTON(Button38);
		CASE_BUTTON(Button39);
		CASE_BUTTON(Button40);
		CASE_BUTTON(Button41);
		CASE_BUTTON(Button42);
		CASE_BUTTON(Button43);
		CASE_BUTTON(Button44);
		CASE_BUTTON(Button45);
		CASE_BUTTON(Button46);
		CASE_BUTTON(Button47);
		CASE_BUTTON(Button48);
		CASE_BUTTON(Button49);
		CASE_BUTTON(Button50);
		CASE_BUTTON(Button51);
		CASE_BUTTON(Button52);
		CASE_BUTTON(Button53);
		CASE_BUTTON(Button54);
		CASE_BUTTON(Button55);
		CASE_BUTTON(Button56);
		CASE_BUTTON(Button57);
		CASE_BUTTON(Button58);
		CASE_BUTTON(Button59);
		CASE_BUTTON(Button60);
		CASE_BUTTON(Button61);
		CASE_BUTTON(Button62);
		CASE_BUTTON(Button63);
		CASE_BUTTON(Button64);
		CASE_BUTTON(Button65);
		CASE_BUTTON(Button66);
		CASE_BUTTON(Button67);
		CASE_BUTTON(Button68);
		CASE_BUTTON(Button69);
		CASE_BUTTON(Button70);
		CASE_BUTTON(Button71);
		CASE_BUTTON(Button72);
		CASE_BUTTON(Button73);
		CASE_BUTTON(Button74);
		CASE_BUTTON(Button75);
		CASE_BUTTON(Button76);
		CASE_BUTTON(Button77);
		CASE_BUTTON(Button78);
		CASE_BUTTON(Button79);
		CASE_BUTTON(Button80);
		CASE_BUTTON(Button81);
		CASE_BUTTON(Button82);
		CASE_BUTTON(Button83);
		CASE_BUTTON(Button84);
		CASE_BUTTON(Button85);
		CASE_BUTTON(Button86);
		CASE_BUTTON(Button87);
		CASE_BUTTON(Button88);
		CASE_BUTTON(Button89);
		CASE_BUTTON(Button90);
		CASE_BUTTON(Button91);
		CASE_BUTTON(Button92);
		CASE_BUTTON(Button93);
		CASE_BUTTON(Button94);
		CASE_BUTTON(Button95);
		CASE_BUTTON(Button96);
		CASE_BUTTON(Button97);
		CASE_BUTTON(Button98);
		CASE_BUTTON(Button99);
		CASE_BUTTON(Button100);
		CASE_BUTTON(Button101);
		CASE_BUTTON(Button102);
		CASE_BUTTON(Button103);
		CASE_BUTTON(Button104);
		CASE_BUTTON(Button105);
		CASE_BUTTON(Button106);
		CASE_BUTTON(Button107);
		CASE_BUTTON(Button108);
		CASE_BUTTON(Button109);
		CASE_BUTTON(Button110);
		CASE_BUTTON(Button111);
		CASE_BUTTON(Button112);
		CASE_BUTTON(Button113);
		CASE_BUTTON(Button114);
		CASE_BUTTON(Button115);
		CASE_BUTTON(Button116);
		CASE_BUTTON(Button117);
		CASE_BUTTON(Button118);

		return GenericInput::InvalidButton;
	}

	std::string GenericInput::GetAxis(Axis axis) {
#define AXIS_NAME(axis_id) if (axis == GenericInput::axis_id##_ev) return #axis_id

		AXIS_NAME(Axis0);
		AXIS_NAME(Axis1);
		AXIS_NAME(Axis2);
		AXIS_NAME(Axis3);
		AXIS_NAME(Axis4);
		AXIS_NAME(Axis5);
		AXIS_NAME(Axis6);
		AXIS_NAME(Axis7);
		AXIS_NAME(Axis8);
		AXIS_NAME(Axis9);
		AXIS_NAME(Axis10);
		AXIS_NAME(Axis11);
		AXIS_NAME(Axis12);
		AXIS_NAME(Axis13);
		AXIS_NAME(Axis14);
		AXIS_NAME(Axis15);
		AXIS_NAME(Axis16);
		AXIS_NAME(Axis17);
		AXIS_NAME(Axis18);
		AXIS_NAME(Axis19);
		AXIS_NAME(Axis20);
		AXIS_NAME(Axis21);
		AXIS_NAME(Axis22);
		AXIS_NAME(Axis23);
		AXIS_NAME(Axis24);
		AXIS_NAME(Axis25);
		AXIS_NAME(Axis26);
		AXIS_NAME(Axis27);
		AXIS_NAME(Axis28);
		AXIS_NAME(Axis29);
		AXIS_NAME(Axis30);
		AXIS_NAME(Axis31);
		AXIS_NAME(Axis32);
		AXIS_NAME(Axis33);
		AXIS_NAME(Axis34);
		AXIS_NAME(Axis35);
		AXIS_NAME(Axis36);
		AXIS_NAME(Axis37);
		AXIS_NAME(Axis38);
		AXIS_NAME(Axis39);
		AXIS_NAME(Axis40);
		AXIS_NAME(Axis41);
		AXIS_NAME(Axis42);
		AXIS_NAME(Axis43);
		AXIS_NAME(Axis44);
		AXIS_NAME(Axis45);
		AXIS_NAME(Axis46);
		AXIS_NAME(Axis47);
		AXIS_NAME(Axis48);
		AXIS_NAME(Axis49);
		AXIS_NAME(Axis50);
		AXIS_NAME(Axis51);
		AXIS_NAME(Axis52);
		AXIS_NAME(Axis53);
		AXIS_NAME(Axis54);
		AXIS_NAME(Axis55);
		AXIS_NAME(Axis56);
		AXIS_NAME(Axis57);
		AXIS_NAME(Axis58);
		AXIS_NAME(Axis59);
		AXIS_NAME(Axis60);
		AXIS_NAME(Axis61);
		AXIS_NAME(Axis62);
		AXIS_NAME(Axis63);
		AXIS_NAME(Axis64);
		AXIS_NAME(Axis65);
		AXIS_NAME(Axis66);
		AXIS_NAME(Axis67);
		AXIS_NAME(Axis68);
		AXIS_NAME(Axis69);
		AXIS_NAME(Axis70);
		AXIS_NAME(Axis71);
		AXIS_NAME(Axis72);
		AXIS_NAME(Axis73);
		AXIS_NAME(Axis74);
		AXIS_NAME(Axis75);
		AXIS_NAME(Axis76);
		AXIS_NAME(Axis77);
		AXIS_NAME(Axis78);
		AXIS_NAME(Axis79);

		return "InvalidAxis";
	}

	GenericInput::Axis GenericInput::GetAxis(const std::string& axis) {
#define CASE_AXIS(axis_id) if (strcmp(axis.c_str(), #axis_id) == 0) return GenericInput::axis_id##_ev

		CASE_AXIS(Axis0);
		CASE_AXIS(Axis1);
		CASE_AXIS(Axis2);
		CASE_AXIS(Axis3);
		CASE_AXIS(Axis4);
		CASE_AXIS(Axis5);
		CASE_AXIS(Axis6);
		CASE_AXIS(Axis7);
		CASE_AXIS(Axis8);
		CASE_AXIS(Axis9);
		CASE_AXIS(Axis10);
		CASE_AXIS(Axis11);
		CASE_AXIS(Axis12);
		CASE_AXIS(Axis13);
		CASE_AXIS(Axis14);
		CASE_AXIS(Axis15);
		CASE_AXIS(Axis16);
		CASE_AXIS(Axis17);
		CASE_AXIS(Axis18);
		CASE_AXIS(Axis19);
		CASE_AXIS(Axis20);
		CASE_AXIS(Axis21);
		CASE_AXIS(Axis22);
		CASE_AXIS(Axis23);
		CASE_AXIS(Axis24);
		CASE_AXIS(Axis25);
		CASE_AXIS(Axis26);
		CASE_AXIS(Axis27);
		CASE_AXIS(Axis28);
		CASE_AXIS(Axis29);
		CASE_AXIS(Axis30);
		CASE_AXIS(Axis31);
		CASE_AXIS(Axis32);
		CASE_AXIS(Axis33);
		CASE_AXIS(Axis34);
		CASE_AXIS(Axis35);
		CASE_AXIS(Axis36);
		CASE_AXIS(Axis37);
		CASE_AXIS(Axis38);
		CASE_AXIS(Axis39);
		CASE_AXIS(Axis40);
		CASE_AXIS(Axis41);
		CASE_AXIS(Axis42);
		CASE_AXIS(Axis43);
		CASE_AXIS(Axis44);
		CASE_AXIS(Axis45);
		CASE_AXIS(Axis46);
		CASE_AXIS(Axis47);
		CASE_AXIS(Axis48);
		CASE_AXIS(Axis49);
		CASE_AXIS(Axis50);
		CASE_AXIS(Axis51);
		CASE_AXIS(Axis52);
		CASE_AXIS(Axis53);
		CASE_AXIS(Axis54);
		CASE_AXIS(Axis55);
		CASE_AXIS(Axis56);
		CASE_AXIS(Axis57);
		CASE_AXIS(Axis58);
		CASE_AXIS(Axis59);
		CASE_AXIS(Axis60);
		CASE_AXIS(Axis61);
		CASE_AXIS(Axis62);
		CASE_AXIS(Axis63);
		CASE_AXIS(Axis64);
		CASE_AXIS(Axis65);
		CASE_AXIS(Axis66);
		CASE_AXIS(Axis67);
		CASE_AXIS(Axis68);
		CASE_AXIS(Axis69);
		CASE_AXIS(Axis70);
		CASE_AXIS(Axis71);
		CASE_AXIS(Axis72);
		CASE_AXIS(Axis73);
		CASE_AXIS(Axis74);
		CASE_AXIS(Axis75);
		CASE_AXIS(Axis76);
		CASE_AXIS(Axis77);
		CASE_AXIS(Axis78);
		CASE_AXIS(Axis79);

		return GenericInput::InvalidAxis_ev;
	}

	std::string GenericInput::GetHat(Hat hate) {
#define HAT_NAME(hat_id) if (hate == GenericInput::hat_id##_ev) return #hat_id

		HAT_NAME(Hat_Down);
		HAT_NAME(Hat_Up);
		HAT_NAME(Hat_Left);
		HAT_NAME(Hat_Right);

		return "InvalidHate";
	}

	GenericInput::Hat GenericInput::GetHat(const std::string& hat) {
#define CASE_HAT(hat_id) if (strcmp(hat.c_str(), #hat_id) == 0) return GenericInput::hat_id##_ev

		CASE_HAT(Hat_Down);
		CASE_HAT(Hat_Up);
		CASE_HAT(Hat_Left);
		CASE_HAT(Hat_Right);

		return GenericInput::InvalidHat_ev;
	}

}    // namespace nkentseu