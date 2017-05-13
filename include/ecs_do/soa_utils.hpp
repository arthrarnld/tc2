#ifndef SOA_UTILS_HPP
#define SOA_UTILS_HPP

#include <cstdint>
#include <cstring>
#include <vector>
#include <unordered_map>

// using handle = uint64_t;


#define SOA_COMPONENT_BASE(D)						\
private:											\
	size_t len, cap;								\
	soa::helper<D> helper;							\
	friend class soa::helper<D>;					\
public:												\
	std::unordered_map<uint64_t, size_t> owned;		\
	uint64_t * owner;								\
	inline size_t create(uint64_t e)				\
		{ return helper.create(e); }				\
	inline void destroy(size_t index)				\
		{ return helper.destroy(index); }			\
	inline size_t size()							\
		{ return len; }


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

			// obj->deref = (size_t*) malloc(sizeof(size_t)*cap);
			obj->owner = new uint64_t[cap];

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
			size_t index = obj->len++;

			if(obj->len == obj->cap)
				grow();

			obj->owner[index] = e;
			obj->owned[e] = index;

			return index;
		}

		void destroy(size_t index)
		{
			if(index == obj->len-1) // last element
			{
				--obj->len;
				obj->owned.erase(obj->owner[index]);
				return;
			}

			uint64_t last_owner = obj->owner[obj->len-1];
			obj->owned[last_owner] = index;
			obj->owner[index] = last_owner;
			for(entry & e : arrays)
			{
				char * start = *e.addr + ((obj->len - 1) * e.size);
				char * end = start + e.size;
				char * dst = *e.addr + (index * e.size);
				std::copy(start, end, dst);
			}
			--obj->len;
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
