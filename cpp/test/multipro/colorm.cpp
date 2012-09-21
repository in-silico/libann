#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <map>
#include <algorithm>
#include <matrix.h>

using namespace std;

Config *c;

void rgb2Yxy(Mat *dest, Mat *org) {
}

int main() {
    c = new Config("colorm.conf");
    readParams();
    delete c;
}
