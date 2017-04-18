#include <wx/wxprec.h>

#ifndef WX_PRECOMP
# include <wx/wx.h>
#endif

class MyApp: public wxApp
{
public:
	virtual bool OnInit();
}

class MainFrame: public wxFrame
{
public:
	MainFrame();

private:
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
}

class SettingsFrame : public wxFrame
{
public:
	SettingsFrame();

};