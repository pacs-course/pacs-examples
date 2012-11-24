/*
 * bc_factory1.cpp
 *
 *  Created on: Nov 28, 2008
 *      Author: forma
 */
#include <algorithm>
#include <iostream>
#include "bcContainer.hpp"


namespace FEM{
bool BCContainer::addBC(BCBase const & bc){
	std::pair<iterator,bool> p=this->insert(bc);
	// Test for double insertion
	if (!p.second) return false;
	// Adjourn the map and return false if the name is 
        // already stored
	if(!M_map.insert(std::make_pair(bc.name(),p.first)).second){
	  std::cerr<<"ERROR: Bcondition named "<<bc.name()
		   <<"is present with two different types!"<<std::endl;
	  return false;
	}
	return true;
}

BCContainer::const_iterator
BCContainer::getBC(BCName const & n) const{
	typedef std::map<BCName,iterator>::const_iterator It;
	It tmp=M_map.find(n);
	if (tmp==M_map.end()) return this->end();
	else return tmp->second;
}

BCContainer::const_iterator BCContainer::getBC(int const n) const{
	return this->getBC(intToBCName(n));
}

std::pair<BCContainer::const_iterator,BCContainer::const_iterator>
BCContainer::getAllBC(BCType type) const{
	// Create a dummy BCBase object
	BCBase tmp(type);
	return std::equal_range(this->begin(),this->end(),tmp,compareOnType);
}

void BCContainer::showMe(std::ostream & stream) const{
	for(const_iterator i=this->begin(); i!=this->end(); ++i ){
		i->showMe(stream);
	}
}
}





