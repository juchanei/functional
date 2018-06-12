#pragma once

#include <iterator>

namespace fp {
	template<class InIt, class OutType, class Fn>
	class Mapped {
	public:
		typedef OutType value_type;
		Mapped(InIt begin, InIt end, Fn fn)
			: _first(begin), _last(end), _fn(fn) {
		}

		void operator++() {
			++_first;
		}

		typename OutType operator*() const {
			return _fn(*_first);
		}

		bool operator!=(const Mapped& param) const {
			return _first != _last;
		}

	private:
		InIt _first;
		InIt _last;
		Fn _fn;
	};

	template<class InIt, class Fn>
	class Filtered {
	public:
		Filtered(InIt begin, InIt end, Fn pred)
			: _current(begin), _begin(begin), _end(end), _pred(pred) {
		}

		void operator++() {
			++_current;
			while (_current != _end && !_pred(*_current)) ++_current;
		}

		typename InIt::value_type operator*() {
			if (!_pred(*_current)) this->operator++();
			return *_current;
		}

		bool operator!=(const Filtered&) {
			return _current != _end;
		}

	private:
		InIt _current;
		InIt _begin;
		InIt _end;
		Fn _pred;
	};

	template<class It>
	class Bound {
	public:
		Bound(It begin, It end) : _begin(begin), _end(end) {}
		It begin() { return _begin; }
		It end() { return _end; }

		template<class Fn>
		auto filter(Fn pred) {
			return make_bound(make_filter(_begin, _end, pred), make_filter(_end, _end, pred));
		}

		template<class Fn>
		auto map(Fn fn) {
			return make_bound(make_mapped(_begin, _end, fn), make_mapped(_end, _end, fn));
		}

		template<class Fn>
		void foreach(Fn fn) {
			auto first = _begin;
			for (; first != _end; ++first) {
				fn(*first);
			}
		}

	private:
		It _begin;
		It _end;
	};

	template<class InIt, class Fn>
	auto make_filter(InIt begin, InIt end, Fn pred) {
		return Filtered<InIt, Fn>(begin, end, pred);
	}

	template<class InIt, class Fn>
	auto make_mapped(InIt begin, InIt end, Fn fn) {
		typedef result_of<Fn(typename InIt::value_type)>::type return_type;
		return Mapped<InIt, return_type, Fn>(begin, end, fn);
	}

	template<class It>
	auto make_bound(It begin, It end) {
		return Bound<It>(begin, end);
	}

	template<class It>
	auto chain(It begin, It end) {
		return Bound<It>(begin, end);
	}

	template<class Collection>
	auto chain(Collection& col) {
		return make_bound(std::begin(col), std::end(col));
	}

	template<class InIt, class Fn>
	auto filter(InIt begin, InIt end, Fn pred) {
		return make_bound(make_filter(begin, end, pred), make_filter(end, end, pred));
	}

	template<class Collection, class Fn>
	auto filter(Collection& col, Fn pred) {
		return filter(std::begin(col), std::end(col), pred);
	}

	template<class InIt, class Fn>
	auto map(InIt begin, InIt end, Fn fn) {
		return make_bound(make_mapped(begin, end, fn), make_mapped(end, end, fn));
	}

	template<class Collection, class Fn>
	auto map(Collection& col, Fn fn) {
		return map(std::begin(col), std::end(col), fn);
	}

	template<class It, class Fn>
	void foreach(It first, It last, Fn fn) {
		for (; first != last; ++first) {
			fn(*first);
		}
	}

	template<class Collection, class Fn>
	void foreach(Collection& col, Fn fn) {
		auto first = std::begin(col);
		foreach(first, std::end(col), fn);
	}
}
