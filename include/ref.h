#ifndef __REF_H__
#define __REF_H__

#include <cstring>
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args){
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename ... Args>
constexpr Ref<T> CastRef(Args&& ... args) {
	return std::static_pointer_cast<T>(std::forward<Args>(args)...);
}

#endif // __REF_H__