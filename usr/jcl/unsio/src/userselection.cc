// ============================================================================
// Copyright Jean-Charles LAMBERT - 2008-2010                                       
// e-mail:   Jean-Charles.Lambert@oamp.fr                                      
// address:  Dynamique des galaxies                                            
//           Laboratoire d'Astrophysique de Marseille                          
//           Ple de l'Etoile, site de Chteau-Gombert                         
//           38, rue Frdric Joliot-Curie                                     
//           13388 Marseille cedex 13 France                                   
//           CNRS U.M.R 6110                                                   
// ============================================================================

#include <iostream>
#include <sstream>
#include "userselection.h"
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "assert.h"
namespace uns {

// ============================================================================
// constructor                                                                 
UserSelection::UserSelection()
{
  nbody   = 0;
  nsel    = 0;
  indx    = NULL;
  pos     = 0;
}
// ============================================================================
// destructor                                                                  
UserSelection::~UserSelection()
{
  if (indx) {    
    delete [] indx;
  }
}
// ============================================================================
// setSelection:                                                               
// according to User's selection (string select) and the differents components 
// (ranges, components) inside the snapshot, the POV is filled up.             
bool UserSelection::setSelection(const std::string _sel,
                                 const ComponentRangeVector * _crv)
{
  pos = 0; // current component selection
  select =_sel; // cop selection              
  crv    = _crv;   // link component range vector
  assert(crv);                   // must not be NULL
  assert((*crv)[0].type=="all"); // first entry must be "all"    
  nbody = (*crv)[0].n;           // #bodies max in the snapshot  
  if (indx) {
    delete [] indx;
  }
  indx = new t_indexes_tab[nbody];
  for (int i=0;i<nbody;i++) {
    indx[i].i=-1;    // reset indexes
    indx[i].p=10000; // set position to high number
  }
  nsel = 0;
  min = max = -1;
  crvsel.clear();  // crv to stro selected component
  bool status=parse();
  if (status || 1 ) { // we force here
    qsort(indx,nbody,sizeof(t_indexes_tab),UserSelection::comparePos);
    // resise crvsel
    crvResize(crvsel);
  }
  return status;
}
// ============================================================================
// parse:                                                                      
bool UserSelection::parse()
{
  bool status=true;
  std::string current_s,next_s;
  next_s = select;
  while ((current_s=parseString(next_s)) != "") {// && (status)) {  // || 1 force parsing
    status=checkComponent(current_s);
  }
  return status;
}
// ============================================================================
// checkComponent                                                              
// find out the component stored in the string                                 
bool UserSelection::checkComponent(const std::string comp)
{
  bool status=true;
  int fail;
  if ((fail=isRange(comp)))
    if((fail=isComponent(comp)))
      status=false;
  return status;
}
// ============================================================================
// isRange                                                                     
// return true is the component is a range of particles                        
int UserSelection::isRange(const std::string comp)
{
  int status;
  std::vector<int> store;
  int ppos=0;
  bool stop=false;
  int cpt=0;
  while (! stop) {
    size_t found = comp.find(':',ppos);
    if (found!=std::string::npos) {
      if (found > (size_t) (ppos)) {
	cpt++;
	std::string str=comp.substr(ppos,found-ppos);
	std::istringstream ss(str);
	int val;
	ss>>val;
	store.push_back(val);
      }
      ppos=found+1; //
    } else { // no more ":"
      if (cpt>0) {
	std::string str=comp.substr(ppos);
	std::istringstream ss(str);
	int val;
	ss>>val;
	store.push_back(val);
      }
      stop=true;
    }
  }
  for (std::vector<int>::iterator it=store.begin(); it!=store.end(); it++) {
    //std::cerr << " range i ="<< it-store.begin() << "\n";
    //std::cerr << " value   =" << *it << "\n";
  }
  if (cpt>0) { // one ":" has been found
    int step=1;
    int first=store[0];
    int last=first;
    if (store.size()>1) {
      last = store[1];
    }
    if (store.size()>2) {
      step = store[2];
    }
    assert(last>=first);
    int npart=last-first+1; // #part
    assert(npart<=nbody);
    fillIndexes(comp,first,last,step,pos); // fill indexes array
    pos++;
    status=0;
  }
  else {
    status=1;
  }
  return status;
}
// ============================================================================
// isComponent                                                                 
// return true is the component is component type                              
int UserSelection::isComponent(const std::string comp)
{
  int status;
  // Regular expression => all|halo|disk ......
  const char *  rx[] = {"all","halo","disk","bulge","stars","gas","bndry","halo2",NULL};
  int i=0;
  int match=-1;
  while (rx[i] && match==-1) {
    if (rx[i]) {
      std::string tmp=rx[i];
      if (tmp == comp) match=i;
    }
    i++;
  }
  if (match == -1) { // not match
    status=1;        // misformated
  }
  else {
    int first,last,step=1;
    // get component's type
    std::string type=rx[match];
    int offset;
    int icrv=ComponentRange::getIndexMatchType(crv,type,offset);
    if (icrv != -1 ) {
      assert(icrv<(int)crv->size());
      first=(*crv)[icrv].first;
      last =(*crv)[icrv].last;
      assert(last>=first);
      int npart=last-first+1; // #part
      assert(npart<=nbody);
      status=0;
      fillIndexes(comp,first,last,step,pos); // fill indexes array
      pos++;
    }
    else {
      status=4;      // type does not exist
    }
  }
  return status;
}
// ============================================================================
// fillIndexes                                                                 
// fill indexes array with new range of particles                              
void UserSelection::fillIndexes(const std::string comp,const int first, const int last, const int step, int pos)
{
  int npart=(last-first+1)/step;
  assert(npart<=nbody);
  for (int i=first; i<=last; i+=step) {
    if (indx[i].i==-1) nsel++; // one more particles
    indx[i].i = i;             // set new particles 
    indx[i].p = pos;
    assert(nsel<=nbody);
  }
  // store selected component in a CRV vector
  uns::ComponentRange cr;
  cr.setData(first,last);
  cr.setType(comp);
  cr.setPosition(pos);
  crvsel.push_back(cr);
  
  int nlast=first+npart-1;
  // step =1  by default now
  findMinMax(first,nlast); // find min and max for later processing
                           // like resizing cvrsel according user selected
                           // data
}
// ============================================================================
// parseString                                                                 
// return the string at the position after the next ',' otherwise ""           
std::string UserSelection::parseString(std::string & next_string)
{
  std::string return_string;
  std::string::size_type coma=next_string.find(",",0);  // try to find ","
  if (coma != std::string::npos) { // found ","
    return_string = next_string.substr(0,coma);
    next_string   = next_string.substr(coma+1,next_string.length());
  } else {                         // not found
    return_string = next_string;
    next_string = "";
  }
  return return_string;
}
// ============================================================================
// findMinMax                                                                  
//                                                                             
void UserSelection::findMinMax(const int first, const int last)
{
  if (min == -1 ) min=first;
  if (max == -1 ) max=last;
  if (first<min) {
    min = first;
    max = last;
  }
}
// ============================================================================
// crvResize                                                                  
//                                                                             
void UserSelection::crvResize(ComponentRangeVector & mycrv)
{
//  std::cerr << "BEFORE Listing of the Users' selectef CRV\n";
//  ComponentRange::list(&mycrv);
  
  // sort mycrv according to its smaller "first" field
  std::sort(mycrv.begin(),mycrv.end(),ComponentRange::compareFirst);
  
  int new_max=max;
  int next_first=0;
  // p
  for (std::vector<ComponentRange>::iterator icrv=mycrv.begin(); icrv<mycrv.end();icrv++) {
    new_max = crvPermut((*icrv),min,new_max,next_first);
  }
  // sort mycrv according to its initial "positions"
  std::sort(mycrv.begin(),mycrv.end(),ComponentRange::comparePos);
  
  //
  //std::cerr << "AFTER Listing of the Users' selectef CRV\n";
  //ComponentRange::list(&mycrv);
}
// ============================================================================
// crvResize                                                                  
//                                                                             
int UserSelection::crvPermut(ComponentRange& cr,int min, int max, int &next_first)
{
  int ret;
  int npart=(cr.last-cr.first+1);
  if (cr.last > max) ret=cr.last;  // new max
  else ret=max;              // keep max

  if (cr.first <= max ) {       // first <= max
    cr.first -= min;            // left move
    cr.last=cr.first+npart-1;      // resize last
  }
  else {                     // first > max
    cr.first = next_first;      // move to the right of latest object
    cr.last=cr.first+npart-1;      // resize last
  }
  next_first=cr.last+1;
  cr.setData(cr.first,cr.last,cr.type);
  return ret; 
}
} //namespace uns
