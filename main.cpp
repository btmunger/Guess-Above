#include <winrt/Windows.Devices.Geolocation.h>
#include <winrt/Windows.Foundation.h>
#include <iostream>
#include <fstream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include "json.hpp"
using json = nlohmann::json;

using namespace std;
using namespace winrt;
using namespace Windows::Devices::Geolocation;
using namespace Windows::Foundation;

double lat = 0, lon = 0;

/* Method for retrieving the location of this device.
   Utilizes Windows Geolocation API */
void set_location() {
    try {
        // Initialize COM apartment for WinRT
        init_apartment(); 

        // Create geolocator object, set accuracy
        Geolocator locator;
        locator.DesiredAccuracy(PositionAccuracy::High);

        // Get the lat / long coordinates
        auto operation = locator.GetGeopositionAsync();
        auto pos = operation.get();  // Blocks until position is ready
        auto coord = pos.Coordinate().Point().Position();

        //wcout << L"Latitude: " << coord.Latitude
        //      << L"\nLongitude: " << coord.Longitude << endl;

        // Set global variables
        lat = coord.Latitude;
        lon = coord.Longitude;

    } catch (...) {
        wcout << L"ERROR: Failed to get location." << endl;
    }
}

/* Load client ID and secret key from config file */
pair<string, string> load_credentials(const string& filename) {
    ifstream file(filename);
    json j;
    file >> j;
    return { j["clientId"].get<string>(), j["clientSecret"].get<string>()};
}

/* Method for retrieving flights above the user */
void get_flight_info() {
    // Define boundaries for lat / long
    const double PI = 3.14159265358979323846;
    double delta_mi = 5;
    double delta_lat = delta_mi / 69.0;
    double delta_lon = delta_mi / (69.0 * cos(lat * PI / 180.0));

    // Opensky API request URL
    string request_url = "/api/states/all?lamin=" + to_string(lat - delta_lat) +
                 "&lomin=" + to_string(lon - delta_lon) +
                 "&lamax=" + to_string(lat + delta_lat) +
                 "&lomax=" + to_string(lon + delta_lon);

    // Use SSL to make request
    httplib::SSLClient cli("opensky-network.org");

    // Set credentials from json file
    auto [clientId, clientSecret] = load_credentials("credentials.json");
    cli.set_basic_auth(clientId.c_str(), clientSecret.c_str());

    // Gather response, check for successful response 
    auto res = cli.Get(request_url.c_str());
    if (res->status != 200) {
        cerr << "Unable to reach OpenSky API. Returned status code " << res->status << "." << endl;
        cerr << "Response body:\n" << res->body << endl;
        return;
    }

    auto j = json::parse(res->body);
    for (auto &plane : j["states"]) {
        string callsign = plane[6].is_null() ? "N/A" : plane[1].get<string>();
        double alt = plane[5].is_null() ? 0.0 : plane[7].get<double>();
        alt *= 3.28084;

        cout << callsign << " " << alt << " ft\n";
    }
}

/* Main method */
int main() {
    set_location();
    get_flight_info();

    return 0;
}