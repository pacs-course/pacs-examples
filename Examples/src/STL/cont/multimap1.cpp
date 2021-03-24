/* The following code example is taken from the book
 * "The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
 * by Nicolai M. Josuttis, Addison-Wesley, 2012
 *
 * (C) Copyright Nicolai M. Josuttis 2012.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
#include <map>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

int main()
{
    // create multimap as string/string dictionary
  multimap<string,string> dict;
    // insert some elements in random order
    dict.insert ( { {"day","Tag"}, {"strange","fremd"},
                    {"car","Auto"}, {"smart","elegant"},
                    {"trait","Merkmal"}, {"strange","seltsam"},
                    {"smart","raffiniert"}, {"smart","klug"},
                    {"clever","raffiniert"} } );

    // print all elements
    cout.setf (ios::left, ios::adjustfield);
    cout << ' ' << setw(10) << "english "
         << "german " << endl;
    cout << setfill('-') << setw(20) << ""
         << setfill(' ') << endl;
    for ( const auto& elem : dict ) {
        cout << ' ' << setw(10) << elem.first
             << elem.second << endl;
    }
    cout << endl;

    // print all values for key "smart"
    string word{"smart"};
    cout << word << ": " << endl;
    for (auto pos = dict.lower_bound(word);
         pos != dict.upper_bound(word);
         ++pos) {
            cout << "    " << pos->second << endl;
    }

    // print all keys for value "raffiniert"
    word = "raffiniert";
    cout << word << ": " << endl;
    /* The classic way
       for (const auto& elem : dict) {
       if (elem.second == word) {
       cout << "    " << elem.first << endl;
       }
       } 
    */
    /* Using find_if  and lambdas*/
    auto f=find_if(dict.begin(),dict.end(),
		   [&word](pair<string,string>const & i){return i.second==word;});
    if(f != dict.end())cout << "    " << f->first << endl;
      
    // Splicing with a map
    // to show that splicing can be made also with
    // another associative container of the same "general type"
    
    map<string,string> dict2{{"fool","crazy"},{"light","frivolous"},
                                                {"smart","cool"}};
    // I want to take out a key (and correspondint value) frome dict and
    // insert them in dict2,
    // without useless memory operation, and complete the operation only
    // if the key is not already present in dict2
    // (which is a map not a multimap)
    word="smart";
    auto node = dict.extract(word);
    if (node.empty())
      {
        std::cout<<"Key: "<<word<<" not present in dict"<<std::endl;
      }
    else
      {
        std::cout<<"Extracted node handler with (key, value)= (" <<node.key()<<", "<<node.mapped()<<")\n";
        // full control (remember std::move!)
        auto [pos,found,nodeDest] = dict2.insert(std::move(node));
        if (!found)
          {
            //! if insertion fails the node is returned
            std::cout<<"insertion of node handle failed:\n"
                     <<"tried to insert (key,value)=("<<nodeDest.key()
                     <<", "<<nodeDest.mapped()<<")\n"
                     <<"but key exists already, with value "<<pos->second
                     <<std::endl;
          }
        else
          std::cout<<"insertion succeded!"<<std::endl;
      }

    
}
