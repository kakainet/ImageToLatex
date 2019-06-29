/*
 * Copyright (c) 2013 Björn Aili
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 */
#ifndef FTL_BASIC_CONCEPTS_H
#define FTL_BASIC_CONCEPTS_H

#include <type_traits>

namespace ftl {
	/**
	 * \defgroup concepts_basic Basic Concepts
	 *
	 * Module containg definitions and checks for various basic concepts.
	 *
	 * \code
	 *   #include <ftl/concepts/basic.h>
	 * \endcode
	 *
	 * \par Dependencies
	 * - `<type_traits>`
	 */

	/**
	 * \page defcons DefaultConstructible
	 *
	 * Any type that has a default constructor.
	 *
	 * This includes types that have an implicit default constructor (by either
	 * not declaring any of the standard constructors, or declaring it as
	 * `default`).
	 *
	 * More formally, the expressions
	 * - `T t;`
	 * - `T t{};`
	 * - `T{}`
	 * - `T()`
	 *
	 * must all be valid and behave as expected. Which is to say, they should
	 * construct an instance of `T` with whatever default semantics are
	 * appropriate.
	 */

	/**
	 * Predicate to check if a type is \ref defcons.
	 *
	 * \par Examples
	 *
	 * Using explicit `value` check:
	 * \code
	 *   template<typename T>
	 *   void foo() {
	 *       static_assert(
	 *           DefaultConstructible<T>::value,
	 *           "foo: T is not an instance of DefaultConstructible"
	 *       );
	 *
	 *       // Construct Ts using its default c-tor
	 *   }
	 * \endcode
	 *
	 * Using implicit bool conversion:
	 * \code
	 *   template<typename T>
	 *   void foo() {
	 *       static_assert(
	 *           DefaultConstructible<T>{},
	 *           "foo: T is not an instance of DefaultConstructible"
	 *       );
	 *
	 *       // Construct Ts using its default c-tor
	 *   }
	 * \endcode
	 *
	 * \ingroup concepts_basic
	 */
	template<typename T>
	struct DefaultConstructible {
		static constexpr bool value = std::is_default_constructible<T>::value;

		constexpr operator bool() const noexcept {
			return value;
		}
	};

	/**
	 * \page movecons MoveConstructible
	 *
	 * Any type that has a move constructor.
	 *
	 * This includes types that have an implicit move constructor (by either not
	 * declaring any of the standard constructors, or declaring it as
	 * `default`).
	 *
	 * More formally, the expressions
	 * - `T t = rv;`
	 * - `T(rv);`
	 *
	 * where `rv` is an rvalue reference of `T` must both be valid and behave
	 * as expected.
	 */

	/**
	 * Predicate to check if a type is \ref movecons.
	 *
	 * \par Examples
	 *
	 * Using explicit `value` member and SFINAE:
	 * \code
	 *   template<typename T, typename = Requires<MoveConstructible<T>::value>>
	 *   void foo() {
	 *       // Consturct Ts using the move c-tor
	 *   }
	 * \endcode
	 *
	 * Using implicit bool conversion:
	 * \code
	 *   template<typename T>
	 *   void foo() {
	 *       static_assert(
	 *           MoveConstructible<T>{},
	 *           "foo: T is not an instance of MoveConstructible"
	 *       );
	 *
	 *       // Construct Ts using its move c-tor
	 *   }
	 * \endcode
	 *
	 * \ingroup concepts_basic
	 */
	template<typename T>
	struct MoveConstructible {
		static constexpr bool value = std::is_move_constructible<T>::value;

		constexpr operator bool() const noexcept {
			return value;
		}
	};

	/**
	 * \page copycons CopyConstructible
	 *
	 * Any type that has a copy constructor.
	 *
	 * This includes types that have an implicit copy constructor (by either not
	 * declaring any of the standard constructors, or declaring it as
	 * `default`).
	 *
	 * More formally, the expressions
	 * - `T t = v;`
	 * - `T(v);`
	 *
	 * where `v` is an instance of `T` must both be valid and result in objects
	 * that are equivalent of `v`, while leaving it completely unmodified.
	 */

	/**
	 * Predicate to check if a type is \ref copycons.
	 *
	 * \par Examples
	 *
	 * Using explicit `value` member and SFINAE:
	 * \code
	 *   template<typename T, typename = Requires<CopyConstructible<T>::value>>
	 *   void foo() {
	 *       // Consturct Ts using the copy c-tor
	 *   }
	 * \endcode
	 *
	 * Using implicit bool conversion:
	 * \code
	 *   template<typename T>
	 *   void foo() {
	 *       static_assert(
	 *           CopyConstructible<T>{},
	 *           "foo: T is not an instance of CopyConstructible"
	 *       );
	 *
	 *       // Construct Ts using its copy c-tor
	 *   }
	 * \endcode
	 *
	 *
	 * \ingroup concepts_basic
	 */
	template<typename T>
	struct CopyConstructible {
		static constexpr bool value = std::is_copy_constructible<T>::value;

		constexpr operator bool() const noexcept {
			return value;
		}
	};

	/**
	 * \page moveassignable MoveAssignable
	 *
	 * Types that can be move assigned to.
	 *
	 * Requires that the expression
	 * - `a = rv;`
	 *
	 * where `rv` is an rvalue reference of `T` is valid. After the operation,
	 * `a` must be equivalent of whatever `rv` was _before_ it. `rv` may be
	 * changed by the operation.
	 */

	/**
	 * Predicate to check if a type is \ref moveassignable.
	 *
	 * \par Examples
	 *
	 * Using implicit bool conversion:
	 * \code
	 *   template<typename T>
	 *   void foo() {
	 *       static_assert(
	 *           MoveAssignable<T>{},
	 *           "foo: T is not an instance of MoveAssignable"
	 *       );
	 *
	 *       // Assign rvalues to Ts
	 *   }
	 * \endcode
	 *
	 * Using explicit `value`:
	 * \code
	 *   template<typename T>
	 *   void foo() {
	 *       static_assert(
	 *           MoveAssignable<T>::value,
	 *           "foo: T is not an instance of MoveAssignable"
	 *       );
	 *
	 *       // Assign rvalues to Ts
	 *   }
	 * \endcode
	 *
	 * \ingroup concepts_basic
	 */
	template<typename T>
	struct MoveAssignable {
		static constexpr bool value = std::is_move_assignable<T>::value;

		constexpr operator bool() const noexcept {
			return value;
		}
	};

	/**
	 * \page copyassignable CopyAssignable
	 *
	 * Types that can be copy assigned to.
	 *
	 * Requires that the expression
	 * - `a = v;`
	 *
	 * where `v` is an instance of `T` is valid. After the operation, `a`
	 * must be equivalent of `v`, while leaving the latter completely
	 * unmodified.
	 */

	/**
	 * Predicate to check if a type is \ref copyassignable.
	 *
	 * \par Examples
	 *
	 * Using explicit `value` and SFINAE
	 * \code
	 *   template<typename T, Requires<CopyAssignable<T>::value>
	 *   void foo() {
	 *       // Assign lvalues to Ts
	 *   }
	 * \endcode
	 *
	 * Using implicit bool conversion:
	 * \code
	 *   template<typename T>
	 *   void foo() {
	 *       static_assert(
	 *           CopyAssignable<T>{},
	 *           "foo: T is not an instance of CopyAssignable"
	 *       );
	 *
	 *       // Assign lvalues to Ts
	 *   }
	 * \endcode
	 *
	 * \ingroup concepts_basic
	 */
	template<typename T>
	struct CopyAssignable {
		static constexpr bool value = std::is_copy_assignable<T>::value;

		constexpr operator bool() const noexcept {
			return value;
		}
	};

	/**
	 * \page destructible Destructible
	 *
	 * Types that can be destructed.
	 *
	 * The expression
	 * - `t.~T();`
	 *
	 * must be valid and result in all resources currently held exclusively by
	 * `t` being freed. No exception must be thrown. Accessing members of `t`
	 * after the destructor has been called may result in undefined or illegal
	 * behaviour.
	 */

	/**
	 * Predicate to check if a type is \ref destructible.
	 *
	 * \ingroup concepts_basic
	 */
	template<typename T>
	struct Destructible {
		static constexpr bool value = std::is_destructible<T>::value;

		constexpr operator bool() const noexcept {
			return value;
		}
	};

	/**
	 * Check that a compile-time predicate holds for an arbitrary list of types.
	 *
	 * \tparam Pred must contain a static, compile time member named `value`, of
	 *              type `bool` or one that is convertible to it.
	 *
	 * \par Examples
	 *
	 * Statically enforce Eq on a variadic type list:
	 * \code
	 *   template<typename...Ts>
	 *   void example() {
	 *       static_assert(All<Eq,Ts...>{}, "All types in Ts must satisfy Eq");
	 *   }
	 * \endcode
	 *
	 * \ingroup concepts_basic
	 */
	template<template<typename> class Pred, typename...Ts>
	struct All;

	template<template<typename> class Pred>
	struct All<Pred> {
		static constexpr bool value = true;

		constexpr operator bool() const noexcept {
			return true;
		}
	};

	template<template<typename> class Pred, typename T, typename...Ts>
	struct All<Pred, T, Ts...> {
		static constexpr bool value = Pred<T>::value && All<Pred,Ts...>::value;

		constexpr operator bool() const noexcept {
			return value;
		}
	};
}

#endif


