/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef  _ICOMPARER_H
#define  _ICOMPARER_H

// Would be cool to use I prefix for interfaces, like C# convention
template <class T>
class IComparer {
public:
	virtual bool compare(const T& left, const T& right) = 0;
};

template <class T>
class DefaultComparer : public IComparer<T> {
	bool compare(const T& left, const T& right) override;
};

template <class T>
bool DefaultComparer<T>::compare(const T& left, const T& right) {
	return left < right;
}
#endif  //_ICOMPARER_H
