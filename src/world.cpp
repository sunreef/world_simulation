#include "world.h"



void World::createAgent()
{
	cv::Point position(m_distribX(RNG), m_distribY(RNG));
	m_agents.push_back(Agent(position, m_initialWealthDistribution(RNG), m_initialMovingProbabilityDistribution(RNG), 20));
}

void World::updateAttractivityMap()
{
	m_attractivityMap.setTo(0.0);
	for (Agent a : m_agents) {
		m_attractivityMap.at<double>(a.getPosition()) += a.getWealth();
	}

	cv::GaussianBlur(m_attractivityMap, m_attractivityMap, cv::Size(5, 5), 1.0, 1.0);

	m_attractivityMap /= 1000.0;
}

Information World::createInformation(size_t agentIndex)
{
	Agent a = m_agents[agentIndex];
	int searchRadius = (int)a.getInformationRadius();
	cv::Point position = a.getPosition();

	cv::Point topLeft(std::max(0, position.x - searchRadius), std::max(0, position.y - searchRadius));
	cv::Point bottomRight(std::min(m_attractivityMap.cols - 1, position.x + searchRadius), std::min(m_attractivityMap.rows - 1, position.y + searchRadius));

	cv::Rect rectArea(topLeft, bottomRight);
	cv::Mat area = m_attractivityMap(rectArea);
	return Information(area, topLeft);
}

World::World()
{
	RNG = std::mt19937(std::random_device()());
	m_attractivityMap = cv::Mat(250, 400, CV_64FC1);

	m_distribX = std::uniform_int_distribution<int>(0, m_attractivityMap.cols - 1);
	m_distribY = std::uniform_int_distribution<int>(0, m_attractivityMap.rows - 1);
	m_initialWealthDistribution = std::normal_distribution<double>(1000.0, 500.0);
	m_initialMovingProbabilityDistribution = std::normal_distribution<double>(0.2, 0.02);

	for (int i = 0; i < 10000; i++) {
		createAgent();
	}
	updateAttractivityMap();
}

World::~World()
{
}

void World::update()
{
	updateAttractivityMap();

#pragma omp parallel for
	for (int i = 0; i < m_agents.size(); i++) {
		Information info = createInformation(i);
		m_agents[i].makeDecision(info);
	}
}

void World::showWorldState()
{
	cv::imshow("Attractivity Map", m_attractivityMap);
	cv::waitKey(50);
}
