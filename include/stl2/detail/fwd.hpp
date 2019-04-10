// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Casey Carter 2015-2016
//  Copyright Eric Niebler 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
#ifndef STL2_DETAIL_FWD_HPP
#define STL2_DETAIL_FWD_HPP

#include <type_traits>
#include <utility>
#include <meta/meta.hpp>

#ifdef __clang__
 #define STL2_HAS_BUILTIN(X) __has_builtin(__builtin_ ## X)
#else // __clang__
 #define STL2_HAS_BUILTIN(X) STL2_HAS_BUILTIN_ ## X
 #if defined(__GNUC__)
  #define STL2_HAS_BUILTIN_unreachable 1
 #endif // __GNUC__
#endif // __clang__

#ifndef STL2_WORKAROUND_GCC_69096
 #if defined(__GNUC__) && __GNUC__ >= 6
  // Return type deduction performed *before* checking constraints.
  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=69096
  #define STL2_WORKAROUND_GCC_69096 1
 #else
  #define STL2_WORKAROUND_GCC_69096 0
 #endif
#endif

#ifndef STL2_WORKAROUND_GCC_79591
 #if defined(__GNUC__) && __GNUC__ >= 6
  // Overloading function template declarations that differ only in their
  // associated constraints does not work properly when at least one declaration
  // is imported with a using declaration.
  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=79591
  #define STL2_WORKAROUND_GCC_79591 1
 #else
  #define STL2_WORKAROUND_GCC_79591 0
 #endif
#endif

#ifndef STL2_WORKAROUND_CLANG_UNKNOWN1
 #if defined(__clang__) && __clang_major < 7
  // Rejects-valid with CTAD nested in parens.
  #define STL2_WORKAROUND_CLANG_UNKNOWN1 1
 #else
  #define STL2_WORKAROUND_CLANG_UNKNOWN1 0
 #endif
#endif

#ifndef STL2_WORKAROUND_CLANG_37556
 #ifdef __clang__ // Workaround https://bugs.llvm.org/show_bug.cgi?id=37556
  // Improper symbol redefinition diagnostic for names in different declarative regions
  #define STL2_WORKAROUND_CLANG_37556 1
 #else
  #define STL2_WORKAROUND_CLANG_37556 0
 #endif
#endif

#ifndef STL2_WORKAROUND_CLANG_40150
 #ifdef __clang__ // Workaround https://bugs.llvm.org/show_bug.cgi?id=40150
  // deleted friend of template class conflicts with its own definition
  #define STL2_WORKAROUND_CLANG_40150 1
 #else
  #define STL2_WORKAROUND_CLANG_40150 0
 #endif
#endif

#ifndef STL2_WORKAROUND_CLANGC_42
 #if defined(__clang__)
  // Failure to short-circuit with constrained-parameter syntax
  // https://github.com/saarraz/clang-concepts/issues/42
  #define STL2_WORKAROUND_CLANGC_42 1
 #else
  #define STL2_WORKAROUND_CLANGC_42 0
 #endif
#endif

#define STL2_OPEN_NAMESPACE \
	namespace std { namespace experimental { namespace ranges { inline namespace v1
#define STL2_CLOSE_NAMESPACE }}}

// General namespace structure:
STL2_OPEN_NAMESPACE {
	namespace detail {
		// Implementation details, not to be accessed by user code.
	}
	namespace ext {
		// Supported extensions beyond what is specified in C++ and
		// the Ranges proposal, acceptable for user code to access.
	}
	inline namespace __cpos {
		// Customization point objects, whose names would otherwise
		// clash with hidden friend functions if they were declared
		// directly in the stl2 namespace.
	}
} STL2_CLOSE_NAMESPACE

// Used to qualify STL2 names
namespace __stl2 = ::std::experimental::ranges;

#if STL2_WORKAROUND_CLANG_UNKNOWN1
#define STL2_NOEXCEPT_RETURN(...) \
	noexcept(noexcept(__VA_ARGS__)) \
	{ return __VA_ARGS__; }

#define STL2_REQUIRES_RETURN(...) \
	requires requires { __VA_ARGS__; } \
	{ return __VA_ARGS__; }

#define STL2_NOEXCEPT_REQUIRES_RETURN(...) \
	noexcept(noexcept(__VA_ARGS__)) \
	requires requires { __VA_ARGS__; } \
	{ return __VA_ARGS__; }
#else
#define STL2_NOEXCEPT_RETURN(...) \
	noexcept(noexcept(__VA_ARGS__)) \
	{ return (__VA_ARGS__); }

#define STL2_REQUIRES_RETURN(...) \
	requires requires { __VA_ARGS__; } \
	{ return (__VA_ARGS__); }

#define STL2_NOEXCEPT_REQUIRES_RETURN(...) \
	noexcept(noexcept(__VA_ARGS__)) \
	requires requires { __VA_ARGS__; } \
	{ return (__VA_ARGS__); }
#endif

#ifndef STL2_ASSERT
 #ifdef NDEBUG
  #define STL2_ASSERT(...) void(0)
 #else
  #include <cassert>
  #define STL2_ASSERT(...) assert((__VA_ARGS__))
 #endif
#endif

#ifndef STL2_EXPENSIVE_ASSERT
 #ifdef STL2_USE_EXPENSIVE_ASSERTS
  #define STL2_EXPENSIVE_ASSERT(...) STL2_ASSERT(__VA_ARGS__)
 #else
  #define STL2_EXPENSIVE_ASSERT(...) void(0)
 #endif
#endif

#ifndef STL2_ASSUME
 #if STL2_HAS_BUILTIN(assume)
  #define STL2_ASSUME(...) __builtin_assume(__VA_ARGS__)
 #elif STL2_HAS_BUILTIN(unreachable)
  // Tell the compiler to optimize on the assumption that the condition holds.
  #define STL2_ASSUME(...) ((__VA_ARGS__) ? void(0) : __builtin_unreachable())
 #else
  #define STL2_ASSUME(...) void(0)
 #endif
#endif

#ifndef STL2_EXPECT
 #ifdef NDEBUG
  #define STL2_EXPECT(...) STL2_ASSUME(__VA_ARGS__)
 #else
  #define STL2_EXPECT(...) STL2_ASSERT(__VA_ARGS__)
 #endif
#endif

#ifdef META_HAS_P1084
#define STL2_RVALUE_REQ(...) __VA_ARGS__
#else // ^^^ Has P1084 / No P1084 vvv
#define STL2_RVALUE_REQ(...) __VA_ARGS__&&
#endif // Detect support for P1084

STL2_OPEN_NAMESPACE {
	namespace ext {
		// tags for manually specified overload ordering
		template<unsigned N>
		struct priority_tag : priority_tag<N - 1> {};
		template<>
		struct priority_tag<0> {};
		inline constexpr priority_tag<4> max_priority_tag{};
	}

	struct __niebloid {
		explicit __niebloid() = default;
		__niebloid(const __niebloid&) = delete;
		__niebloid& operator=(const __niebloid&) = delete;
	};
} STL2_CLOSE_NAMESPACE

#endif // STL2_DETAIL_FWD_HPP
