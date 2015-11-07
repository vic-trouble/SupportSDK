#ifndef __GAMECORE_COREDELEGATE_H__
#define __GAMECORE_COREDELEGATE_H__

namespace SDK
	{

	class CoreDelegate
		{
		public:
			virtual ~CoreDelegate(){}

			virtual void OnCreate() = 0;
			virtual void OnTerminate() = 0;

			virtual void OnCollapse() = 0;
			virtual void OnActivate() = 0;

			// // TODO: remove update and draw methods from delegate
			virtual void Update(float i_elapsed_time) = 0;
			virtual void Draw() = 0;
		};

	} // SDK

#endif