#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MyApp : public wxApp {
  public:
	virtual bool OnInit();
};

class SettingsFrame : public wxDialog {
  public:
	explicit SettingsFrame(wxWindow *parent, const std::string &title);
	void OnClose(wxCloseEvent &event);
};

class MainFrame : public wxFrame {
  public:
	explicit MainFrame();

  private:
	SettingsFrame *m_settings_frame;

	void OnHello(wxCommandEvent &event);
	void OnExit(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);
};

class ControllerStatusPanel : public wxPanel {
  public:
	ControllerStatusPanel(wxWindow *parent);

private:
	std::array<wxPanel *, 4> m_controller_boxes;
};