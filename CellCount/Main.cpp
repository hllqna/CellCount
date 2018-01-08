#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "ImageProcess.h"

int main(int argc, char * argv[])
// takes arguments:
//				path to folder with images, e.g. C:\\Users\\Natalya\\Documents\\Materials\\
// not compulsory: min{B,G,R} - max {B,G,R} for masking marker1; the same for marker2 - all together as 12 numbers in range 0-255

{

	int minB1 = 40, minG1 = 0, minR1 = 0, maxB1 = 255, maxG1 = 255, maxR1 = 60; //for marker1(blue)
	int minB2 = 60, minG2 = 0, minR2 = 55, maxB2 = 255, maxG2 = 255, maxR2 = 255; //for marker2(blue)

	if (argc > 1)
	{
		if (argc == 14)
		{
			minB1 = atoi(argv[2]); 	minG1 = atoi(argv[3]);	minR1 = atoi(argv[4]);
			maxB1 = atoi(argv[5]); 	maxG1 = atoi(argv[6]);	maxR1 = atoi(argv[7]);
			minB2 = atoi(argv[8]); 	minG2 = atoi(argv[9]); 	minR2 = atoi(argv[10]);
			maxB2 = atoi(argv[11]); maxG2 = atoi(argv[12]); maxR2 = atoi(argv[13]);
		}

		std::string path(argv[1]);
		path += "*.jpg";	
		cv::Mat src, result;
		std::vector<cv::String> fn;
		glob(path, fn, false); //getting all jpg images from folder

		std::vector<cv::Mat> images;
		size_t imageCount = fn.size(); //number of jpg files in images folder


		for (size_t imageNum = 0; imageNum < imageCount; imageNum++) {

			src = cv::imread(fn[imageNum], 1);
			result = src;
			//cv::namedWindow("Source", cv::WINDOW_NORMAL);
			//cv::imshow("Source", src);
			std::ofstream outputFile;
			std::string outfilename = fn[imageNum] + "_out.csv"; //Warning: the csv file is saved into the folder with processed images


			cv::blur(src, src, cv::Size(3, 3));


			ImageProcess marker1(src, result), marker2(src, result);
			marker1.setRGB(minB1, minG1, minR1, maxB1, maxG1, maxR1); //blue
			marker1.setDilationAndErosion(2, 21);
			marker1.setColours(255, 150, 0, 0, 0, 255);


			marker2.setRGB(minB2, minG2, minR2, maxB2, maxG2, maxR2); //red
			marker2.setDilationAndErosion(0, 16);
			marker2.setColours(0, 0, 255, 0, 255, 0);

			marker1.process();
			marker2.process();

			// Getting results
			outputFile.open(outfilename, std::ofstream::trunc);
			char separator = ';'; //replace semicolon with comma if data in csv-file is in one column
			outputFile << "" << separator << "marker1(blue)" << separator << "marker2(red)" << std::endl;
			outputFile << "count" << separator << marker1.contours.size() << separator << marker2.contours.size() << std::endl;
			outputFile << "marker_type" << separator << "centerX" << separator << "centerY" << std::endl;
			for (size_t i = 0; i < marker1.contours.size(); i++)
			{
				outputFile << 1 << separator << marker1.center[i].x << separator << marker1.center[i].y << std::endl;
			}
			for (size_t i = 0; i < marker2.contours.size(); i++)
			{
				outputFile << 2 << separator << marker2.center[i].x << separator << marker2.center[i].y << std::endl;
			}

			outputFile.close();

			//	cv::namedWindow("Result", cv::WINDOW_NORMAL);
			//cv::imshow("Result", result); 
			//cv::waitKey(0);
			marker1.~ImageProcess();
			marker2.~ImageProcess();
		}

		return 0;
		
	}
	else
	{
		std::cerr << "Expected at least one argument (path), found none" << std::endl;
		return -1;
	}

}
