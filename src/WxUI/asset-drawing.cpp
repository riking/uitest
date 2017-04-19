
#include "asset.h"
#include <wx/dc.h>
#include <include/wx/window.h>
#include <memory>
#include <include/wx/wx.h>

enum {
	// Transparent buttons, light BODY body, blue L/R buttons
	ASSET_PAIR_WAITING_1 = 1,
	// Transparent buttons, light BODY body, hidden L/R buttons
	ASSET_PAIR_WAITING_2,
	// Transparent buttons, very light BODY body, light blue L/R buttons
	ASSET_PAIR_MISSING_1,
	// Transparent buttons, very light BODY body, hidden L/R buttons
	ASSET_PAIR_MISSING_2,
	// BUTTONS buttons, BODY body, with border
	ASSET_PAIRED,
	// this one might be invalid for us?
	//ASSET_PAIRED_MISSING,
	// Transparent buttons, BUTTONS body
	ASSET_PAIRED_SMALL,
	// Transparent buttons, lighter than BUTTONS body
	ASSET_PAIRED_MISSING_SMALL,
};

enum {
	ASSET_UPRIGHT_L = 10,
	ASSET_UPRIGHT_R,
	ASSET_SIDEWAYS_L,
	ASSET_SIDEWAYS_R,
};

struct DynamicAssetRequest {
	int asset_style;
	int orientation;
	uint32_t body_color;
	uint32_t buttons_color;
	uint32_t border_color; // only for ASSET_PAIRED
	// values not either NULL or IsIdentity() invalidate caching
	wxAffineMatrix2D *rotation;
};

namespace {

constexpr uint8_t alphaBlend(uint8_t fg, uint8_t bg, double alpha) {
	return bg + ((uint8_t)(alpha * (fg - bg)));
}

constexpr uint32_t pairWaitingColor(uint32_t body_color) {
	// 33% blend with white
	return (alphaBlend((uint8_t) ((body_color & 0xFF0000) >> 16), 0xFF, 1. / 3) << 16) |
			(alphaBlend((uint8_t) ((body_color & 0x00FF00) >> 8), 0xFF, 1. / 3) << 8) |
			(alphaBlend((uint8_t) ((body_color & 0x0000FF) >> 0), 0xFF, 1. / 3) << 0);
}

void DrawSidewaysPairWaiting(wxWindowBase *dip, wxDC *dc, DynamicAssetRequest &req) {
	const wxBrush brushBody = wxBrush(pairWaitingColor(req.body_color));
	const wxBrush *brushButtons = wxTRANSPARENT_BRUSH;
	const int tenpx = dip->FromDIP(10);
	const int button_size = dip->FromDIP(4);
	const int stick_size = button_size * 2;
	const int sl_size = tenpx;
	const int sl_border = dip->FromDIP(3);
	const int sl_body = sl_size - sl_border;
	const int sl_flat = stick_size;

	// __/¯¯|__|¯¯\__
	// |   .-.  o   |
	// \___^-^_°o°__/

	dc->SetBrush(brushBody);
	// joycon body
	dc->DrawRectangle(wxPoint(tenpx * 1, tenpx * 3), wxSize(tenpx * 5, tenpx * 1));
	dc->DrawEllipticArc(wxPoint(tenpx * 1, tenpx * 4), wxSize(tenpx, tenpx), 180, 270);
	dc->DrawRectangle(wxPoint(tenpx * 2, tenpx * 4), wxSize(tenpx * 3, tenpx * 1));
	dc->DrawEllipticArc(wxPoint(tenpx * 5, tenpx * 4), wxSize(tenpx, tenpx), 270, 0);
	// sl/sr background
	const int sl_edge_to_border = tenpx - sl_border;
	if (req.asset_style == ASSET_PAIR_WAITING_2 || req.asset_style == ASSET_PAIR_MISSING_2) {
		dc->DrawRectangle(wxPoint(tenpx * 1 + sl_edge_to_border, tenpx * 3 - sl_border),
			wxSize(sl_body + sl_border * 2, sl_border));
		dc->DrawRectangle(wxPoint(tenpx * 6 - sl_edge_to_border - (sl_body + sl_border * 2),
			tenpx * 3 - sl_border), wxSize(sl_body + sl_border * 2, sl_border));
	} else {
		// todo sr
		dc->DrawEllipticArc(wxPoint(tenpx * 1 + sl_edge_to_border, tenpx * 3 - sl_size),
				wxSize(sl_size, sl_size), 90, 180);
		dc->DrawRectangle(wxPoint(tenpx * 1 + sl_edge_to_border + sl_size, tenpx * 3 - sl_size),
			wxSize(sl_body + sl_border, sl_size));
		dc->SetBrush(*wxBLUE_BRUSH);
		dc->DrawEllipticArc(wxPoint(tenpx * 1 + sl_edge_to_border + sl_border, tenpx * 3 - sl_body),
							wxSize(sl_body, sl_body), 90, 180);
		dc->DrawRectangle(wxPoint(tenpx * 1 + sl_edge_to_border + sl_size + sl_border, tenpx * 3 - sl_body),
						  wxSize(sl_body, sl_size));
		dc->SetBrush(brushBody);
	}
}
}

std::shared_ptr<wxBitmap> DynamicAsset(wxWindowBase *win, DynamicAssetRequest &req) {
	std::shared_ptr<wxBitmap> bitmap = std::make_shared<wxBitmap>();
	{
		wxMemoryDC draw_context;
		bitmap->Create(96, 96, draw_context);
		draw_context.SelectObject(*bitmap);
		DrawSidewaysPairWaiting(win, &draw_context, req);
		draw_context.SelectObject(wxNullBitmap);
	}
	return bitmap;
}
