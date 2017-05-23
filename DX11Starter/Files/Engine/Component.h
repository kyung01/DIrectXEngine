#pragma once
#include <memory>
#include <map> 
#include <vector> 
#include <d3d11.h>
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

namespace KEngine {

	namespace KComponent {

		enum ComponentId {
		};
		enum ComponentType {
		};
	

		struct Component {
		private:
		protected:
		public:
		};

		struct ComponentIndex {
			//type_info type;
			ComponentId id; //what's my type?
			ComponentType type; //what's my type?
			int at; //where am I?
			int size; // how mnay bytes do I occupy here?
		};
	}


}