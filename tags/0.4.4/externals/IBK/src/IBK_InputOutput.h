/*	Copyright (c) 2001-2016, Institut für Bauklimatik, TU Dresden, Germany

	Written by A. Nicolai, H. Fechner, St. Vogelsang, A. Paepcke, J. Grunewald
	All rights reserved.

	This file is part of the IBK Library.

	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation 
	   and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its contributors
	   may be used to endorse or promote products derived from this software without
	   specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
	ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


	This library contains derivative work based on other open-source libraries. 
	See OTHER_LICENCES and source code headers for details.

*/

#ifndef IBK_InputOutputH
#define IBK_InputOutputH

#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <stdexcept>
#include <fstream>

#ifdef _MSC_VER
	typedef __int64 int64_t;
	typedef unsigned __int64 uint64_t;
	typedef unsigned __int32 uint32_t;
	typedef unsigned __int16 uint16_t;
#else
	#include <stdint.h>
#endif

#include "IBK_NotificationHandler.h"
#include "IBK_Exception.h"
#include "IBK_configuration.h"

namespace IBK {

/*! Tries to read a vector of numbers of a string.
	This can fail for a number of reasons, therefore the user can check for
	the outcome by passing a pointer to an error string.
	\code
	// string must hold white-space separated double values
	std::string data = "12.4 12.4\n"
		"123.2 321.3"
	\endcode
*/
bool read_vector(const std::string& data, std::vector<double>& vec, std::string * errmsg = NULL);

/*! Tries to write a vector of numbers to the stream 'out'.
	\sa read_vector
*/
void write_vector(std::ostream & out, unsigned int n, const double * v, unsigned int width);

/*! Writes data into an output file for a time point given in timestr. Between the timestr and the
	vector data a tab character is placed and also between all columns of the vector vec. A
	typical output line for a vector of 3 elements and a timestring of 1.34 would look like:
	   '1.34    12.33    43.0    33.0'
*/
std::ostream& write_vector(std::ostream& out, const std::string& timestr, const std::vector<double>& vec);

/*! Debug helper, writes a string to an output stream in the following format.
	\code
	std::vector<double> vec;
	IBK::dump_vector(std::cout, "myVector", vec.size(), &vec[0], 10);
	//	gives:
	//	myVector:
	//	[         1]
	//	[         2]
	//	[         6]
	//	[         2]
	//	[         3]
	\endcode
*/
void dump_vector(std::ostream & out, const char * name, unsigned int n, const double * v, unsigned int width);

/*! Writes the data in the vector into a human readible format.
	\param out Output stream
	\param name Some c-string with the name of the vector
	\param n Length of the vector
	\param v Pointer to linear memory array of size n
	\param width Optional width of numbers.
*/
void write_vector(std::ostream & out, const char * name, unsigned int n, const double * v, unsigned int width = 4);

/*! Reads a string in binary format from the binary input stream 'in'.
	Corresponds to string format as written by write_string_binary().
*/
void read_string_binary(std::istream& in, std::string& str, uint32_t max_len);

/*! Reads a string in binary format from the binary input stream 'in'.
	Corresponds to string format as written by write_string_binary_with_termination_character().
	This format was used before Version 6 of Delphin.
*/
void read_string_binary_with_termination_character(std::istream& in, std::string& str, uint32_t max_len);

/*! Writes a string in binary format into the binary output stream 'out'.
	The string is written in pascal mode: the lenghts of the string is written beforehand as 32bit integer value.
	Termination characters are not written.
*/
void write_string_binary(std::ostream& out, const std::string& str);

/*! Writes a string in binary format into the binary output stream 'out'.
	The string is written in pascal mode including termination character:
	The lenghts of the string (+1 for term char) is written beforehand as 32bit integer value,
	followed by the string including its termination character \0. Empty strings have length 1, and
	consist only of the termination character.
	This format was used before Version 6 of Delphin.
*/
void write_string_binary_with_termination_character(std::ostream& out, const std::string& str);

/*! Reads a vector of elements of type T from a stream in binary mode.
	\param in Input stream (opened in binary mode)
	\param vec Target vector.
	\param max_len A sensible upper limit for the vector size, to avoid memory allocation faults when currupt binary data is read.
*/
template<typename T>
void read_vector_binary(std::istream& in, std::vector<T>& vec, uint32_t max_len) {
	uint32_t len;
	in.read(reinterpret_cast<char *>(&len), sizeof(uint32_t));
	if (len > max_len)
		throw IBK::Exception("Vector length exceeds given maximum!", "[read_vector_binary]");
	if (len > 0) {
		vec.resize(len);
		in.read(reinterpret_cast<char *>(&vec[0]), sizeof(T)*len);
	}
	else {
		vec.clear();
	}
}

/*! Writes a vector of elements of type T to a stream.
	Before the memory of the vector is written, the number of elements in the vector
	is written is 32 bit integer value.
*/
template<typename T>
void write_vector_binary(std::ostream& out, const std::vector<T>& vec) {
	uint32_t len = static_cast<uint32_t>(vec.size());
	out.write(reinterpret_cast<char *>(&len), sizeof(uint32_t));
	if (!vec.empty())
		out.write(reinterpret_cast<const char *>(&vec[0]), sizeof(T)*len);
}


/*! Function that writes a 32-bit integer in an binary output stream.
	\param out Must be an binary output file stream.
	\param number The number we want to be written into the stream.
*/
inline void write_uint32_binary( std::ostream &out, uint32_t number ) {
	out.write( (char *)&number, sizeof(uint32_t));
}


/*! Function that reads a 32-bit integer from an binary input stream.
	\param in Must be an binary input stream.
	\param number The number we want to read from the stream.
*/
inline void read_uint32_binary( std::istream &in, uint32_t &number ){
	in.read( reinterpret_cast<char*>(&number), sizeof(uint32_t) );
}


/*! Reads an ASCII file in binary mode into a list of strings.

	This is an efficient readLines() implementation with additional abortion criteria.
	After the call to the function the stream is positioned directly after the last line that was read.
	If the file was completely read, the stream state is set to be at the end of the file.
	\param in Input file stream (binary mode).
	\param strList Vector to hold read string list.
	\param lastLineTokens A list of strings that mark the last line of the data section to be read. If empty,
		or if none of the lines started with one of the lastLineTokens, the file is read to the end.
	\param notify Optional pointer to notification handler.
*/
void read_string_list_binary( std::istream &in,
							  const std::vector<std::string> & lastLineTokens,
							  std::list<std::string> & strList,
							  IBK::NotificationHandler * notify = NULL );

/*! Reads an ASCII file in binary mode into a list of strings.

	This is an efficient readLines() implementation with additional abortion criteria.
	After the call to the function the stream is positioned directly after the last line that was read.
	If the file was completely read, the stream state is set to be at the end of the file.
	\param in Input file stream (binary mode).
	\param lastLineTokens A list of strings that mark the last line of the data section to be read. If empty,
		or if none of the lines started with one of the lastLineTokens, the file is read to the end.
	\param strList Vector to hold read string list.
	\param notify Optional pointer to notification handler.
*/
void read_string_vector_binary( std::istream &in,
							  const std::vector<std::string> & lastLineTokens,
							  std::vector<std::string> & strList,
							  IBK::NotificationHandler * notify = NULL );


} // namespace IBK

/*! \file IBK_InputOutput.h
	\brief Contains
*/

#endif // IBK_InputOutputH