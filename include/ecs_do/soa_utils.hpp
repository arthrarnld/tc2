#ifndef SOA_UTILS_HPP
#define SOA_UTILS_HPP

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include "constants.hpp"
#include "entity.hpp"

// using handle = uint64_t;

#if defined(DO_LOOKUP_STD_UNORDERED_MAP)
	#include <unordered_map>
	#define OWNED_DECL std::unordered_map<uint64_t, size_t> owned;
#elif defined(DO_LOOKUP_ENTITY_ARRAY)
	#define OWNED_DECL		\
		uint64_t owned_cap;	\
		size_t * owned;
#else
	#error "No lookup macro set."
#endif

#define SOA_COMPONENT_BASE(D)									\
	private:													\
		friend class soa::helper<D>;							\
		size_t len, cap;										\
		soa::helper<D> helper;									\
		OWNED_DECL												\
	public:														\
		uint64_t * owner;										\
		inline size_t create(uint64_t e)						\
			{ return helper.create(e); }						\
		inline void destroy(size_t index, size_t rid = nil)		\
			{ return helper.destroy(index, rid); }				\
		inline void kill(uint64_t e, size_t rid = nil) {		\
			size_t idx = lookup(e);								\
			if(idx != nil)										\
				helper.destroy(idx, rid);						\
		}														\
		inline size_t size()									\
			{ return len; }										\
		inline size_t lookup(uint64_t e)						\
			{ return helper.lookup(e); }						\
		inline void swap(size_t a, size_t b)					\
			{ helper.swap(a, b); }

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

			#ifdef DO_LOOKUP_ENTITY_ARRAY
				obj->owned = new size_t[cap];
				for(int i = 0; i < cap; ++i)
					obj->owned[i] = nil;
				obj->owned_cap = cap;
			#endif

			for(entry & e : this->arrays)
				*e.addr = new char[e.size*obj->cap];
		}

		~helper()
		{
			delete [] obj->owner;
			#ifdef DO_LOOKUP_ENTITY_ARRAY
				delete [] obj->owned;
			#endif

			for(entry & e : arrays)
				delete [] *e.addr;
		}

		size_t create(uint64_t e)
		{
			size_t i = obj->len++;

			if(obj->len == obj->cap)
				grow();

			obj->owner[i] = e;


			#if defined(DO_LOOKUP_STD_UNORDERED_MAP)
				obj->owned[e] = i;
			#elif defined(DO_LOOKUP_ENTITY_ARRAY)
				uint64_t eidx = index(e);
				if(eidx >= obj->owned_cap)
					grow_owned(eidx);
				obj->owned[eidx] = i;
			#endif

			return i;
		}

		void destroy(size_t idx, size_t replacement_idx)
		{
			uint64_t eidx = index(obj->owner[idx]);
			if(idx == obj->len-1) // last element
			{
				--obj->len;
				#if defined(DO_LOOKUP_STD_UNORDERED_MAP)
					obj->owned[obj->owner[idx]] = nil;
				#elif defined(DO_LOOKUP_ENTITY_ARRAY)
					obj->owned[eidx] = nil;
				#endif

				return;
			}

			uint64_t last_owner = obj->owner[obj->len-1];

			#if defined(DO_LOOKUP_STD_UNORDERED_MAP)
				obj->owned[last_owner] = idx;
			#elif defined(DO_LOOKUP_ENTITY_ARRAY)
				obj->owned[index(last_owner)] = idx;
			#endif

			obj->owner[idx] = last_owner;
			size_t ridx = replacement_idx == nil ? obj->len-1 : replacement_idx;
			for(entry & e : arrays)
			{
				char * start = *e.addr + (ridx * e.size);
				char * end = start + e.size;
				char * dst = *e.addr + (idx * e.size);
				std::copy(start, end, dst);
			}
			--obj->len;
		}

		size_t lookup(uint64_t e)
		{
			#if defined(DO_LOOKUP_STD_UNORDERED_MAP)
				auto it = obj->owned.find(e);
				if(it == obj->owned.end())
					return nil;
				return it->second;
			#elif defined(DO_LOOKUP_ENTITY_ARRAY)
				uint64_t eidx = index(e);
				if(eidx >= obj->owned_cap)
					return nil;
				return obj->owned[eidx];
			#endif
		}

		void swap(size_t a, size_t b)
		{
			#if defined(DO_LOOKUP_STD_UNORDERED_MAP)
				uint64_t aowner = obj->owner[a];
				uint64_t bowner = obj->owner[b];
				obj->owned[aowner] = b;
				obj->owned[bowner] = a;
			#elif defined(DO_LOOKUP_ENTITY_ARRAY)
				std::swap(obj->owned[a], obj->owned[b]);
			#endif

			std::swap(obj->owner[a], obj->owner[b]);

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
				memcpy(&baddr, bucket, e.size);
			}
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

		#ifdef DO_LOOKUP_ENTITY_ARRAY
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
		#endif

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
