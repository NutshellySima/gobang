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

#include "stdafx.h"
#include "evaluation.h"

void evaluation::reset_point(chessboard &board, int row, int col) noexcept
{
	for (int i = 0; i < 3; ++i)
	{
		for (int ii = 0; ii < 20; ++ii)
		{
			board.layer_4[i][ii] = 0;
		}
	}
	if (row < 0 || col < 0 || row >= 15 || col >= 15)
	{
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
	else
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			board.layer_3[row][ii][0] = 0;
		}
		for (int ii = 0; ii < 15; ++ii)
		{
			board.layer_3[ii][col][1] = 0;
		}
		int x, y;
		if (row < col)
		{
			y = col - row;
			x = 0;
		}
		else
		{
			y = 0;
			x = row - col;
		}
		for (; x < 15 && y < 15; ++x, ++y)
		{
			board.layer_3[x][y][2] = 0;
		}
		if (14 - row < col)
		{
			y = col - 14 + row;
			x = 14;
		}
		else
		{
			y = 0;
			x = row + col;
		}
		for (; x >= 0 && y < 15; --x, ++y)
		{
			board.layer_3[x][y][3] = 0;
		}
	}
}

void evaluation::analyse_line(const std::array<uint8_t, 15> &line, int num, const int pos) noexcept
{
	std::fill_n(result.begin(), num, TODO);
	if (num < 5)
	{
		std::fill_n(result.begin(), num, ANALYSED);
		return;
	}
	int8_t stone = line[pos];
	int8_t inverse = nturn[stone];
	--num;
	int xl = pos, xr = pos;
	while (xl > 0)
	{
		if (line[xl - 1] != stone)
			break;
		--xl;
	}
	while (xr < num)
	{
		if (line[xr + 1] != stone)
			break;
		++xr;
	}
	int left_range = xl, right_range = xr;
	while (left_range > 0)
	{
		if (line[left_range - 1] == inverse)
			break;
		--left_range;
	}
	while (right_range < num)
	{
		if (line[right_range + 1] == inverse)
			break;
		++right_range;
	}
	if (right_range - left_range < 4)
	{
		for (int k = left_range; k <= right_range; ++k)
			result[k] = ANALYSED;
		return;
	}
	for (int k = xl; k <= xr; ++k)
		result[k] = ANALYSED;
	int srange = xr - xl;
	if (srange >= 4)
	{
		result[pos] = FIVE;
		return;
	}
	if (srange == 3)
	{
		bool leftfour = false;
		if (xl > 0)
		{
			if (line[xl - 1] == 0)
				leftfour = true;
		}
		if (xr < num)
		{
			if (line[xr + 1] == 0)
			{
				if (leftfour)
					result[pos] = FOUR;
				else
					result[pos] = SFOUR;
			}
			else if (leftfour)
				result[pos] = SFOUR;
			return;
		}
	}
	if (srange == 2)
	{
		bool left3 = false;
		if (xl > 0)
		{
			if (line[xl - 1] == 0)
			{
				if (xl > 1 && line[xl - 2] == stone)
				{
					result[xl] = SFOUR;
					result[xl - 2] = ANALYSED;
				}
				else
					left3 = true;
			}
			else if (xr == num || line[xr + 1] != 0)
				return;
		}
		if (xr < num)
		{
			if (line[xr + 1] == 0)
			{
				if (xr < num - 1 && line[xr + 2] == stone)
				{
					result[xr] = SFOUR;
					result[xr + 2] = ANALYSED;
				}
				else if (left3)
					result[xr] = THREE;
				else
					result[xr] = STHREE;
			}
			else if (result[xl] == SFOUR)
				return;
			else if (left3)
				result[pos] = STHREE;
		}
		else
		{
			if (result[xl] == SFOUR)
				return;
			if (left3)
				result[pos] = STHREE;
		}
		return;
	}
	if (srange == 1)
	{
		bool left2 = false;
		if (xl > 2)
		{
			if (line[xl - 1] == 0)
			{
				if (line[xl - 2] == stone)
				{
					if (line[xl - 3] == stone)
					{
						result[xl - 3] = ANALYSED;
						result[xl - 2] = ANALYSED;
						result[xl] = SFOUR;
					}
					else if (line[xl - 3] == 0)
					{
						result[xl - 2] = ANALYSED;
						result[xl] = STHREE;
					}
				}
				else
					left2 = true;
			}
		}
		if (xr < num)
		{
			if (line[xr + 1] == 0)
			{
				if (xr < num - 2 && line[xr + 2] == stone)
				{
					if (line[xr + 3] == stone)
					{
						result[xr + 3] = ANALYSED;
						result[xr + 2] = ANALYSED;
						result[xr] = SFOUR;
					}
					else if (line[xr + 3] == 0)
					{
						result[xr + 2] = ANALYSED;
						if (left2)
							result[xr] = THREE;
						else
							result[xr] = STHREE;
					}
				}
				else
				{
					if (result[xl] == SFOUR)
						return;
					if (result[xl] == STHREE)
					{
						result[xl] = THREE;
						return;
					}
					if (left2)
						result[pos] = TWO;
					else
						result[pos] = STWO;
				}
			}
			else
			{
				if (result[xl] == SFOUR)
					return;
				if (left2)
					result[pos] = STWO;
			}
		}
		return;
	}
	return;
}

void evaluation::analysis_horizon(chessboard &board, int i, int j) noexcept
{
	analyse_line(std::ref(board.layer_2[0][i]), 15, j);
	for (int x = 0; x < 15; ++x)
	{
		if (result[x] != TODO)
			board.layer_3[i][x][0] = result[x];
	}
}

void evaluation::analysis_vertical(chessboard &board, int i, int j) noexcept
{
	analyse_line(std::ref(board.layer_2[1][j]), 15, i);
	for (int x = 0; x < 15; ++x)
	{
		if (result[x] != TODO)
			board.layer_3[x][j][1] = result[x];
	}
}

void evaluation::analysis_left(chessboard &board, int i, int j) noexcept
{
	int x, y, k = 0;
	if (i < j)
	{
		x = j - i;
		y = 0;
	}
	else
	{
		x = 0;
		y = i - j;
	}
	k = 15 - abs(i - j);
	analyse_line(board.layer_2[2][i - j + 14], k, j - x);
	for (int s = 0; s < k; ++s)
	{
		if (result[s] != TODO)
			board.layer_3[y + s][x + s][2] = result[s];
	}
}

void evaluation::analysis_right(chessboard &board, int i, int j) noexcept
{
	int x, y, k = 0;
	if (14 - i < j)
	{
		x = j - 14 + i;
		y = 14;
	}
	else
	{
		x = 0;
		y = i + j;
	}
	k = 15 - abs(i + j - 14);
	analyse_line(board.layer_2[3][i + j], k, j - x);
	for (int s = 0; s < k; ++s)
	{
		if (result[s] != TODO)
			board.layer_3[y - s][x + s][3] = result[s];
	}
}

int evaluation::__evaluate(chessboard &board, int turn, const int row, const int col, bool pure) noexcept
{
	reset_point(board, row, col);
	evaluate_point(board, row, col);

	if (pure)
		return 0;

	for (int i = 0; i < 15; ++i)
	{
		for (int j = 0; j < 15; ++j)
		{
			int stone = board.board[i][j];
			if (stone != 0)
			{
				for (int k = 0; k < 4; ++k)
				{
					int ch = board.layer_3[i][j][k];
					if (ch >= STWO && ch <= FIVE)
						++board.layer_4[stone][ch];
				}
			}
		}
	}
	if (board.layer_4[WHITE][SFOUR] >= 2)
		++board.layer_4[WHITE][FOUR];
	if (board.layer_4[BLACK][SFOUR] >= 2)
		++board.layer_4[BLACK][FOUR];

	if (board.layer_4[BLACK][FIVE])
		return 9999 * checkturn(BLACK, turn);
	if (board.layer_4[WHITE][FIVE])
		return 9999 * checkturn(WHITE, turn);
	int wvalue = 0, bvalue = 0;
	if (turn == WHITE)
	{
		if (board.layer_4[WHITE][FOUR] > 0)
			return 9990;
		if (board.layer_4[WHITE][SFOUR] > 0)
			return 9980;
		if (board.layer_4[BLACK][FOUR] > 0)
			return -9970;
		if (board.layer_4[BLACK][SFOUR] && board.layer_4[BLACK][THREE])
			return -9960;
		if (board.layer_4[WHITE][THREE] && board.layer_4[BLACK][SFOUR] == 0)
			return 9950;
		if (board.layer_4[BLACK][THREE] > 1 &&
			board.layer_4[WHITE][SFOUR] == 0 &&
			board.layer_4[WHITE][THREE] == 0 &&
			board.layer_4[WHITE][STHREE] == 0)
			return -9940;
		if (board.layer_4[WHITE][THREE] > 1)
			wvalue += 2000;
		else if (board.layer_4[WHITE][THREE])
			wvalue += 200;
		if (board.layer_4[BLACK][THREE] > 1)
			bvalue += 500;
		else if (board.layer_4[BLACK][THREE])
			bvalue += 100;
		if (board.layer_4[WHITE][STHREE])
			wvalue += board.layer_4[WHITE][STHREE] * 10;
		if (board.layer_4[BLACK][STHREE])
			bvalue += board.layer_4[BLACK][STHREE] * 10;
		if (board.layer_4[WHITE][TWO])
			wvalue += board.layer_4[WHITE][TWO] * 4;
		if (board.layer_4[BLACK][TWO])
			bvalue += board.layer_4[BLACK][TWO] * 4;
		if (board.layer_4[WHITE][STWO])
			wvalue += board.layer_4[WHITE][STWO];
		if (board.layer_4[BLACK][STWO])
			bvalue += board.layer_4[BLACK][STWO];
	}
	else
	{
		if (board.layer_4[BLACK][FOUR] > 0)
			return 9990;
		if (board.layer_4[BLACK][SFOUR] > 0)
			return 9980;
		if (board.layer_4[WHITE][FOUR] > 0)
			return -9970;
		if (board.layer_4[WHITE][SFOUR] && board.layer_4[WHITE][THREE])
			return -9960;
		if (board.layer_4[BLACK][THREE] && board.layer_4[WHITE][SFOUR] == 0)
			return 9950;
		if (board.layer_4[WHITE][THREE] > 1 &&
			board.layer_4[BLACK][SFOUR] == 0 &&
			board.layer_4[BLACK][THREE] == 0 &&
			board.layer_4[BLACK][STHREE] == 0)
			return -9940;
		if (board.layer_4[BLACK][THREE] > 1)
			bvalue += 2000;
		else if (board.layer_4[BLACK][THREE])
			bvalue += 200;
		if (board.layer_4[WHITE][THREE] > 1)
			wvalue += 500;
		else if (board.layer_4[WHITE][THREE])
			wvalue += 100;
		if (board.layer_4[BLACK][STHREE])
			bvalue += board.layer_4[BLACK][STHREE] * 10;
		if (board.layer_4[WHITE][STHREE])
			wvalue += board.layer_4[WHITE][STHREE] * 10;
		if (board.layer_4[BLACK][TWO])
			bvalue += board.layer_4[BLACK][TWO] * 4;
		if (board.layer_4[WHITE][TWO])
			wvalue += board.layer_4[WHITE][TWO] * 4;
		if (board.layer_4[BLACK][STWO])
			bvalue += board.layer_4[BLACK][STWO];
		if (board.layer_4[WHITE][STWO])
			wvalue += board.layer_4[WHITE][STWO];
	}
	return checkturn(turn, WHITE) * (wvalue - bvalue);
}

void evaluation::evaluate_point(chessboard &board, int row, int col) noexcept
{
	if (row < 0 || col < 0 || row >= 15 || col >= 15)
	{
		for (int i = 0; i < 15; ++i)
		{
			for (int j = 0; j < 15; ++j)
			{
				if (board.board[i][j] != 0)
				{
					if (board.layer_3[i][j][0] == TODO)
						analysis_horizon(board, i, j);
					if (board.layer_3[i][j][1] == TODO)
						analysis_vertical(board, i, j);
					if (board.layer_3[i][j][2] == TODO)
						analysis_left(board, i, j);
					if (board.layer_3[i][j][3] == TODO)
						analysis_right(board, i, j);
				}
			}
		}
	}
	else
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			if (board.board[row][ii] != 0 && board.layer_3[row][ii][0] == TODO)
				analysis_horizon(board, row, ii);
		}
		for (int ii = 0; ii < 15; ++ii)
		{
			if (board.board[ii][col] != 0 && board.layer_3[ii][col][1] == TODO)
				analysis_vertical(board, ii, col);
		}
		int x, y;
		if (row < col)
		{
			y = col - row;
			x = 0;
		}
		else
		{
			y = 0;
			x = row - col;
		}
		for (; x < 15 && y < 15; ++x, ++y)
		{
			if (board.board[x][y] != 0 && board.layer_3[x][y][2] == TODO)
				analysis_left(board, x, y);
		}
		if (14 - row < col)
		{
			y = col - 14 + row;
			x = 14;
		}
		else
		{
			y = 0;
			x = row + col;
		}
		for (; x >= 0 && y < 15; --x, ++y)
		{
			if (board.board[x][y] != 0 && board.layer_3[x][y][3] == TODO)
				analysis_right(board, x, y);
		}
	}
}

int evaluation::evaluate(chessboard &board, const int turn, const int row, const int col, bool pure) noexcept
{
	int score = __evaluate(board, turn, row, col, pure);
	if (pure)
		return 0;
	const int stone = nturn[turn];
	if (abs(score) > 9000)
	{
		const int x = sign(score);
		for (int i = 0; i < 20; ++i)
			if (board.layer_4[stone][i] > 0)
				score += (i * x);
	}
	return score;
}
