#include "information.h"

const cv::Mat & Information::getArea() const
{
	return m_areaAttractivity;
}

const cv::Point & Information::getCorner() const
{
	return m_topLeftPosition;
}
