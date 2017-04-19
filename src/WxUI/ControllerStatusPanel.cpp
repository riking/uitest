
#include "Controllers/ControllerStatus.h"
#include "asset.h"
#include "uihello.h"

ControllerStatusPanel::ControllerStatusPanel(wxWindow *parent)
    : wxPanel(parent) {
	wxBoxSizer *const main_sizer = new wxBoxSizer(wxVERTICAL);

	const int space5 = FromDIP(5);

	wxBoxSizer *const text_container = new wxBoxSizer(wxHORIZONTAL);
	{
		wxBoxSizer *const text_sizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *const text_press =
		    new wxStaticText(this, wxID_ANY, "Press");
		wxStaticText *const text_on =
		    new wxStaticText(this, wxID_ANY, "on the controller.");
		wxImage *const buttons_image =
		    new wxImage(*GetAsset("shoulder-buttons.png"), wxBITMAP_TYPE_PNG);
		wxSize const buttons_dimensions = wxSize(FromDIP(16 * 3), FromDIP(16));
		buttons_image->SetMaskColour(255, 255, 255);
		buttons_image->Rescale(buttons_dimensions.x, buttons_dimensions.y,
		                       wxIMAGE_QUALITY_BOX_AVERAGE);
		wxBitmap *const buttons_bitmap = new wxBitmap(*buttons_image);
		wxStaticBitmap *const text_buttons =
		    new wxStaticBitmap(this, wxID_ANY, *buttons_bitmap,
		                       wxDefaultPosition, buttons_dimensions);

		text_sizer->Add(text_press);
		text_sizer->AddSpacer(space5);
		text_sizer->Add(text_buttons);
		text_sizer->AddSpacer(space5);
		text_sizer->Add(text_on);
		text_container->Add(text_sizer, 0, wxLEFT | wxRIGHT | wxEXPAND);
	}

	wxBoxSizer *const controllers_container = new wxBoxSizer(wxHORIZONTAL);
	{
		wxBoxSizer *const controllers_sizer = new wxBoxSizer(wxHORIZONTAL);
		std::array<wxPanel *, 4> controller_boxes = {};
		const wxSize box_size = wxSize(FromDIP(128), FromDIP(128));
		const int box_spacer = FromDIP(16);

		for (int i = 0; i < 4; i++) {
			wxPanel *const panel = new wxPanel(
			    this, wxID_ANY, wxDefaultPosition, box_size, wxBORDER_SIMPLE);
			wxBoxSizer *const panel_sizer = new wxBoxSizer(wxVERTICAL);
			char buf[2];
			buf[0] = (char)('1' + i);
			buf[1] = 0;
			wxStaticText *const text_number =
			    new wxStaticText(panel, wxID_ANY, wxString(buf));
			text_number->SetOwnBackgroundColour(wxColour(0x7f7f7f));
			text_number->SetOwnForegroundColour(wxColour(0xd0d0d0));
			panel_sizer->Add(text_number, 0, wxLEFT | wxALIGN_CENTER);
			panel->SetSizer(panel_sizer);
			controller_boxes[i] = panel;
		}
		controllers_sizer->AddSpacer(box_spacer);
		controllers_sizer->Add(controller_boxes[0]);
		controllers_sizer->AddSpacer(box_spacer);
		controllers_sizer->Add(controller_boxes[1]);
		controllers_sizer->AddSpacer(box_spacer);
		controllers_sizer->Add(controller_boxes[2]);
		controllers_sizer->AddSpacer(box_spacer);
		controllers_sizer->Add(controller_boxes[3]);
		controllers_sizer->AddSpacer(box_spacer);
		controllers_container->Add(controllers_sizer, 0,
		                           wxLEFT | wxRIGHT | wxEXPAND);
	}

	main_sizer->AddSpacer(space5 * 2);
	main_sizer->Add(text_container, 0, wxALIGN_CENTER_HORIZONTAL);
	main_sizer->AddSpacer(space5 * 4);
	main_sizer->Add(controllers_container, 0, wxALIGN_CENTER_HORIZONTAL);
	main_sizer->AddSpacer(space5 * 4);

	SetSizer(main_sizer);
}