#include <iostream>
#include <vector>
#include <string>
#include <iterator>  // istream_iterator
#include <algorithm> // copy, sort, unique_copy
#include <set>
// compile: clang++ book_1_stream_iterators.cpp -o book_1_stream_iterators && ./book_1_stream_iterators
/*
   testing with input:
      echo "this is a \nsimple test" | ./book_1_stream_iterators
      echo "this is a \\nsimple test" | ./book_1_stream_iterators
      echo "pc ram ram ram pc laptop" | ./book_1_stream_iterators
      cat Makefile | ./book_1_stream_iterators
*/

int main()
{
   std::vector<std::string> coll;
   std::copy(
      std::istream_iterator<std::string>(std::cin),
      std::istream_iterator<std::string>(),
      std::back_inserter(coll));

   std::cout << "passed in strings:\n";
   std::copy(coll.begin(), coll.end(), std::ostream_iterator<std::string>(std::cout, " "));
   std::cout << "\n";

   std::sort(coll.begin(), coll.end());
   std::cout << "filtered:\n";
   std::unique_copy(
      coll.cbegin(),
      coll.cend(),
      std::ostream_iterator<std::string>(std::cout, "\n"));
   return 0;
}

// The above has 1 declaration and 3 statements.
// The same can be achieved with 1 declaration and 1 statement (p.394):
void alternative_impl()
{
   std::set<std::string> coll(
      (std::istream_iterator<std::string>(std::cin)),
      std::istream_iterator<std::string>());
   /* or (p.257):
   std::set<std::string> coll{
      std::istream_iterator<std::string>(std::cin),
      std::istream_iterator<std::string>()};
   */
   std::copy(coll.cbegin(), coll.cend(), std::ostream_iterator<std::string>(std::cout, "\n"));
}
