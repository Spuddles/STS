#pragma once

namespace STS
{
	template <class T>
	class RangeParam
	{
	public:
				RangeParam(T s, T e, T step);
				RangeParam() {};
		virtual ~RangeParam();

		void	reset();
		bool	step();
		size_t	getStepCount() const;
		T		get() const;

	private:
		T		m_start;
		T		m_end;
		T		m_step;
		size_t	m_count;
	};

	template <class T>
	RangeParam<T>::RangeParam(T s, T e, T step) :
		m_start(s), m_end(e), m_step(step), m_count(0)
	{
	}

	template <class T>
	RangeParam<T>::~RangeParam()
	{
	}

	template <class T>
	void RangeParam<T>::reset()
	{
		m_count = 0;
	}

	template <class T>
	bool RangeParam<T>::step()
	{
		m_count++;
		return (((m_count * m_step) + m_start) <= m_end);
	}

	template <class T>
	size_t RangeParam<T>::getStepCount() const
	{
		T steps = 1 + ((m_end - m_start) / m_step);

		return static_cast<size_t>(steps);
	}

	template <class T>
	T RangeParam<T>::get() const
	{
		return (m_start + (m_step*m_count));
	}
} // namespace STS