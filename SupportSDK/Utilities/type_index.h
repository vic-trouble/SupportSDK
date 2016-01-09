#pragma once

#include <typeinfo>

namespace std
{
	class type_index {
	public:
		type_index(const type_info& rhs) { target = &rhs; }
		bool operator==(const type_index& rhs) const { return target->hash_code() == rhs.target->hash_code(); }
		bool operator!=(const type_index& rhs) const { return *target != *rhs.target; }
		bool operator< (const type_index& rhs) const { return target->before(*rhs.target); }
		bool operator<= (const type_index& rhs) const { return !rhs.target->before(*target); }
		bool operator> (const type_index& rhs) const { return rhs.target->before(*target); }
		bool operator>= (const type_index& rhs) const { return !target->before(*rhs.target); }
		size_t hash_code() const { return target->hash_code(); }
		const char* name() const { return target->name(); }
	private:
		const type_info* target;    //  exposition only.
		// Note that the use of a pointer here, rather than a reference,
		// means that the default copy constructor and assignment
		// operator will be provided and work as expected.
	};

	template<>
	struct hash< type_index > : public std::unary_function< type_index, size_t > {
		size_t operator()(type_index index) const { return index.hash_code(); }
	};
} // std