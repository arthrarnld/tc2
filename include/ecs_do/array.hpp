#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <algorithm>

template<typename T>
struct array
{
public:
	using value_type = T;
	using iterator = value_type*;
	using const_iterator = const value_type*;

	array(size_t cap = 10)
		: m_data(new value_type[cap])
		, m_cap(cap)
		, m_len(0)
	{

	}

	~array()
	{
		delete [] m_data;
	}

	void swap(unsigned i, unsigned j)
	{
		std::swap(m_data[i], m_data[j]);
	}

	void push_back(const value_type & v)
	{
		if(m_len == m_cap)
			expand();
		m_data[m_len] = v;
		++m_len;
	}

	template<typename ... Args>
	void emplace_back(Args && ... args)
	{
		if(m_len == m_cap)
			expand();
		new (m_data+m_len) value_type(args...);
		++m_len;
	}

	void remove(unsigned i)
	{
		if(i < m_len-1) // not last element
			swap(i, m_len-1);
		--m_len;
	}

	size_t size()
	{
		return m_len;
	}

	value_type & operator[](unsigned i)
	{
		return m_data[i];
	}

	value_type & front()
	{
		return m_data[0];
	}

	value_type & back()
	{
		return m_data[m_len-1];
	}

	iterator begin()
		{ return m_data; }
	iterator end()
		{ return m_data + m_len; }

	const_iterator begin() const
		{ return m_data; }
	const_iterator end() const
		{ return m_data + m_len; }


private:
	value_type * m_data;
	size_t m_cap;
	size_t m_len;

	void expand()
	{
		value_type * new_buf = new value_type[m_cap*2];
		std::copy(m_data, m_data + m_cap, new_buf);
		delete [] m_data;
		m_data = new_buf;
		m_cap *= 2;
	}
};

#endif // ARRAY_HPP
