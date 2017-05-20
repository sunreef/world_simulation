#pragma once
#include <random>
#include <iostream>

#include "information.h"
#include "decision.h"

#include <opencv2/core.hpp>

class Agent
{
private:
	std::mt19937 RNG;
	std::uniform_real_distribution<double> distrib;

	double m_wealth;
	double m_movingProbability;
	cv::Point m_position;
	double m_informationRadius;
	double m_attractivenessThreshold = 0.1;



public:
	Agent(cv::Point& position, double wealth, double movingProbability, double informationRadius);
	~Agent();

	const cv::Point& getPosition();
	double getWealth();
	double getInformationRadius();

	Decision makeDecision(const Information& info);
};

