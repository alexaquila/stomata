#ifndef TEST_H
#define TEST_H

#include <iostream>
#include "data.h"


class test
{
	public:
		test(std::vector<data> *datasets);
		virtual ~test();

		void startTesting();
	protected:
	private:
		std::vector<data> *datasets;
};

#endif // TEST_H
