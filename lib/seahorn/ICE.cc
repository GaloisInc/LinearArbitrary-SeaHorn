#include "seahorn/ICE.hh"
#include "seahorn/HornifyModule.hh"
#include "seahorn/HornClauseDBTransf.hh"
#include "seahorn/HornClauseDB.hh"
#include "seahorn/GuessCandidates.hh"

#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"

#include "ufo/Expr.hpp"
#include "ufo/Smt/Z3n.hpp"
#include "ufo/Smt/EZ3.hh"
#include <vector>
#include <boost/logic/tribool.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional/optional.hpp>
#include <boost/tokenizer.hpp>
#include "boost/range/algorithm/reverse.hpp"
#include "seahorn/HornClauseDBWto.hh"
#include <algorithm>

#include "ufo/Stats.hh"

#include <iostream>
#include <string>

#include <stdlib.h>
#include <stdio.h>

using namespace llvm;

static llvm::cl::opt<std::string>
ICEInvDump("horn-ice-inv-dump", llvm::cl::desc("ICE Invariants Dump File:"),
               llvm::cl::init(""), llvm::cl::value_desc("filename"));

static llvm::cl::opt<std::string>
C5ExecPath("horn-ice-c5-exec-path", llvm::cl::desc("C5 Executable File Path:"),
               llvm::cl::init(""), llvm::cl::value_desc("filename"));

static llvm::cl::opt<unsigned>
RuleSampleLen ("horn-rule-sample-length", cl::Hidden, cl::init (101));

static llvm::cl::opt<unsigned>
RuleSampleWidth("horn-rule-sample-width", cl::Hidden, cl::init (1));

static llvm::cl::opt<std::string>
SVMExecPath("horn-ice-svm-exec-path", llvm::cl::desc("SVM Executable File Path:"),
               llvm::cl::init(""), llvm::cl::value_desc("filename"));

static llvm::cl::opt<unsigned>
SVMCParameter("horn-ice-svm-c-paramter", cl::Hidden, cl::init (100000));

static llvm::cl::opt<unsigned>
SVMCoeffBound("horn-ice-svm-coeff-bound", cl::Hidden, cl::init (0));

static llvm::cl::opt<unsigned>
SVMAHyperplane("horn-ice-svm-a-hyperplane", cl::Hidden, cl::init (0));

static llvm::cl::opt<unsigned>
ICEMod("horn-ice-mod", cl::Hidden, cl::init (0));

static llvm::cl::opt<unsigned>
ICESVMFreqPos("horn-ice-svm-call-freq-pos", cl::Hidden, cl::init (50));

static llvm::cl::opt<unsigned>
ICESVMFreqNeg("horn-ice-svm-call-freq-neg", cl::Hidden, cl::init (100));

static llvm::cl::opt<unsigned>
LC("horn-ice-lc", cl::Hidden, cl::init (0));

static llvm::cl::opt<unsigned>
ICECatch("horn-ice-svm-caching", cl::Hidden, cl::init (0));

static llvm::cl::opt<unsigned>
ICELocalStrengthen("horn-ice-local-strengthening", cl::Hidden, cl::init(0));

static llvm::cl::opt<unsigned>
ICEOct("horn-ice-oct", cl::Hidden, cl::init(1));

static llvm::cl::opt<unsigned>
ICEICE("horn-ice-ice", cl::Hidden, cl::init(0));

static llvm::cl::opt<unsigned>
ICEPred("horn-ice-pred", cl::Hidden, cl::init(0));

namespace seahorn
{
  #define SAT_OR_INDETERMIN true
  #define UNSAT false

  /*ICEPass methods begin*/

  char ICEPass::ID = 0;

  bool ICEPass::runOnModule (Module &M)
  {
    HornifyModule &hm = getAnalysis<HornifyModule> ();

    Stats::resume ("ICE inv");
    ICE ice(hm);
    ice.setupC5();
    ice.genInitialCandidates(hm.getHornClauseDB());
    ice.runICE();
    LOG("ice-res", errs() << "RUN ICE SUCCESSCULLY\n";);
    // LOG("ice", errs() << "RUN ICE SUCCESSCULLY\n";);
    Stats::stop ("ICE inv");

    return false;
  }

  void ICEPass::getAnalysisUsage (AnalysisUsage &AU) const
  {
    AU.addRequired<HornifyModule> ();
    AU.setPreservesAll();
  }

  /*ICEPass methods end*/

  /*ICE methods begin*/

  void ICE::saveInvsToSmtLibFile()
  {
	  auto &db = m_hm.getHornClauseDB();
	  ZSolver<EZ3> solver(m_hm.getZContext());

	  for(Expr rel : db.getRelations())
	  {
		  ExprVector arg_list;
		  for(int i=0; i<bind::domainSz(rel); i++)
		  {
			  Expr arg_i_type = bind::domainTy(rel, i);
			  Expr arg_i = bind::fapp(bind::constDecl(variant::tag(bind::fname(rel), variant::variant(i, mkTerm<std::string> ("V", rel->efac ()))), arg_i_type));
			  arg_list.push_back(arg_i);
		  }
		  Expr fapp = bind::fapp(rel, arg_list);
		  Expr cand_app = m_candidate_model.getDef(fapp);
		  LOG("ice", errs() << "HEAD: " << *fapp << "\n";);
		  LOG("ice", errs() << "CAND: " << *cand_app << "\n";);

		  solver.assertExpr(mk<IMPL>(fapp, cand_app));
	  }
	  std::ofstream ofs(ICEInvDump.c_str());
	  solver.toSmtLib(ofs);
  }

  void ICE::addInvarCandsToProgramSolver()
  {
	  auto &db = m_hm.getHornClauseDB();
	  for(Expr rel : db.getRelations())
	  {
		  ExprVector arg_list;
		  for(int i=0; i<bind::domainSz(rel); i++)
		  {
			  Expr arg_i_type = bind::domainTy(rel, i);
			  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
			  arg_list.push_back(arg_i);
		  }
		  Expr fapp = bind::fapp(rel, arg_list);
		  Expr cand_app = m_candidate_model.getDef(fapp);
		  LOG("candidates", errs() << "HEAD: " << *fapp << "\n";);
		  LOG("candidates", errs() << "CAND: " << *cand_app << "\n";);
		  if(!isOpX<TRUE>(cand_app))
		  {
			  LOG("candidates", errs() << "ADD CONSTRAINT\n";);
			  db.addConstraint(fapp, cand_app);
		  }
	  }
  }

  // Fixme. Helper function should be put into a util file
  std::vector<std::string> split_string(const std::string& str,
                                        const std::string& delimiter)
  {
      std::vector<std::string> strings;

      std::string::size_type pos = 0;
      std::string::size_type prev = 0;
      while ((pos = str.find(delimiter, prev)) != std::string::npos)
      {
          strings.push_back(str.substr(prev, pos - prev));
          prev = pos + 1;
      }

      // To get the last substring (or only, if delimiter is not found)
      strings.push_back(str.substr(prev));

      return strings;
  }

  void ICE::svmLearn (Expr targetName) { // (ExprVector targets) {
	  auto &db = m_hm.getHornClauseDB();

	  if (targetName == NULL && ICECatch == 0) {
		  m_svmattr_name_to_expr_map.clear ();
		  m_svmattr_name_to_str_map.clear ();
	  }

//	  for (Expr target : targets) {
//		  LOG("ice", errs() << "Hyperlane Learning for " << *target << "\n";);
//
//		  auto rels_it = db.getRelations().begin();
//		  while (bind::fname(*rels_it) != target) {
//			  rels_it++;
//			  assert (rels_it != targets.end());
//		  }
//		  Expr rel = *rels_it;

	  for (Expr rel : db.getRelations()) {
		  if (targetName != NULL && ICECatch == 0) {
			  if (targetName != bind::fname(rel)) continue;
			  else { // Remove previously found attributes.
				  Expr C5_rel_name = m_rel_to_c5_rel_name_map.find(bind::fname(rel))->second;
				  std::stringstream ossA;
				  ossA << C5_rel_name;
				  ExprMap::iterator itr1 = m_svmattr_name_to_expr_map.begin();
				  while (itr1 != m_svmattr_name_to_expr_map.end()) {
					  std::stringstream ossB;
					  ossB << itr1->first;
				      if (ossB.str().find(ossA.str()) != -1) {
				         itr1 = m_svmattr_name_to_expr_map.erase(itr1);
				      } else {
				         ++itr1;
				      }
				  }
				  std::map<Expr, std::string>::iterator itr2 = m_svmattr_name_to_str_map.begin();
				  while (itr2 != m_svmattr_name_to_str_map.end()) {
					  std::stringstream ossB;
					  ossB << itr2->first;
					  if (ossB.str().find(ossA.str()) != -1) {
						 itr2 = m_svmattr_name_to_str_map.erase(itr2);
					  } else {
						 ++itr2;
					  }
				  }
			  }
		  }

		  // Excluse boolean variables for svm learning
		  std::vector<int> exclusives;
		  ExprVector arg_list;
		  for(int i=0; i<bind::domainSz(rel); i++)
		  {
			  if (unknowns[rel][i]) { // Exclude unknowns from invariant inference.
				  exclusives.push_back(i);
			  	  continue;
			  }
			  Expr arg_i_type = bind::domainTy(rel, i);
			  std::ostringstream oss;
			  oss << arg_i_type;
			  if (oss.str().compare("BOOL") == 0) {
				  exclusives.push_back(i);
			  } else {
				  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
				  arg_list.push_back(arg_i);
			  }
		  }

		  Expr C5_rel_name = m_rel_to_c5_rel_name_map.find(bind::fname(rel))->second;

		  LOG("ice", errs() << "SVM DATA FILES ARE GENERATING\n";);
		  //generate .data file
		  std::ofstream data_of(m_C5filename + ".svm.data");
		  if(!data_of) return;

		  int pn, nn;
		  pn = nn = 0;

		  for(auto it = m_cex_list.begin(); it!=m_cex_list.end(); ++it)
		  {
			  if (it->getPredName() == bind::fname(rel)) {
				  if(m_pos_data_set.count(*it) != 0)
				  {
					  DataPoint pos_dp = *it;

					  data_of << "1";
					  pn ++;

					  int ind = 0;
					  for(Expr attr : pos_dp.getAttrValues())
					  {
						  // Not excluded as a boolean var.
						  if (exclusives.empty() || std::find(exclusives.begin(), exclusives.end(), ind) == exclusives.end()) {
							  data_of << " " << *attr;
						  }
						  ind ++;
					  }

					  data_of << "\n";
				  }
				  else if(m_neg_data_set.count(*it) != 0)
				  {
					  DataPoint neg_dp = *it;

					  data_of << "0";
					  nn ++;

					  int ind = 0;
					  for(Expr attr : neg_dp.getAttrValues())
					  {
						  if (exclusives.empty() || std::find(exclusives.begin(), exclusives.end(), ind) == exclusives.end()) {
							  data_of << " " << *attr;
						  }
						  ind ++;
					  }

					  data_of << "\n";
				  }
			  }
		  }

		  data_of.close();

		  // Call SVM to learn invariants

		  LOG("ice", errs() << "SVM DATA FILES ARE GENERATED\n";);

		  FILE *fp;
		  FILE *wp;
		  wp = fopen("SVM_temp","w+");
		  std::ostringstream oss;
		  oss << C5_rel_name;
		  std::string command = SVMExecPath +
				  " -c " + std::to_string(SVMCParameter) +
				  " -t " + std::to_string(SVMCoeffBound) +
				  " -a " + std::to_string(SVMAHyperplane) +
				  " -v " + std::to_string(arg_list.size()) +
				  " -p " + std::to_string(pn) +
				  " -n " + std::to_string(nn) +
				  " -g " + std::to_string(LC) +
				  " -f " + oss.str() + " " +
				  m_C5filename + ".svm.data";

		  LOG("ice", errs() << "Call SVM: " << command << "\n");

		  std::string access = "r";
		  if((fp = popen(command.c_str(), access.c_str())) == NULL)
		  {
			  perror("popen failed!\n");
			  return;
		  }
		  char buf[1024];

		  size_t status = fread(buf, sizeof(char), sizeof(buf), fp);
		  if(status == 0)
		  {
			  LOG("ice", errs() << "read from popen failed!\n";);
			  return;
		  }
		  fwrite(buf, 1, sizeof(buf), wp);

		  pclose(fp);
		  fclose(wp);

		  //std::ifstream  fsrc(m_C5filename + ".svm.data", std::ios::binary);
		  //std::ofstream  fdst(m_C5filename + std::to_string(n_svm_calls) + ".svm.data", std::ios::binary);
		  //fdst << fsrc.rdbuf();

		  n_svm_calls ++;

		  //parse the output of SVM
/*        std::ifstream if_svm(m_C5filename + ".inv");
		  std::ostringstream svm_buf;

		  char ch;
		  while(svm_buf && if_svm.get(ch))
		  { svm_buf.put(ch); }

		  std::string svm_string =  svm_buf.str();

		  Expr candidate;
		  ExprVector currVec;
		  std::vector<Expr> sol;

		  Expr zero = mkTerm<mpz_class>(0, rel->efac());
		  std::vector<std::string> lines = split_string (svm_string, "\n");
		  for (auto itr = lines.begin(); itr != lines.end(); itr++) {
			  std::string line = *itr;
			  if (line.compare("true") == 0) {
				  currVec.push_back(mk<TRUE> (rel->efac ()));
				  break;
			  } else if (line.compare("false") == 0) {
				  currVec.push_back(mk<FALSE> (rel->efac ()));
				  break;
		  	  } else if (line.compare("DISJUNCT") == 0) {
		  		  if (currVec.size() == 1)
		  			  sol.push_back (currVec.front());
		  		  else sol.push_back(mknary<AND> (currVec));
				  currVec.clear ();
			  } else {
				  ExprVector addargs;
				  std::vector<std::string> thetas = split_string (line, " ");
				  addargs.push_back (mkTerm<mpz_class>(atoi(thetas[0].c_str()), rel->efac()));
				  for (int i = 1; i < thetas.size(); i++) {
					  int coeff = atoi(thetas[i].c_str());
					  if (coeff == 0)
						  continue;

					  Expr c = mkTerm<mpz_class>(atoi(thetas[i].c_str()), rel->efac());
					  addargs.push_back (mk<MULT> (c, arg_list.at(i-1)));
				  }
				  currVec.push_back (mk<GEQ>(mknary<PLUS> (addargs), zero));
			  }
		  }
*/
		  std::ifstream if_svm(m_C5filename + ".attr");
		  std::ostringstream svm_buf;

		  char ch;
		  while(svm_buf && if_svm.get(ch))
		  { svm_buf.put(ch); }

		  if_svm.close();

		  std::string svm_string =  svm_buf.str();
		  Expr zero = mkTerm<mpz_class>(0, rel->efac());
		  std::vector<std::string> lines = split_string (svm_string, "\n");
		  int ind = ICECatch == 0 ? 0 : m_svmattr_name_to_expr_map.size();  //0;
		  bool dt_learning = true;
		  for (auto itr = lines.begin(); itr != lines.end(); itr++) {
			  std::string line = *itr;
			  if (line.compare("true") == 0) {
				  //Expr fapp = bind::fapp(rel, arg_list);
				  //m_candidate_model.addDef(fapp, mk<TRUE> (rel->efac ()));
				  // Not necessary for decision tree learning
				  //dt_learning = false;
				  //break;
			  } else if (line.compare("false") == 0) {
				  //Expr fapp = bind::fapp(rel, arg_list);
				  //m_candidate_model.addDef(fapp, mk<FALSE> (rel->efac ()));
				  // Not necessary for decision tree learning
				  //dt_learning = false;
				  //break;
			  } else {
				  ExprVector addargs;
				  std::ostringstream attross;
				  std::vector<std::string> thetas = split_string (line, " ");
//				  bias will be adjusted by decision tree.
//				  addargs.push_back (mkTerm<mpz_class>(atoi(thetas[0].c_str()), rel->efac()));
				  //bool nonOctagon = false; // Ignore any octagon hyperlanes which we already cover.
				  for (int i = 1; i < thetas.size(); i++) {
					  int coeff = atoi(thetas[i].c_str());
					  if (coeff == 0)
						  continue;

					  //if (coeff != 1 && coeff != -1) nonOctagon = true;

					  Expr c = mkTerm<mpz_class>(atoi(thetas[i].c_str()), rel->efac());
					  addargs.push_back (mk<MULT> (c, arg_list.at(i-1)));

					  attross << "(" << thetas[i].c_str() << "*" << C5_rel_name << "!" << arg_list.at(i-1) << ")+";
				  }

				  if (addargs.size () > 1 /*&& nonOctagon*/) {
					  Expr arg_i_type = sort::intTy(rel->efac());
					  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(ind, mkTerm<std::string> ("SVM", rel->efac ())), arg_i_type));
					  Expr attr_name_i = variant::tag(C5_rel_name, bind::fname(bind::fname(arg_i)));
					  m_svmattr_name_to_expr_map.insert(std::make_pair(attr_name_i, mknary<PLUS> (addargs)));
							  //mk<GEQ>(mknary<PLUS> (addargs), zero)));
					  std::string strrep = attross.str();
					  m_svmattr_name_to_str_map.insert(std::make_pair(attr_name_i, strrep.substr(0, strrep.length() - 1)));
					  LOG("ice", errs() << "SVM inferred a hyperlane: " << strrep.substr(0, strrep.length() - 1) << "\n");
				  }
			  }
			  ind++;
		  }
/*
 	 	  // And then make a call to C5 to obtain
		  LOG("ice", errs() << "CALL C5 TO GENERATEs INVARIANT of SVM CLASSIFIER\n";);
		  // Decision tree learning composing classifier
		  if (dt_learning) {
			  wp = fopen("C5_temp","w+");
			  command = C5ExecPath + " -I 1 -m 1 -f " + m_C5filename;
			  if((fp = popen(command.c_str(), access.c_str())) == NULL)
			  {
				  perror("popen failed!\n");
				  return;
			  }

			  status = fread(buf, sizeof(char), sizeof(buf), fp);
			  if(status == 0)
			  {
				  LOG("ice", errs() << "read from popen failed!\n";);
				  return;
			  }
			  fwrite(buf, 1, sizeof(buf), wp);

			  pclose(fp);
			  fclose(wp);

			  //parse the .json file to ptree
			  std::ifstream if_json(m_C5filename + ".json");
			  std::ostringstream json_buf;

			  while(json_buf && if_json.get(ch))
			  { json_buf.put(ch); }

			  std::string json_string = json_buf.str();

			  boost::property_tree::ptree pt;
			  std::stringstream ss(json_string);
			  try
			  { boost::property_tree::json_parser::read_json(ss, pt); }
			  catch(boost::property_tree::ptree_error & e)
			  { LOG("ice", errs() << "READ JSON ERROR!\n";); return; }

			  // Fixme. Bad code on calling decision tree learning. totally 3 places.
			  ExprVector tv;
			  tv.push_back (bind::fname(*(db.getRelations().begin())));
			  tv.push_back (target);
			  //parse ptree to invariant format
			  convertPtreeToInvCandidate(pt, tv);
		  }
*/

/*		  if (currVec.size() == 1)
			  sol.push_back (currVec.front());
		  else
			  sol.push_back(mknary<AND>(currVec));

		  if (sol.size () == 1) {
			  candidate = sol.front();
		  } else {
			  candidate = mknary<OR> (sol);
		  }

		  Expr fapp = bind::fapp(rel, arg_list);
		  m_candidate_model.addDef(fapp, candidate);

		  LOG("ice", errs() << "Invariant Learning for " << *target << " succeeded.\n";);     */
	  }

	  //Fixme: enforce to prove all queries are unsat.
/*	  invalidateQueries(db);

	  extractFacts(db, targets);

	  LOG("ice", errs() << "NEW CANDIDATES MAP:\n";);
	  for(Expr rel : db.getRelations())
	  {
		  ExprVector arg_list;
		  for(int i=0; i<bind::domainSz(rel); i++)
		  {
			  Expr arg_i_type = bind::domainTy(rel, i);
			  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
			  arg_list.push_back(arg_i);
		  }
		  Expr fapp = bind::fapp(rel, arg_list);
		  Expr cand = m_candidate_model.getDef(fapp);
		  LOG("ice", errs() << *fapp << " : " << *cand << "\n";);
	  }
*/
  }


  void ICE::setupC5() {
	  m_C5filename = "FromCmd";

	  //convert predicate names to the name format of C5
	  auto &db = m_hm.getHornClauseDB();
	  int rel_index = 0;
	  for(Expr rel : db.getRelations())
	  {
		  Expr C5_rel_name = variant::variant(rel_index, mkTerm<std::string>(std::string("PRED"), rel->efac()));
		  m_rel_to_c5_rel_name_map.insert(std::make_pair(bind::fname(rel), C5_rel_name));
		  m_c5_rel_name_to_rel_map.insert(std::make_pair(C5_rel_name, bind::fname(rel)));
		  rel_index ++;
	  }

	  //consider whether collecting integer constants in the rule is useful.
	  if (ICEMod) extractConstants(db);
	  //consider unknowns which are definitely not useful in invariant inference.
	  extractUnknowns(db);

	  //print the map from predicate name to C5-form predicate name
	  LOG("ice", errs() << "REL NAME TO C5 NAME MAP:\n";);
	  for(auto it = m_rel_to_c5_rel_name_map.begin(); it != m_rel_to_c5_rel_name_map.end(); ++it)
	  {
		  LOG("ice", errs() << *(it->first) << ", " << *(it->second) << "\n";);
	  }
  }

  //Set .names file and .interval file
  //Only set up for the predicate we want to re-Learn.
  void ICE::initC5(ExprVector targets)
  {
	  auto &db = m_hm.getHornClauseDB();

	  m_attr_name_to_expr_map.clear();

	  m_pred_name_to_expr_map.clear();

	  std::ofstream names_of(m_C5filename + ".names");
	  if(!names_of)return;

	  std::ofstream intervals_of(m_C5filename + ".intervals");
	  if(!intervals_of)return;

	  int lowerInterval = 2;
	  int upperInterval = 2;

	  names_of << "invariant.\n";

	  //first attribute is the predicate names
	  names_of << "$pc: ";
	  int counter=0;
	  for(Expr rel : db.getRelations())
	  {
		  if(std::find(targets.begin(), targets.end(), bind::fname(rel)) != targets.end()) {
			  Expr C5_rel_name = m_rel_to_c5_rel_name_map.find(bind::fname(rel))->second;
			  if(counter == targets.size()-1)
			  {
				  names_of << *C5_rel_name << ".\n";
			  }
			  else
			  {
				  names_of << *C5_rel_name << ",";
			  }
			  counter ++;
		  }
	  }

	  //each argument of each predicate is an attribute
	  for(Expr rel : db.getRelations())
	  {
		  if(std::find(targets.begin(), targets.end(), bind::fname(rel)) != targets.end()) {
			  Expr C5_rel_name = m_rel_to_c5_rel_name_map.find(bind::fname(rel))->second;
			  for(int i=0; i<bind::domainSz(rel); i++)
			  {
				  if(isOpX<INT_TY>(bind::domainTy(rel, i)) || isOpX<BOOL_TY>(bind::domainTy(rel, i)))
				  {
					  if (unknowns[rel][i]) // Exclude unknowns from invariant inference.
						  continue;
					  Expr arg_i_type = bind::domainTy(rel, i);
					  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
					  Expr attr_name_i = variant::tag(C5_rel_name, bind::fname(bind::fname(arg_i)));
					  m_attr_name_to_expr_map.insert(std::make_pair(attr_name_i, arg_i));
					  names_of << attr_name_i << ": continuous.\n";
					  upperInterval ++;
				  }
				  else
				  {
					  LOG("ice", errs() << "NOT INT OR BOOL TYPE!\n";);
				  }
			  }
			  //implicit attributes which have the form x % n.
			  if (ICEMod > 0 && !ruleConstants.empty()) {
				  for(int i=0; i<bind::domainSz(rel); i++)
				  {
					  if (unknowns[rel][i]) // Exclude unknowns from invariant inference.
						  continue;
					  for (int cons : ruleConstants) {
						  if(isOpX<INT_TY>(bind::domainTy(rel, i)))
						  {
							  Expr arg_i_type = bind::domainTy(rel, i);
							  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
							  Expr attr_name_i = variant::tag(C5_rel_name, bind::fname(bind::fname(arg_i)));
							  names_of << attr_name_i << "mod" << cons << ":= ("
									  << attr_name_i << " % " << cons << " + " << cons << ") % " << cons << ".\n";
							  upperInterval ++;
						  }
					  }
				  }
			  }
			  //implicit attributes which have the form x1 +/- x2
			  if (ICEOct) {
			  for(int i=0; i<bind::domainSz(rel); i++)
			  {
				  if (unknowns[rel][i]) // Exclude unknowns from invariant inference.
					  continue;
				  for(int j=i+1; j<bind::domainSz(rel); j++)
				  {
					  if (unknowns[rel][j]) // Exclude unknowns from invariant inference.
						  continue;
					  if(isOpX<INT_TY>(bind::domainTy(rel, i)) && isOpX<INT_TY>(bind::domainTy(rel, j)))
					  {
						  Expr arg_type = bind::domainTy(rel, i);
						  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_type));
						  Expr arg_j = bind::fapp(bind::constDecl(variant::variant(j, mkTerm<std::string> ("V", rel->efac ())), arg_type));
						  Expr attr_name_i = variant::tag(C5_rel_name, bind::fname(bind::fname(arg_i)));
						  Expr attr_name_j = variant::tag(C5_rel_name, bind::fname(bind::fname(arg_j)));
						  names_of << attr_name_i << "+" << attr_name_j << ":= " << attr_name_i << " + " << attr_name_j << ".\n";
						  names_of << attr_name_i << "-" << attr_name_j << ":= " << attr_name_i << " - " << attr_name_j << ".\n";
						  upperInterval += 2;
					  }
				  }
			  }
			  }
			  if (ICEPred) { // Add a number of fixed relaional predicates ... Fixme: Should be parameterized ...
			  int ind = 0;
			  for (int v = -2; v <= 2; v++)
			  for(int i=0; i<bind::domainSz(rel); i++)
			  {
				  if(isOpX<INT_TY>(bind::domainTy(rel, i)))
				  {
					  Expr arg_type = bind::domainTy(rel, i);
					  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_type));
					  Expr attr_name_i = variant::tag(C5_rel_name, bind::fname(bind::fname(arg_i)));

					  // String representation
					  std::ostringstream ross;
					  ross << attr_name_i << " <= " << v;

					  Expr arg_ind_type = sort::intTy(rel->efac());
					  Expr arg_ind = bind::fapp(bind::constDecl(variant::variant(ind, mkTerm<std::string> ("ICEPred", rel->efac ())), arg_ind_type));
					  Expr attr_name_ind = variant::tag(C5_rel_name, bind::fname(bind::fname(arg_ind)));
					  Expr val = mkTerm<mpz_class>(v, rel->efac());
					  m_pred_name_to_expr_map.insert(std::make_pair(attr_name_ind, mk<LEQ>(arg_i, val)));
					  ind++;

					  names_of << *attr_name_ind << ":= " << ross.str() << ".\n";
					  upperInterval ++;

					  ross.str(""); ross.clear();
					  ross << v << " <= " << attr_name_i;

					  arg_ind = bind::fapp(bind::constDecl(variant::variant(ind, mkTerm<std::string> ("ICEPred", rel->efac ())), arg_ind_type));
					  attr_name_ind = variant::tag(C5_rel_name, bind::fname(bind::fname(arg_ind)));
					  m_pred_name_to_expr_map.insert(std::make_pair(attr_name_ind, mk<LEQ>(val, arg_i)));
					  ind++;

					  names_of << *attr_name_ind << ":= " << ross.str() << ".\n";
					  upperInterval ++;
				  }
			  }
			  for(int i=0; i<bind::domainSz(rel); i++)
			  {
				  if (unknowns[rel][i]) // Exclude unknowns from invariant inference.
					  continue;
				  for(int j=0; j<bind::domainSz(rel); j++)
				  {
					  if (j == i)
						  continue;
					  if (unknowns[rel][j]) // Exclude unknowns from invariant inference.
						  continue;
					  if(isOpX<INT_TY>(bind::domainTy(rel, i)) && isOpX<INT_TY>(bind::domainTy(rel, j)))
					  {
						  Expr arg_type = bind::domainTy(rel, i);
						  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_type));
						  Expr arg_j = bind::fapp(bind::constDecl(variant::variant(j, mkTerm<std::string> ("V", rel->efac ())), arg_type));
						  Expr attr_name_i = variant::tag(C5_rel_name, bind::fname(bind::fname(arg_i)));
						  Expr attr_name_j = variant::tag(C5_rel_name, bind::fname(bind::fname(arg_j)));

						  // String representation
						  std::ostringstream ross;
						  ross << attr_name_i << " - " << attr_name_j << " <= 0";

						  Expr arg_ind_type = sort::intTy(rel->efac());
						  Expr arg_ind = bind::fapp(bind::constDecl(variant::variant(ind, mkTerm<std::string> ("ICEPred", rel->efac ())), arg_ind_type));
						  Expr attr_name_ind = variant::tag(C5_rel_name, bind::fname(bind::fname(arg_ind)));
						  Expr zero = mkTerm<mpz_class>(0, rel->efac());
						  m_pred_name_to_expr_map.insert(std::make_pair(attr_name_ind, mk<LEQ>(mk<MINUS>(arg_i, arg_j), zero)));
						  ind++;

						  names_of << *attr_name_ind << ":= " << ross.str() << ".\n";
						  upperInterval ++;
					  }
				  }
			  }
			  }
			  // attributes found by SVM -- which must be related to C5_rel_name
			  std::ostringstream ossR; ossR << C5_rel_name;
			  for(std::map<Expr, std::string>::iterator it = m_svmattr_name_to_str_map.begin(); it!= m_svmattr_name_to_str_map.end(); ++it)
			  {
				  std::ostringstream ossA; ossA << *(it->first);
				  if (ossA.str().find(ossR.str()) != std::string::npos) {
					  // This is indeed related to C5_rel_name
					  names_of << *(it->first) << ":= " << (it->second) << ".\n";
					  upperInterval ++;
				  }
			  }

			  std::string interval_line;
			  if(bind::domainSz(rel) == 0)
			  {
				  interval_line = boost::lexical_cast<std::string>(lowerInterval) + " " + boost::lexical_cast<std::string>(upperInterval) + "\n";
			  }
			  else
			  {
				  interval_line = boost::lexical_cast<std::string>(lowerInterval) + " " + boost::lexical_cast<std::string>(upperInterval - 1) + "\n";
			  }
			  intervals_of << interval_line;
			  lowerInterval = upperInterval;
			  upperInterval = lowerInterval;
		  }
	  }

      names_of << "invariant: true, false.\n";
	  names_of.close();
	  intervals_of.close();
  }

  void ICE::C5learn(ExprVector targets)
  {
	  initC5 (targets);

	  generateC5DataAndImplicationFiles(targets);

	  LOG("ice", errs() << "DATA & IMPL FILES ARE GENERATED\n";);

	  FILE *fp;
	  FILE *wp;
	  wp = fopen("C5_temp","w+");
	  std::string command = C5ExecPath + " -I 1 -m 1 -f " + m_C5filename;
	  //std::string command = "/home/chenguang/Desktop/C50-ICE/C50/c5.0dbg -I 1 -m 1 -f " + m_C5filename;
	  std::string access = "r";
	  if((fp = popen(command.c_str(), access.c_str())) == NULL)
	  {
		  perror("popen failed!\n");
		  return;
	  }
	  char buf[1024];

	  size_t status = fread(buf, sizeof(char), sizeof(buf), fp);
	  if(status == 0)
	  {
		  LOG("ice", errs() << "read from popen failed!\n";);
		  return;
	  }
	  fwrite(buf, 1, sizeof(buf), wp);

	  pclose(fp);
	  fclose(wp);

	  //parse the .json file to ptree
	  std::ifstream if_json(m_C5filename + ".json");
	  std::ostringstream json_buf;

	  char ch;
	  while(json_buf && if_json.get(ch))
	  { json_buf.put(ch); }

	  if_json.close();

	  std::string json_string =  json_buf.str();

	  boost::property_tree::ptree pt;
	  std::stringstream ss(json_string);
	  try
	  { boost::property_tree::json_parser::read_json(ss, pt); }
	  catch(boost::property_tree::ptree_error & e)
	  { LOG("ice", errs() << "READ JSON ERROR!\n";); return; }

	  //parse ptree to invariant format
	  convertPtreeToInvCandidate(pt, targets);

	  auto &db = m_hm.getHornClauseDB();

	  //Fixme: enforce to prove all queries are unsat.
	  invalidateQueries(db);

	  extractFacts(db, targets);

	  //print the invariant map after this learning round
	  LOG("ice", errs() << "NEW CANDIDATES MAP:\n";);
	  for(Expr rel : db.getRelations())
	  {
		  ExprVector arg_list;
		  for(int i=0; i<bind::domainSz(rel); i++)
		  {
			  Expr arg_i_type = bind::domainTy(rel, i);
			  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
			  arg_list.push_back(arg_i);
		  }
		  Expr fapp = bind::fapp(rel, arg_list);
		  Expr cand = m_candidate_model.getDef(fapp);
		  LOG("ice", errs() << *fapp << " : " << *cand << "\n";);
	  }
  }

  void ICE::generateC5DataAndImplicationFiles(ExprVector targets)
  {
  	  //generate .data file
  	  std::ofstream data_of(m_C5filename + ".data");
  	  if(!data_of)return;

  	  auto &db = m_hm.getHornClauseDB();

  	  LOG("ice", errs()<<"Neg sample size: " << m_neg_data_set.size() << "\n");
  	  LOG("ice", errs()<<"Pos sample size: " << m_pos_data_set.size() << "\n");

  	  for(auto it = m_cex_list.begin(); it!=m_cex_list.end(); ++it)
  	  {
  		  if (std::find(targets.begin(), targets.end(), it->getPredName()) != targets.end()) {
			  if(m_pos_data_set.count(*it) != 0)
			  {
				  DataPoint pos_dp = *it;
				  data_of << outputDataPoint(targets, pos_dp);
				  data_of << ",true\n";
			  }
			  else if(m_neg_data_set.count(*it) != 0)
			  {
				  DataPoint neg_dp = *it;
				  data_of << outputDataPoint(targets, neg_dp);
				  data_of << ",false\n";
			  }
			  else if(ICEICE && m_impl_cex_set.count(*it) != 0)
			  {
				  DataPoint impl_dp = *it;
				  data_of << outputDataPoint(targets, impl_dp);
				  data_of << ",?\n";
			  }
  		  }
  	  }

//  	  for (DataPoint dp : m_potential_pos_data_set) {
//  		  data_of << outputDataPoint(dp);
//  		  data_of << ",true\n";
//  	  }
//  	  for (DataPoint dp : m_potential_neg_data_set) {
//		  data_of << outputDataPoint(dp);
//		  data_of << ",false\n";
//	  }

  	  data_of.close();

  	  //generate .implications file
  	  if (ICEICE) {
  	  std::ofstream implications_of(m_C5filename + ".implications");
  	  if(!implications_of)return;

  	  for(std::pair<DataPoint, DataPoint> impl_pair : m_impl_pair_set)
  	  {
  		  DataPoint start_point = impl_pair.first;
  		  if (std::find(targets.begin(), targets.end(), start_point.getPredName()) != targets.end()) {
  			  std::map<DataPoint, int>::iterator it = m_data_point_to_index_map.find(start_point);
			  assert(it != m_data_point_to_index_map.end());
			  int start_index = it->second;

			  DataPoint end_point = impl_pair.second;
			  std::map<DataPoint, int>::iterator itr = m_data_point_to_index_map.find(end_point);
			  assert(itr != m_data_point_to_index_map.end());
			  int end_index = itr->second;

			  implications_of << start_index << " " << end_index << "\n";
  		  }
  	  }

  	  implications_of.close();
  	  }
  }

  std::string ICE::outputDataPoint(ExprVector targets, DataPoint p)
  {
  	  auto &db = m_hm.getHornClauseDB();
  	  std::ostringstream oss;
  	  Expr pred_name = p.getPredName();
  	  Expr C5_pred_name = m_rel_to_c5_rel_name_map.find(pred_name)->second;
  	  oss << C5_pred_name;

  	  for(Expr rel : db.getRelations())
  	  {
  		  if(bind::fname(rel) == pred_name)
  		  {
  			  int i = 0;
  			  for(Expr attr : p.getAttrValues())
  			  {
  				  if (!unknowns[rel][i]) oss << "," << *attr;
  				  i++;
  			  }
  		  }
  		  else if (std::find(targets.begin(), targets.end(), bind::fname(rel)) != targets.end())
  		  {
  			  for(int i=0; i<bind::domainSz(rel); i++)
  			  {
  				  if (!unknowns[rel][i]) oss << ",?";
  			  }
  		  }
  	  }

  	  return oss.str();
  }

  void ICE::convertPtreeToInvCandidate(boost::property_tree::ptree pt, ExprVector targets)
  {
	  auto &db = m_hm.getHornClauseDB();
	  //if only have the root node
	  if(pt.get<std::string>("children") == std::string("null"))
	  {
		  LOG("ice", errs() << "PT HAS NO CHILDREN\n";);
		  Expr candidate;
		  if(pt.get<std::string>("classification") == "true" || pt.get<std::string>("classification") == "True")
		  {
			 candidate = mk<TRUE>(db.getExprFactory());
		  }
		  else if(pt.get<std::string>("classification") == "false" || pt.get<std::string>("classification") == "False")
		  {
			 candidate = mk<FALSE>(db.getExprFactory());
		  }
		  for(Expr rel : db.getRelations())
		  {
			  ExprVector arg_list;
			  for(int i=0; i<bind::domainSz(rel); i++)
			  {
				  Expr arg_i_type = bind::domainTy(rel, i);
				  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
				  arg_list.push_back(arg_i);
			  }
			  Expr fapp = bind::fapp(rel, arg_list);
			  m_candidate_model.addDef(fapp, candidate);
		  }
		  return;
	  }

	  boost::property_tree::ptree children = pt.get_child("children");
	  auto rels_it = db.getRelations().begin();
	  for(auto child_it : children)
	  {
		  while (std::find(targets.begin(), targets.end(), bind::fname(*rels_it)) == targets.end()) {
			  rels_it++;
			  assert (rels_it != targets.end());
		  }

		  Expr candidate;

		  Expr rel = *rels_it;
		  Expr C5_rel_name = m_rel_to_c5_rel_name_map.find(bind::fname(rel))->second;
		  std::ostringstream oss;
		  oss << C5_rel_name;
		  LOG("ice", errs() << "TAG: " << oss.str() << "\n";);

		  boost::property_tree::ptree sub_pt = child_it.second;
		  if(sub_pt.get<std::string>("children") == std::string("null"))
		  {
			  if(sub_pt.get<std::string>("classification") == "true" || sub_pt.get<std::string>("classification") == "True")
			  {
				 candidate = mk<TRUE>(db.getExprFactory());
			  }
			  else if(sub_pt.get<std::string>("classification") == "false" || sub_pt.get<std::string>("classification") == "False")
			  {
				 candidate = mk<FALSE>(db.getExprFactory());
			  }
		  }
		  else
		  {
			  std::list<Expr> stack;
			  stack.push_back(mk<TRUE>(db.getExprFactory()));
			  std::list<std::list<Expr>> final_formula = constructFormula(stack, sub_pt);
			  ExprVector disjunctions;
			  for(std::list<std::list<Expr>>::iterator disj_it = final_formula.begin(); disj_it != final_formula.end(); ++disj_it)
			  {
				  ExprVector conjunctions;
				  for(std::list<Expr>::iterator conj_it = (*disj_it).begin(); conj_it != (*disj_it).end(); ++conj_it)
				  {
					  conjunctions.push_back(*conj_it);
				  }
				  Expr disjunct = mknary<AND>(conjunctions.begin(), conjunctions.end());
				  disjunctions.push_back(disjunct);
			  }
			  if(disjunctions.size() == 1)
			  {
				  candidate = disjunctions[0];
			  }
			  else
			  {
				  candidate = mknary<OR>(disjunctions.begin(), disjunctions.end());
			  }
		  }
		  LOG("ice", errs() << "NEW CANDIDATE: " << *candidate << "\n";);

		  ExprVector arg_list;
		  for(int i=0; i<bind::domainSz(rel); i++)
		  {
			  Expr arg_i_type = bind::domainTy(rel, i);
			  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
			  arg_list.push_back(arg_i);
		  }
		  Expr fapp = bind::fapp(rel, arg_list);

		  //Expr prevCand = m_candidate_model.getDef(fapp);

		  //if (isOpX<TRUE>(prevCand) || isOpX<FALSE> (prevCand))
			//  m_candidate_model.addDef(fapp, candidate);
		  //else
			  m_candidate_model.addDef(fapp, candidate);
		  rels_it++;
	  }
  }

  std::list<std::list<Expr>> ICE::constructFormula(std::list<Expr> stack, boost::property_tree::ptree sub_pt)
  {
	  Expr decision_expr;
	  std::list<std::list<Expr>> final_formula;
	  //leaf node
	  if(sub_pt.get<std::string>("children") == std::string("null"))
	  {
		  LOG("ice", errs() << "LEAF NODE\n";);
		  if(sub_pt.get<std::string>("classification") == "true" || sub_pt.get<std::string>("classification") == "True")
		  {
			 std::list<Expr> new_conjunct = stack;
			 final_formula.push_back(new_conjunct);
			 return final_formula;
		  }
		  else if(sub_pt.get<std::string>("classification") == "false" || sub_pt.get<std::string>("classification") == "False")
		  {
			 return final_formula;
		  }
	  }
	  //internal node
	  else
	  {
		  LOG("ice", errs() << "INTERNAL NODE\n";);
		  std::string attr_name = sub_pt.get<std::string>("attribute");
		  LOG("ice", errs() << "CUT ATTRIBUTE: " << attr_name << "\n";);

		  if(attr_name.find("+") != -1)
		  {
			  decision_expr = plusAttrToDecisionExpr(sub_pt, attr_name);
		  }
		  else if(attr_name.find("-") != -1)
		  {
			  decision_expr = minusAttrToDecisionExpr(sub_pt, attr_name);
		  }
		  else if (attr_name.find("mod") != -1)
		  {
			  decision_expr = modAttrToDecisionExpr(sub_pt, attr_name);
		  }
		  else if (attr_name.find("SVM") != -1)
		  {
			  Expr attr_expr;
			  for(ExprMap::iterator it = m_svmattr_name_to_expr_map.begin(); it!= m_svmattr_name_to_expr_map.end(); ++it)
			  {
				  std::ostringstream oss;
				  oss << *(it->first);
				  if(oss.str() == attr_name)
				  {
					  attr_expr = it->second;
				  }
			  }

			  if (isOpX<GEQ>(attr_expr)) {
				  decision_expr = mk<NEG>(attr_expr);
				  int cut = sub_pt.get<int>("cut");
				  assert(cut == 0);
			  } else if (isOpX<PLUS>(attr_expr)) {
				  int cut = sub_pt.get<int>("cut");
				  Expr threshold = mkTerm<mpz_class>(cut, attr_expr->efac());
				  decision_expr = mk<LEQ>(attr_expr, threshold);
			  } else {
				  LOG("ice", errs() << "DECISION NODE TYPE WRONG!\n";);
				  return final_formula;
			  }
		  }
		  else if (attr_name.find("ICEPred") != -1)
		  {
			  Expr attr_expr;
			  for(ExprMap::iterator it = m_pred_name_to_expr_map.begin(); it!= m_pred_name_to_expr_map.end(); ++it)
			  {
				  std::ostringstream oss;
				  oss << *(it->first);
				  if(oss.str() == attr_name)
				  {
					  attr_expr = it->second;
				  }
			  }

			  if (isOpX<LEQ>(attr_expr)) {
				  decision_expr = attr_expr; //mk<NEG>(attr_expr);
				  int cut = sub_pt.get<int>("cut");
				  assert(cut == 0);
			  } else {
				  LOG("ice", errs() << "DECISION NODE TYPE WRONG!\n";);
				  return final_formula;
			  }
		  }
		  else
		  {
			  Expr attr_expr;
			  for(ExprMap::iterator it = m_attr_name_to_expr_map.begin(); it!= m_attr_name_to_expr_map.end(); ++it)
			  {
				  std::ostringstream oss;
				  oss << *(it->first);
				  if(oss.str() == attr_name)
				  {
					  attr_expr = it->second;
				  }
			  }

			  if(bind::isBoolConst(attr_expr) /*|| isOpX<GEQ>(attr_expr)*/)
			  {
				 decision_expr = mk<NEG>(attr_expr);
				 int cut = sub_pt.get<int>("cut");
				 assert(cut == 0);
			  }
			  else if(bind::isIntConst(attr_expr) /*|| isOpX<PLUS>(attr_expr)*/)
			  {
				 int cut = sub_pt.get<int>("cut");
				 Expr threshold = mkTerm<mpz_class>(cut, attr_expr->efac());
				 decision_expr = mk<LEQ>(attr_expr, threshold);
			  }
			  else
			  {
				 LOG("ice", errs() << "DECISION NODE TYPE WRONG!\n";);
				 return final_formula;
			  }
		  }
		  stack.push_back(decision_expr);
		  //assert(sub_pt.children().size() == 2);
		  boost::property_tree::ptree::assoc_iterator child_itr = sub_pt.get_child("children").ordered_begin();
		  std::list<std::list<Expr>> final_formula_left = constructFormula(stack, child_itr->second);
		  stack.pop_back();
		  stack.push_back(mk<NEG>(decision_expr));
		  std::list<std::list<Expr>> final_formula_right = constructFormula(stack, (++child_itr)->second);
		  stack.pop_back();
		  final_formula_left.insert(final_formula_left.end(), final_formula_right.begin(), final_formula_right.end());
		  return final_formula_left;
	  }
	  return final_formula;
  }

  //given an attribute which is x+y, return the expr
  Expr ICE::plusAttrToDecisionExpr(boost::property_tree::ptree sub_pt, std::string attr_name)
  {
	  typedef boost::tokenizer< boost::char_separator<char>> t_tokenizer;
	  boost::char_separator<char> sep("+");
	  t_tokenizer tok(attr_name, sep);
	  std::string left_name = *(tok.begin());
	  std::string right_name = *(++tok.begin());
	  Expr left_expr;
	  Expr right_expr;
	  for(ExprMap::iterator it = m_attr_name_to_expr_map.begin(); it!= m_attr_name_to_expr_map.end(); ++it)
	  {
		  std::ostringstream oss;
		  oss << *(it->first);
		  if(oss.str() == left_name)
		  {
			  left_expr = it->second;
		  }
		  if(oss.str() == right_name)
		  {
			  right_expr = it->second;
		  }
	  }
	  if(!bind::isIntConst(left_expr) || !bind::isIntConst(right_expr))
	  {
		  LOG("ice", errs() << "OPERAND TYPE WRONG!\n";);
	  }
	  int cut = sub_pt.get<int>("cut");
	  Expr threshold = mkTerm<mpz_class>(cut, left_expr->efac());
	  Expr decision_expr = mk<LEQ>(mk<PLUS>(left_expr, right_expr), threshold);

	  return decision_expr;
  }

  //given an attribute which is x-y, return the expr
  Expr ICE::minusAttrToDecisionExpr(boost::property_tree::ptree sub_pt, std::string attr_name)
  {
	  typedef boost::tokenizer< boost::char_separator<char>> t_tokenizer;
	  boost::char_separator<char> sep("-");
	  t_tokenizer tok(attr_name, sep);
	  std::string left_name = *(tok.begin());
	  std::string right_name = *(++tok.begin());
	  Expr left_expr;
	  Expr right_expr;
	  for(ExprMap::iterator it = m_attr_name_to_expr_map.begin(); it!= m_attr_name_to_expr_map.end(); ++it)
	  {
		  std::ostringstream oss;
		  oss << *(it->first);
		  if(oss.str() == left_name)
		  {
			  left_expr = it->second;
		  }
		  if(oss.str() == right_name)
		  {
			  right_expr = it->second;
		  }
	  }
	  if(!bind::isIntConst(left_expr) || !bind::isIntConst(right_expr))
	  {
		  LOG("ice", errs() << "OPERAND TYPE WRONG!\n";);
	  }
	  int cut = sub_pt.get<int>("cut");
	  Expr threshold = mkTerm<mpz_class>(cut, left_expr->efac());
	  Expr decision_expr = mk<LEQ>(mk<MINUS>(left_expr, right_expr), threshold);

	  return decision_expr;
  }

  //given an attribute which is x%y, return the expr
  Expr ICE::modAttrToDecisionExpr(boost::property_tree::ptree sub_pt, std::string attr_name)
  {
	  typedef boost::tokenizer< boost::char_separator<char>> t_tokenizer;
	  boost::char_separator<char> sep("mod");
	  t_tokenizer tok(attr_name, sep);
	  std::string left_name = *(tok.begin());
	  std::string right_name = *(++tok.begin());
	  Expr left_expr = NULL;
	  Expr right_expr = NULL;
	  for(ExprMap::iterator it = m_attr_name_to_expr_map.begin(); it!= m_attr_name_to_expr_map.end(); ++it)
	  {
		  std::ostringstream oss;
		  oss << *(it->first);
		  if(oss.str() == left_name)
		  {
			  left_expr = it->second;
		  }
		  if(oss.str() == right_name)
		  {
			  right_expr = it->second;
		  }
	  }

	  if (right_expr == NULL) {
		  bool right_has_only_digits = (right_name.find_first_not_of( "0123456789" ) == -1);
		  if (right_has_only_digits && bind::isIntConst(left_expr)) {
			  right_expr = mkTerm<mpz_class>(atoi(right_name.c_str()), left_expr->efac());
		  } else
			  LOG("ice", errs() << "OPERAND TYPE WRONG!\n";);
	  } else if ( !bind::isIntConst(right_expr) ) {
		  LOG("ice", errs() << "OPERAND TYPE WRONG!\n";);
	  }
	  if(!bind::isIntConst(left_expr)) {
		  LOG("ice", errs() << "OPERAND TYPE WRONG!\n";);
	  }

	  int cut = sub_pt.get<int>("cut");
	  Expr threshold = mkTerm<mpz_class>(cut, left_expr->efac());
	  Expr decision_expr = mk<LEQ>(mk<MOD>(left_expr, right_expr), threshold);

	  return decision_expr;
  }


  // Collect unknowns in the rules
  void ICE::extractUnknowns(HornClauseDB &db) {
	  ExprVector pred_apps;
	  for(auto it = db.getRules().begin(); it != db.getRules().end(); ++it)
	  {
		  HornRule r = *it;
	  	  pred_apps.push_back(r.head());
	  	  get_all_pred_apps(r.body(), db, std::back_inserter(pred_apps));
	  }

	  for (Expr app : pred_apps) {
		  Expr rel = bind::fname(app);
		  int size = bind::domainSz(rel);

		  auto it = unknowns.find(rel);
		  if (unknowns.end() == it) {
			  std::vector<bool> flags(size);
			  for (int i = 0; i < size; i++)
				  flags[i] = true;
			  unknowns[rel] = flags;
		  }

		  for(int i=0; i<bind::domainSz(rel); i++)
		  {
			  Expr name = app->arg(i+1);
			  std::ostringstream oss;
			  oss << name;
			  if (oss.str().find("@unknown") != -1 || oss.str().find ("_nondet_") != -1) {
				  unknowns[rel][i] = false;
			  }
		  }
	  }

	  for(std::map<Expr, std::vector<bool>>::iterator itr =
			  unknowns.begin(); itr != unknowns.end(); ++itr) {
		  //LOG("ice", errs() << "Rel: " << *(itr->first) << ":\n");
		  //for (bool b : itr->second) {
			  //LOG("ice", errs() << b << " ");
		  //}
		  //LOG("ice", errs() << "\n");
		  for (int i = 0; i < itr->second.size(); i++) {
			  itr->second[i] = !itr->second[i];
		  }
	  }
	  //LOG("ice", errs() << "Unknown search done.\n");
  }

  // Collect integers in the rules ...
  // Fixme. It seems we only need to consider mod operations.
  void ICE::extractConstants(HornClauseDB &db) {
	  struct IsREM : public std::unary_function<Expr, bool>
	  {
		  IsREM () {}

	  	  bool operator() (Expr e)
	  	  {return isOpX<REM>(e);}
	  };

	  for(auto it = db.getRules().begin(); it != db.getRules().end(); ++it)
	  {
		  HornRule r = *it;
		  ExprVector body_pred_apps;

//		  ExprVector cache;
//		  cache.push_back(r.body());
//		  while (cache.size () > 0) {
//			  Expr curr = cache.front();
//			  cache.erase (cache.begin());
//
//			  if (isOpX<REM>(curr)) {
//				  body_pred_apps.push_back(curr);
//			  }
//			  else
//				  for (Expr arg : mk_it_range (curr->args_begin(), curr->args_end ())) {
//					  cache.push_back (arg);
//				  }
//		  }
		  filter (r.body(), IsREM(), std::back_inserter(body_pred_apps));

		  for (Expr p : body_pred_apps) {
			  Expr mod = p->right();
			  std::ostringstream oss;
			  oss << mod;
			  bool right_has_only_digits = (oss.str().find_first_not_of( "0123456789" ) == -1);
			  if (right_has_only_digits) {
				  int cons = atoi(oss.str().c_str());
				  if (cons > 0) ruleConstants.insert(cons);
				  else if (cons < 0) ruleConstants.insert(-1*cons);
					  //auto it = ruleConstants.find (r);
					  //if (it != ruleConstants.end()) {
						  //it->second.insert(atoi(oss.str().c_str()));
					  //} else {
						  //ruleConstants.insert(std::make_pair(r, ...));
					  //}
			  }
		  }
	  }
  }


  // Substitute relations with their solution.
  // Enforce t is mapped to s if s is not null.
  Expr ICE::extractRelation(HornRule r, HornClauseDB &db, Expr t, Expr s)
  {
    Expr ruleBody = r.body();
    ExprMap body_map;
    ExprVector body_pred_apps;
    get_all_pred_apps(ruleBody, db, std::back_inserter(body_pred_apps));

    for (Expr p : body_pred_apps) {
    	if (p == t) {
    		if (s == NULL)
    			body_map.insert (std::make_pair (p, mk<TRUE> (p->efac ())));
    		else
    			body_map.insert (std::make_pair (p, s));
    	}
    	else
    		body_map.insert (std::make_pair (p, m_candidate_model.getDef(p)));
    }

    Expr body_constraints = replace(ruleBody, body_map);
    return body_constraints;
  }


  //ice.genInitialCandidates(hm.getHornClauseDB());
  void ICE::genInitialCandidates (HornClauseDB &db)
  {
	  for(Expr rel : db.getRelations())
	  {
		  ExprVector arg_list;
		  for(int i=0; i<bind::domainSz(rel); i++)
		  {
			  Expr arg_i_type = bind::domainTy(rel, i);
			  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
			  arg_list.push_back(arg_i);
		  }
		  Expr fapp = bind::fapp(rel, arg_list);
		  Expr True = mk<TRUE>(rel->efac());
		  Expr False = mk<FALSE>(rel->efac());

		  for (auto q : db.getQueries ()) {
			  Expr query = q.get();
			  if (bind::fname (query) == rel) {
				  m_candidate_model.addDef(fapp, False);
			  } else {
				  m_candidate_model.addDef(fapp, True);
			  }
		  }
	  }

	  ExprVector empty;
	  extractFacts (db, empty);
  }

  // For now always try to prove a query is false.
  void ICE::invalidateQueries (HornClauseDB &db)
  {
	  for(Expr rel : db.getRelations())
	  {
		  ExprVector arg_list;
		  for(int i=0; i<bind::domainSz(rel); i++)
		  {
			  Expr arg_i_type = bind::domainTy(rel, i);
			  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
			  arg_list.push_back(arg_i);
		  }
		  Expr fapp = bind::fapp(rel, arg_list);
		  Expr False = mk<FALSE>(rel->efac());

		  for (auto q : db.getQueries ()) {
			  Expr query = q.get();
			  if (bind::fname (query) == rel) {
				  m_candidate_model.addDef(fapp, False);
			  }
		  }
	  }
  }

  // Match wheter an example corresponds to a fact.
  bool ICE::matchFacts (HornClauseDB &db, DataPoint p) {
	  for(auto it = db.getRules().begin(); it != db.getRules().end(); ++it)
	  {
		  HornRule r = *it;
		  if (isOpX<TRUE>(r.body()) && isOpX<FAPP>(r.head()) && p.getPredName() == bind::fname(bind::fname(r.head()))) {
			  Expr head = r.head();
			  Expr rel = bind::fname(head);
			  bool matched = false;
			  for(int i=0; i<bind::domainSz(rel); i++)
			  {
				  Expr arg_i_value = head->arg(i+1);
				  LOG("ice", errs() << *arg_i_value << ",";);
				  Expr arg_i_type = bind::domainTy(rel, i);
				  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));

				  if(isOpX<TRUE>(arg_i_value))
				  {
					  std::list<Expr>::iterator it = p.getAttrValues().begin();
					  std::advance(it, i);
					  LOG("ice", errs() << "(" <<**it << "),";);
					  std::ostringstream oss;
					  oss << **it;
					  if(oss.str().compare("1") == 0) {
					  //if(isOpX<TRUE>(*it)) {
						  matched = true;
					  } else {
						  matched = false;
						  break;
					  }
				  }
				  else if(isOpX<FALSE>(arg_i_value))
				  {
					  std::list<Expr>::iterator it = p.getAttrValues().begin();
					  std::advance(it, i);
					  LOG("ice", errs() << "(" <<**it << "),";);
					  std::ostringstream oss;
					  oss << **it;
					  if(oss.str().compare("0") == 0) {
					  //if(isOpX<FALSE>(*it)) {
						  matched = true;
					  } else {
						  matched = false;
						  break;
					  }
				  }
				  else if(bind::isBoolConst(arg_i_value))
				  {
					  matched = true;
				  }
				  else if(bind::isIntConst(arg_i_value))
				  {
					  matched = true;
				  }
				  else { /* Other kind of constructs in fact rules not implemented yet ...*/ }
			  }

			  if (matched) {
				  LOG("ice", errs() << "\nMatched!\n");
				  return true;
			  }
		  }
	  }

	  LOG("ice", errs() << "\nUnMatched!\n");
	  return false;
  }

  // Dealing with fact rules inside Horn System.
  // <1> Scan all the fact rules (true -> f (...)) <2>
  void ICE::extractFacts (HornClauseDB &db, ExprVector targets) {
	  LOG("ice", errs() << "Extracting Fact Rules ...\n");
	  for(auto it = db.getRules().begin(); it != db.getRules().end(); ++it)
	  {
		  HornRule r = *it;
		  if (isOpX<TRUE>(r.body()) && isOpX<FAPP>(r.head())) {
			  Expr head = r.head();
			  Expr rel = bind::fname(head);

			  if (targets.empty() || std::find(targets.begin(), targets.end(), bind::fname(rel)) != targets.end()) {
				  ExprVector arg_list;
				  bool fact = false;
				  Expr currSolve = mk<TRUE>(rel->efac());
				  for(int i=0; i<bind::domainSz(rel); i++)
				  {
					  Expr arg_i_value = head->arg(i+1);
					  LOG("ice", errs() << *arg_i_value << ",";);
					  Expr arg_i_type = bind::domainTy(rel, i);
					  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
					  arg_list.push_back(arg_i);

					  if(bind::isBoolConst(arg_i_value))
					  {
						  LOG("ice", errs() << "UNCERTAIN VALUE Don't Care: " << *arg_i_value << "\n";);
					  }
					  else if(bind::isIntConst(arg_i_value))
					  {
						  LOG("ice", errs() << "UNCERTAIN VALUE Don't Care: " << *arg_i_value << "\n";);
					  }
					  else if(isOpX<TRUE>(arg_i_value))
					  {
						  fact = true;
						  currSolve = mk<AND>(currSolve, arg_i);
					  }
					  else if(isOpX<FALSE>(arg_i_value))
					  {
						  fact = true;
						  currSolve = mk<AND>(currSolve, mk<NEG>(arg_i));
					  }
					  else { /* Other kind of constructs in fact rules not implemented yet ...*/ }
				  }

				  if (fact) {
					  // Add fact rules into solutions.
					  Expr fapp = bind::fapp(rel, arg_list);
					  Expr preSolve = m_candidate_model.getDef(fapp);
					  m_candidate_model.addDef(fapp, mk<OR>(preSolve, currSolve));
				  }
			  }
		  }
	  }
	  LOG("ice", errs() << "Extracted Fact Rules.\n");
  }

  int ICE::countSamples (Expr pred, bool positive) {
	  std::set<DataPoint>::iterator it;
	  int count = 0;
	  if (positive) {
		  for (it = m_pos_data_set.begin(); it != m_pos_data_set.end(); ++it) {
			  if (it->getPredName() == pred) {
				  count++;
			  }
		  }
	  } else {
		  for (it = m_neg_data_set.begin(); it != m_neg_data_set.end(); ++it) {
			  if (it->getPredName() == pred) {
				  count++;
			  }
		  }
	  }
	  return count;
  }

  void ICE::clearNegSamples (Expr app, bool b) {
	  /*m_cex_list.erase(std::remove_if(m_cex_list.begin(), m_cex_list.end(),
			  [app,b,this](DataPoint p) {
		  	  	  if (b)
		  	  		  return p.getPredName() == bind::fname(bind::fname(app))
		  	  				  && m_neg_data_set.find(p) != m_neg_data_set.end();
		  	  	  else
		  	  		  return p.getPredName () == app
		  	  				  && m_neg_data_set.find(p) != m_neg_data_set.end();
			  }), m_cex_list.end());
	  //m_neg_data_set.erase(std::remove_if(m_neg_data_set.begin(), m_neg_data_set.end(),
	  //		  [body_app](DataPoint p) {
	  //	  	  	  return p.getPredName() == bind::fname(bind::fname(body_app)); }), m_neg_data_set.end());
	  for (std::set<DataPoint>::iterator it = m_neg_data_set.begin(); it != m_neg_data_set.end(); ) {
		  if (b) {
			  if (it->getPredName() == bind::fname(bind::fname(app))) {
				  m_neg_data_set.erase (it++);
			  } else {
				  ++it;
			  }
		  } else {
			  if (it->getPredName() == app) {
				  m_neg_data_set.erase (it++);
			  } else {
				  ++it;
			  }
		  }
	  } */
	  exit (-3);
  }

  bool ICE::generatePostiveSamples (HornClauseDB &db, HornRule r, ZSolver<EZ3> solver, int& index, bool& run) {
	  Expr body_app = r.head();
	  if (bind::domainSz(bind::fname(body_app)) <= 0) {
		  LOG ("ice", errs () << "Rule cannot be refined.\n");
		  exit (-3);
	  }

	  Expr r_head_cand = m_candidate_model.getDef(r.head());

	  LOG("ice", errs() << "TRYING TO ADD some CounterExample.\n";);

	  solver.reset();

	  solver.assertExpr(mk<NEG>(r_head_cand));

	  Expr body_forumla = extractRelation(r, db, NULL, NULL);

	  LOG ("ice", errs() << "Verification condition: " << *r_head_cand << " <- " << *body_forumla << "\n");

	  solver.assertExpr(body_forumla);

	  //solver.toSmtLib(errs());
	  boost::tribool result = solver.solve();
	  if(result != UNSAT)
	  {
		  LOG("ice", errs() << "SAT, NEED TO ADD More Examples\n";);
		  //get cex
		  ZModel<EZ3> m = solver.getModel();
		  //print cex
		  LOG("ice", errs() << "(";);
		  LOG("ice", errs() << ") -> (";);
		  for(int i=0; i<bind::domainSz(bind::fname(body_app)); i++)
		  {
			  Expr arg_i = body_app->arg(i+1);
			  Expr arg_i_value = m.eval(arg_i);
			  LOG("ice", errs() << *arg_i_value << ",";);
		  }
		  LOG("ice", errs() << ")\n";);

		  //add counterexample
		  std::list<Expr> attr_values;
		  for(int i=0; i<bind::domainSz(bind::fname(body_app)); i++)
		  {
			  Expr arg_i = body_app->arg(i+1);
			  Expr arg_i_value = m.eval(arg_i);

			  //deal with uncertain values in cexs
			  if(bind::isBoolConst(arg_i_value))
			  {
				  LOG("ice", errs() << "UNCERTAIN VALUE: " << *arg_i_value << "\n";);
				  Expr uncertain_value = mk<FALSE>(arg_i_value->efac());
				  arg_i_value = uncertain_value;
			  }
			  else if(bind::isIntConst(arg_i_value))
			  {
				  LOG("ice", errs() << "UNCERTAIN VALUE: " << *arg_i_value << "\n";);
				  Expr uncertain_value = mkTerm<mpz_class>(0, arg_i_value->efac());
				  arg_i_value = uncertain_value;
			  }

			  //convert true/false to 1/0 in C5 data point
			  if(isOpX<TRUE>(arg_i_value))
			  {
				  arg_i_value = mkTerm<mpz_class>(1, arg_i_value->efac());
			  }
			  else if(isOpX<FALSE>(arg_i_value))
			  {
				  arg_i_value = mkTerm<mpz_class>(0, arg_i_value->efac());
			  }

			  //deal with too large integer value like: -0xffffffb
			  std::ostringstream oss;
			  oss << arg_i_value;
			  if(oss.str().find("-0x") == 0)
			  {
				  LOG("ice", errs() << "TOO LARGE VALUE, OVERFLOW: " << *arg_i_value << "\n";);
				  Expr uncertain_value = mkTerm<mpz_class>(0, arg_i_value->efac());
				  arg_i_value = uncertain_value;
			  }

			  attr_values.push_back(arg_i_value);
		  }

		  DataPoint pos_dp(bind::fname(bind::fname(body_app)), attr_values);
		  int orig_size = m_pos_data_set.size();
		  addPosCex(pos_dp);
		  if(m_pos_data_set.size() == orig_size + 1) //no duplicate
		  {
			  if (SVMExecPath.compare("") != 0 && m_neg_data_set.size() > /*50*/ICESVMFreqPos) {
				  LOG("ice", errs() << "SVM based Hyperlane Learning!\n");
				  svmLearn (NULL);
			  }
			  if (!ICEICE) {
			  m_cex_list.erase(std::remove_if(m_cex_list.begin(), m_cex_list.end(),
					  [pos_dp,body_app,this](DataPoint p) {
				  	  	  return p.getPredName() == bind::fname(bind::fname(body_app))
				  	  	  	  	  && m_neg_data_set.find(p) != m_neg_data_set.end();
				  	  	  //return p == pos_dp;
			  	  	  }), m_cex_list.end());
			  //m_neg_data_set.erase(std::remove_if(m_neg_data_set.begin(), m_neg_data_set.end(),
			  //		  [body_app](DataPoint p) {
			  //	  	  	  return p.getPredName() == bind::fname(bind::fname(body_app)); }), m_neg_data_set.end());
			  for (std::set<DataPoint>::iterator it = m_neg_data_set.begin(); it != m_neg_data_set.end(); ) {
				  if (it->getPredName() == bind::fname(bind::fname(body_app))) {
				  //if (*it == pos_dp) {
					  m_neg_data_set.erase (it++);
				  } else {
					  ++it;
				  }
			  }
			  m_neg_data_count.erase (pos_dp.getPredName());
			  }
			  m_cex_list.push_back(pos_dp);
			  addDataPointToIndex(pos_dp, index);
			  LOG("ice", errs() << "POS CEX, INDEX IS " << index << "\n";);
			  index++;

			  run = sampleLinearHornCtrs(body_app, pos_dp, index);
		  }
		  else //it is a duplicate data point
		  {
			  LOG("ice", errs() << "Duplicated positive points should be impossible.\n");
			  clearNegSamples (body_app, true);
			  //exit (-3);
		  }

		  return true;
	  } else {
		  return false;
	  }
  }

  // Given a rule head, extract all rules using it in body, then add all such rules to the beginning of workList
  void addUsedToWorkList(HornClauseDB &db, std::list<HornRule> &workList, HornRule r)
  {
  	  for(auto it = db.getRules().begin(); it != db.getRules().end(); ++it)
  	  {
  		  if(*it == r)
  		  {
  			  continue;
  		  }
  		  HornRule rule = *it;
  		  Expr r_body = rule.body();
  		  ExprVector body_pred_apps;
  		  get_all_pred_apps(r_body, db, std::back_inserter(body_pred_apps));

  		  for (Expr body_pred_app : body_pred_apps) {
  			  if (bind::fname(body_pred_app) == bind::fname(r.head())) {
  	  			  if(std::find(workList.begin(), workList.end(), *it) == workList.end())
  	  			  {
  	  				  workList.push_front(*it);
  	  			  }
  	  			  break;
  			  }
  		  }
  	  }
  }

  // Given a rule, extract all rules using its body_apps in head, then add all such rules to the end of workList
  void addNewToWorkList (HornClauseDB &db, std::list<HornRule> &workList, HornRule r) {
	  Expr r_body = r.body();
	  ExprVector body_pred_apps;
	  get_all_pred_apps(r_body, db, std::back_inserter(body_pred_apps));

	  for(auto it = db.getRules().begin(); it != db.getRules().end(); ++it)
	  {
		  if(*it == r)
		  {
			  continue;
		  }
		  HornRule rule = *it;
		  for (Expr body_pred_app : body_pred_apps) {
			  if (bind::fname(body_pred_app) == bind::fname(rule.head())) {
				  if(std::find(workList.begin(), workList.end(), *it) == workList.end())
				  {
					  workList.push_back(*it);
				  }
				  break;
			  }
		  }
	  }
  }

  bool ICE::solveConstraints(HornClauseDB &db, bool &isChanged, int &index)
  {
	  std::list<HornRule> workList;
	  workList.insert(workList.end(), db.getRules().begin(), db.getRules().end());
	  workList.reverse();

  	  LOG("ice", errs() << "=========================== Constraint Solving of Horn Clauses ============================\n";);
	  ZSolver<EZ3> solver(m_hm.getZContext());

	  while (!workList.empty())
	  //for (auto it = db.getRules().rbegin(); it != db.getRules().rend(); ++it)
	  //for(auto it = db.getRules().begin(); it != db.getRules().end(); ++it)
	  {
		  HornRule r = workList.front();
		  workList.pop_front();
		  // HornRule r = *it;

		  LOG("ice", errs() << "VERIFY Horn Rule: " << *(r.head()) << " <- " << *(r.body()) << "\n";);

		  bool upd = false; int counter = 0; bool posUpd = false;

		  Expr r_body = r.body();
		  ExprVector body_pred_apps;
		  get_all_pred_apps(r_body, db, std::back_inserter(body_pred_apps));

		  //if(body_pred_apps.size() > 1)
		  //{
		  //	  LOG ("ice", errs() << "Nonlinear Horn Constraints are not supported right now.\n");
		  //	  exit (-3);
		  //}

		  // This is important as ICE is not able to handle it.
		  //if(bind::fname(r.head()) != bind::fname(body_app)) {
		  //  continue;
		  //}

		  bool cleanBody = true;
		  for (Expr body_app : body_pred_apps) {
			  if (bind::domainSz(bind::fname(body_app)) <= 0) {
				  // This is clean.
				  // cleanBody = true;
			  } else {
				  cleanBody = false;
				  break;
			  }
		  }

		  if (body_pred_apps.size() <= 0 || cleanBody) {
				  // bind::domainSz(bind::fname(body_pred_apps[0])) <= 0) {
			  if (bind::domainSz(bind::fname(r.head ())) <= 0) {
				  LOG ("ice", errs() << "Verify a rule without unknown invariants.\n");
				  Expr r_head_cand = m_candidate_model.getDef(r.head());
				  solver.reset();
				  solver.assertExpr(mk<NEG>(r_head_cand));
				  Expr body_forumla = extractRelation(r, db, NULL, NULL);
				  LOG ("ice", errs() << "Verification condition: " << *r_head_cand << " <- " << *body_forumla << "\n");
				  solver.assertExpr(body_forumla);
				  boost::tribool result = solver.solve();
				  if(result != UNSAT) {
					  errs()<<"Program is buggy.\n";
					  std::list<Expr> attr_values;
					  DataPoint pos_dp(bind::fname(bind::fname(r.head())), attr_values);
					  addPosCex(pos_dp);
					  failurePoint = m_pos_list.size()-1;
					  return false;
				  }
			  } else {
				  // Head is possibly need to be refined!
				  LOG ("ice", errs() << "Generate Initial Program State Samples.\n");
				  do {
					  bool run = true;
					  upd = generatePostiveSamples (db, r, solver, index, run);
					  if (!run) return false;
					  if (upd) {
						  isChanged = true; posUpd = true;
						  // Which predicates will be changed in this iteration of solving.
						  ExprVector changedPreds;
						  // FixMe. Bad Code.
						  //if (SVMExecPath.compare("") == 0) {
						  	  changedPreds.push_back (bind::fname(*(db.getRelations().begin())));
						  //}
						  changedPreds.push_back(bind::fname(bind::fname(r.head())));
						  //if (SVMExecPath.compare("") == 0)
							  C5learn (changedPreds);
						  //else
						  //	  svmLearn (changedPreds);
					  }
				  } while (upd);
				  // --- Extend work list as we just go through a strengthening loop ----
				  addUsedToWorkList (db, workList, r);
			  }
		  } else {
			  //Expr body_app = body_pred_apps[0];

			  //Expr preSolve = m_candidate_model.getDef(body_app);
			  if (ICELocalStrengthen) {
			  HornDbModel global_cache;
			  for (Expr body_app : body_pred_apps) {
				  ExprVector arg_list;
				  Expr rel = bind::fname (body_app);
				  for(int i=0; i<bind::domainSz(rel); i++)
				  {
					  Expr arg_i_type = bind::domainTy(rel, i);
					  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
					  arg_list.push_back(arg_i);
				  }
				  Expr fapp = bind::fapp(rel, arg_list);
				  global_cache.addDef(fapp, m_candidate_model.getDef(fapp));
			  }
			  do {
				  // Only strengthen the lhs, keeping rhs unchanged.
				  // Save what rhs is.
				  Expr r_head = r.head();
				  Expr r_head_cand = m_candidate_model.getDef(r_head);
				  {
					  solver.reset();
					  solver.assertExpr(mk<NEG>(r_head_cand));
					  Expr body_forumla = extractRelation(r, db, NULL, NULL);
					  solver.assertExpr(body_forumla);
					  boost::tribool result = solver.solve();
					  if(result == UNSAT) { // Successfully prove the rule with current solution.
						  LOG("ice", errs() << "-- Finally the rule is proved at counter " << counter << "!\n");
						  break;
				  	  }
				  }

				  // Save previous solution of lhs.
				  // After the following do-while, update solution of lhs conjoining the previous solution
				  HornDbModel local_cache;
				  for (Expr body_app : body_pred_apps) {
					  ExprVector arg_list;
					  Expr rel = bind::fname (body_app);
					  for(int i=0; i<bind::domainSz(rel); i++)
					  {
						  Expr arg_i_type = bind::domainTy(rel, i);
						  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
						  arg_list.push_back(arg_i);
					  }
					  Expr fapp = bind::fapp(rel, arg_list);
					  local_cache.addDef(fapp, m_candidate_model.getDef(fapp));
				  }

				  do {
					  counter ++;

					  LOG("ice", errs() << "Rule Verification Round " << counter << "\n");

					  // Which predicates will be changed in this iteration of solving.
					  ExprVector changedPreds;
					  // FixMe. Bad Code.
					  //if (SVMExecPath.compare("") == 0) {
					  changedPreds.push_back (bind::fname(*(db.getRelations().begin())));
					  //}

					  //Expr currSolve = m_candidate_model.getDef(body_app);
					  //if (currSolve != preSolve) {
					  //	  m_candidate_model.addDef(body_app, mk<AND>(preSolve, currSolve));
					  //}

					  upd = false;
					  //Expr r_head = r.head();
					  //Expr r_head_cand = m_candidate_model.getDef(r_head);

					  LOG("ice", errs() << "TRYING TO ADD some CounterExample.\n";);

					  solver.reset();

					  solver.assertExpr(mk<NEG>(r_head_cand));

					  Expr body_forumla = extractRelation(r, db, NULL, NULL);

					  LOG ("ice", errs() << "Verification condition: " << *r_head_cand << " <- " << *body_forumla << "\n");

					  solver.assertExpr(body_forumla);

					  //solver.toSmtLib(errs());
					  boost::tribool result = solver.solve();
					  if(result != UNSAT)
					  {
						  LOG("ice", errs() << "SAT, NEED TO ADD The Counterexample\n";);
						  upd = true; isChanged = true;
						  //get cex
						  ZModel<EZ3> m = solver.getModel();
						  //print cex
						  std::set<DataPoint> negPoints;
						  for (Expr body_app : body_pred_apps) {
							  if (bind::domainSz(bind::fname(body_app)) <= 0)
								  // No counterexample can be obtained from it because it is clean.
								  continue;

							  LOG("ice", errs() << "(";);
							  for(int i=0; i<bind::domainSz(bind::fname(body_app)); i++)
							  {
								  Expr arg_i = body_app->arg(i+1);
								  Expr arg_i_value = m.eval(arg_i);
								  LOG("ice", errs() << *arg_i_value << ",";);
							  }
							  LOG("ice", errs() << ") -> (";);
							  for(int i=0; i<bind::domainSz(bind::fname(r_head)); i++)
							  {
								  Expr arg_i = r_head->arg(i+1);
								  Expr arg_i_value = m.eval(arg_i);
								  LOG("ice", errs() << *arg_i_value << ",";);
							  }
							  LOG("ice", errs() << ")\n";);

							  // Presumbaly add counterexample
							  std::list<Expr> attr_values;
							  for(int i=0; i<bind::domainSz(bind::fname(body_app)); i++)
							  {
								  Expr arg_i = body_app->arg(i+1);
								  Expr arg_i_value = m.eval(arg_i);

								  //deal with uncertain values in cexs
								  if(bind::isBoolConst(arg_i_value))
								  {
									  LOG("ice", errs() << "UNCERTAIN VALUE: " << *arg_i_value << "\n";);
									  Expr uncertain_value = mk<FALSE>(arg_i_value->efac());
									  arg_i_value = uncertain_value;
								  }
								  else if(bind::isIntConst(arg_i_value))
								  {
									  LOG("ice", errs() << "UNCERTAIN VALUE: " << *arg_i_value << "\n";);
									  Expr uncertain_value = mkTerm<mpz_class>(0, arg_i_value->efac());
									  arg_i_value = uncertain_value;
								  }

								  //convert true/false to 1/0 in C5 data point
								  if(isOpX<TRUE>(arg_i_value))
								  {
									  arg_i_value = mkTerm<mpz_class>(1, arg_i_value->efac());
								  }
								  else if(isOpX<FALSE>(arg_i_value))
								  {
									  arg_i_value = mkTerm<mpz_class>(0, arg_i_value->efac());
								  }

								  //deal with too large integer value like: -0xffffffb
								  std::ostringstream oss;
								  oss << arg_i_value;
								  if(oss.str().find("-0x") == 0)
								  {
									  LOG("ice", errs() << "TOO LARGE VALUE, OVERFLOW: " << *arg_i_value << "\n";);
									  Expr uncertain_value = mkTerm<mpz_class>(0, arg_i_value->efac());
									  arg_i_value = uncertain_value;
								  }

								  attr_values.push_back(arg_i_value);
							  }

							  // If the counterexample is already labeled positive;
							  // Add its successive (aka. state transition) to positives instead.

							  DataPoint neg_dp(bind::fname(bind::fname(body_app)), attr_values);
							  negPoints.insert(neg_dp);
						  }

						  bool foundPos = true;
						  for (DataPoint neg_dp : negPoints) {
							  auto searched = m_pos_data_set.find(neg_dp);
							  if (searched != m_pos_data_set.end() || matchFacts (db, neg_dp)) {
								  // Found it in positive samples!
								  // foundPos = true;
							  } else {
								  foundPos = false;
								  break;
							  }
						  }
						  //auto searched = m_pos_data_set.find(neg_dp);
						  //if (searched != m_pos_data_set.end()) {
						  if (foundPos) {
							  if (bind::domainSz(bind::fname(r_head)) <= 0) {
								  errs()<<"Program is buggy.\n";
			  	  	  	  	  	  std::list<Expr> attr_values;
								  DataPoint pos_dp(bind::fname(bind::fname(r.head())), attr_values);
								  addPosCex(pos_dp);
								  failurePoint = m_pos_list.size()-1;
								  std::list<int> preIndices;
								  for (DataPoint neg_dp : negPoints) {
									  auto searched = m_pos_data_set.find(neg_dp);
									  if (searched != m_pos_data_set.end()) {
										  preIndices.push_back(m_pos_index_map[neg_dp]);
									  }
								  }
								  postree.insert(std::make_pair (m_pos_list.size()-1, preIndices));
								  return false;
							  }

							  std::list<Expr> attr_values;
							  for(int i=0; i<bind::domainSz(bind::fname(r_head)); i++)
							  {
								  Expr arg_i = r_head->arg(i+1);
								  Expr arg_i_value = m.eval(arg_i);

								  //deal with uncertain values in cexs
								  if(bind::isBoolConst(arg_i_value))
								  {
									  LOG("ice", errs() << "UNCERTAIN VALUE: " << *arg_i_value << "\n";);
									  Expr uncertain_value = mk<FALSE>(arg_i_value->efac());
									  arg_i_value = uncertain_value;
								  }
								  else if(bind::isIntConst(arg_i_value))
								  {
									  LOG("ice", errs() << "UNCERTAIN VALUE: " << *arg_i_value << "\n";);
									  Expr uncertain_value = mkTerm<mpz_class>(0, arg_i_value->efac());
									  arg_i_value = uncertain_value;
								  }

								  //convert true/false to 1/0 in C5 data point
								  if(isOpX<TRUE>(arg_i_value))
								  {
									  arg_i_value = mkTerm<mpz_class>(1, arg_i_value->efac());
								  }
								  else if(isOpX<FALSE>(arg_i_value))
								  {
									  arg_i_value = mkTerm<mpz_class>(0, arg_i_value->efac());
								  }

								  //deal with too large integer value like: -0xffffffb
								  std::ostringstream oss;
								  oss << arg_i_value;
								  if(oss.str().find("-0x") == 0)
								  {
									  LOG("ice", errs() << "TOO LARGE VALUE, OVERFLOW: " << *arg_i_value << "\n";);
									  Expr uncertain_value = mkTerm<mpz_class>(0, arg_i_value->efac());
									  arg_i_value = uncertain_value;
								  }

								  attr_values.push_back(arg_i_value);
							  }
							  DataPoint pos_dp(bind::fname(bind::fname(r_head)), attr_values);
							  int orig_size = m_pos_data_set.size();
							  addPosCex(pos_dp);
							  if(m_pos_data_set.size() == orig_size + 1) //no duplicate
							  {
								  if (SVMExecPath.compare("") != 0 && m_neg_data_set.size() > /*50*/ICESVMFreqPos) {
									  LOG("ice", errs() << "SVM based Hyperlane Learning!\n");
									  svmLearn (NULL);
								  }

								  m_cex_list.erase(std::remove_if(m_cex_list.begin(), m_cex_list.end(),
										  [pos_dp,r_head,this](DataPoint p) {
											  return p.getPredName() == bind::fname(bind::fname(r_head))
											  		  && m_neg_data_set.find(p) != m_neg_data_set.end();
									  	  	  //return p == pos_dp;
								  	  	  }), m_cex_list.end());
								  //m_neg_data_set.erase(std::remove_if(m_neg_data_set.begin(), m_neg_data_set.end(),
								  //		  [r_head](DataPoint p) {
								  //			  return p.getPredName() == bind::fname(bind::fname(r_head)); }), m_neg_data_set.end());
								  for (std::set<DataPoint>::iterator it = m_neg_data_set.begin(); it != m_neg_data_set.end(); ) {
									  if (it->getPredName() == bind::fname(bind::fname(r_head))) {
									  //if (*it == pos_dp) {
										  m_neg_data_set.erase (it++);
									  } else {
										  ++it;
									  }
								  }
								  m_neg_data_count.erase (pos_dp.getPredName());

								  // ---- Clean negative samples for body apps as well
								  for (Expr body_app : body_pred_apps) {
									  if (bind::domainSz(bind::fname(body_app)) <= 0)
										  continue;

									  m_cex_list.erase(std::remove_if(m_cex_list.begin(), m_cex_list.end(),
											  [body_app,this](DataPoint p) {
												  return p.getPredName() == bind::fname(bind::fname(body_app))
														  && m_neg_data_set.find(p) != m_neg_data_set.end();
												  //return p == pos_dp;
											  }), m_cex_list.end());
									  //m_neg_data_set.erase(std::remove_if(m_neg_data_set.begin(), m_neg_data_set.end(),
									  //		  [r_head](DataPoint p) {
									  //			  return p.getPredName() == bind::fname(bind::fname(r_head)); }), m_neg_data_set.end());
									  for (std::set<DataPoint>::iterator it = m_neg_data_set.begin(); it != m_neg_data_set.end(); ) {
										  if (it->getPredName() == bind::fname(bind::fname(body_app))) {
										  //if (*it == pos_dp) {
											  m_neg_data_set.erase (it++);
										  } else {
											  ++it;
										  }
									  }
									  m_neg_data_count.erase (bind::fname(bind::fname(body_app)));
								  }
								  // ---- Doubly check if the above code is necessary

								  m_cex_list.push_back(pos_dp);
								  addDataPointToIndex(pos_dp, index);

			  	  	  	  	  	  std::list<int> preIndices;
			  	  	  	  	  	  for (DataPoint neg_dp : negPoints) {
									  auto searched = m_pos_data_set.find(neg_dp);
									  if (searched != m_pos_data_set.end()) {
										  preIndices.push_back(m_pos_index_map[neg_dp]);
									  }
								  }
			  	  	  	  	  	  postree.insert(std::make_pair (m_pos_list.size()-1, preIndices));

								  LOG("ice", errs() << "POS CEX, INDEX IS " << index << "\n";);
								  index++;
								  posUpd = true;

								  bool run = sampleLinearHornCtrs(r_head, pos_dp, index);
			  	  	  	  	  	  if (!run) return false;

								  changedPreds.push_back(pos_dp.getPredName());
							  }
							  else //it is a duplicate data point
							  {
								  LOG("ice", errs() << "Duplicated positive points should be impossible.\n");
								  clearNegSamples (r_head, true);
								  //exit (-3);
							  }
						  } else {
							  for (DataPoint neg_dp : negPoints) {
								  auto searched = m_pos_data_set.find(neg_dp);
								  if (searched != m_pos_data_set.end() || matchFacts (db, neg_dp)) {
									  // Found this in positive set.
								  } else {
									  int orig_size = m_neg_data_set.size();
									  addNegCex(neg_dp);
									  if(m_neg_data_set.size() == orig_size + 1) //no duplicate
									  {
										  m_cex_list.push_back(neg_dp);
										  addDataPointToIndex(neg_dp, index);
										  LOG("ice", errs() << "NEG CEX, INDEX IS " << index << "\n";);
										  index++;

										  if (changedPreds.size() <= 1 ||
												  std::find(changedPreds.begin(), changedPreds.end(), neg_dp.getPredName()) == changedPreds.end())
											  changedPreds.push_back(neg_dp.getPredName());

										  if (SVMExecPath.compare("") != 0)
												  //&& m_neg_data_set.size() > 100 && m_neg_data_set.size() % 100 == 0) {
										  {
											  std::map<Expr, int>::iterator it = m_neg_data_count.find(neg_dp.getPredName());
											  if (it != m_neg_data_count.end() && it->second > ICESVMFreqNeg && it->second % ICESVMFreqNeg == 0) {
												  LOG("ice", errs() << "SVM based Hyperplane Learning!\n");
												  svmLearn (neg_dp.getPredName());
											  }
										  }
									  }
									  else //it is a duplicate data point
									  {
										  LOG("ice", errs() << "Duplicated negative points should be impossible.\n");
										  clearNegSamples (neg_dp.getPredName(), false);
										  //exit (-3);
									  }
								  }
							  }
						  }
					  }

					  if (upd) {
						  // Ask machine learning for a new invariant for body_app.
						  // Expr pre = m_candidate_model.getDef(body_app);
						  //if (SVMExecPath.compare("") == 0)
							  C5learn (changedPreds);
							  if (posUpd) {
								  /*for (Expr body_app : body_pred_apps) {
									  if (bind::fname(bind::fname(r_head)) == bind::fname(bind::fname(body_app)))
									  	  continue;
									  ExprVector arg_list;
									  Expr rel = bind::fname (body_app);
									  for(int i=0; i<bind::domainSz(rel); i++)
									  {
										  Expr arg_i_type = bind::domainTy(rel, i);
										  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
										  arg_list.push_back(arg_i);
									  }
									  Expr fapp = bind::fapp(rel, arg_list);
									  Expr preSolve = global_cache.getDef(fapp);
									  m_candidate_model.addDef(fapp, preSolve);
								  }*/
								  break;
							  }
							  else
								  for (Expr body_app : body_pred_apps) {
									  if (std::find(changedPreds.begin(), changedPreds.end(), bind::fname(bind::fname(body_app)))
									  	  == changedPreds.end()) {
									  } else {
										  ExprVector arg_list;
										  Expr rel = bind::fname (body_app);
										  for(int i=0; i<bind::domainSz(rel); i++)
										  {
											  Expr arg_i_type = bind::domainTy(rel, i);
											  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
											  arg_list.push_back(arg_i);
										  }
										  Expr fapp = bind::fapp(rel, arg_list);

										  Expr preSolve = local_cache.getDef(fapp);
										  Expr currSolve = m_candidate_model.getDef(fapp);
										  m_candidate_model.addDef(fapp, mk<AND>(preSolve, currSolve));
									  }
								  }
						  //else
						  //	  svmLearn (changedPreds);
					  }
				  } while (upd);
			  } while (true);
		  	  }
			  else {
			  do {
				  counter ++;

				  LOG("ice", errs() << "Rule Verification Round " << counter << "\n");

				  // Which predicates will be changed in this iteration of solving.
				  ExprVector changedPreds;
				  // FixMe. Bad Code.
				  //if (SVMExecPath.compare("") == 0) {
				  	  changedPreds.push_back (bind::fname(*(db.getRelations().begin())));
				  //}

				  //Expr currSolve = m_candidate_model.getDef(body_app);
				  //if (currSolve != preSolve) {
				//	  m_candidate_model.addDef(body_app, mk<AND>(preSolve, currSolve));
				  //}

				  upd = false;
				  Expr r_head = r.head();
				  Expr r_head_cand = m_candidate_model.getDef(r_head);

				  LOG("ice", errs() << "TRYING TO ADD some CounterExample.\n";);

				  solver.reset();

				  solver.assertExpr(mk<NEG>(r_head_cand));

				  Expr body_forumla = extractRelation(r, db, NULL, NULL);

				  LOG ("ice", errs() << "Verification condition: " << *r_head_cand << " <- " << *body_forumla << "\n");

				  solver.assertExpr(body_forumla);

				  //solver.toSmtLib(errs());
				  boost::tribool result = solver.solve();
				  if(result != UNSAT)
				  {
					  LOG("ice", errs() << "SAT, NEED TO ADD The Counterexample\n";);
					  upd = true; isChanged = true;
					  //get cex
					  ZModel<EZ3> m = solver.getModel();
					  //print cex
					  std::set<DataPoint> negPoints;
					  for (Expr body_app : body_pred_apps) {
						  if (bind::domainSz(bind::fname(body_app)) <= 0)
							  // No counterexample can be obtained from it because it is clean.
							  continue;

						  LOG("ice", errs() << "(";);
						  for(int i=0; i<bind::domainSz(bind::fname(body_app)); i++)
						  {
							  Expr arg_i = body_app->arg(i+1);
							  Expr arg_i_value = m.eval(arg_i);
							  LOG("ice", errs() << *arg_i_value << ",";);
						  }
						  LOG("ice", errs() << ") -> (";);
						  for(int i=0; i<bind::domainSz(bind::fname(r_head)); i++)
						  {
							  Expr arg_i = r_head->arg(i+1);
							  Expr arg_i_value = m.eval(arg_i);
							  LOG("ice", errs() << *arg_i_value << ",";);
						  }
						  LOG("ice", errs() << ")\n";);

						  // Presumbaly add counterexample
						  std::list<Expr> attr_values;
						  for(int i=0; i<bind::domainSz(bind::fname(body_app)); i++)
						  {
							  Expr arg_i = body_app->arg(i+1);
							  Expr arg_i_value = m.eval(arg_i);

							  //deal with uncertain values in cexs
							  if(bind::isBoolConst(arg_i_value))
							  {
								  LOG("ice", errs() << "UNCERTAIN VALUE: " << *arg_i_value << "\n";);
								  Expr uncertain_value = mk<FALSE>(arg_i_value->efac());
								  arg_i_value = uncertain_value;
							  }
							  else if(bind::isIntConst(arg_i_value))
							  {
								  LOG("ice", errs() << "UNCERTAIN VALUE: " << *arg_i_value << "\n";);
								  Expr uncertain_value = mkTerm<mpz_class>(0, arg_i_value->efac());
								  arg_i_value = uncertain_value;
							  }

							  //convert true/false to 1/0 in C5 data point
							  if(isOpX<TRUE>(arg_i_value))
							  {
								  arg_i_value = mkTerm<mpz_class>(1, arg_i_value->efac());
							  }
							  else if(isOpX<FALSE>(arg_i_value))
							  {
								  arg_i_value = mkTerm<mpz_class>(0, arg_i_value->efac());
							  }

							  //deal with too large integer value like: -0xffffffb
							  std::ostringstream oss;
							  oss << arg_i_value;
							  if(oss.str().find("-0x") == 0)
							  {
								  LOG("ice", errs() << "TOO LARGE VALUE, OVERFLOW: " << *arg_i_value << "\n";);
								  Expr uncertain_value = mkTerm<mpz_class>(0, arg_i_value->efac());
								  arg_i_value = uncertain_value;
							  }

							  attr_values.push_back(arg_i_value);
						  }

						  // If the counterexample is already labeled positive;
						  // Add its successive (aka. state transition) to positives instead.

						  DataPoint neg_dp(bind::fname(bind::fname(body_app)), attr_values);
						  negPoints.insert(neg_dp);
					  }

					  bool foundPos = true;
					  for (DataPoint neg_dp : negPoints) {
						  auto searched = m_pos_data_set.find(neg_dp);
						  if (searched != m_pos_data_set.end() || matchFacts (db, neg_dp)) {
							  // Found it in positive samples!
							  // foundPos = true;
						  } else {
							  foundPos = false;
							  break;
						  }
					  }
					  //auto searched = m_pos_data_set.find(neg_dp);
					  //if (searched != m_pos_data_set.end()) {
					  if (foundPos) {
						  if (bind::domainSz(bind::fname(r_head)) <= 0) {
							  errs()<<"Program is buggy.\n";
							  std::list<Expr> attr_values;
							  DataPoint pos_dp(bind::fname(bind::fname(r.head())), attr_values);
							  addPosCex(pos_dp);
							  failurePoint = m_pos_list.size()-1;
							  std::list<int> preIndices;
							  for (DataPoint neg_dp : negPoints) {
								  auto searched = m_pos_data_set.find(neg_dp);
								  if (searched != m_pos_data_set.end()) {
									  preIndices.push_back(m_pos_index_map[neg_dp]);
								  }
							  }
							  postree.insert(std::make_pair (m_pos_list.size()-1, preIndices));
							  return false;
						  }

						  std::list<Expr> attr_values;
						  for(int i=0; i<bind::domainSz(bind::fname(r_head)); i++)
						  {
							  Expr arg_i = r_head->arg(i+1);
							  Expr arg_i_value = m.eval(arg_i);

							  //deal with uncertain values in cexs
							  if(bind::isBoolConst(arg_i_value))
							  {
								  LOG("ice", errs() << "UNCERTAIN VALUE: " << *arg_i_value << "\n";);
								  Expr uncertain_value = mk<FALSE>(arg_i_value->efac());
								  arg_i_value = uncertain_value;
							  }
							  else if(bind::isIntConst(arg_i_value))
							  {
								  LOG("ice", errs() << "UNCERTAIN VALUE: " << *arg_i_value << "\n";);
								  Expr uncertain_value = mkTerm<mpz_class>(0, arg_i_value->efac());
								  arg_i_value = uncertain_value;
							  }

							  //convert true/false to 1/0 in C5 data point
							  if(isOpX<TRUE>(arg_i_value))
							  {
								  arg_i_value = mkTerm<mpz_class>(1, arg_i_value->efac());
							  }
							  else if(isOpX<FALSE>(arg_i_value))
							  {
								  arg_i_value = mkTerm<mpz_class>(0, arg_i_value->efac());
							  }

							  //deal with too large integer value like: -0xffffffb
							  std::ostringstream oss;
							  oss << arg_i_value;
							  if(oss.str().find("-0x") == 0)
							  {
								  LOG("ice", errs() << "TOO LARGE VALUE, OVERFLOW: " << *arg_i_value << "\n";);
								  Expr uncertain_value = mkTerm<mpz_class>(0, arg_i_value->efac());
								  arg_i_value = uncertain_value;
							  }

							  attr_values.push_back(arg_i_value);
						  }
						  DataPoint pos_dp(bind::fname(bind::fname(r_head)), attr_values);
						  int orig_size = m_pos_data_set.size();
						  addPosCex(pos_dp);
						  if(m_pos_data_set.size() == orig_size + 1) //no duplicate
						  {
							  if (SVMExecPath.compare("") != 0 && m_neg_data_set.size() > /*50*/ICESVMFreqPos) {
								  LOG("ice", errs() << "SVM based Hyperlane Learning!\n");
								  svmLearn (NULL);
							  }
							  if (!ICEICE) {
							  m_cex_list.erase(std::remove_if(m_cex_list.begin(), m_cex_list.end(),
									  [pos_dp,r_head,this](DataPoint p) {
										  return p.getPredName() == bind::fname(bind::fname(r_head))
										  		  && m_neg_data_set.find(p) != m_neg_data_set.end();
								  	  	  //return p == pos_dp;
							  	  	  }), m_cex_list.end());
							  //m_neg_data_set.erase(std::remove_if(m_neg_data_set.begin(), m_neg_data_set.end(),
							  //		  [r_head](DataPoint p) {
							  //			  return p.getPredName() == bind::fname(bind::fname(r_head)); }), m_neg_data_set.end());
							  for (std::set<DataPoint>::iterator it = m_neg_data_set.begin(); it != m_neg_data_set.end(); ) {
								  if (it->getPredName() == bind::fname(bind::fname(r_head))) {
								  //if (*it == pos_dp) {
									  m_neg_data_set.erase (it++);
								  } else {
									  ++it;
								  }
							  }
							  m_neg_data_count.erase (pos_dp.getPredName());

							  // ---- Clean negative samples for body apps as well
							  for (Expr body_app : body_pred_apps) {
								  if (bind::domainSz(bind::fname(body_app)) <= 0)
									  continue;

								  m_cex_list.erase(std::remove_if(m_cex_list.begin(), m_cex_list.end(),
										  [body_app,this](DataPoint p) {
											  return p.getPredName() == bind::fname(bind::fname(body_app))
													  && m_neg_data_set.find(p) != m_neg_data_set.end();
											  //return p == pos_dp;
										  }), m_cex_list.end());
								  //m_neg_data_set.erase(std::remove_if(m_neg_data_set.begin(), m_neg_data_set.end(),
								  //		  [r_head](DataPoint p) {
								  //			  return p.getPredName() == bind::fname(bind::fname(r_head)); }), m_neg_data_set.end());
								  for (std::set<DataPoint>::iterator it = m_neg_data_set.begin(); it != m_neg_data_set.end(); ) {
									  if (it->getPredName() == bind::fname(bind::fname(body_app))) {
									  //if (*it == pos_dp) {
										  m_neg_data_set.erase (it++);
									  } else {
										  ++it;
									  }
								  }
								  m_neg_data_count.erase (bind::fname(bind::fname(body_app)));
							  }
							  // ---- Doubly check if the above code is necessary
							  }
							  m_cex_list.push_back(pos_dp);
							  addDataPointToIndex(pos_dp, index);

							  std::list<int> preIndices;
							  for (DataPoint neg_dp : negPoints) {
								  auto searched = m_pos_data_set.find(neg_dp);
								  if (searched != m_pos_data_set.end()) {
									  preIndices.push_back(m_pos_index_map[neg_dp]);
							  	  }
							  }
							  postree.insert(std::make_pair (m_pos_list.size()-1, preIndices));

							  LOG("ice", errs() << "POS CEX, INDEX IS " << index << "\n";);
							  index++;
							  posUpd = true;

							  bool run = sampleLinearHornCtrs(r_head, pos_dp, index);
							  if (!run) return false;

							  changedPreds.push_back(pos_dp.getPredName());
						  }
						  else //it is a duplicate data point
						  {
							  LOG("ice", errs() << "Duplicated positive points should be impossible.\n");
							  clearNegSamples (r_head, true);
							  //exit (-3);
						  }
					  } else {
						  bool surebad = false;
						  if (ICEICE) {
							  if (negPoints.size() > 1) {
								  errs() << "ICE learning is not suitable for this program.\n";
								  exit (-3);
							  }
							  if (bind::domainSz(bind::fname(r_head)) <= 0) surebad = true;
						  }
						  if (ICEICE && !surebad && negPoints.size() == 1) {
							  // Add Implication samples.
							  std::list<Expr> attr_values;
							  for(int i=0; i<bind::domainSz(bind::fname(r_head)); i++)
							  {
								  Expr arg_i = r_head->arg(i+1);
								  Expr arg_i_value = m.eval(arg_i);

								  //deal with uncertain values in cexs
								  if(bind::isBoolConst(arg_i_value))
								  {
									  LOG("ice", errs() << "UNCERTAIN VALUE: " << *arg_i_value << "\n";);
									  Expr uncertain_value = mk<FALSE>(arg_i_value->efac());
									  arg_i_value = uncertain_value;
								  }
								  else if(bind::isIntConst(arg_i_value))
								  {
									  LOG("ice", errs() << "UNCERTAIN VALUE: " << *arg_i_value << "\n";);
									  Expr uncertain_value = mkTerm<mpz_class>(0, arg_i_value->efac());
									  arg_i_value = uncertain_value;
								  }

								  //convert true/false to 1/0 in C5 data point
								  if(isOpX<TRUE>(arg_i_value))
								  {
									  arg_i_value = mkTerm<mpz_class>(1, arg_i_value->efac());
								  }
								  else if(isOpX<FALSE>(arg_i_value))
								  {
									  arg_i_value = mkTerm<mpz_class>(0, arg_i_value->efac());
								  }

								  //deal with too large integer value like: -0xffffffb
								  std::ostringstream oss;
								  oss << arg_i_value;
								  if(oss.str().find("-0x") == 0)
								  {
									  LOG("ice", errs() << "TOO LARGE VALUE, OVERFLOW: " << *arg_i_value << "\n";);
									  Expr uncertain_value = mkTerm<mpz_class>(0, arg_i_value->efac());
									  arg_i_value = uncertain_value;
								  }

								  attr_values.push_back(arg_i_value);
							  }
							  DataPoint pos_dp(bind::fname(bind::fname(r_head)), attr_values);
							  for (DataPoint neg_dp : negPoints) {
								  if (neg_dp.getPredName() != pos_dp.getPredName()) {
									  errs() << "ICE learning is not suitable for this program.\n";
									  exit (-3);
								  }
								  if(m_impl_cex_set.count(neg_dp) == 0)
								  {
									  addImplCex(neg_dp);
									  m_cex_list.push_back(neg_dp);
									  addDataPointToIndex(neg_dp, index);
									  LOG("ice", errs() << "IMPL CEX, INDEX IS " << index << "\n";);
									  index++;
								  }
								  if(m_impl_cex_set.count(pos_dp) == 0)
								  {
									  addImplCex(pos_dp);
									  m_cex_list.push_back(pos_dp);
									  addDataPointToIndex(pos_dp, index);
									  LOG("ice", errs() << "IMPL CEX, INDEX IS " << index << "\n";);
									  index++;
								  }
								  addImplPair(std::make_pair(neg_dp, pos_dp));
								  changedPreds.push_back(neg_dp.getPredName());
							  }
						  }
						  else
						  for (DataPoint neg_dp : negPoints) {
							  auto searched = m_pos_data_set.find(neg_dp);
							  if (searched != m_pos_data_set.end() || matchFacts (db, neg_dp)) {
								  // Found this in positive set.
							  } else {
								  int orig_size = m_neg_data_set.size();
								  addNegCex(neg_dp);
								  if(m_neg_data_set.size() == orig_size + 1) //no duplicate
								  {
									  m_cex_list.push_back(neg_dp);
									  addDataPointToIndex(neg_dp, index);
									  LOG("ice", errs() << "NEG CEX, INDEX IS " << index << "\n";);
									  index++;

									  if (changedPreds.size() <= 1 ||
											  std::find(changedPreds.begin(), changedPreds.end(), neg_dp.getPredName()) == changedPreds.end())
										  changedPreds.push_back(neg_dp.getPredName());

									  if (SVMExecPath.compare("") != 0)
											  //&& m_neg_data_set.size() > 100 && m_neg_data_set.size() % 100 == 0) {
									  {
										  std::map<Expr, int>::iterator it = m_neg_data_count.find(neg_dp.getPredName());
										  if (it != m_neg_data_count.end() && it->second > ICESVMFreqNeg && it->second % ICESVMFreqNeg == 0) {
											  LOG("ice", errs() << "SVM based Hyperplane Learning!\n");
											  svmLearn (neg_dp.getPredName());
										  }
									  }
								  }
								  else //it is a duplicate data point
								  {
									  LOG("ice", errs() << "Duplicated negative points should be impossible.\n");
									  clearNegSamples (neg_dp.getPredName(), false);
									  //exit (-3);
								  }
							  }
						  }
					  }
				  }

				  if (upd) {
					  // Ask machine learning for a new invariant for body_app.
					  // Expr pre = m_candidate_model.getDef(body_app);
					  //if (SVMExecPath.compare("") == 0)
						  C5learn (changedPreds);
					  //else
					  //	  svmLearn (changedPreds);
				  }
			  } while (upd);
			  }
			  // --- Extend worklist as we just go through a strengthening loop ----
			  if (counter > 1) {
				  if (posUpd) addUsedToWorkList (db, workList, r);
				  else addNewToWorkList (db, workList, r);
			  }
		  }
	  }
	  LOG("ice", errs() << "==================================================================\n";);
	  return true;
  }

  // Sample Horn Constraint System.
  // Fixme. Not suitable for non-linear Horn Constraint System.
  bool ICE::sampleLinearHornCtrs (Expr head, DataPoint p, int &index) {
	  std::map<Expr, ExprVector> relationToPositiveStateMap;
	  std::map<HornRule, int> transitionCount;
	  ExprVector equations;

	  for(int i=0; i<=bind::domainSz(head); i++)
	  {
		  Expr var = bind::domainTy(head, i);
		  std::list<Expr>::iterator it = p.getAttrValues().begin();
		  std::advance(it, i);
		  Expr value = *it;

		  Expr arg_i_type = bind::domainTy(bind::fname (head), i);
		  std::ostringstream oss;
		  oss << *arg_i_type;

		  if (oss.str().compare("BOOL") == 0) {
			  oss.str(""); oss.clear(); oss << *value;
			  if (oss.str().compare("1") == 0) {
				  value = mk<TRUE>(var->efac());
			  } else if (oss.str().compare("0") == 0){
				  value = mk<FALSE>(var->efac());
			  } else {
				  exit (-3);
			  }
		  }

		  LOG("ice", errs() << "VAR: " << *var << "\n";);
		  LOG("ice", errs() << "VALUE: " << *value << "\n";);
		  equations.push_back(mk<EQ>(var, value));
	  }
	  Expr state_assignment;
	  if(equations.size() > 1)
	  {
		  state_assignment = mknary<AND>(equations.begin(), equations.end());
	  }
	  else
	  {
		  state_assignment = equations[0];
	  }
	  LOG("ice", errs() << "STATE ASSIGNMENT: " << *state_assignment << "\n";);

	  if(relationToPositiveStateMap.find(bind::fname(head)) == relationToPositiveStateMap.end())
	  {
		  ExprVector states;
		  states.push_back(state_assignment);
		  relationToPositiveStateMap.insert(std::make_pair(bind::fname(head), states));
	  }
	  else
	  {
		  ExprVector &states = relationToPositiveStateMap.find(bind::fname(head))->second;
		  states.push_back(state_assignment);
	  }

	  bool run = getReachableStates(transitionCount, relationToPositiveStateMap, head, p, index);
	  if (!run) return false;

	  LOG("ice", errs() << "THE WHOLE STATE MAP:\n";);
	  for(std::map<Expr, ExprVector>::iterator itr = relationToPositiveStateMap.begin(); itr != relationToPositiveStateMap.end(); ++itr)
	  {
		  LOG("ice", errs() << "KEY: " << *(itr->first) << "\n";);
		  LOG("ice", errs() << "VALUE: [";);
		  for(ExprVector::iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
		  {
			  LOG("ice", errs() << *(*itr2) << ", ";);
		  }
		  LOG("ice", errs() << "]\n";);
	  }
	  return true;
  }

  // Sample Horn Constraint System.
  // Fixme. Not suitable for non-linear Horn Constraint System.
  bool ICE::getReachableStates(std::map<HornRule, int> &transitionCount, std::map<Expr, ExprVector> &relationToPositiveStateMap,
		  	  	  	  Expr from_pred, DataPoint p, int &index)
  {
	  auto &db = m_hm.getHornClauseDB();
	  for(HornClauseDB::RuleVector::iterator itr = db.getRules().begin(); itr != db.getRules().end(); ++itr)
	  {
		  HornRule r = *itr;
		  std::map<HornRule, int>::iterator itc = transitionCount.find(r);
		  if (itc != transitionCount.end() && itc->second >= RuleSampleLen) {
			  // Avoid infinitely unroll a rule!
			  // Fixme. Set maximum unrolling number to be 101 or ...
			  continue;
		  }

		  ExprVector body_preds;
		  get_all_pred_apps(r.body(), db, std::back_inserter(body_preds));

		  if(body_preds.size() == 1 && bind::fname(body_preds[0]) == bind::fname(from_pred))
		  {
			  ExprVector equations;
			  for(int i=0; i<=bind::domainSz(body_preds[0]); i++)
			  {
				  Expr var = bind::domainTy(body_preds[0], i);
				  std::list<Expr>::iterator it = p.getAttrValues().begin();
				  std::advance(it, i);
				  Expr value = *it;

				  Expr arg_i_type = bind::domainTy(bind::fname (body_preds[0]), i);
				  std::ostringstream oss;
				  oss << *arg_i_type;

				  if (oss.str().compare("BOOL") == 0) {
					  oss.str(""); oss.clear(); oss << *value;
					  if (oss.str().compare("1") == 0) {
						  value = mk<TRUE>(var->efac());
					  } else if (oss.str().compare("0") == 0){
						  value = mk<FALSE>(var->efac());
					  } else {
						  exit (-3);
					  }
				  }

				  LOG("ice", errs() << "VAR: " << *var << "\n";);
				  LOG("ice", errs() << "VALUE: " << *value << "\n";);
				  equations.push_back(mk<EQ>(var, value));
			  }
			  Expr state_assignment;
			  if(equations.size() > 1)
			  {
				  state_assignment = mknary<AND>(equations.begin(), equations.end());
			  }
			  else
			  {
				  state_assignment = equations[0];
			  }

			  bool run = getRuleHeadState(transitionCount, relationToPositiveStateMap, r, state_assignment, m_pos_index_map[p], index);
			  if (!run) return false;
		  }
	  }
	  return true;
  }


  // Sample Horn Constraint System.
  // Fixme. Not suitable for non-linear Horn Constraint System.
  bool ICE::getRuleHeadState(std::map<HornRule, int> &transitionCount, std::map<Expr, ExprVector> &relationToPositiveStateMap,
		  	  	  HornRule r, Expr from_pred_state, int pindex, int &index)
  {
	  LOG("ice", errs() << "RULE HEAD: " << *(r.head()) << "\n";);
	  LOG("ice", errs() << "RULE BODY: " << *(r.body()) << "\n";);

	  auto &db = m_hm.getHornClauseDB();
	  ZSolver<EZ3> solver(m_hm.getZContext());

	  solver.assertExpr(from_pred_state);
	  solver.assertExpr(extractTransitionRelation(r, db));
	  //solver.toSmtLib(outs());

	  int iteri = 0;
	  boost::tribool isSat = solver.solve();
	  while(isSat)
	  {
		  if(bind::domainSz(bind::fname(r.head())) == 0)
		  {
			  //Fixme. reach a predicate with empty signature.
			  errs()<<"Program is buggy.\n";
			  std::list<Expr> attr_values;
			  DataPoint pos_dp(bind::fname(bind::fname(r.head())), attr_values);
			  addPosCex(pos_dp);
			  failurePoint = m_pos_list.size()-1;
			  std::list<int> preIndices;
			  preIndices.push_back(pindex);
			  postree.insert(std::make_pair (m_pos_list.size()-1, preIndices));
			  return false;
		  }

		  ZModel<EZ3> model = solver.getModel();
		  ExprVector equations;
		  std::list<Expr> attr_values;
		  ExprVector abstractEquations; // Do not assgin concrete values to uncertainties.
		  for(int i=0; i<=bind::domainSz(r.head()); i++)
		  {
			  Expr var = bind::domainTy(r.head(), i);
			  Expr value = model.eval(var);

			  if(bind::isBoolConst(value))
			  {
				  Expr uncertain_value = mk<FALSE>(value->efac());
				  value = uncertain_value;
			  }
			  else if(bind::isIntConst(value))
			  {
				  Expr uncertain_value = mkTerm<mpz_class>(0, value->efac());
				  value = uncertain_value;
			  } else
			  {
				  abstractEquations.push_back(mk<NEQ>(var, value));
			  }

			  LOG("ice", errs() << "VAR: " << *var << "\n";);
			  LOG("ice", errs() << "VALUE: " << *value << "\n";);
			  equations.push_back(mk<EQ>(var, value));

			  //convert true/false to 1/0 in C5 data point
			  if(isOpX<TRUE>(value))
			  {
				  value = mkTerm<mpz_class>(1, value->efac());
			  }
			  else if(isOpX<FALSE>(value))
			  {
				  value = mkTerm<mpz_class>(0, value->efac());
			  }
			  attr_values.push_back(value);
		  }
		  Expr state_assignment;
		  if(equations.size() > 1)
		  {
			  state_assignment = mknary<AND>(equations.begin(), equations.end());
		  }
		  else
		  {
			  state_assignment = equations[0];
		  }
		  LOG("ice", errs() << "STATE ASSIGNMENT: " << *state_assignment << "\n";);

		  DataPoint pos_dp(bind::fname(bind::fname(r.head())), attr_values);
		  int orig_size = m_pos_data_set.size();
		  addPosCex(pos_dp);

		  if(m_pos_data_set.size() == orig_size + 1) //no duplicate
		  {
			  Expr r_head = r.head();
			  if (!ICEICE) {
			  m_cex_list.erase(std::remove_if(m_cex_list.begin(), m_cex_list.end(),
					  [pos_dp,r_head,this](DataPoint p) {
			  	  return p.getPredName() == bind::fname(bind::fname(r_head))
			  	  	  	  	  && m_neg_data_set.find(p) != m_neg_data_set.end();
			  }), m_cex_list.end());


			  for (std::set<DataPoint>::iterator it = m_neg_data_set.begin(); it != m_neg_data_set.end(); ) {
				  if (it->getPredName() == bind::fname(bind::fname(r_head))) {
					  //if (*it == pos_dp) {
					  m_neg_data_set.erase (it++);
				  } else {
					  ++it;
				  }
			  }
			  m_neg_data_count.erase (pos_dp.getPredName());
			  }
			  m_cex_list.push_back(pos_dp);
			  addDataPointToIndex(pos_dp, index);

			  std::list<int> preIndices;
			  preIndices.push_back(pindex);
			  postree.insert(std::make_pair (m_pos_list.size()-1, preIndices));

			  LOG("ice", errs() << "POS CEX, INDEX IS " << index << "\n";);
			  index++;

			  ExprVector arg_list;
			  Expr rel = bind::fname (r_head);
			  for(int i=0; i<bind::domainSz(rel); i++)
			  {
				  Expr arg_i_type = bind::domainTy(rel, i);
				  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
				  arg_list.push_back(arg_i);
			  }
			  Expr fapp = bind::fapp(rel, arg_list);
			  Expr True = mk<TRUE>(rel->efac());
			  m_candidate_model.addDef(fapp, True);

			  if(relationToPositiveStateMap.find(bind::fname(r.head())) == relationToPositiveStateMap.end())
			  {
				  ExprVector states;
				  states.push_back(state_assignment);
				  relationToPositiveStateMap.insert(std::make_pair(bind::fname(r.head()), states));
			  }
			  else
			  {
				  ExprVector &states = relationToPositiveStateMap.find(bind::fname(r.head()))->second;
				  states.push_back(state_assignment);
			  }

			  std::map<HornRule, int>::iterator itc = transitionCount.find(r);
			  if (itc == transitionCount.end()) {
				  transitionCount.insert(std::make_pair(r, 1));
			  } else {
				  itc->second = itc->second + 1;
			  }

			  bool run = getReachableStates(transitionCount, relationToPositiveStateMap, r.head(), pos_dp, index);
			  if (!run) return false;

			  itc = transitionCount.find(r);
			  itc->second = itc->second - 1;
		  }

		  // Iterate with the next possible solution.
		  iteri ++;
		  if (iteri >= RuleSampleWidth) {
			  // Enough samples ?
			  break;
		  }

		  Expr notagain;
		  if(abstractEquations.size() > 1)
		  {
			  notagain = mknary<OR>(abstractEquations.begin(), abstractEquations.end());
		  } else if (abstractEquations.size() == 1) {
			  notagain = abstractEquations[0];
		  } else {
			  // There is nothing to be re-sampled ?
			  break;
		  }

		  solver.assertExpr(notagain);
		  isSat = solver.solve();
	  }
	  return true;
  }

  void countInvSize (Expr body_formula) {
      if (isOpX<OR> (body_formula)) {
    	  for (Expr arg : mk_it_range (body_formula->args_begin(), body_formula->args_end ())) {
    		  if (isOpX<AND> (arg)) {
				  std::cout << arg->arity() << ", ";
			  } else {
				  std::cout << "1, ";
			  }
    	  }
      } else {
    	  if (isOpX<AND> (body_formula)) {
			  std::cout << body_formula->arity() << ", ";
		  } else {
			  std::cout << "1, ";
		  }
      }
      std::cout << "\n";
  }

  //ice.runICE();
  void ICE::runICE()
  {
	  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  	  //load the Horn clause database
  	  auto &db = m_hm.getHornClauseDB ();
  	  db.buildIndexes ();
  	  LOG("ice", errs() << "DB: \n" << db;);

  	  int index = 0;
  	  bool isChanged = true;
  	  int c = 0;
  	  n_svm_calls = 0;
  	  failurePoint = -1;
  	  bool no_verification_found_error;
  	  while(isChanged) //There are still some counterexamples to find
  	  {
  		  isChanged = false;

  		  no_verification_found_error = solveConstraints(db, isChanged, index);

  		  if (!no_verification_found_error) { // A buggy program is catched.
  			  break;
  		  }

  		  c ++;
  	  }

  	  std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
  	  std::cout << "************** CHCs Solved in " <<
  			  (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /1000000.0 << " (secs) **************\n\n";

  	  if (no_verification_found_error) {
  		  std::cout << "************** Program is correct **************\n";
		  LOG("ice", errs() << "FINAL INVARIANTS MAP:\n";);
		  for(Expr rel : db.getRelations())
		  {
			  ExprVector arg_list;
			  for(int i=0; i<bind::domainSz(rel); i++)
			  {
				  Expr arg_i_type = bind::domainTy(rel, i);
				  Expr arg_i = bind::fapp(bind::constDecl(variant::variant(i, mkTerm<std::string> ("V", rel->efac ())), arg_i_type));
				  arg_list.push_back(arg_i);
			  }
			  Expr fapp = bind::fapp(rel, arg_list);
			  Expr cand = m_candidate_model.getDef(fapp);
			  LOG("ice", errs() << "REL: " << *fapp << ", CAND: " << *cand << "\n";);
			  std::cout << "REL: " << *fapp << " -- invariant size: ";
			  countInvSize (cand);
		  }
		  std::cout << "************** Program Correctness End **************\n\n";
  	  } else {
  		  std::cout << "************** Program is buggy **************\n";
  		  std::cout << "Buggy Trace:\n";
  		  drawPosTree ();
  		  std::cout << "************** Buggy Trace End **************\n\n";
  	  }

  	  std::cout <<"************** Learning Statistics **************:\n";
  	  std::cout <<"Total CHC size: " << db.getRules().size() << "\n";
  	  std::cout <<"Total Relation size: " << db.getRelations().size() << "\n";
  	  std::cout <<"Total Var size: " << db.getVars().size() << "\n";
  	  std::cout <<"Neg sample size: " << m_neg_data_set.size() << "\n";
  	  std::cout <<"Pos sample size: " << m_pos_data_set.size() << "\n";
  	  std::cout <<"Total sample size: " << m_neg_data_set.size() + m_pos_data_set.size() << "\n";
  	  std::cout <<"Iteration number: " << index << "\n";
  	  std::cout <<"************** Learning Statistics End **************\n\n";

  	  // Dump the invariants into file, for validation with Houdini
  	  if (!ICEInvDump.empty ())
  	  {
  		  saveInvsToSmtLibFile();
  	  }

  	  addInvarCandsToProgramSolver();
  }
}
