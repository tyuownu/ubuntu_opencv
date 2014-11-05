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
std::vector<cv::Point3f> compute3dPosition(cv::Mat dep,std::vector<cv::KeyPoint> kp)
{
	std::cout<<"start compute 3d position"<<std::endl;
	std::vector<cv::Point3f> location;
	for(int i=0; i<kp.size(); i++)
	{
		double u = kp[i].pt.x, v = kp[i].pt.y;
		unsigned short d = dep.at<unsigned short>(round(v),round(u));
		if (d == 0)
		{
			continue;
		}
		double z = double(d)/camera_factor;
		double x = ( u - camera_cx) * z / camera_fx;
		double y = ( v - camera_cy) * z / camera_fy;
		location.push_back(cv::Point3f(x,y,z));
	}
	return location;
}
