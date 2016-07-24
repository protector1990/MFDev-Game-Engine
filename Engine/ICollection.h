/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef  _ICOLLECTION_H
#define  _ICOLLECTION_H

// Would be cool to use I prefix for interfaces, like C# convention
// Break collection functionalities between classes and use multiple inheritance
// Examples would be: ICollection (only basic stuff), ISortedCollection, IMap, IHashed, ISet etc...
template <class T>
class ICollection {
public:
	virtual ~ICollection(){}
	virtual bool contains(T t) const = 0;
	virtual void insert(T t) = 0;
	virtual void mUnion(const ICollection<T>* other) = 0;
	virtual void append(const ICollection<T>* other) = 0;
	virtual T remove(T t) = 0;
	virtual size_t size() const = 0;
};

#endif  //_ICOLLECTION_H
