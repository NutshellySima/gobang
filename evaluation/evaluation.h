/*
   Copyright 2017 Chijun Sima

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once
#include"stdafx.h"

#define DllExport   __declspec( dllexport )  

class DllExport evaluation
{
public:
	evaluation()noexcept {};
	int evaluate(chessboard&, const int turn, const int row, const int col)noexcept;
private:
	inline void reset(chessboard& board)noexcept
	{
		memset(count, 0, sizeof(count));
		for (int i = 0; i < 15; ++i)
		{
			for (int ii = 0; ii < 15; ++ii)
			{
				board.layer_3[i][ii][0] = 0;
				board.layer_3[i][ii][1] = 0;
				board.layer_3[i][ii][2] = 0;
				board.layer_3[i][ii][3] = 0;
			}
		}
	}
	void analyse_line(const std::array<uint8_t, 15>& line, int num, const int pos)noexcept;
	void analysis_horizon(chessboard&, int i, int j)noexcept;
	void analysis_vertical(chessboard&, int i, int j)noexcept;
	void analysis_left(chessboard&, int i, int j)noexcept;
	void analysis_right(chessboard&, int i, int j)noexcept;
	inline int checkturn(const int &side, const int &turn)const noexcept
	{
		if (side == turn)
			return 1;
		return -1;
	}
	inline int sign(int number)const noexcept
	{
		if (number < 0)
			return -1;
		return 1;
	}
	int __evaluate(chessboard&, int turn, const int row, const int col)noexcept;
	const int STWO = 1, STHREE = 2, SFOUR = 3, TWO = 4, THREE = 5, FOUR = 6, FIVE = 7, DFOUR = 8,
		FOURT = 9, DTHREE = 10, NOTYPE = 11, ANALYSED = 255, TODO = 0;
	const int BLACK = 1, WHITE = 2;
	const std::array<int, 3>nturn = { 0,2,1 };
	std::array<int, 15>result;
	int count[3][20];
};
