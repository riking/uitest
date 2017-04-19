
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
	const int stick1 = dip->FromDIP(24);

	dc->SetBrush(brushBody);
	dc->DrawRectangle(wxPoint(stick1 * 1, stick1 * 3), wxSize(stick1 * 5, stick1 * 1));
	dc->DrawEllipticArc(wxPoint(stick1 * 1, stick1 * 4), wxSize(stick1, stick1), 180, 270);
	dc->DrawRectangle(wxPoint(stick1 * 2, stick1 * 4), wxSize(stick1 * 3, stick1 * 1));
	dc->DrawEllipticArc(wxPoint(stick1 * 5, stick1 * 4), wxSize(stick1, stick1), 270, 0);
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
