#pragma once
#include<algorithm>
template<typename T>
class Vector
{
private:
	T* _Myfirst,*_Mylast,* _Myend;
	size_t _Max = 1000000;
	size_t _Calculate_growth(size_t _Newsize)
	{
		size_t _Oldcapacity = _Myend - _Myfirst;
		if (_Oldcapacity > _Max - _Oldcapacity / 2) return _Max;
		size_t _Newcapacity = _Oldcapacity + _Oldcapacity / 2;
		if (_Newcapacity < _Newsize) return _Newsize;
		return _Newcapacity;
	}
	template<typename _It>
	T* _Move_forward(T*_Dst, _It _First,_It _Last) 
	{
		while (_First != _Last)
		{
			*(_Dst++) = *(_First++);
		}
		return _Dst;
	}
	T* _Move_backward(T* _Dst, T* _First, T* _Last)
	{
		while (_Last != _First)
		{
			*(_Dst--) = *(_Last--);
		}
		return _Dst;
	}

public:
	Vector():_Myfirst(nullptr),_Mylast(nullptr),_Myend(nullptr) {};
	template<typename _It>
	Vector(_It _First,_It _Last) 
	{
		if (_First == _Last) return;
		size_t _Size = (_Last - _First);	
		_Myfirst = new T[_Size];
		_Mylast = _Myend = _Myfirst + _Size;
		for (size_t i = 0; i < _Size; i++)
			_Myfirst[i] = *_First++;
	};
	void push_back(const T& _Newele)
	{
		if (_Mylast == _Myend)
		{
			size_t _Oldsize = _Mylast - _Myfirst;
			size_t _Newcapacity = _Calculate_growth(_Oldsize + 1);
			T* _Oldfirst = _Myfirst, *_Oldlast = _Mylast;
			_Myfirst = new T[_Newcapacity];
			_Myend = _Myfirst + _Newcapacity;
			_Mylast = _Move_forward(_Myfirst, _Oldfirst, _Oldlast);
			*(_Mylast++) = _Newele;
			delete[] _Oldfirst;
			return;
		}
		*(_Mylast++) = _Newele;
		return;
	}

	template<typename _It>
	T* insert1(size_t _Pos, _It _First, _It _Last)
	{
		size_t _Oldsize = _Mylast - _Myfirst;
		while (_First != _Last)
		{
			push_back(*_First);
			_First++;
		}
		T* _Whereoff = _Myfirst + _Pos;
		std::rotate(_Whereoff, _Myfirst + _Oldsize, _Mylast);
		return _Whereoff;
	}

	template<typename _It>
	T* insert2(size_t _Pos, _It _First, _It _Last)
	{
		size_t _Count = _Last - _First;
		size_t _Oldsize = _Mylast - _Myfirst;
		size_t _Oldcapacity = _Myend - _Myfirst;
		size_t _Newsize = _Oldsize + _Count;
		T* _Oldlast = _Mylast	,*_Whereoff = _Myfirst + _Pos;
		if (_Oldcapacity < _Newsize)
		{
			size_t _Newcapacity = _Calculate_growth(_Newsize);
			T* _Oldfirst = _Myfirst;
			_Myfirst = new T[_Newcapacity];
			_Myend = _Myfirst + _Newcapacity;
			T* _Result = _Move_forward(_Myfirst, _Oldfirst, _Whereoff);
			_Mylast = _Move_forward(_Result, _First, _Last);
			_Mylast = _Move_forward(_Mylast, _Whereoff, _Oldlast);
			delete[] _Oldfirst;
			return _Result;
		}
		_Mylast = _Myfirst + _Newsize;
		_Move_backward(_Mylast,_Whereoff,_Oldlast);
		_Move_forward(_Whereoff, _First, _Last);
		return  _Whereoff;
	}

	T& operator[](int _Elem) { if (_Elem < (_Mylast - _Myfirst)) return *(_Myfirst + _Elem); else abort(); }
};
