#pragma once
#include <iostream>
namespace NGraphic {
	class Shader {
	private:
	protected:
	public:
		enum KType{DEFAULT=0,DEFERRED=1};
		KType type;
		virtual void input(std::string id, float* value) = 0;
		virtual void input(std::string id, int* value) = 0;
	};
}