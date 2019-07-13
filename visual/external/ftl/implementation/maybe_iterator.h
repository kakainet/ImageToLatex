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
#ifndef FTL_MAYBE_ITERATOR_H
#define FTL_MAYBE_ITERATOR_H

#include <memory>

namespace ftl {
	template<typename...Ts>
	class sum_type;

	struct Nothing;

	template<typename T, typename...Ts>
	constexpr T& get(sum_type<Ts...>&);

	template<typename T, typename...Ts>
	constexpr const T& get(const sum_type<Ts...>&);

	namespace _dtl {

		template<typename T>
		class maybe_iterator
		: public std::iterator<std::forward_iterator_tag,T> {
		public:
			maybe_iterator() = default;
			maybe_iterator(const maybe_iterator&) = default;
			maybe_iterator(maybe_iterator&&) = default;
			~maybe_iterator() = default;

			explicit constexpr maybe_iterator(sum_type<T,Nothing>* m) noexcept
			: ref{m && m->template is<T>() ? m : nullptr} {}

			maybe_iterator& operator++ () noexcept {
				this->ref = nullptr;
				return *this;
			}

			maybe_iterator operator++ (int) noexcept {
				auto it = *this;
				this->ref = nullptr;
				return it;
			}

			constexpr T& operator* () const {
				return get<T>(*this->ref);
			}

			constexpr T* operator-> () const {
				return std::addressof(get<T>(*this->ref));
			}

			maybe_iterator& operator= (const maybe_iterator&) = default;
			maybe_iterator& operator= (maybe_iterator&&) = default;

			constexpr bool operator== (const maybe_iterator& m) const noexcept {
				return ref == m.ref;
			}

			constexpr bool operator!= (const maybe_iterator& m) const noexcept {
				return ref != m.ref;
			}

		private:
			sum_type<T,Nothing>* ref = nullptr;
		};

		template<typename T>
		class const_maybe_iterator
		: public std::iterator<std::forward_iterator_tag,T> {
		public:
			const_maybe_iterator() = default;
			const_maybe_iterator(const const_maybe_iterator&) = default;
			const_maybe_iterator(const_maybe_iterator&&) = default;
			~const_maybe_iterator() = default;

			explicit constexpr
			const_maybe_iterator(const sum_type<T,Nothing>* m) noexcept
			: ref{m && m->template is<T>() ? m : nullptr} {}

			const_maybe_iterator& operator++ () noexcept {
				this->ref = nullptr;
				return *this;
			}

			const_maybe_iterator operator++ (int) noexcept {
				auto it = *this;
				this->ref = nullptr;
				return it;
			}

			constexpr const T& operator* () const {
				return get<T>(*this->ref);
			}

			constexpr const T* operator-> () const {
				return std::addressof(get<T>(*this->ref));
			}

			const_maybe_iterator& operator= (const const_maybe_iterator&)
			   	= default;
			const_maybe_iterator& operator= (const_maybe_iterator&&) = default;
		
			constexpr bool operator== (const const_maybe_iterator& m)
			const noexcept {
				return ref == m.ref;
			}

			constexpr bool operator!= (const const_maybe_iterator& m)
			const noexcept {
				return ref != m.ref;
			}

		private:
			const sum_type<T,Nothing>* ref = nullptr;
		};
	}
}

#endif


