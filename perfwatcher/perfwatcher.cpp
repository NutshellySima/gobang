
#include "stdafx.h"

using namespace std;

int main()
{
	Cwrapper wrapper;
	chrono::steady_clock clock;
	int turn = 2;
	auto start = clock.now();
	while (wrapper.Fullboard() == false)
	{
		if (turn == 1)
			turn = 2;
		else
			turn = 1;
		auto x = wrapper.Putcomputer(turn, 10000);
		wrapper.Put(get<1>(x), get<2>(x), turn);
		if (wrapper.Checkwin(get<1>(x), get<2>(x)))
			break;
		cout << turn << ": " << static_cast<char>(get<1>(x) + 'A') << " " << static_cast<char>('A' + get<2>(x)) << ": " << get<0>(x) << endl;
	}
	auto end = clock.now();
	cout << "Duration: " << chrono::duration_cast<chrono::seconds>(end - start).count() << " s." << endl;
	return 0;
}
