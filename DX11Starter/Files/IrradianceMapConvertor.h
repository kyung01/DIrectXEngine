#pragma once
class IrradianceMapConvertor {
private:
protected:
	bool isItialized;
	float *cubemap;
	float *irradiance;
public:
	IrradianceMapConvertor();
	~IrradianceMapConvertor();
	void init(int width, int height);
	void run();
};