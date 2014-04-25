#ifndef ABB_BASE_DEFINE_HPP_
#define ABB_BASE_DEFINE_HPP_


#define ABB_BASE_DISALLOW_COPY_AND_ASSIGN(TypeName) \
	private:\
  	TypeName(const TypeName&);               \
  	TypeName& operator=(const TypeName&)

#endif
