/*
 * main_bc.cpp
 *
 *  Created on: Nov 28, 2008
 *      Author: forma
 */
#include "setbc.hpp"
#include <iostream>
#include <algorithm>
// we need to link setbc where bcContainer is defined

int main(){
	using namespace FEM;
	using std::vector;
	//BcGet bcGet ;
	using std::cout;
	using std::endl;
	// Normally this data is read form file!
	int wallnodes[5]={1,2,5,6,9};
	int wallnodes2[3]={1,4,7};
	int farfieldfaces[3]={13,15,12};

	//BCContainer & allBC(bcGet.bConditions);
	BCContainer & allBC(boundaryConditions);
	allBC.getBC("Wall1")->set_entities(vector<int>(wallnodes,wallnodes+5));
	allBC.getBC("Wall2")->set_entities(vector<int>(wallnodes2,wallnodes2+3));
	allBC.getBC("FarField1")->set_entities(
			vector<int>(farfieldfaces,farfieldfaces+3));
	allBC.showMe();
	typedef BCContainer::const_iterator It;
	std::cout<<std::endl<<" Type Dirichet:"<<std::endl;
	std::pair<It,It> d=allBC.getAllBC(Dirichlet);
	vector<int> allDiriEntities;
	// Collect all Dirichlet entities
	for (It k=d.first;k!=d.second;++k){
		k->showMe();
		allDiriEntities.insert(allDiriEntities.end(),k->entities().begin(),
				k->entities().end());
	}
	std::sort(allDiriEntities.begin(),allDiriEntities.end());
	std::vector<int>::iterator newEnd=unique(
			allDiriEntities.begin(),allDiriEntities.end());
	std::vector<int>(allDiriEntities.begin(),newEnd).swap(allDiriEntities);
	cout<<"Collected Dirichet entities"<<endl;
	for (std::vector<int>::iterator k=allDiriEntities.begin();
			k<allDiriEntities.end();++k)cout<<*k<<" ";
	cout<<endl;
}
