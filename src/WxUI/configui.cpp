
#include "uihello.h"
#include <wx/notebook.h>

SettingsFrame::SettingsFrame(wxWindow *parent, const std::string &title)
    : wxDialog(parent, wxID_ANY, title) {
	wxNotebook *const notebook = new wxNotebook(this, wxID_ANY);

	const int space5 = FromDIP(5);

	// -- FIRST PAGE --
	{
		wxPanel *const page_general = new wxPanel(notebook);
		notebook->AddPage(page_general, "General");
		wxBoxSizer *const szr_general = new wxBoxSizer(wxVERTICAL);

		// - basic settings
		wxStaticBoxSizer *const group_basic =
		    new wxStaticBoxSizer(wxVERTICAL, page_general, "Basic Settings");
		wxCheckBox *const dual_core_checkbox =
		    new wxCheckBox(page_general, wxID_ANY, "Enable Dual Core");
		wxCheckBox *const enable_cheats_checkbox =
		    new wxCheckBox(page_general, wxID_ANY, "Enable Cheats");
		group_basic->AddSpacer(space5);
		group_basic->Add(dual_core_checkbox, 0, wxLEFT | wxRIGHT, space5);
		group_basic->AddSpacer(space5);
		group_basic->Add(enable_cheats_checkbox, 0, wxLEFT | wxRIGHT, space5);

		wxStaticBoxSizer *const group_second =
		    new wxStaticBoxSizer(wxVERTICAL, page_general, "Extra Settings");
		wxCheckBox *const settings_neat_checkbox =
		    new wxCheckBox(page_general, wxID_ANY, "Settings are neat");
		wxButton *const button_frobs =
		    new wxButton(page_general, wxID_ANY, "Release the Frobs");
		group_second->AddSpacer(space5);
		group_second->Add(settings_neat_checkbox, 0, wxLEFT, space5);
		group_second->AddSpacer(space5);
		group_second->Add(button_frobs, 0, wxRIGHT, space5);

		szr_general->AddSpacer(space5);
		szr_general->Add(group_basic, 0, wxEXPAND | wxLEFT | wxRIGHT, space5);
		szr_general->AddSpacer(space5);
		szr_general->Add(group_second, 0, wxEXPAND | wxLEFT | wxRIGHT, space5);
		page_general->SetSizer(szr_general);

		button_frobs->Enable(settings_neat_checkbox->IsChecked());
		settings_neat_checkbox->Bind(
		    wxEVT_CHECKBOX, [=](wxCommandEvent &event) {
			    button_frobs->Enable(settings_neat_checkbox->IsChecked());
		    });
		button_frobs->Bind(wxEVT_BUTTON, [=](wxCommandEvent &event) {
			wxLogMessage("Surprise!");
		});
	}

	wxBoxSizer *const main_sizer = new wxBoxSizer(wxVERTICAL);
	main_sizer->AddSpacer(space5);
	main_sizer->Add(notebook, 1, wxEXPAND | wxLEFT | wxRIGHT, space5);
	main_sizer->SetMinSize(FromDIP(400), 0);
	SetSizerAndFit(main_sizer);
}

void SettingsFrame::OnClose(wxCloseEvent &event) { Hide(); }