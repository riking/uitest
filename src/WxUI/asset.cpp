
#include "asset.h"

wxFileInputStream *GetAsset(const char *filename) {
	// TODO an actual assets root...
	return new wxFileInputStream(
	    wxString(std::string("../assets/") + filename));
}
