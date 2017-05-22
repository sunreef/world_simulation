#pragma once

#include "resource.h"

class Water : Resource
{
private:
	cv::Mat m_waterMap;

public:
	Water(int width, int height);
	~Water();

	// Inherited via Resource
	virtual const cv::Mat& getResourceInArea(const cv::Rect& area) override;
};

