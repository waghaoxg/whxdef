#pragma once
#include "opencv\cxcore.h"

using namespace cv;

void detectFeaturePoints( Mat &img, vector<Point2d> &corners,
                              int maxCorners, double qualityLevel, double minDistance,
                              Mat mask, int blockSize,
                              bool useHarrisDetector, double harrisK );