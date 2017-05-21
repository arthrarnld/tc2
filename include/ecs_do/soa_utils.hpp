#ifndef SOA_UTILS_HPP
#define SOA_UTILS_HPP

#include <cstdint>
#include <cstring>
#include <vector>
#include <unordered_map>

#include "constants.hpp"
#include "entity.hpp"

// using handle = uint64_t;


#define SOA_COMPONENT_BASE(D)						\
private:											\
	size_t len, cap;								\
	soa::helper<D> helper;							\
	uint64_t owned_cap;								\
	friend class soa::helper<D>;					\
	size_t * owned;									\
public:												\
	uint64_t * owner;								\
	inline size_t create(uint64_t e)				\
		{ return helper.create(e); }				\
	inline void destroy(size_t index)				\
		{ return helper.destroy(index); }			\
	inline void kill(uint64_t e) {					\
		size_t idx = lookup(e);						\
		if(idx != nil)								\
			helper.destroy(idx);					\
	}												\
	inline size_t size()							\
		{ return len; }								\
	inline size_t lookup(uint64_t e)				\
		{ return helper.lookup(e); }


namespace soa
{
	template<typename T>
	struct helper
	{
	public:
		template<typename ... Ts>
		helper(T * o, size_t cap, Ts ** ... arrays)
			: arrays(sizeof...(Ts))
		{
			obj = o;
			o->len = 0;
			o->cap = cap;
			arrays_to_entries<0,sizeof...(Ts)>{}.go(*this, arrays...);

			obj->owner = new uint64_t[cap];

			obj->owned = new size_t[cap];
			for(int i = 0; i < cap; ++i)
				obj->owned[i] = nil;
			obj->owned_cap = cap;

			for(entry & e : this->arrays)
				*e.addr = new char[e.size*obj->cap];
		}

		~helper()
		{
			delete [] obj->owner;
			for(entry & e : arrays)
			{
				delete [] *e.addr;
			}
		}

		size_t create(uint64_t e)
		{
			size_t i = obj->len++;

			if(obj->len == obj->cap)
				grow();

			obj->owner[i] = e;

			uint64_t eidx = index(e);
			if(eidx >= obj->owned_cap)
				grow_owned(eidx);

			obj->owned[eidx] = i;

			return i;
		}

		void destroy(size_t idx)
		{
			uint64_t eidx = index(obj->owner[idx]);
			if(idx == obj->len-1) // last element
			{
				--obj->len;
				obj->owned[eidx] = nil;
				return;
			}

			uint64_t last_owner = obj->owner[obj->len-1];
			obj->owned[index(last_owner)] = idx;
			obj->owner[idx] = last_owner;
			for(entry & e : arrays)
			{
				char * start = *e.addr + ((obj->len - 1) * e.size);
				char * end = start + e.size;
				char * dst = *e.addr + (idx * e.size);
				std::copy(start, end, dst);
			}
			--obj->len;
		}

		inline size_t lookup(uint64_t e)
		{
			uint64_t idx = index(e);
			return idx > obj->owned_cap ? nil : obj->owned[idx];
		}

	private:
		struct entry
		{
			size_t size;
			char ** addr;
		};

		void grow()
		{
			size_t newcap = obj->cap*2;

			uint64_t * newowner = new uint64_t[newcap];
			std::copy(obj->owner, obj->owner+obj->len, newowner);
			delete [] obj->owner;
			obj->owner = newowner;

			for(entry & e : arrays)
			{
				char * newarr = new char[e.size*newcap];
				std::copy(*e.addr, *e.addr+(e.size*obj->cap), newarr);
				delete [] *e.addr;
				*e.addr = newarr;
			}

			obj->cap = newcap;
		}

		void grow_owned(uint64_t idx)
		{
			size_t newcap = idx < 2*obj->owned_cap ? 2*obj->owned_cap : idx+1;

			size_t * newowned = new size_t[newcap];
			std::copy(obj->owned, obj->owned+obj->owned_cap, newowned);
			delete [] obj->owned;

			for(int i = obj->owned_cap; i < newcap; ++i)
				newowned[i] = nil;

			obj->owned = newowned;
			obj->owned_cap = newcap;
		}

		template<size_t I, size_t S>
		struct arrays_to_entries
		{
			template<typename H, typename ... Ts>
			void go(helper & h, H ** head, Ts ** ... tail)
			{
				h.arrays[I] = entry{sizeof(H), (char**)head};
				*head = nullptr;
				arrays_to_entries<I+1, S-1>{}.go(h, tail...);
			}
		};

		template<size_t I>
		struct arrays_to_entries<I,1>
		{
			template<typename H>
			void go(helper & h, H ** head)
			{
				h.arrays[I] = entry{sizeof(H), (char**)head};
				*head = nullptr;
			}
		};

		T * obj;
		std::vector<entry> arrays;
	};
}

#endif // SOA_UTILS_HPP
