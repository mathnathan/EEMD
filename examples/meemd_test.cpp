#include "meemd.h"
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {

    const string fileName = "../../data/lena.bmp";

    MEEMD m;

    m.load( fileName );
    m.show();

    return 0;
}
