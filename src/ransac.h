#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>




#define camera_factor 50000.0
#define camera_fx 517.3
#define camera_fy 516.5
#define camera_cx 318.6
#define camera_cy 255.3
void compute3dPosition(cv::Mat dep1,
					   cv::Mat dep2,
					   std::vector<cv::KeyPoint> kp1,
					   std::vector<cv::KeyPoint> kp2,
					   std::vector<cv::Point3f>& location1,
					   std::vector<cv::Point3f>& location2)
{
	std::cout<<"start compute 3d position"<<std::endl;
	for(int i=0; i<kp1.size(); i++)
	{
		double u1 = kp1[i].pt.x, v1 = kp1[i].pt.y;
		double u2 = kp2[i].pt.x, v2 = kp2[i].pt.y;
		unsigned short d1 = dep1.at<unsigned short>(round(v1),round(u1));
		unsigned short d2 = dep2.at<unsigned short>(round(v2),round(u2));
		if (d1 == 0 || d2 == 0)
		{
			continue;
		}
		double z1 = double(d1)/camera_factor;
		double x1 = ( u1 - camera_cx) * z1 / camera_fx;
		double y1 = ( v1 - camera_cy) * z1 / camera_fy;
		location1.push_back(cv::Point3f(x1,y1,z1));
		z1 = double(d2)/camera_factor;
		x1 = ( u2 - camera_cx) * z1 / camera_fx;
		y1 = ( v2 - camera_cy) * z1 / camera_fy;
		location2.push_back(cv::Point3f(x1,y1,z1));
	}
}
