#include <IrradianceMapConvertor.h>


IrradianceMapConvertor::IrradianceMapConvertor() :
	isItialized(false)
{
}

IrradianceMapConvertor::~IrradianceMapConvertor()
{
	if (isItialized) {
		delete cubemap;
		delete irradiance;
	}
}
