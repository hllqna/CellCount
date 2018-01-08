#ifndef IMAGE_PROCESS
#define IMAGE_PROCESS

class ImageProcess
{
public:
	ImageProcess(cv::Mat &src, cv::Mat &result);
	~ImageProcess();
	void setRGB(unsigned int minB, unsigned int minG, unsigned int minR,
		unsigned int maxB, unsigned int maxG, unsigned int maxR);
	void setDilationAndErosion(int dilation_size, int erosion_size);
	void setColours(int bB, int bG, int bR, int nB, int nG, int nR);
	int process();
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Point2f> center;


private:
	cv::Mat src, mask, detected_edges, result;
	cv::Scalar colorBound, colorNum;
	int dilation_size, erosion_size;
	unsigned int minR, minG, minB, maxR, maxG, maxB;
	std::vector<std::vector<cv::Point> > cont;
	std::vector<cv::Vec4i> hierarchy;


};

#endif // !IMAGE_PROCESS

