#include <unordered_map>
#include <iostream>
#include <string>
#include <algorithm>// for find_if
int main()
{
  std::unordered_map<std::string,std::string> rub;
  // note the double braces!
  rub.insert({{"Marco","023456"},
             {"Silvia","32900678"},
             {"Mark","3471198007"},
              {"Elise","0375889777"}});
  // If it is only one value I am spared the double brace 
  rub.insert({"Carlo","0345999"});
  // The second Marco is NOT inserted it is a map not a multimap
  rub.insert({"Marco","1234567"});
 // but I can do also more explicitely 
  rub.insert(std::make_pair("Giulia","329009877"));  

  // Insert with check
  { // I open a scope so I can reuse it and status
    auto [it,status] = rub.insert({"Marco","1234567"});
    if(status==false)
    {
      std::cerr<<"Marco is already inserted with number "<<it->second<<std::endl;
        }
  }
  std::cout<<"The map has "<<rub.size()<<" Elements:\n";
  // Traverse the map
  for (auto [key,value]:rub)
  {
    std::cout<<key<<" "<<value<<std::endl;
  }
  
  // Finding for a value correspondig to a key
  // The safe way
  auto it = rub.find("Marco");
  if (it != rub.end())
    std::cout<<"Marco's number= "<<it->second<<std::endl;
  // The less safe way (but nicer)
  // Here if Silvia is not present IT WILL BE INSERTED!
  auto numberSilvia = rub["Silvia"];
  std::cout<<"Silvia's number= "<<numberSilvia<<std::endl;
  // Search by value
  it=std::find_if(rub.begin(),rub.end(),[&numberSilvia](auto const & i)
                                     {return i.second==numberSilvia;});
  if (it !=rub.end())
    std::cout<<"The name corresponding to "<<it->second<<" is "<<it->first<<std::endl;
  // Take out (I don't like Silvia anymore :)
  rub.erase(it); // Silvia has been erased

  
  // Now some more complex stuff
  // Modifying a key with the new tools of C++17
  // more efficient than erasing + inserting
  // Elise is in fact Alice, I should correct
  // Take the node handler
  auto nh = rub.extract("Elise");
  // I can check if Elise is present: if not the handler is empty!
  if (!nh.empty())
  {
    nh.key()="Alice";
    // move the handler back to rub
    rub.insert(std::move(nh));
  }
  // Let's check
  std::cout<<"The map has now "<<rub.size()<<" Elements:\n";
  // Traverse the map
  for (auto [key,value]:rub)
  {
    std::cout<<key<<" "<<value<<std::endl;
  }
  // I want to use move semantic in inserting a new element Not very
  // relevant here (our objects are small), but useful in other
  // situations. I may use insert or emplace. In this case if the
  // element is already present the element to be inserted may be lost
  std::string numberMarco="023456789";

  auto [iter,status]=rub.insert({"Marco",std::move(numberMarco)});
  // the new Marco has not been inserted since already present, but the string il lost;
  if(status==false)
  {
    std::cout<<"Marco not inserted\n";
    std::cout<<"Number of Marco size is "<<numberMarco.size()<<std::endl;
  }
// This may be irrelevant. But sometimes we want to keep the value
 numberMarco="023456789";
 // Note that with emplace and try_emplace I can avoid the bracket
 // I use tis since iter and status are already defined
 std::tie(iter,status)=rub.try_emplace("Marco",std::move(numberMarco));
   if(status==false)
  {
    std::cout<<"Marco not inserted\n";
    std::cout<<"Number of Marco size is still "<<numberMarco.size()<<std::endl;
  }
   // This time numberMarco is still alive and kicking.

   // A last utility. Maybe I want to insert or assign a new value:
   // If the key is present give a new value. If not create another entry in the map
   // It can now be done with a single call
 std::tie(iter,status)=rub.insert_or_assign("Eleonora","067899665544");
 if (status)
 {
   std::cout<<iter->first<<" was added to the map"<<std::endl;
 }
 else
 {
     std::cout<<iter->first<<" was already present"<<std::endl; 
     std::cout<<iter->first<<" The number has been changed to "<<
       iter->second<<std::endl; 
 }
// Try again, this time with Marco
std::tie(iter,status)=rub.insert_or_assign("Marco","01987654321");
 if (status)
 {
   std::cout<<iter->first<<" was added to the map"<<std::endl;
 }
 else
 {
     std::cout<<iter->first<<" was already present"<<std::endl; 
     std::cout<<" The number has been changed to "<<
       iter->second<<std::endl; 
 }

 // Lets finish up by printing again all elements
   std::cout<<"The map has "<<rub.size()<<" Elements:\n";
  // Traverse the map
  for (auto [key,value]:rub)
  {
    std::cout<<key<<" "<<value<<std::endl;
  }
  
   
}
