// match.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include "matcher.h"
#include "ransac.h"
//#include "function.h"
#include <iostream>
#include <fstream>
//#include <vector>
#include <cmath>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
//#include "opencv2/legacy/legacy.hpp"
//#include "opencv2/nonfree/nonfree.hpp"

double dist(cv::Point3f p1,cv::Point3f p2);

int main(int argc,char **argv)
{
	// Read input images
	char* str1;
	char* str2;
	char* str3;
	char* str4;
	str1 = argv[1]; str2 = argv[2];
	str3 = argv[3]; str4 = argv[4];
	cv::Mat image1 = cv::imread(str1,0);
	cv::Mat image2 = cv::imread(str2,0);
	cv::Mat imageD1 = cv::imread(str3,CV_LOAD_IMAGE_ANYDEPTH);
	cv::Mat imageD2 = cv::imread(str4,CV_LOAD_IMAGE_ANYDEPTH);
//	cv::Mat image1= cv::imread("/home/tyu/ubuntu_opencv/image/1.png",0);
//	cv::Mat image2= cv::imread("/home/tyu/ubuntu_opencv/image/2.png",0);
//	cv::Mat imageD1 = cv::imread("/home/tyu/ubuntu_opencv/image/1d.png",CV_LOAD_IMAGE_ANYDEPTH);
//	cv::Mat imageD2 = cv::imread("/home/tyu/ubuntu_opencv/image/2d.png",CV_LOAD_IMAGE_ANYDEPTH);
	if (!image1.data || !image2.data)
	{
		std::cout<<"Cannt load the image data!\n";
		return 0;
	}

    // Display the images
//	cv::namedWindow("Right Image");
//	cv::imshow("Right Image",image1);
//	cv::namedWindow("Left Image");
//	cv::imshow("Left Image",image2);

	// Prepare the matcher
	RobustMatcher rmatcher;
	rmatcher.setConfidenceLevel(0.98);
	rmatcher.setMinDistanceToEpipolar(1.0);
	rmatcher.setRatio(0.65f);
	cv::Ptr<cv::FeatureDetector> pfd= new cv::SurfFeatureDetector(15000); 
	rmatcher.setFeatureDetector(pfd);

	// Match the two images
	std::vector<cv::DMatch> matches;
	std::vector<cv::KeyPoint> keypoints1, keypoints2;
	cv::Mat fundemental= rmatcher.match(image1,image2,matches, keypoints1, keypoints2);
	std::cout<<fundemental<<std::endl;
	std::cout<<keypoints1.size()<<std::endl;

	// draw the matches
//	cv::Mat imageMatches;
/*	cv::drawMatches(image1,keypoints1,  // 1st image and its keypoints
		            image2,keypoints2,  // 2nd image and its keypoints
					matches,			// the matches
					imageMatches,		// the image produced
					cv::Scalar(1,255,255)); // color of the lines*/
//	std::cout<<"imageMatches\'s size is :"<<imageMatches.size()<<std::endl;

/*	/////draw lines///////
	line(image1,
			cv::Point(0,255.3),
			cv::Point(640,255.3),
			cv::Scalar(0,0,0)
			);

		);*/
//	cv::namedWindow("Matches");
//	cv::imshow("Matches",imageMatches);


	/////////////////////
	
	// Convert keypoints into Point2f	
//	std::vector<cv::Point2f> points1, points2;
	std::vector<cv::Point3f> points_xyz1,points_xyz2;
	compute3dPosition(imageD1,imageD2,keypoints1,keypoints2,points_xyz1,points_xyz2);

	std::cout<<points_xyz1.size()<<std::endl;
	std::cout<<points_xyz2.size()<<std::endl;





	cv::Mat p1 = cv::Mat(points_xyz1).t();
	cv::Mat p2 = cv::Mat(points_xyz2).t();
	cv::Mat mat;
//	mat = p1;
//	std::cout<<mat<<std::endl;
	int iterations = 0;
	double distance = 100.0;
	double Error = 0.0;
	while(iterations < 0)
	{
		cv::Mat np1 , np2, np1_, np2_;
		int num = 0;
		for(int i=0; i<points_xyz1.size(); i++)
		{
			num++;
			cv::Mat temp = p1.col(i).clone();
//			temp.copyTo(np1.col(i));
			np1.push_back(temp);
			temp = p2.col(i).clone();
			np2.push_back(temp);
		}
		iterations++;
	}

/*	for (std::vector<cv::DMatch>::const_iterator it= matches.begin();
			 it!= matches.end();
			 ++it) {

			 // Get the position of left keypoints
			 float xpoint1= keypoints1[it->queryIdx].pt.x;
			 float ypoint1= keypoints1[it->queryIdx].pt.y;
			 
			 double z = imageD1.at<ushort>(ypoint1,xpoint1);
			 
			 // Get the position of right keypoints
			 float xpoint2= keypoints2[it->trainIdx].pt.x;
			 float ypoint2= keypoints2[it->trainIdx].pt.y;
			 
			 
			 double z1 = imageD2.at<ushort>(ypoint2,xpoint2);
			 if(z != 0 &&z1 != 0)
			{
				points_xyz1.push_back(cv::Point3f(xpoint1,ypoint1,z/5000));
				points_xyz2.push_back(cv::Point3f(xpoint2,ypoint2,z1/5000));
				points1.push_back(cv::Point2f(xpoint1,ypoint1));
				points2.push_back(cv::Point2f(xpoint2,ypoint2));
//				cv::circle(image1,cv::Point(xpoint1,ypoint1),5,cv::Scalar(1,1,255),3);
//				cv::circle(image2,cv::Point(xpoint2,ypoint2),5,cv::Scalar(1,1,255),3);
				
			 }
			 else
				 continue;
			 
			 
	}*/




	std::ofstream p1out("p1.txt"), p2out("p2.txt");
	if(!p1out||!p2out)
		std::cout<<"Can not open the file!"<<std::endl;

//	out<<"fundemental = "<<std::endl<<std::endl;
//	out<<fundemental<<std::endl<<std::endl;
	
//	out<<"p1 = "<<std::endl<<std::endl;
//true location
	//write p1 location to out.txt
	for(int i = 0 ; i < points_xyz1.size(); i++)
		p1out<<points_xyz1[i].x<<" "<<points_xyz1[i].y<<" "<<points_xyz1[i].z<<std::endl;
//	out<<std::endl;
	//write p2 location to out.txt
//	out<<"p2 = "<<std::endl<<std::endl;
	for(int i = 0 ; i < points_xyz2.size(); i++)
		p2out<<points_xyz2[i].x<<" "<<points_xyz2[i].y<<" "<<points_xyz2[i].z<<std::endl;
//	out<<"p1 = "<<std::endl<<std::endl<<points_xyz1<<std::endl;
//	out<<"p2 = "<<std::endl<<std::endl<<points_xyz2<<std::endl;
//	out.close();
	p1out.close();p2out.close();

	//////////////////////////centroid/////////
//	std::cout<<"The centroid is :"<<std::endl<<centroid(points_xyz1)<<std::endl;
//	std::cout<<"The distance between each points to the centroid: "<<std::endl<<centroid_dist(points_xyz1,centroid(points_xyz1))<<std::endl;
	




/*	// Draw the epipolar lines
	std::vector<cv::Vec3f> lines1; 
	cv::computeCorrespondEpilines(cv::Mat(points1),1,fundemental,lines1);
		
	for (std::vector<cv::Vec3f>::const_iterator it= lines1.begin();
			 it!=lines1.end(); ++it) {

			 cv::line(image2,cv::Point(0,-(*it)[2]/(*it)[1]),
				             cv::Point(image2.cols,-((*it)[2]+(*it)[0]*image2.cols)/(*it)[1]),
							 cv::Scalar(255,255,255));
	}

	std::vector<cv::Vec3f> lines2; 
	cv::computeCorrespondEpilines(cv::Mat(points2),2,fundemental,lines2);
	
	for (std::vector<cv::Vec3f>::const_iterator it= lines2.begin();
		     it!=lines2.end(); ++it) {

			 cv::line(image1,cv::Point(0,-(*it)[2]/(*it)[1]),
				             cv::Point(image1.cols,-((*it)[2]+(*it)[0]*image1.cols)/(*it)[1]),
							 cv::Scalar(255,255,255));
	}

    // Display the images with epipolar lines
	cv::namedWindow("Right Image Epilines (RANSAC)");
	cv::imshow("Right Image Epilines (RANSAC)",image1);
	cv::namedWindow("Left Image Epilines (RANSAC)");
	cv::imshow("Left Image Epilines (RANSAC)",image2);

	cv::waitKey();*/
	return 0;
}

double dist(cv::Point3f p1,cv::Point3f p2)
{
	double d = pow(p1.x - p2.x,2) + 
		pow(p1.y - p2.y,2) + 
		pow(p1.z - p2.z,2);
}
