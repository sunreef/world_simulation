#include "agent.h"

const cv::Point& Agent::getPosition()
{
	return m_position;
}

double Agent::getWealth()
{
	return m_wealth;
}

double Agent::getInformationRadius()
{
	return m_informationRadius;
}

Decision Agent::makeDecision(const Information & info)
{
	double value = distrib(RNG);
	if (value < m_movingProbability) {
		std::vector < cv::Point> possiblePositions;
		std::vector<double> attractiveness;

		cv::Mat area = info.getArea();
		cv::Point corner = info.getCorner();
		for (int i = 0; i < area.rows; i++) {
			double* ptr = area.ptr<double>(i);
			for (int j = 0; j < area.cols; j++) {
				if (ptr[j] > m_attractivenessThreshold) {
					possiblePositions.push_back(cv::Point(j, i));
					attractiveness.push_back(ptr[j]);
				}
			}
		}

		if (attractiveness.size() > 0) {
			for (int i = 1; i < attractiveness.size(); i++) {
				attractiveness[i] += attractiveness[i - 1];
			}
			double randomValue = distrib(RNG) * attractiveness[attractiveness.size() - 1];
			for (int i = 0; i < attractiveness.size(); i++) {
				if (randomValue < attractiveness[i]) {
					m_position = corner + possiblePositions[i];
					break;
				}
			}
		}
	}


	return Decision();
}

Agent::Agent(cv::Point& position, double wealth, double movingProbability, double informationRadius) :
	m_position(position), m_wealth(wealth), m_movingProbability(movingProbability), m_informationRadius(informationRadius)
{
	RNG = std::mt19937(std::random_device()());
	distrib = std::uniform_real_distribution<double>(0.0, 1.0);
}

Agent::~Agent()
{
}
