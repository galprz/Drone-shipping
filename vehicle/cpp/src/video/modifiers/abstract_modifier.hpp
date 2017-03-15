#ifndef abstract_modifier_hpp
#define abstract_modifier_hpp
#include <opencv2/core/mat.hpp>

namespace VehicleModule{
	namespace Video{
		namespace Modifiers{
			class AbstractModifier
			{
			public:
				//apply the modification on the image
				virtual void apply(cv::Mat & image) = 0;
			};
		};
	};
};

#endif
