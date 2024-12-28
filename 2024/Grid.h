//==============================================================================
//	Name:
//		Grid.h
//
//	Description:
//		Advent of Code 2024 Grid class
//		(https://adventofcode.com/2024)
//------------------------------------------------------------------------------
#ifndef GRID
#define GRID



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include <format>
#include <iomanip>
#include <sstream>
#include <vector>





//==============================================================================
//		GridPosition
//------------------------------------------------------------------------------
struct GridPosition
{
	int rowIndex;
	int colIndex;

	bool operator<(const GridPosition& other) const;
};





//==============================================================================
//		std::formatter<GridPosition, char> - Specialisation of the std::formatter
//		template for grid positions.
//------------------------------------------------------------------------------
template <>
struct std::formatter<GridPosition, char>;





//==============================================================================
//		Grid
//------------------------------------------------------------------------------
template<typename T>
class Grid
{
public:
	                                        Grid(size_t height, size_t width, T val);
	                                        Grid(const std::vector<std::vector<T>>& grid);
	                                        Grid(std::vector<std::vector<T>>&& grid);

	std::vector<T>&                         operator[](size_t index);
	const std::vector<T>&                   operator[](size_t index) const;

	T&                                      operator[](const GridPosition& pos);
	const T&                                operator[](const GridPosition& pos) const;

	auto                                    begin() { return mGrid.begin(); }
	auto                                    end  () { return mGrid.end();   }

	auto                                    begin() const { return mGrid.cbegin(); }
	auto                                    end  () const { return mGrid.cend();   }

	size_t                                  GetHeight() const { return mHeight; }
	size_t                                  GetWidth () const { return mWidth;  }

	bool                                    WithinBounds(const GridPosition& pos);


private:
	size_t                                  mHeight;
	size_t                                  mWidth;
	std::vector<std::vector<T>>             mGrid;
};





//==============================================================================
//		std::formatter<Grid<T>, char> - Specialisation of the std::formatter
//		template for grids.
//------------------------------------------------------------------------------
template <typename T>
struct std::formatter<Grid<T>, char>;





//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Grid.inl"



#endif // GRID
