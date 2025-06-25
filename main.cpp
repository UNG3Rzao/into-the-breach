#include <iostream>
#include "field.h"
#include "engine.h"
using namespace std;

int main()
{
  Field f(8, 8);
  loadMap(cin, f);
  play(f, cin, cout);

  return 0;
}
