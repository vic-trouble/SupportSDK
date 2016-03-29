#ifndef	__GAMECORE_STREAM_H__
#define __GAMECORE_STREAM_H__

namespace SDK
{
	
	namespace FS
	{
		class Stream
		{
		public:
			virtual ~Stream(){}
			virtual std::istream& Get() = 0;
			virtual bool IsValid() const = 0;
		};

		typedef std::unique_ptr<Stream> StreamPtr;
	} // FS

} // SDK

#endif