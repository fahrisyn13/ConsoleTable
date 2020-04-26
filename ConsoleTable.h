/* https://github.com/766F6964/ConsoleTable
module name:
  ConsoleTable.h

abstract:
  ConsoleTable is a library that allows you to organize your data in a table based structure.
*/

#ifndef CONSOLETABLE_H
#define CONSOLETABLE_H
#include <string>
#include <vector>
#include "Extensions.h"
using namespace Extensions;
using std::initializer_list;
using std::ostream;
using std::string;
using std::vector;
class ConsoleTable : private Console, private Window {
public:
	enum class BorderStyle {
		BasicStyle = 0,
		LineStyle = 1,
		DoubleLineStyle = 2,
		InvisibleStyle = 3
	};

	typedef vector<string> Headers;
	typedef vector<vector<string>> Rows;
	typedef vector<size_t> Widths;

	/*
		Initialize a new ConsoleTable
		\param headers Stringlist of the tables Attribute
	*/
	ConsoleTable(
		initializer_list<string> headers
		);

	/*
		Set the distance from the text to the cell border
		\param padding Spaces between the text and the cell border
	*/
	void SetPadding(
		unsigned short padding
		);

	/*
		Set the style of the table, default is BasicStyle
		\param style The table style
	*/
	void SetBorderStyle(
		BorderStyle style
		);

	/*
		Sort the table field based on the first column
		\param ascending Should table be sorted ascending or descending
		\return True if sorting was successful, otherwise false
	*/
	bool Sort(
		bool ascending
		);

	/*
		Add a new row to the table
		\param row A list of string to add as row
		\return True if the falie was added successfully, otherwise false
	*/
	bool AddRow(
		initializer_list<string> row
		);

	/*
		Remove a row from the table by the row index
		\param index The index of the row that should be removed
		\return true if the row was removed successfully, otherwise false
	*/
	bool RemoveRow(
		unsigned short index
		);

	/*
		Update an existing table cell with new data
		\param row The index of the row that needs to be updated
		\param header The index of the column that needs to be updated
		\param newRow The new data that shound be assigned to the cell
	*/
	void UpdateRow(
		unsigned short row,
		unsigned short header,
		string newRow
		);

	/*
		Update a header with new text
		\param header Index of the header that should be updated
		\param newHeader The new header
	*/
	void UpdateHeader(
		unsigned short header,
		string newHeader
		);

	/*
		Operator of the AddRow() function
		\param row A list of string to add as row
		\return this
	*/
	ConsoleTable& operator+=(
		initializer_list<string> row
		);

	/*
		Operator of the RemoveRow() function
		\param rowIndex The index of the row that should be remove
		\return this
	*/
	ConsoleTable& operator-=(
		uint32_t rowIndex
		);

	/*
		Get widest data
		\return widest data
	*/
	size_t GetMaxData();
private:
	/* Holds all header strings of the table */
	Headers headers;

	/* Holds all rows of the table */
	Rows rows;

	/* Hold the size of widest string of each column of the table */
	Widths widths;

	/* Defines row type */
	struct RowType {
		string
			left,
			intersect,
			right;
	};

	/* Defines table style rows (top, middle, bottom, etc) */
	struct TableStyle {
		string
			horizontal,
			vertical;
		RowType
			top,
			middle,
			bottom;
	};
	/* _setmode(_fileno(stdout), _O_U16TEXT); #include <fcntl.h> #include <io.h> */

	/* Basic style - works on all systems, used as default style */
	TableStyle BasicStyle = { "-", "|", {"+", "+", "+"}, {"+", "+", "+"}, {"+", "+", "+"} };

	/* Single lined style - requires speecial character support */
	TableStyle LineStyle = { "━", "┃", {"┏", "┳", "┓"}, {"┣", "╋", "┫"}, {"┗", "┻", "┛"} };

	/* Single double style - requires speecial character support */
	TableStyle DoubleLineStyle = { "═", "║", {"╔", "╦", "╗"}, {"╠", "╬", "╣"}, {"╚", "╩", "╝"} };

	/* No visible table outlines - works on all systems */
	TableStyle InvisibleStyle = { " ", " ", {" ", " ", " "}, {" ", " ", " "}, {" ", " ", " "} };

	/* Current table style */
	TableStyle style = BasicStyle;

	/* Space character constant */
	const string SpaceCharacter = " ";

	/* The distance between the cell text and the cell border */
	unsigned short padding = 1;

	/*
		Returns a formatted horizontal separation line for thetable
		\param rowTyle The type of the row (top, middle, bottom)
		\return The formatted row string
	*/
	string GetLine(
		RowType rowType
		) const;

	/*
		Returns a formatted header string
		\param header The Headers-object that holds the header
		\return The formatted header string
	*/
	string GetHeaders(
		Headers headers
		) const;

	/*
		Returns a formatted row string
		\param rows The Rows-object that holds all rows of the table
		\return A formatted string of all rows in the table
	*/
	string GetRows(
		Rows rows
		) const;

	/*
		Writes the entire table with all its contents in the output stream
		This can be used to display the table using the std::cout function
		\param out The output stream the table shound be written to
		\param consoleTable The ConsoleTable-object
		\return Output stream with the formatted table string
	*/
	friend ostream& operator<<(
		ostream& out,
		const ConsoleTable& consoleTable
		);
};

/*
	Repeats a given string n times
	\param other The string to repeat
	\param repeats The amount the string should be repeated
	\return The repeated string
*/
string operator*(
	const string& other,
	int repeats
	);
#endif // !CONSOLETABLE_H