#include "water.h"



Water::Water(int width, int height)
{
	m_waterMap = cv::Mat(height, width, CV_32FC1);
}


Water::~Water()
{
}

const cv::Mat & Water::getResourceInArea(const cv::Rect & area)
{
	return m_waterMap(area);
}
