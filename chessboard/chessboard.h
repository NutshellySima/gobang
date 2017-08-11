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

class DllExport chessboard {
public:
	chessboard()noexcept;
	void reset()noexcept;
	int get(const int row, const int col)const noexcept;
	bool put(const int row, const int col, const int x)noexcept;
	bool Fullboard()const noexcept;
	int undo(const int row, const int col)noexcept;
	int checkpoint(const int row, const int col)const noexcept;
	std::vector<std::tuple<int, int8_t, int8_t>> genmove()const noexcept;
	std::array<std::array<int8_t, 15>, 15>board;
private:
	int check()const noexcept;
	void update_remotecell(const int row, const int col, const bool add)noexcept;
	std::array<std::array<uint8_t, 15>, 15>remote_cell;
	unsigned int number;
	evaluation evaluator;
};
