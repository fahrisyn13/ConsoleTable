#include "ConsoleTable.h"
#include <algorithm>
#include <iostream>
#include <sstream>
using std::greater;
using std::invalid_argument;
using std::less;
using std::max;
using std::out_of_range;
using std::sort;
using std::stringstream;

ConsoleTable::ConsoleTable(
	initializer_list<string> headers
	) : headers{ headers } {
	for (auto& column : headers)
		widths.push_back(column.length());
}

void ConsoleTable::SetPadding(
	unsigned short padding
	) {
	padding = padding;
}

void ConsoleTable::SetBorderStyle(
	BorderStyle style
	) {
	switch (style) {
	case BorderStyle::BasicStyle:
		this->style = BasicStyle;
		break;
	case BorderStyle::LineStyle:
		this->style = LineStyle;
		break;
	case BorderStyle::DoubleLineStyle:
		this->style = DoubleLineStyle;
		break;
	case BorderStyle::InvisibleStyle:
		this->style = InvisibleStyle;
		break;
	}
}

bool ConsoleTable::Sort(
	bool ascending
	) {
	if (ascending)
		sort(rows.begin(), rows.end(), less<vector<string>>());
	else
		sort(rows.begin(), rows.end(), greater<vector<string>>());
	return true;
}

bool ConsoleTable::AddRow(
	initializer_list<string> row
	) {
	if (row.size() > widths.size())
		throw invalid_argument{ "Appended row size must be same as header size" };
	auto r = vector<string>{ row };
	rows.push_back(r);
	for (unsigned short i = 0; i < r.size(); ++i)
		widths[i] = max(r[i].size(), widths[i]);
	return true;
}

bool ConsoleTable::RemoveRow(
	unsigned short index
	) {
	if (index > rows.size())
		return false;
	rows.erase(rows.begin() + index);
	return true;
}

void ConsoleTable::UpdateRow(
	unsigned short row,
	unsigned short header,
	string newRow
	) {
	if (row > rows.size() - 1)
		throw out_of_range{ "Row index out of range" };
	if (header > headers.size() - 1)
		throw out_of_range{ "Header index out of range" };
	rows[row][header] = newRow;
}

void ConsoleTable::UpdateHeader(
	unsigned short header,
	string newHeader
	) {
	if (header > headers.size())
		throw out_of_range{ "Header index out of range" };
	headers[header] = newHeader;
}

ConsoleTable& ConsoleTable::operator+=(
	initializer_list<string> row
	) {
	if (row.size() > widths.size())
		throw invalid_argument{ "Appended row size must be same as header size" };
	AddRow(row);
	return *this;
}

ConsoleTable& ConsoleTable::operator-=(
	uint32_t rowIndex
	) {
	if (rows.size() < rowIndex)
		throw out_of_range{ "Row index out of range" };
	RemoveRow(rowIndex);
	return *this;
}

size_t ConsoleTable::GetMaxData() {
	size_t
		widthSize = widths.size(),
		maxLength = 0;

	for (unsigned short i = 0; i < widthSize; ++i)
		maxLength += widths[i];
	maxLength += 2 * widths.size() + widthSize + 5;
	return maxLength;
}

string ConsoleTable::GetLine(
	RowType rowType
	) const {
	stringstream line;

	line << "  " << rowType.left;
	for (unsigned short i = 0; i < widths.size(); ++i) {
		for (unsigned int j = 0; j < (widths[i] + padding + padding); ++j)
			line << style.horizontal;
		line << (i == widths.size() - 1 ? rowType.right : rowType.intersect);
	}
	return line.str() + '\n';
}

string ConsoleTable::GetHeaders(
	Headers headers
	) const {
	stringstream line;

	line << "  " << style.vertical;
	for (unsigned short i = 0; i < headers.size(); ++i) {
		string text = headers[i];
		line << SpaceCharacter * padding + text + SpaceCharacter * (widths[i] - text.length()) + SpaceCharacter * padding;
		line << style.vertical;
	}
	line << '\n';
	return line.str();
}

string ConsoleTable::GetRows(
	Rows rows
	) const {
	stringstream line;

	for (auto& row : rows) {
		line << "  " << style.vertical;

		for (unsigned short j = 0; j < row.size(); ++j) {
			string text = row[j];
			line << SpaceCharacter * padding + text + SpaceCharacter * (widths[j] - text.length()) + SpaceCharacter * padding;
			line << style.vertical;
		}
		line << '\n';
	}
	return line.str();
}

ostream& operator<<(
	ostream& out,
	const ConsoleTable& consoleTable
	) {
	out << consoleTable.GetLine(consoleTable.style.top);
	out << consoleTable.GetHeaders(consoleTable.headers);
	out << consoleTable.GetLine(consoleTable.style.middle);
	out << consoleTable.GetRows(consoleTable.rows);
	out << consoleTable.GetLine(consoleTable.style.bottom);
	return out;
}

string operator*(
	const string& other,
	int repeats
	) {
	string ret;

	ret.reserve(other.size() * repeats);
	for (; repeats; --repeats)
		ret.append(other);
	return ret;
}