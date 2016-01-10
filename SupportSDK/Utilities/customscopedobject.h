#ifndef	__UTILITIES_CUSTOMSCOPED_OBJECT_H__
#define __UTILITIES_CUSTOMSCOPED_OBJECT_H__

/*
Two types of usage exist. Suppose we need push matrix and then pop it
struct GlMatrix {}; // just tag to know what type of specialized template to use
1. AdditionalData is void:
	template <>
	struct ScopedHelper <GlMatrix>
	{
		static bool Push()
		{
			Core::GetRenderer()->PushMatrix();
			return true;
		}

		static void Pop(bool)
		{
			Core::GetRenderer()->PopMatrix();
		}
	};
	Somewhere in code:
	CustomScopedObject<GlMatrix> scopedMatrix;

2. AdditionalData = IRenderer*
	template <>
	struct ScopedHelper <GlMatrix, IRenderer*>
	{
		static bool Push(IRenderer* ip_renderer)
		{
			ip_renderer->PushMatrix();
			return true;
		}

		static void Pop(IRenderer* ip_renderer, bool)
		{
			ip_renderer->PopMatrix();
		}
	};

	Somewhere in code:
	CustomScopedObject<GlMatrix, IRenderer*> scopedMatrix(p_renderer);

	
*/

template <typename T, typename AdditionalData = void, typename PushResult = bool>
struct ScopedHelper
{
	static PushResult Push(AdditionalData);
	static void Pop(PushResult, AdditionalData);
};

template<typename T, typename AdditionalData = void, typename PushResult = bool, class Enable = void>
class CustomScopedObject;

template <typename T, typename AdditionalData, typename PushResult>
struct CustomScopedObject < T, AdditionalData, PushResult, typename std::enable_if< std::is_void<AdditionalData>::value>::type >
{
	PushResult m_result;

	CustomScopedObject()
	{
		m_result = ScopedHelper<T, AdditionalData>::Push();
	}

	~CustomScopedObject()
	{
		ScopedHelper<T, AdditionalData>::Pop(m_result);
	}
};

template <typename T, typename AdditionalData, typename PushResult>
struct CustomScopedObject < T, AdditionalData, PushResult, typename std::enable_if< !std::is_void<AdditionalData>::value>::type >
{
	AdditionalData m_data;
	PushResult m_result;
	CustomScopedObject(AdditionalData i_data)
		: m_data(i_data)
	{
		m_result = ScopedHelper<T, AdditionalData, PushResult>::Push(m_data);
	}

	~CustomScopedObject()
	{
		ScopedHelper<T, AdditionalData, PushResult>::Pop(m_result, m_data);
	}
};

#endif
