#pragma once
#include <fstream>
#include <list>
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
#include <iostream>
namespace NGraphic {
	namespace NBuffer {
		struct ProbeParameter {

			Vector3 position;
			float c1,
				c2, c3, c4, c5,
				c6, c7, c8, c9;

		};
	}

}