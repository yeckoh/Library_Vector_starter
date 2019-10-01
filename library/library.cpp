/*
 * file:	library.cpp
 * auth:	wchang 00960978
 * created:	30 Sept 2k19
 * lastModf:1 Oct 2k19
 * prof:	K. Perkins @ CNU
 * assgn:	cs327 p3 - vectors
 */
//===========================================================
/*
 * simulate a library with books and patrons
 */
//===========================================
/*
 * despite what checkout()'s description says,
 * iterate through books then patrons, instead of patrons then books
 * ie: bogus patrons dont matter if the book doesnt exist in the first place
 * eg: iff a book exists, then check for a bogus patron
 */
//=========================

#include <string>
#include <vector>
#include <algorithm>
#include <time.h>

#include "../includes_usr/library.h"
#include "../includes_usr/datastructures.h"
#include "../includes_usr/fileIO.h"

using namespace std;

vector<patron> patrons;
vector<book> books;

/*
 * clear books and patrons containers
 * then reload them from disk 
 */
void reloadAllData(){
	books.clear();
	patrons.clear();

	loadBooks(books, BOOKFILE.c_str());
	loadPatrons(patrons, PATRONFILE.c_str());
}

/* checkout a book to a patron
 * first load books and patrons from disk
 * make sure patron enrolled (patronid is assigned to a patron in patrons container)
 * make sure book in collection (bookid is assigned to a book in books container)
 * 
 * see if patron can check out any more books 
 * 	if not return TOO_MANY_OUT patron has the MAX_BOOKS_ALLOWED_OUT
 * 	
 * if so then check the book out to the patron, set the following fields for the book in the
 * books container
 * book.loaned_to_patron_id = patronid;
 * book.state = OUT;
 * 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 *         PATRON_NOT_ENROLLED
 * 		   BOOK_NOT_IN_COLLECTION
 *         TOO_MANY_OUT patron has the max number of books allowed checked out
 */
int checkout(int bookid, int patronid){
	reloadAllData();

	for(vector<book>::iterator itrb = books.begin(); itrb != books.end(); ++itrb) {
		if(itrb->book_id == bookid) {
			for(vector<patron>::iterator itrp = patrons.begin(); itrp != patrons.end(); ++itrp) {
				if(itrp->patron_id == patronid) {
					if(itrp->number_books_checked_out == MAX_BOOKS_ALLOWED_OUT)
						return TOO_MANY_OUT;
					itrb->loaned_to_patron_id = patronid;
					itrb->state = OUT;
					++itrp->number_books_checked_out;
					saveBooks(books, BOOKFILE.c_str());
					savePatrons(patrons, PATRONFILE.c_str());
					return SUCCESS;
				}
			}
			return PATRON_NOT_ENROLLED;
		}
	}
	return BOOK_NOT_IN_COLLECTION;
}

/* check a book back in 
 * first load books and patrons from disk
 * make sure book in collection (bookid is assigned to a book in books container)
 *  	
 * if so find the the patron the book is checked out to and decrement his/hers number_books_checked_out
 * then check the book back in by marking the book.loaned_to_patron_id = NO_ONE and the book.state = IN;
 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 * 		   BOOK_NOT_IN_COLLECTION
 */
int checkin(int bookid){
	reloadAllData();

	for(vector<book>::iterator itrb = books.begin(); itrb != books.end(); ++itrb) {
		if(itrb->book_id == bookid) {
			for(vector<patron>::iterator itrp = patrons.begin(); itrp != patrons.end(); ++itrp) {
				if(itrp->patron_id == itrb->loaned_to_patron_id) {
					--itrp->number_books_checked_out;
					itrb->loaned_to_patron_id = NO_ONE;
					itrb->state = IN;
					saveBooks(books, BOOKFILE.c_str());
					savePatrons(patrons, PATRONFILE.c_str());
					return SUCCESS;
				}
			}
		}
	}
	return BOOK_NOT_IN_COLLECTION;
}

/*
 * enroll a patron, duplicate names are fine as patrons are uniquely identified by their patronid
 * first load books and patrons from disk
 * create a patron object, initialize its fields as appropriate, assign him/her the nextPatronID
 * then push the patron onto the patrons container
 * save all changes to the patrons container to disk
 * return 
 *    the patron_id of the person added
 */
int enroll(std::string &name){
	reloadAllData();
	patron new_patron;
	new_patron.name = name;
	if(patrons.empty())
		new_patron.patron_id = 0;
	else
		new_patron.patron_id = ++patrons.back().patron_id;
	patrons.push_back(new_patron);
	savePatrons(patrons, PATRONFILE.c_str());
	return new_patron.patron_id;
}

/*
 * the number of books in the books container
 * (ie. if 3 books returns 3)
 * 
 */
int numbBooks(){
	reloadAllData();
	return books.size();
}

/*
 * the number of patrons in the patrons container
 * (ie. if 3 patrons returns 3)
 */
int numbPatrons(){
	reloadAllData();
	return patrons.size();
}

/*the number of books patron has checked out
 *
 *returns a positive number indicating how many books are checked out 
 *        or PATRON_NOT_ENROLLED         
 */
int howmanybooksdoesPatronHaveCheckedOut(int patronid){
	for(vector<patron>::iterator itr = patrons.begin(); itr != patrons.end(); ++itr) {
		if(itr->patron_id == patronid)
			return itr->number_books_checked_out;
	}
	return PATRON_NOT_ENROLLED;
}

/* search through patrons container to see if patronid is there
 * if so returns the name associated with patronid in the variable name
 * 
 * returns SUCCESS found it and name in name
 *         PATRON_NOT_ENROLLED no patron with this patronid
 */
int whatIsPatronName(std::string &name,int patronid){
	for(vector<patron>::iterator itr = patrons.begin(); itr != patrons.end(); ++itr) {
		if(itr->patron_id == patronid) {
			name = itr->name;
			return SUCCESS;
		}
	}
	return PATRON_NOT_ENROLLED;
}

