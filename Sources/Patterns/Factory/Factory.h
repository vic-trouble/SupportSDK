#ifndef	__PATTERNS_FACTORY_H__
#define __PATTERNS_FACTORY_H__

#include <memory>
#include <vector>
#include <algorithm>

#include <Utilities/HashFunctions.h>

namespace SDK
{
	template <typename BaseType, typename IdentifierType, typename ReturnType>
	struct DefaultErrorPolicy
	{
		static ReturnType OnUnknownType(const IdentifierType& i_id)
		{
			return ReturnType();
		}

		static void OnCreatorExist(const IdentifierType& i_id)
		{
			// TODO: what default behavior should be: exception? log?
			return;
		}

		static void OnWriterExist(size_t i_type_hash)
		{
			// TODO: what default behavior should be: exception? log?
			return;
		}

		static void OnUnknownWriteType(size_t i_type_hash)
		{
			return;
		}
	};

	template <
		typename BaseClass,
		typename IdentifierType,
		typename ReturnType = std::unique_ptr<BaseClass>,
		template <typename, typename, typename> class ErrorPolicy = DefaultErrorPolicy,
		size_t (*HashFunction)(const IdentifierType&) = &Utilities::hash_function<IdentifierType>,
		typename WriterObject = int,
		typename... CreatorParameters
	>
	class Factory
	{
	public:
		typedef ReturnType(*Creator)(CreatorParameters...);
		typedef ErrorPolicy<BaseClass, IdentifierType, ReturnType> _ErrorPolicy;
		
	private:
		struct Writer
		{
			virtual ~Writer() {}
			virtual void Write(WriterObject o_writer, const BaseClass& i_object) const = 0;
		};

	private:
		typedef std::pair<size_t, Creator> ProductPair;
		std::vector<ProductPair> m_creators;

		using WriterPair = std::pair<size_t/*type hash*/, std::unique_ptr<Writer>>;
		std::vector<WriterPair> m_writers;

	private:
		const ProductPair* Find(size_t i_hash) const
		{
			const auto it = std::find_if(m_creators.begin(), m_creators.end(), [i_hash](const ProductPair& pp)
			{				
				return pp.first == i_hash;
			});
			if (it == m_creators.end())
				return nullptr;
			return &(*it);
		}

		const WriterPair* FindWriter(size_t i_hash) const
		{
			const auto it = std::find_if(m_writers.begin(), m_writers.end(), [i_hash](const WriterPair& pp)
			{
				return pp.first == i_hash;
			});
			if (it == m_writers.end())
				return nullptr;
			return &(*it);
		}

	private:
		template<typename T>
		struct has_gettype_method
		{
			template<typename U, size_t(U::*)()const> struct SFINAE {};
			template<typename U> static char Test(SFINAE<U, &U::GetType>*);
			template<typename U> static int Test(...);
			static constexpr bool Has = sizeof(Test<T>(0)) == sizeof(char);
		};

		size_t GetTypeHash(const BaseClass& i_object, std::true_type) const
		{
			return i_object.GetType();
		}

		size_t GetTypeHash(const BaseClass& i_object, std::false_type) const
		{
			return typeid(i_object).hash_code();
		}

		size_t GetTypeHash(const BaseClass& i_object) const
		{
			return GetTypeHash(i_object, std::integral_constant<bool, has_gettype_method<BaseClass>::Has>());
		}

	public:		
		void Register(const IdentifierType& i_id, Creator i_creator)
		{
			const size_t hash = HashFunction(i_id);
			if (Find(hash) != nullptr)
				return _ErrorPolicy::OnCreatorExist(i_id);
					
			m_creators.emplace_back(hash, i_creator);
			std::sort(m_creators.begin(), m_creators.end(), [](const ProductPair& left, const ProductPair& right)
			{
				return left.first < right.first;
			});
		}

		void Unregister(const IdentifierType& i_id)
		{
			const size_t hash = HashFunction(i_id);
			m_creators.erase(
				std::find_if(m_creators.begin(), m_creators.end(), [hash](const ProductPair& p_p)
					{return p_p.first == hash; }),
					m_creators.end()
				);
		}

		ReturnType Create(const IdentifierType& i_id, CreatorParameters... i_parameters) const
		{
			const size_t hash = HashFunction(i_id);
			const ProductPair* p_p = Find(hash);
			if (p_p == nullptr)
				return _ErrorPolicy::OnUnknownType(i_id);
			return p_p->second(std::forward<CreatorParameters>(i_parameters...)...);
		}


		template <typename Type>
		void Register(void(*WriterFunc)(WriterObject o_writer, const Type& i_object))
		{
			static const size_t type_hash = typeid(Type).hash_code();
			if (FindWriter(type_hash) != nullptr)
				return _ErrorPolicy::OnWriterExist(type_hash);

			struct WriterImpl : Writer
			{
				void(*WriterFunc)(WriterObject o_writer, const Type& i_object);
				WriterImpl(void(*func)(WriterObject o_writer, const Type& i_object))
					: WriterFunc(func)
				{}
				virtual void Write(WriterObject o_writer, const BaseClass& i_object) const
				{
					WriterFunc(o_writer, static_cast<const Type&>(i_object));
				}
			};
			m_writers.emplace_back(type_hash, std::unique_ptr<Writer>(new WriterImpl(WriterFunc)));
			std::sort(m_writers.begin(), m_writers.end(), [](const WriterPair& left, const WriterPair& right)
			{
				return left.first < right.first;
			});
		}

		void Unregister(size_t i_type_hash)
		{
			m_writers.erase(
				std::find_if(m_writers.begin(), m_writers.end(), [i_type_hash](const WriterPair& p_p)
			{return p_p.first == i_type_hash; }),
				m_writers.end()
				);
		}

		void Write(WriterObject o_writer, const BaseClass& i_object) const
		{
			const size_t type_hash = GetTypeHash(i_object);
			const WriterPair* p_p = FindWriter(type_hash);
			if (p_p == nullptr)
				return _ErrorPolicy::OnUnknownWriteType(type_hash);
			p_p->second->Write(o_writer, i_object);
		}

		template <typename Type>
		void Register(const IdentifierType& i_id, Creator i_creator, void(*WriterFunc)(WriterObject o_writer, const Type& i_object))
		{
			Register(i_id, i_creator);
			Register<Type>(WriterFunc);
		}
	};


} // SDK

#endif