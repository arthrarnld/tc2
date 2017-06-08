#ifndef SOA_UTILS_HPP
#define SOA_UTILS_HPP

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include "common/constants.hpp"
#include "common/util.hpp"
#include "entity.hpp"

template<size_t N>
void reset_partitions(size_t (&partitions)[N])
{
	for(int i = 0; i < N; ++i)
		partitions[i] = 0;
}


#define SOA_COMPONENT_COMMON(D)									\
	private:													\
		friend class soa::helper<D>;							\
		soa::helper<D> helper;									\
	public:														\
		uint64_t owned_cap;										\
		size_t len, cap;										\
		size_t * owned;											\
		uint64_t * owner;										\
		inline size_t create(uint64_t e)						\
			{ return helper.create(e); }						\
		inline size_t size()									\
			{ return len; }										\
		inline size_t lookup(uint64_t e)						\
			{ return helper.lookup(e); }						\
		inline void swap(size_t a, size_t b)					\
			{ helper.swap(a, b); }

#define SOA_COMPONENT_BASE(D)									\
	SOA_COMPONENT_COMMON(D)										\
	inline void reset_partitions() { }

#define SOA_PARTITIONED_COMPONENT_BASE(D)						\
	SOA_COMPONENT_COMMON(D)										\
	inline void reset_partitions()								\
		{ ::reset_partitions(partitions); }


namespace soa
{
	template<typename T>
	struct helper
	{
	public:
		template<typename ... Ts>
		void init(T * o, size_t cap, Ts ** ... arrays)
		{
			obj = o;
			o->len = 0;
			o->cap = cap;
			arrays_to_entries(*this, arrays...);

			obj->owner = new uint64_t[cap];

			obj->owned = new size_t[cap];
			std::fill(obj->owned, obj->owned+cap, nil);
			obj->owned_cap = cap;

			obj->reset_partitions();


			for(entry & e : this->arrays)
				*e.addr = new char[e.size*obj->cap];
		}


		helper() = default;
		helper(const helper &) = delete;
		helper & operator=(const helper &) = delete;

		~helper()
		{
			delete [] obj->owner;
			delete [] obj->owned;

			for(entry & e : arrays)
				delete [] *e.addr;
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

		// void destroy(size_t idx, size_t replacement_idx)
		// {
		// 	uint64_t eidx = index(obj->owner[idx]);
		// 	if(idx == obj->len-1) // last element
		// 	{
		// 		--obj->len;
		// 		obj->owned[eidx] = nil;
		//
		// 		return;
		// 	}
		//
		// 	uint64_t last_owner = obj->owner[obj->len-1];
		//
		// 	obj->owned[index(last_owner)] = idx;
		//
		// 	obj->owner[idx] = last_owner;
		// 	size_t ridx = replacement_idx == nil ? obj->len-1 : replacement_idx;
		// 	for(entry & e : arrays)
		// 	{
		// 		char * start = *e.addr + (ridx * e.size);
		// 		char * end = start + e.size;
		// 		char * dst = *e.addr + (idx * e.size);
		// 		std::copy(start, end, dst);
		// 	}
		// 	--obj->len;
		// }

		size_t lookup(uint64_t e)
		{
			uint64_t eidx = index(e);
			if(eidx >= obj->owned_cap)
				return nil;
			return obj->owned[eidx];
		}

		void swap(size_t a, size_t b)
		{
			if(a == b)
				return;

			size_t a_owner_idx = index(obj->owner[a]);
			A(a_owner_idx < obj->owned_cap && a_owner_idx != nil);
			obj->owned[a_owner_idx] = b;

			size_t b_owner_idx = index(obj->owner[b]);
			A(b_owner_idx < obj->owned_cap && b_owner_idx != nil);
			obj->owned[b_owner_idx] = a;
			// std::swap(obj->owned[A], obj->owned[B]);

			// std::swap(obj->owner[a], obj->owner[b]);
			uint64_t ow_a = obj->owner[a];
			obj->owner[a] = obj->owner[b];
			obj->owner[b] = ow_a;

			for(entry & e : arrays)
			{
				char * aaddr = *e.addr+(a*e.size);
				char * baddr = *e.addr+(b*e.size);
				char bucket[e.size];
				// a -> bucket
				memcpy(bucket, aaddr, e.size);
				// b -> a
				memcpy(aaddr, baddr, e.size);
				// bucket -> b
				memcpy(baddr, bucket, e.size);
			}
		}

		size_t get_entry_size(int i) {
			return arrays[i].size;
		}

	private:
		struct entry
		{
			size_t size;
			char ** addr;

			entry() : size(0), addr(nullptr) {}
			entry(size_t s, char ** a) : size(s), addr(a) {}
		};

		void grow()
		{
			size_t newcap = obj->cap*2;

			uint64_t * newowner = new uint64_t[newcap];
			memcpy(newowner, obj->owner, obj->len*sizeof(uint64_t));
			// std::copy(obj->owner, obj->owner+obj->len, newowner);
			delete [] obj->owner;
			obj->owner = newowner;

			for(entry & e : arrays)
			{
				char * newarr = new char[e.size*newcap];
				// std::copy(*e.addr, *e.addr+(e.size*obj->cap), newarr);
				memcpy(newarr, *e.addr, e.size*obj->cap);
				delete [] *e.addr;
				*e.addr = newarr;
			}

			obj->cap = newcap;
		}

		void grow_owned(uint64_t idx)
		{
			size_t newcap = idx < 2*obj->owned_cap ? 2*obj->owned_cap : idx+1;

			size_t * newowned = new size_t[newcap];
			memcpy(newowned, obj->owned, obj->owned_cap*sizeof(size_t));
			// std::copy(obj->owned, obj->owned+obj->owned_cap, newowned);
			delete [] obj->owned;

			for(int i = obj->owned_cap; i < newcap; ++i)
				newowned[i] = nil;

			obj->owned = newowned;
			obj->owned_cap = newcap;
		}

		template<typename H, typename ... Ts>
		void arrays_to_entries(helper & h, H ** head, Ts ** ... tail)
		{
			h.arrays.emplace_back(sizeof(H), (char**)head);
			*head = nullptr;
			arrays_to_entries(h, tail...);
		}

		template<typename H>
		void arrays_to_entries(helper & h, H ** head)
		{
			h.arrays.emplace_back(sizeof(H), (char**)head);
			*head = nullptr;
		}

		T * obj;
		std::vector<entry> arrays;
	};
}

#endif // SOA_UTILS_HPP
