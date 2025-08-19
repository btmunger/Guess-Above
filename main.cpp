#include <winrt/Windows.Devices.Geolocation.h>
#include <winrt/Windows.Foundation.h>
#include <iostream>
#include "httplib.h"

using namespace std;
using namespace winrt;
using namespace Windows::Devices::Geolocation;
using namespace Windows::Foundation;

int lat = 0, lon = 0;

void set_location() {
    try {
        // Initialize COM apartment for WinRT
        init_apartment(); 

        Geolocator locator;
        locator.DesiredAccuracy(PositionAccuracy::High);

        auto operation = locator.GetGeopositionAsync();
        auto pos = operation.get();  // Blocks until position is ready
        auto coord = pos.Coordinate().Point().Position();

        // wcout << L"Latitude: " << coord.Latitude 
        //       << L"\nLongitude: " << coord.Longitude
        //       << L"\nAltitude: " << coord.Altitude << endl;

        lat = coord.Latitude;
        lon = coord.Longitude;

    } catch (...) {
        wcout << L"ERROR: Failed to get location." << endl;
    }
}

void get_flight_info() {
    const double PI = 3.14159265358979323846;
    double delta_mi = 2.5;
    double delta_lat = delta_mi / 69.0;
    double delta_lon = delta_mi / (69.0 * cos(lat * PI / 180.0));

    string request_url = "/api/states/all?lamin=" + to_string(lat - delta_lat) +
                 "&lomin=" + to_string(lon - delta_lon) +
                 "&lamax=" + to_string(lat + delta_lat) +
                 "&lomax=" + to_string(lon + delta_lon);

    httplib::SSLClient cli("opensky-network.org");
}

int main() {
    set_location();

    return 0;
}