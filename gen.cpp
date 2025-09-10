#include <iostream>

using namespace std;

int main() {
  cout << "P3" << '\n';

  int imgWidth = 256;
  int imgHeight = 256;

  cout << imgWidth << " " << imgHeight << "\n255\n";

  for (int h = 0; h < imgHeight; h++) {
    for (int w = 0; w < imgWidth; w++) {
      cout << w << " " << h << " " << "0" << "  ";
    }
    cout << '\n';
  }
}
