#ifndef VXL_COMPILER_DETECTION_H
#  error This file may only be included from vcl_compiler_detection.h
#endif

#define VXL_COMPILER_VERSION_MAJOR 0
#define VXL_COMPILER_VERSION_MINOR 0
#define VXL_COMPILER_VERSION_PATCH 0

// For unknown compilers, assume all or non C++11 support
// based solely on the value of __cplusplus

#if __cplusplus >= 201103L
# define VXL_COMPILER_CXX_AGGREGATE_DEFAULT_INITIALIZERS 1
# define VXL_COMPILER_CXX_ALIAS_TEMPLATES 1
# define VXL_COMPILER_CXX_ALIGNAS 1
# define VXL_COMPILER_CXX_ALIGNOF 1
# define VXL_COMPILER_CXX_ATTRIBUTES 1
# define VXL_COMPILER_CXX_ATTRIBUTE_DEPRECATED 1
# define VXL_COMPILER_CXX_AUTO_TYPE 1
# define VXL_COMPILER_CXX_BINARY_LITERALS 1
# define VXL_COMPILER_CXX_CONSTEXPR 1
# define VXL_COMPILER_CXX_CONTEXTUAL_CONVERSIONS 1
# define VXL_COMPILER_CXX_DECLTYPE_INCOMPLETE_RETURN_TYPES 1
# define VXL_COMPILER_CXX_DECLTYPE 1
# define VXL_COMPILER_CXX_DECLTYPE_AUTO 1
# define VXL_COMPILER_CXX_DEFAULT_FUNCTION_TEMPLATE_ARGS 1
# define VXL_COMPILER_CXX_DEFAULTED_FUNCTIONS 1
# define VXL_COMPILER_CXX_DEFAULTED_MOVE_INITIALIZERS 1
# define VXL_COMPILER_CXX_DELEGATING_CONSTRUCTORS 1
# define VXL_COMPILER_CXX_DELETED_FUNCTIONS 1
# define VXL_COMPILER_CXX_DIGIT_SEPARATORS 1
# define VXL_COMPILER_CXX_ENUM_FORWARD_DECLARATIONS 1
# define VXL_COMPILER_CXX_EXPLICIT_CONVERSIONS 1
# define VXL_COMPILER_CXX_EXTENDED_FRIEND_DECLARATIONS 1
# define VXL_COMPILER_CXX_EXTERN_TEMPLATES 1
# define VXL_COMPILER_CXX_FINAL 1
# define VXL_COMPILER_CXX_FUNC_IDENTIFIER 1
# define VXL_COMPILER_CXX_GENERALIZED_INITIALIZERS 1
# define VXL_COMPILER_CXX_GENERIC_LAMBDAS 1
# define VXL_COMPILER_CXX_INHERITING_CONSTRUCTORS 1
# define VXL_COMPILER_CXX_INLINE_NAMESPACES 1
# define VXL_COMPILER_CXX_LAMBDAS 1
# define VXL_COMPILER_CXX_LAMBDA_INIT_CAPTURES 1
# define VXL_COMPILER_CXX_LOCAL_TYPE_TEMPLATE_ARGS 1
# define VXL_COMPILER_CXX_LONG_LONG_TYPE 1
# define VXL_COMPILER_CXX_NOEXCEPT 1
# define VXL_COMPILER_CXX_NONSTATIC_MEMBER_INIT 1
# define VXL_COMPILER_CXX_NULLPTR 1
# define VXL_COMPILER_CXX_OVERRIDE 1
# define VXL_COMPILER_CXX_RANGE_FOR 1
# define VXL_COMPILER_CXX_RAW_STRING_LITERALS 1
# define VXL_COMPILER_CXX_REFERENCE_QUALIFIED_FUNCTIONS 1
# define VXL_COMPILER_CXX_RELAXED_CONSTEXPR 1
# define VXL_COMPILER_CXX_RETURN_TYPE_DEDUCTION 1
# define VXL_COMPILER_CXX_RIGHT_ANGLE_BRACKETS 1
# define VXL_COMPILER_CXX_RVALUE_REFERENCES 1
# define VXL_COMPILER_CXX_SIZEOF_MEMBER 1
# define VXL_COMPILER_CXX_STATIC_ASSERT 1
# define VXL_COMPILER_CXX_STRONG_ENUMS 1
# define VXL_COMPILER_CXX_THREAD_LOCAL 1
# define VXL_COMPILER_CXX_TRAILING_RETURN_TYPES 1
# define VXL_COMPILER_CXX_UNICODE_LITERALS 1
# define VXL_COMPILER_CXX_UNIFORM_INITIALIZATION 1
# define VXL_COMPILER_CXX_UNRESTRICTED_UNIONS 1
# define VXL_COMPILER_CXX_USER_LITERALS 1
# define VXL_COMPILER_CXX_VARIABLE_TEMPLATES 1
# define VXL_COMPILER_CXX_VARIADIC_MACROS 1
# define VXL_COMPILER_CXX_VARIADIC_TEMPLATES 1
# define VXL_COMPILER_CXX_TEMPLATE_TEMPLATE_PARAMETERS 1

#else
// No C++11 support
# define VXL_COMPILER_CXX_AGGREGATE_DEFAULT_INITIALIZERS 0
# define VXL_COMPILER_CXX_ALIAS_TEMPLATES 0
# define VXL_COMPILER_CXX_ALIGNAS 0
# define VXL_COMPILER_CXX_ALIGNOF 0
# define VXL_COMPILER_CXX_ATTRIBUTES 0
# define VXL_COMPILER_CXX_ATTRIBUTE_DEPRECATED 0
# define VXL_COMPILER_CXX_AUTO_TYPE 0
# define VXL_COMPILER_CXX_BINARY_LITERALS 0
# define VXL_COMPILER_CXX_CONSTEXPR 0
# define VXL_COMPILER_CXX_CONTEXTUAL_CONVERSIONS 0
# define VXL_COMPILER_CXX_DECLTYPE_INCOMPLETE_RETURN_TYPES 0
# define VXL_COMPILER_CXX_DECLTYPE 0
# define VXL_COMPILER_CXX_DECLTYPE_AUTO 0
# define VXL_COMPILER_CXX_DEFAULT_FUNCTION_TEMPLATE_ARGS 0
# define VXL_COMPILER_CXX_DEFAULTED_FUNCTIONS 0
# define VXL_COMPILER_CXX_DEFAULTED_MOVE_INITIALIZERS 0
# define VXL_COMPILER_CXX_DELEGATING_CONSTRUCTORS 0
# define VXL_COMPILER_CXX_DELETED_FUNCTIONS 0
# define VXL_COMPILER_CXX_DIGIT_SEPARATORS 0
# define VXL_COMPILER_CXX_ENUM_FORWARD_DECLARATIONS 0
# define VXL_COMPILER_CXX_EXPLICIT_CONVERSIONS 0
# define VXL_COMPILER_CXX_EXTENDED_FRIEND_DECLARATIONS 0
# define VXL_COMPILER_CXX_EXTERN_TEMPLATES 0
# define VXL_COMPILER_CXX_FINAL 0
# define VXL_COMPILER_CXX_FUNC_IDENTIFIER 0
# define VXL_COMPILER_CXX_GENERALIZED_INITIALIZERS 0
# define VXL_COMPILER_CXX_GENERIC_LAMBDAS 0
# define VXL_COMPILER_CXX_INHERITING_CONSTRUCTORS 0
# define VXL_COMPILER_CXX_INLINE_NAMESPACES 0
# define VXL_COMPILER_CXX_LAMBDAS 0
# define VXL_COMPILER_CXX_LAMBDA_INIT_CAPTURES 0
# define VXL_COMPILER_CXX_LOCAL_TYPE_TEMPLATE_ARGS 0
# define VXL_COMPILER_CXX_LONG_LONG_TYPE 0
# define VXL_COMPILER_CXX_NOEXCEPT 0
# define VXL_COMPILER_CXX_NONSTATIC_MEMBER_INIT 0
# define VXL_COMPILER_CXX_NULLPTR 0
# define VXL_COMPILER_CXX_OVERRIDE 0
# define VXL_COMPILER_CXX_RANGE_FOR 0
# define VXL_COMPILER_CXX_RAW_STRING_LITERALS 0
# define VXL_COMPILER_CXX_REFERENCE_QUALIFIED_FUNCTIONS 0
# define VXL_COMPILER_CXX_RELAXED_CONSTEXPR 0
# define VXL_COMPILER_CXX_RETURN_TYPE_DEDUCTION 0
# define VXL_COMPILER_CXX_RIGHT_ANGLE_BRACKETS 0
# define VXL_COMPILER_CXX_RVALUE_REFERENCES 0
# define VXL_COMPILER_CXX_SIZEOF_MEMBER 0
# define VXL_COMPILER_CXX_STATIC_ASSERT 0
# define VXL_COMPILER_CXX_STRONG_ENUMS 0
# define VXL_COMPILER_CXX_THREAD_LOCAL 0
# define VXL_COMPILER_CXX_TRAILING_RETURN_TYPES 0
# define VXL_COMPILER_CXX_UNICODE_LITERALS 0
# define VXL_COMPILER_CXX_UNIFORM_INITIALIZATION 0
# define VXL_COMPILER_CXX_UNRESTRICTED_UNIONS 0
# define VXL_COMPILER_CXX_USER_LITERALS 0
# define VXL_COMPILER_CXX_VARIABLE_TEMPLATES 0
# define VXL_COMPILER_CXX_VARIADIC_MACROS 0
# define VXL_COMPILER_CXX_VARIADIC_TEMPLATES 0
# define VXL_COMPILER_CXX_TEMPLATE_TEMPLATE_PARAMETERS 0
#endif
