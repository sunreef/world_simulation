#pragma once
#include <opencv2/core.hpp>

class Resource
{

public:
	Resource();
	~Resource();

	virtual const cv::Mat& getResourceInArea(const cv::Rect& area) = 0;
};

