#ifndef __DECISIONMAKING_EXCEPTIONS_H__
#define __DECISIONMAKING_EXCEPTIONS_H__

#include <exception>

namespace SDK
	{

	namespace DecisionMaking
		{

		class DecisionMakingException : public std::exception
			{
			public:
				DecisionMakingException(const std::string& i_message = std::string())
					: exception(i_message.c_str())
					{}
			};

		class NotFoundException : public DecisionMakingException
			{
			public:
				NotFoundException(const std::string& i_message)
					: DecisionMakingException(i_message.c_str())
					{}
			};

		class RootNodeAlreadyExist : public DecisionMakingException
			{
			public:
				RootNodeAlreadyExist()
					: DecisionMakingException()
					{}
			};

		class NodeDoesNotExistInTree : public DecisionMakingException
			{
			public:
				NodeDoesNotExistInTree()
					: DecisionMakingException()
					{}
			};

		class RootNodeIsNull : public DecisionMakingException
			{
			public:
				RootNodeIsNull()
					: DecisionMakingException()
					{}
			};

		} // DecisionMaking

	} // SDK

#endif