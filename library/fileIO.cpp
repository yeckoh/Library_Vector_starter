#include "../includes_usr/fileIO.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
/* clears, then loads books from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries in books
 * 			SUCCESS if all data is loaded
 * */
int loadBooks(vector<book> &books, const char* filename)
{
	ifstream fin(filename, fstream::in);
	if(!fin.is_open())
		return COULD_NOT_OPEN_FILE;
	if(fin.peek() == ifstream::traits_type::eof())
		return NO_BOOKS_IN_LIBRARY;
	book book_loader;
	string line = "";
	string token = "";
	while(getline(fin, line)) {
		stringstream line_stream(line);
		getline(line_stream, token, ',');
		book_loader.book_id = stoi(token);
		getline(line_stream, token, ',');
		book_loader.title = token;
		getline(line_stream, token, ',');
		book_loader.author = token;
		getline(line_stream, token, ',');
		book_loader.state = book_checkout_state(stoi(token));
		getline(line_stream, token, ',');
		book_loader.loaned_to_patron_id = stoi(token);
		books.push_back(book_loader);
	}
	return SUCCESS;
}

/* serializes books to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries books (do not create file)
 * 			SUCCESS if all data is saved
 * */
int saveBooks(vector<book> &books, const char* filename)
{
	if(books.empty())
		return NO_BOOKS_IN_LIBRARY;
	ofstream fout(filename);
	if(!fout.is_open())
		return COULD_NOT_OPEN_FILE;

	for(vector<book>::iterator itr = books.begin(); itr != books.end(); ++itr) {
		fout 	<< itr->book_id << ','
				<< itr->title	<< ','
				<< itr->author	<< ','
				<< itr->state	<< ','
				<< itr->loaned_to_patron_id;
		//if(next(itr) != books.end())
		fout	<< '\n';
	}
	return SUCCESS;
}

/* clears, then loads patrons from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons
 * 			SUCCESS if all data is loaded
 * */
int loadPatrons(vector<patron> &patrons, const char* filename)
{
	ifstream fin(filename, fstream::in);
	if(!fin.is_open())
		return COULD_NOT_OPEN_FILE;
	if(fin.peek() == ifstream::traits_type::eof())
		return NO_PATRONS_IN_LIBRARY;

	patron patron_loader;
	string line = "";
	string token = "";
	while(getline(fin, line)) {
		stringstream line_stream(line);
		getline(line_stream, token, ',');
		patron_loader.patron_id = stoi(token);
		getline(line_stream, token, ',');
		patron_loader.name = token;
		getline(line_stream, token, ',');
		patron_loader.number_books_checked_out = stoi(token);
		patrons.push_back(patron_loader);
	}
	return SUCCESS;
}

/* serializes patrons to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons  (do not create file)
 * 			SUCCESS if all data is saved
 * */
int savePatrons(vector<patron> &patrons, const char* filename)
{
	if(patrons.empty())
		return NO_PATRONS_IN_LIBRARY;
	ofstream fout(filename);
	if(!fout.is_open())
		return COULD_NOT_OPEN_FILE;

	for(vector<patron>::iterator itr = patrons.begin(); itr != patrons.end(); ++itr) {
		fout	<< itr->patron_id 					<< ','
				<< itr->name						<< ','
				<< itr->number_books_checked_out	<< '\n';
	}
	return SUCCESS;
}
