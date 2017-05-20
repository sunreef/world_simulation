#pragma once

#include <opencv2/core.hpp>

class Information
{
private:
	cv::Point m_topLeftPosition;
	cv::Mat m_areaAttractivity;

public:
	Information(const cv::Mat& areaAttractivity, const cv::Point& cornerPosition) : m_areaAttractivity(areaAttractivity), m_topLeftPosition(cornerPosition) {};
	~Information() {};

	const cv::Mat& getArea() const;
	const cv::Point& getCorner() const;
};

