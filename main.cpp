#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>

#include <stdio.h>

using namespace std;

char const * GetSource(uint8_t source)
{
  switch (source)
  {
  case 0: return "Apple";
  case 1: return "Windows";
  case 2: return "Android";
  case 3: return "Google";
  case 4: return "Tizen";
  case 5: return "Predictor";
  default: return "Unknown";
  }
}

template <typename T>
void read(FILE * f, T * data)
{
  fread(data, sizeof(T), 1, f);
}

int main(int argc, const char * argv [])
{
  if (argc != 2)
  {
    cout << "Please specify path to gps_track.bin file as first parameter" << endl;
    return -1;
  }

  char const * const filePath = argv[1];

  unique_ptr<FILE, decltype(&fclose)> fptr(fopen(filePath, "rb"), &fclose);
  FILE * f = fptr.get();

  if (nullptr == f)
  {
    cout << "Unable to open file " << filePath << endl;
    return -1;
  }

  size_t constexpr itemSize = 65; // 65 bytes per point

  fseek(f, 0, SEEK_END);
  size_t const fileSize = ftell(f);
  size_t const itemCount = fileSize / itemSize;

  cout << endl << "File size: " << fileSize << " bytes" << endl
       << itemCount << " items in file" << endl;
  if ((fileSize % itemSize) != 0)
    cout << "File unexpectedly ends" << endl;
  cout << endl;

  fseek(f, 0, SEEK_SET);

  uint32_t version = 0;
  read(f, &version);

  if (version != 1)
  {
    cout << "Unsupported version" << endl;
    return -1;
  }

  for (size_t i = 0; i < itemCount; ++i)
  {
    double timestamp = 0;
    double latitude = 0;
    double longitude = 0;
    double altitude = 0;
    double speed = 0;
    double bearing = 0;
    double horizontalAccuracy = 0;
    double verticalAccuracy = 0;
    uint8_t source = 0;

    read(f, &timestamp);
    read(f, &latitude);
    read(f, &longitude);
    read(f, &altitude);
    read(f, &speed);
    read(f, &bearing);
    read(f, &horizontalAccuracy);
    read(f, &verticalAccuracy);
    read(f, &source);

    time_t t = static_cast<time_t>(timestamp);

    string s = ctime(&t);
    s.erase(find_if(s.begin(), s.end(), [](char c)->bool{ return c == '\r' || c == '\n'; }), s.end());

    cout << (i + 1) << ") " << s << " source: " << GetSource(source) << endl
         << "  lat: " << latitude << " lon: " << longitude << " alt: " << altitude << endl
         << "  speed: " << speed << endl
         << "  bearing: " << bearing << endl
         << "  accuracy h: " << horizontalAccuracy << " v: " << verticalAccuracy << endl
         << endl;
  }

  return 0;
}
