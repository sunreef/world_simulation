#pragma once

#include <vector>
#include <memory>
#include <random>
#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "agent.h"

class World
{
private:
	std::mt19937 RNG;
	std::uniform_int_distribution<int> m_distribX, m_distribY;

	std::normal_distribution<double> m_initialWealthDistribution;
	std::normal_distribution<double> m_initialMovingProbabilityDistribution;

	std::vector<Agent> m_agents;

	cv::Mat m_attractivityMap;


	void createAgent();
	void updateAttractivityMap();
	Information createInformation(size_t agentIndex);



public:
	World();
	~World();

	void update();
	void showWorldState();
};

