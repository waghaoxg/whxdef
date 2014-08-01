#include "detectFeaturePoints.h"
#include "imgproc\imgproc.hpp"
#include "core\core.hpp"
#include "highgui\highgui.hpp"
#include "nonfree\features2d.hpp"
//#include "nonfree\features2d.hpp"
//#include 
using namespace std;

template<typename T> struct greaterThanPtr
{
    bool operator()(const T* a, const T* b) const { return *a > *b; }
};

void detectFeaturePoints( Mat &img, vector<Point2d> &corners,
                              int maxCorners, double qualityLevel, double minDistance,
                              Mat mask, int blockSize,
                              bool useHarrisDetector, double harrisK )
{
	//0. pyrdown
	double scaleDown = 0.75;
	Size szImg = img.size();
	//Size szImgDown1((int)(szImg.width * scaleDown), (int)(szImg.height * scaleDown));
	//double scaleUp1 =  (double)szImg.width / (double)szImgDown1.width;
	//Size szImgDown2((int)(szImgDown1.width * scaleDown), (int)(szImgDown1.height * scaleDown));
	//double scaleUp2 =  (double)szImg.width / (double)szImgDown2.width;

	Mat imgDown1, imgDown2;
	pyrDown(img, imgDown1);//, szImgDown1);
	pyrDown(img, imgDown2);//, szImgDown2);
	double scaleUp1 =  (double)szImg.width / (double)imgDown1.size().width;
	double scaleUp2 =  (double)szImg.width / (double)imgDown2.size().width;
#if 0
	imshow("imgDown1",imgDown1);
	imshow("imgDown2",imgDown2);
#endif
	//1.detect
    Mat eig, eigDown1, eigDown2; 
	cornerMinEigenVal( img, eig, blockSize, 3 );
	cornerMinEigenVal( imgDown1, eigDown1, blockSize, 3 );
	cornerMinEigenVal( imgDown2, eigDown2, blockSize, 3 );

    //surf
	double hessianThreshold = 150;
   SURF surf(hessianThreshold);
	
   vector<KeyPoint> keyPtsDown2;
   vector<Point2f> cornerSURFDown2;
   surf(imgDown2, Mat(), keyPtsDown2);
   KeyPoint::convert(keyPtsDown2, cornerSURFDown2);
#if 1
   	Mat disp_img = imgDown2.clone();
	for (size_t i = 0; i < cornerSURFDown2.size(); i++)
	{
		circle(disp_img, cornerSURFDown2[i], 4, cv::Scalar(255,0,0), 2,8);
	}

	//namedWindow("kankan",0);
	namedWindow("result",0);
	////imshow("kankan", img);
	imshow("result", disp_img);
#endif
	//goodFeatureToTrack

	Mat tmp, eig_threshold;
//	cornerMinEigenVal( image, eig, blockSize, 3 );
//	//down sample
//	//corner
//	//surf
//	//2.sort
//	//3.minDis
//
//
//    //Mat image = _image.getMat(), mask = _mask.getMat();
//
//    //CV_Assert( qualityLevel > 0 && minDistance >= 0 && maxCorners >= 0 );
//    //CV_Assert( mask.empty() || (mask.type() == CV_8UC1 && mask.size() == image.size()) );
//
//    //if( useHarrisDetector )
//     //   cornerHarris( image, eig, blockSize, 3, harrisK );
//    //else
//
//    double maxVal = 0;
//    minMaxLoc( eig, 0, &maxVal, 0, 0, mask );
//	threshold( eig, eig_threshold, maxVal * qualityLevel, 0, THRESH_TOZERO );
//	dilate( eig_threshold, tmp, Mat());
//
//    Size imgsize = image.size();
//
//    vector<const float*> tmpCorners;
//
//    // collect list of pointers to features - put them into temporary image
//    for( int y = 1; y < imgsize.height - 1; y++ )
//    {
//        const float* eig_data = (const float*)eig.ptr(y);
//        const float* tmp_data = (const float*)tmp.ptr(y);
//        const uchar* mask_data = mask.data ? mask.ptr(y) : 0;
//
//        for( int x = 1; x < imgsize.width - 1; x++ )
//        {
//            float val = eig_data[x];
//            if( val != 0 && val == tmp_data[x] && (!mask_data || mask_data[x]) )
//                tmpCorners.push_back(eig_data + x);
//        }
//    }
//
//    sort( tmpCorners, greaterThanPtr<float>() );
//    vector<Point2f> corners;
//    size_t i, j, total = tmpCorners.size(), ncorners = 0;
//
//    if(minDistance >= 1)
//    {
//         // Partition the image into larger grids
//        int w = image.cols;
//        int h = image.rows;
//
//        const int cell_size = cvRound(minDistance);
//        const int grid_width = (w + cell_size - 1) / cell_size;
//        const int grid_height = (h + cell_size - 1) / cell_size;
//
//        std::vector<std::vector<Point2f> > grid(grid_width*grid_height);
//
//        minDistance *= minDistance;
//
//        for( i = 0; i < total; i++ )
//        {
//            int ofs = (int)((const uchar*)tmpCorners[i] - eig.data);
//            int y = (int)(ofs / eig.step);
//            int x = (int)((ofs - y*eig.step)/sizeof(float));
//
//            bool good = true;
//
//            int x_cell = x / cell_size;
//            int y_cell = y / cell_size;
//
//            int x1 = x_cell - 1;
//            int y1 = y_cell - 1;
//            int x2 = x_cell + 1;
//            int y2 = y_cell + 1;
//
//            // boundary check
//            x1 = std::max(0, x1);
//            y1 = std::max(0, y1);
//            x2 = std::min(grid_width-1, x2);
//            y2 = std::min(grid_height-1, y2);
//
//            for( int yy = y1; yy <= y2; yy++ )
//            {
//                for( int xx = x1; xx <= x2; xx++ )
//                {
//                    vector <Point2f> &m = grid[yy*grid_width + xx];
//
//                    if( m.size() )
//                    {
//                        for(j = 0; j < m.size(); j++)
//                        {
//                            float dx = x - m[j].x;
//                            float dy = y - m[j].y;
//
//                            if( dx*dx + dy*dy < minDistance )
//                            {
//                                good = false;
//                                goto break_out;
//                            }
//                        }
//                    }
//                }
//            }
//
//            break_out:
//
//            if(good)
//            {
//                // printf("%d: %d %d -> %d %d, %d, %d -- %d %d %d %d, %d %d, c=%d\n",
//                //    i,x, y, x_cell, y_cell, (int)minDistance, cell_size,x1,y1,x2,y2, grid_width,grid_height,c);
//                grid[y_cell*grid_width + x_cell].push_back(Point2f((float)x, (float)y));
//
//                corners.push_back(Point2f((float)x, (float)y));
//                ++ncorners;
//
//                if( maxCorners > 0 && (int)ncorners == maxCorners )
//                    break;
//            }
//        }
//    }
//    else
//    {
//        for( i = 0; i < total; i++ )
//        {
//            int ofs = (int)((const uchar*)tmpCorners[i] - eig.data);
//            int y = (int)(ofs / eig.step);
//            int x = (int)((ofs - y*eig.step)/sizeof(float));
//
//            corners.push_back(Point2f((float)x, (float)y));
//            ++ncorners;
//            if( maxCorners > 0 && (int)ncorners == maxCorners )
//                break;
//        }
//    }
//
//    Mat(corners).convertTo(_corners, _corners.fixedType() ? _corners.type() : CV_32F);
//
//    /*
//    for( i = 0; i < total; i++ )
//    {
//        int ofs = (int)((const uchar*)tmpCorners[i] - eig.data);
//        int y = (int)(ofs / eig.step);
//        int x = (int)((ofs - y*eig.step)/sizeof(float));
//
//        if( minDistance > 0 )
//        {
//            for( j = 0; j < ncorners; j++ )
//            {
//                float dx = x - corners[j].x;
//                float dy = y - corners[j].y;
//                if( dx*dx + dy*dy < minDistance )
//                    break;
//            }
//            if( j < ncorners )
//                continue;
//        }
//
//        corners.push_back(Point2f((float)x, (float)y));
//        ++ncorners;
//        if( maxCorners > 0 && (int)ncorners == maxCorners )
//            break;
//    }
//*/
}
