export module foobar;

import std;

export namespace Foomatic {
	class Foobar {
	public:
		void helloFoo();
	};

	void Foobar::helloFoo() {
		std::cout << "Hello C++20 Modules" << std::endl;
	}
}
