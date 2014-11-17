#ifndef __ABB_BASE_DEFINE_HPP__
#define __ABB_BASE_DEFINE_HPP__


#define ABB_BASE_DISALLOW_COPY_AND_ASSIGN(TypeName) \
	private:\
  	TypeName(const TypeName&);               \
  	TypeName& operator=(const TypeName&)

#endif
