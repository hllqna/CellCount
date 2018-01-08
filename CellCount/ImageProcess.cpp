#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <fstream>
#include <stdlib.h>
#include "ImageProcess.h"


ImageProcess::ImageProcess(cv::Mat &src, cv::Mat &result)
{
	this->src = src;
	this->result = result;
}

ImageProcess::~ImageProcess()
{
	contours.clear();
	center.clear();
	cont.clear();
	hierarchy.clear();
}

void ImageProcess::setRGB(unsigned int minB, unsigned int minG, unsigned int minR, 
	unsigned int maxB, unsigned int maxG, unsigned int maxR) 
{
	this->minR = minR;
	this->minG = minG;
	this->minB = minB;
	this->maxR = maxR;
	this->maxG = maxG;
	this->maxB = maxB;
}

void ImageProcess::setDilationAndErosion(int dilation_size, int erosion_size)
{
	this->dilation_size = dilation_size;
	this->erosion_size = erosion_size;
}

void ImageProcess::setColours(int bB, int bG, int bR, int nB, int nG, int nR)
{
	this->colorBound = cv::Scalar(bB, bG, bR);
	this->colorNum = cv::Scalar(nB, nG, nR);
}

int ImageProcess::process() 
{
	cv::inRange(src, cv::Scalar(minB, minG, minR), cv::Scalar(maxB, maxG, maxR), mask);
	//	cv::namedWindow("Mask", cv::WINDOW_NORMAL);
	//	cv::imshow("Mask", mask);

	//Dilation
	int dilation_type = cv::MORPH_CROSS;
	cv::Mat elementDilation = cv::getStructuringElement(dilation_type,
		cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		cv::Point(dilation_size, dilation_size));
	dilate(mask, mask, elementDilation);

	//Erosion
	int erosion_type = cv::MORPH_ELLIPSE;
	cv::Mat elementErosion = cv::getStructuringElement(erosion_type,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		cv::Point(erosion_size, erosion_size));
	erode(mask, mask, elementErosion);

	//Canny detection
	int lowThreshold = 100;
	int ratio = 3;
	int kernel_size = 3;
	Canny(mask, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

	// Finding contours
	findContours(detected_edges, cont, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	for (size_t i = 0; i < cont.size(); i++)
	{
		if (!(hierarchy[i][3] >= 0))   //use an outer bound of the detected edge
			contours.push_back(cont[i]);
	}

	// Approximate contours to polygons + get bounding circles
	std::vector<std::vector<cv::Point> > contours_poly(contours.size());
	center.resize(contours.size());
	std::vector<float>radius(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
		cv::minEnclosingCircle((cv::Mat)contours_poly[i], center[i], radius[i]);
	}

	// Draw polygonal contour + bonding circles
	std::string outputNum;
	int thickness = 3;
	for (int i = 0; i< contours.size(); i++)
	{

		cv::drawContours(result, contours_poly, i, colorBound, thickness, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
		cv::circle(result, center[i], (int)radius[i], colorBound, thickness, 8, 0);
		cv::drawMarker(result, center[i], colorBound);
		outputNum = std::to_string(i);
		cv::putText(result, outputNum, center[i], CV_FONT_HERSHEY_PLAIN, 5, colorNum);
	}




	return 0;
}

