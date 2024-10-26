#pragma once
template<class T, class U>
struct Pair
{
private:
	T _first;
	U _second;
public:
	Pair(const T& first, const U& second);
	Pair() = default;
	const T& getFirst() const;
	const U& getSecond() const;

	void setFirst(const T& to);
	void setSecond(const U& to);
};

template<class T, class U>
Pair<T, U>::Pair(const T& first, const U& second) : _first(first), _second(second) {}


template<class T, class U>
const T& Pair<T, U>::getFirst() const
{
	return _first;
}

template<class T, class U>
const U& Pair<T, U>::getSecond() const
{
	return _second;
}

template<class T, class U>
void Pair<T, U>::setFirst(const T& to)
{
	_first = to;
}

template<class T, class U>
void Pair<T, U>::setSecond(const U& to)
{
	_second = to;
}