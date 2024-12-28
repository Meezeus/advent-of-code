//==============================================================================
//	Name:
//		Grid.inl
//
//	Description:
//		Advent of Code 2024 Grid class
//		(https://adventofcode.com/2024)
//------------------------------------------------------------------------------



//==============================================================================
//		GridPosition::operator<(const GridPosition& other) const
//------------------------------------------------------------------------------
bool GridPosition::operator<(const GridPosition& other) const
{
	return (rowIndex <  other.rowIndex) ||
	       (rowIndex == other.rowIndex  && colIndex < other.colIndex);
}





//==============================================================================
//		Grid::Grid(size_t height, size_t width, T val);
//------------------------------------------------------------------------------
template<typename T>
Grid<T>::Grid(size_t height, size_t width, T val)
	: mHeight(height)
	, mWidth (width)
	, mGrid  (height, std::vector<T>(width, val))
{}





//==============================================================================
//		Grid::Grid(const std::vector<std::vector<T>>& grid)
//------------------------------------------------------------------------------
template<typename T>
Grid<T>::Grid(const std::vector<std::vector<T>>& grid)
	: mHeight(grid.size())
	, mWidth (grid[0].size())
	, mGrid  (grid)
{}





//==============================================================================
//		Grid::Grid(std::vector<std::vector<T>>&& grid)
//------------------------------------------------------------------------------
template<typename T>
Grid<T>::Grid(std::vector<std::vector<T>>&& grid)
	: mHeight(grid.size())
	, mWidth (grid[0].size())
	, mGrid  (std::move(grid))
{}





//==============================================================================
//		Grid::operator[](size_t index)
//------------------------------------------------------------------------------
template<typename T>
std::vector<T>& Grid<T>::operator[](size_t index)
{
	return mGrid[index];
}





//==============================================================================
//		Grid::operator[](size_t index) const
//------------------------------------------------------------------------------
template<typename T>
const std::vector<T>& Grid<T>::operator[](size_t index) const
{
	return mGrid[index];
}





//==============================================================================
//		Grid::operator[](const Position& pos)
//------------------------------------------------------------------------------
template<typename T>
T& Grid<T>::operator[](const GridPosition& pos)
{
	return mGrid[pos.rowIndex][pos.colIndex];
}





//==============================================================================
//		Grid::operator[](const Position& pos) const
//------------------------------------------------------------------------------
template<typename T>
const T& Grid<T>::operator[](const GridPosition& pos) const
{
	return mGrid[pos.rowIndex][pos.colIndex];
}





//==============================================================================
//		Grid::WithinBounds(const GridPosition& pos)
//------------------------------------------------------------------------------
template<typename T>
bool Grid<T>::WithinBounds(const GridPosition& pos)
{
	if (pos.rowIndex < 0 || pos.rowIndex >= static_cast<int>(GetHeight()))
	{
		return false;
	}

	if (pos.colIndex < 0 || pos.colIndex >= static_cast<int>(GetWidth()))
	{
		return false;
	}

	return true;
}





//==============================================================================
//		std::formatter<Grid<T>, char> - Specialisation of the std::formatter
//		template for grids.
//------------------------------------------------------------------------------
template <typename T>
struct std::formatter<Grid<T>, char>
{
	constexpr auto parse(format_parse_context& ctx)
	{
		return ctx.begin(); // No special parsing required.
	}

	template <typename FormatContext>
	auto format(const Grid<T>& grid, FormatContext& ctx) const
	{
		// Find the maximum width of any element in the grid.
		size_t maxWidth = 0;
		for (size_t rowIndex = 0; rowIndex < grid.GetHeight(); ++rowIndex)
		{
			for (size_t colIndex = 0; colIndex < grid.GetWidth(); ++colIndex)
			{
				std::ostringstream oss;
				oss << grid[rowIndex][colIndex];
				maxWidth = std::max(maxWidth, oss.str().length());
			}
		}

		// Format the grid row by row with aligned columns.
		std::string result;
		for (size_t rowIndex = 0; rowIndex < grid.GetHeight(); ++rowIndex)
		{
			for (size_t colIndex = 0; colIndex < grid.GetWidth(); ++colIndex)
			{
				std::ostringstream oss;
				oss << std::setw(maxWidth) << grid[rowIndex][colIndex];
				result += oss.str();
				if (colIndex != grid.GetWidth() - 1)
				{
					result += " ";
				}
			}
			if (rowIndex != grid.GetHeight() - 1)
			{
				result += "\n";
			}
		}
		return std::format_to(ctx.out(), "{}", result);
	}
};
