#include "Probe.h"

NGraphic::NScene::Probe::Probe():
m_influenceDistance(10)
{
	setFOV(3.14 / 2 + 0.05f);
}
