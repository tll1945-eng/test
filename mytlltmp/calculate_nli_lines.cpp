#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>

#include <mysql/mysql.h>
#include <strstream>
#include <map>
#include<cmath>

#include <cstring>
#include <set>

#include <unistd.h>



#include "/home/tll1945/Work/merge_csv/my_head_file.h"

using namespace std;




vector<string> n1;
vector<int> n2;
vector<int> n3;
vector<int> n4;
vector<int> n5;

struct dependence_record{
	string kind;
 	int begin_offset;
        int fprintf_line_number;   
        int ii;
        struct dependence_record *next; 
};


struct stmt_features{	
	int execution_times = 0;
	int value_factor = 0 ;
	int path_factor = 0;
	int generalized_value_factor = 0;
	int generalized_path_factor = 0; 	
	int distance_factor = 0;
	double info_hidden_factor  = 0.0;
	bool zext_factor = false;
	int dynamic_cmpinst_factor = 0;
	int static_cmpinst_factor = 0;
	double av_factor  = 0.0;
	double sd_factor = 0.0;
	bool calling_stmt_flag = false;
};

struct stmt_info{
	int nli_line = 0 ;
	map<int,int> exec_result_num_map;
	stmt_features sf;
	int test_result = -1;
	bool calling_stmt_flag = false;//tanlili add 2019.11.21  
	
};


void get_exec_nli_lines_for_curr_rand_tc(std::set<int> & rand_tc_exe_nli_lines){
	int nli_line,t,v,p,gv,gp;
	int d;
	bool zx;
	int dc,sc;
	double av,sd;
	bool c;
       
        //ifstream stmt_exec_features_file();
    	//stmt_exec_features_file.open("/home/tll1945/Work/giri-stmt-feature/test/HelloWorld/stmt_exec_features.txt");
	ifstream rand_tc_exec_nli_lines_file("/home/tll1945/Work/giri-execution-stmt/test/HelloWorld/rand_tc_exected_nli_lines.txt");
 
	 if (!rand_tc_exec_nli_lines_file.is_open()) {
  		std::cout<< "Error in read_rand_tc_exected_nli_lines_of_curr_tc: " << "\n";
      		exit(0);
    	}

	

	
	while (rand_tc_exec_nli_lines_file >> nli_line ){
		rand_tc_exe_nli_lines.insert(nli_line);
	}
	rand_tc_exec_nli_lines_file.close();
	
}       

void display_exec_nli_lines_for_curr_rand_tc(std::set<int> & rand_tc_exe_nli_lines){
	std::cout <<  std::endl;
	std::cout << "display_exec_nli_lines_for_curr_rand_tc" << std::endl;
	for(std::set<int>::iterator iter = rand_tc_exe_nli_lines.begin();iter != rand_tc_exe_nli_lines.end();iter++){
		std::cout << *iter << std::endl;
	}
}

void write_rand_tc_exec_nli_lines_to_csv_file(std::set<int> rand_tc_exe_nli_lines,const char * filename){
std::ofstream outfile;
   	outfile.open(filename);
 	for(std::set<int>::iterator iter=rand_tc_exe_nli_lines.begin(); iter!=rand_tc_exe_nli_lines.end();iter++){
		outfile << *iter << "," << std::endl;
	}
	
	outfile.close();

}


void write_mut_check_difficult_level_to_csv_file(std::map<int,vector<int> > mut_kill_rslts_by_each_tc,const char * mut_check_difficult_level_file, std::string tested_file,int at_least_exe_tc_num){


	
	std::ofstream outfile;
	outfile.open(mut_check_difficult_level_file);
	outfile << "mutid" << ","<< "total_exec_random_tc_num" << ","<< "kill_random_tc_num" << "," <<     "difficult_level" << "," << "tested_file" << std::endl;

 	for(std::map<int,vector<int>>::iterator iter=mut_kill_rslts_by_each_tc.begin(); iter!=mut_kill_rslts_by_each_tc.end();iter++){

		if(iter->second.size() >= at_least_exe_tc_num){

			int sum = 0 ; 
			for(int i  = 0 ; i < iter->second.size(); i++){
				sum  = sum + (iter->second)[i];
			}
			int mutid = iter->first;
			double difficult_level =  (double)sum/(double)iter->second.size();
			outfile << mutid << "," << iter->second.size() << "," << sum << "," << difficult_level << "," << tested_file << std::endl;

		}

	}
	
	outfile.close();
}





void cumm_stmt_exec_features_from_curr_tc(vector<stmt_info> & stmt_info_vec){
	int nli_line,t,v,p,gv,gp;
       
        //ifstream stmt_exec_features_file();
    	//stmt_exec_features_file.open("/home/tll1945/Work/giri-stmt-feature/test/HelloWorld/stmt_exec_features.txt");
	ifstream stmt_exec_features_file("/home/tll1945/Work/giri-stmt-feature/test/HelloWorld/stmt_exec_features.txt");
 
	 if (!stmt_exec_features_file.is_open()) {
  		std::cout<< "Error in read_stmt_exec_features_of_curr_tc: " << "\n";
      		exit(0);
    	}

	std::string nli_line_str;
	std::string t_str;
	std::string v_str;
	std::string p_str;
	std::string gv_str;
	std::string gp_str;


	stmt_exec_features_file >> nli_line_str >> t_str >> v_str >> p_str >> gv_str >> gp_str;
	while (stmt_exec_features_file >> nli_line >> t >> v >> p >> gv >> gp){
		for(int i =0 ; i < stmt_info_vec.size(); i++){
			if(stmt_info_vec[i].nli_line == nli_line){
				stmt_info_vec[i].sf.execution_times = stmt_info_vec[i].sf.execution_times + t;
				stmt_info_vec[i].sf.value_factor = stmt_info_vec[i].sf.value_factor + v;
				stmt_info_vec[i].sf.path_factor = stmt_info_vec[i].sf.path_factor + p;
				stmt_info_vec[i].sf.generalized_value_factor = stmt_info_vec[i].sf.generalized_value_factor + gv;
				stmt_info_vec[i].sf.generalized_path_factor = stmt_info_vec[i].sf.generalized_path_factor + gp;
				continue;
			}
		}
	}
	stmt_exec_features_file.close();
	
}       


void record_stmt_exec_features_from_curr_tc(vector<stmt_info> & stmt_info_vec){
	int nli_line,t,v,p,gv,gp;
       
        //ifstream stmt_exec_features_file();
    	//stmt_exec_features_file.open("/home/tll1945/Work/giri-stmt-feature/test/HelloWorld/stmt_exec_features.txt");
	ifstream stmt_exec_features_file("/home/tll1945/Work/giri-stmt-feature/test/HelloWorld/stmt_exec_features.txt");
 
	 if (!stmt_exec_features_file.is_open()) {
  		std::cout<< "Error in read_stmt_exec_features_of_curr_tc: " << "\n";//error !!!!!!, 2019.9.28
      		exit(0);
    	}

	std::string nli_line_str;
	std::string t_str;
	std::string v_str;
	std::string p_str;
	std::string gv_str;
	std::string gp_str;


	stmt_exec_features_file >> nli_line_str >> t_str >> v_str >> p_str >> gv_str >> gp_str;
	while (stmt_exec_features_file >> nli_line >> t >> v >> p >> gv >> gp){
		for(int i =0 ; i < stmt_info_vec.size(); i++){
			if(stmt_info_vec[i].nli_line == nli_line){
				stmt_info_vec[i].sf.execution_times =  t;
				stmt_info_vec[i].sf.value_factor =  v;
				stmt_info_vec[i].sf.path_factor =  p;
				stmt_info_vec[i].sf.generalized_value_factor = gv;
				stmt_info_vec[i].sf.generalized_path_factor = gp;
				continue;
			}
		}
	}
	stmt_exec_features_file.close();
	
}       



void display_recorded_stmt_exec_features(vector<stmt_info> stmt_info_vec){
	
      	std::cout<<"display_recorded_stmt_exec_features: " << std::endl;
        
      	for(int i = 0 ; i < stmt_info_vec.size(); i++){
		std::cout<<std::endl;
		std::cout << "stmt_info_vec["<< i<<"].nli_line: "<< stmt_info_vec[i].nli_line<<std::endl;

		std::cout << "stmt_info_vec["<< i<<"].sf.execution_times: "<< stmt_info_vec[i].sf.execution_times <<std::endl;		
		std::cout << "stmt_info_vec["<< i<<"].sf.value_factor: "<< stmt_info_vec[i].sf.value_factor<<std::endl;
		std::cout << "stmt_info_vec["<< i<<"].sf.path_factor: "<< stmt_info_vec[i].sf.path_factor<<std::endl;
		std::cout << "stmt_info_vec["<< i<<"].sf.generalized_value_factor: "<< stmt_info_vec[i].sf.generalized_value_factor<<std::endl;
		std::cout << "stmt_info_vec["<< i<<"].sf.generalized_path_factor: "<< stmt_info_vec[i].sf.generalized_path_factor<<std::endl;
		std::cout << "stmt_info_vec["<< i<<"].sf.info_hidden_factor: "<< stmt_info_vec[i].sf.info_hidden_factor<<std::endl;

		std::cout << "stmt_info_vec["<< i<<"].distance_factor: "<< stmt_info_vec[i].sf.distance_factor<<std::endl;//tanlili add 2019.11.21
	}
}       


int init_stmt_info_vec(vector<stmt_info> & stmt_info_vec, int total_nli_line){
	for(int i = 0 ; i < total_nli_line; i++){
		stmt_info st_if;
		st_if.nli_line = i+1;
		stmt_info_vec.push_back(st_if);
	}
}


void generate_mut_check_rslt(string suite_file_name, string newcompare_path,string tested_file,std::set<int>  rand_tc_exe_nli_lines,std::map<int,std::vector<int> > & mut_kill_rslts_by_each_tc,string test_case){

 	string rm_str = "rm ";
	
	string rm_tc_file_cmd = rm_str + suite_file_name;
	system(rm_tc_file_cmd.c_str());

	std::ofstream outfile4suite;
   	outfile4suite.open(suite_file_name);
 	
	outfile4suite << test_case  << std::endl;
	
	
	outfile4suite.close();

	string generate_newcompare_cmd = "gcc " + newcompare_path + "newcompare.c -g -O0 -o " + newcompare_path + "newcompare -lmysqlclient -std=c++11";
	system(generate_newcompare_cmd.c_str());


	//string enter_newcompare_dir_cmd = "chdir() " + newcompare_path;
	

	//system(enter_newcompare_dir_cmd.c_str());



	const char *path = newcompare_path.c_str();
	if(-1 == chdir(path))
	{
		printf("error %s\n", path);
		exit(1);
	}

	system("./newcompare");



	if(-1 == chdir("/home/tll1945/Work/giri-execution-stmt/test/HelloWorld"))
	{
		printf("error %s\n", path);
		exit(1);
	}



	std::ofstream outFile; 
	

    	LocTable loc_table;
    	LocTable * loc_table_p =  &loc_table;
    
    	loc_table_p->set_nli_table();


	ExeGenMUts exe_gen_muts;
    	exe_gen_muts.init();
        exe_gen_muts.connect();	
	exe_gen_muts.rec_mut_kill_rslt(loc_table_p, tested_file, rand_tc_exe_nli_lines, mut_kill_rslts_by_each_tc);
}




void add_test_result_of_each_tc_for_each_nli_stmt(vector<stmt_info> & stmt_info_vec,int exec_result){
	for(int i = 0; i < stmt_info_vec.size(); i++){
		if(stmt_info_vec[i].sf.execution_times >= 1){
			map<int, int>::iterator iter;
			iter = stmt_info_vec[i].exec_result_num_map.find(exec_result);
			
			if(iter != stmt_info_vec[i].exec_result_num_map.end()){
				iter->second = iter->second +1;
				
			}else{
				stmt_info_vec[i].exec_result_num_map.insert(make_pair(exec_result,1));
			}
		}
	}
}



void get_stmt_exec_features_and_exec_result_for_curr_tc(vector<stmt_info> & stmt_info_vec, int  exec_result){
	int nli_line,t,v,p,gv,gp;
	int d;
	bool zx;
	int dc,sc;
	double av,sd;
	bool c;
       
        //ifstream stmt_exec_features_file();
    	//stmt_exec_features_file.open("/home/tll1945/Work/giri-stmt-feature/test/HelloWorld/stmt_exec_features.txt");
	ifstream stmt_exec_features_file("/home/tll1945/Work/giri-stmt-feature/test/HelloWorld/stmt_exec_features.txt");
 
	 if (!stmt_exec_features_file.is_open()) {
  		std::cout<< "Error in read_stmt_exec_features_of_curr_tc: " << "\n";
      		exit(0);
    	}

	std::string nli_line_str;
	std::string t_str;
	std::string v_str;
	std::string p_str;
	std::string gv_str;
	std::string gp_str;
 
        std::string d_str;//tanlili add 2019.11.21
        std::string zx_str;
	std::string dc_str;
	std::string sc_str;
	std::string av_str;
	std::string sd_str;
	std::string c_str;

	stmt_exec_features_file >> nli_line_str >> t_str >> v_str >> p_str >> gv_str >> gp_str >> d_str >> zx_str >> dc_str >> sc_str >> av_str >> sd_str >> c_str;
	while (stmt_exec_features_file >> nli_line >> t >> v >> p >> gv >> gp >> d >> zx >> dc >> sc >> av >> sd >> c){
		for(int i =0 ; i < stmt_info_vec.size(); i++){
			if(stmt_info_vec[i].nli_line == nli_line){
				stmt_info_vec[i].sf.execution_times =  t;
				stmt_info_vec[i].sf.value_factor =  v;
				stmt_info_vec[i].sf.path_factor =  p;
				stmt_info_vec[i].sf.generalized_value_factor =  gv;
				stmt_info_vec[i].sf.generalized_path_factor =  gp;
				stmt_info_vec[i].sf.distance_factor =  d;
				stmt_info_vec[i].sf.zext_factor =  zx ;
				stmt_info_vec[i].sf.dynamic_cmpinst_factor =  dc;
				stmt_info_vec[i].sf.static_cmpinst_factor =  sc;
				stmt_info_vec[i].sf.av_factor =  av;
				stmt_info_vec[i].sf.sd_factor =  sd;
				
				stmt_info_vec[i].sf.calling_stmt_flag = c;

				
				continue;
			}
		}
	}
	stmt_exec_features_file.close();
	
}       







void calculate_info_hidden_factor(vector<stmt_info> & stmt_info_vec){
	double hidden_fac = 0.0;
	for(unsigned int i = 0 ; i < stmt_info_vec.size(); i++){
		if(stmt_info_vec[i].sf.execution_times >0){
			map<int,int>::iterator iter;
    			iter = stmt_info_vec[i].exec_result_num_map.begin();
			int size = 0;
    			while(iter != stmt_info_vec[i].exec_result_num_map.end()){
				size = size + iter->second;
        			iter++;
    			}

			iter = stmt_info_vec[i].exec_result_num_map.begin();
			double hidden_fac = 0.0 ;
			while(iter != stmt_info_vec[i].exec_result_num_map.end()){				
				double p = (double)(iter->second) /(double)size;
        			hidden_fac = hidden_fac - p*log(p); //tanlili modify hidden_fac - log(p) to hidden_fac - p*log(p), 2019.11.22
        			iter++;
    			}
			stmt_info_vec[i].sf.info_hidden_factor = hidden_fac;
		}
	}
}

void exec_rand_test_case(string test_case,vector<stmt_info> & stmt_info_vec, int total_nli_line,int i,string suite_file_name, string newcompare_path,string tested_file,std::map<int,vector<int>> & mut_kill_rslts_by_each_tc){

	system("rm hello-world.bc");
	system("rm hello-world.ll");
	system("rm hello-world.trace");
	system("rm hello-world.trace.bc");
	system("rm tmp-hello-world.bc");
	system("rm hello-world.trace.s");
	system("rm hello-world.trace.exe");
	system("rm stmt_exec_features.txt");
	system("rm tmp.txt");

        system("make -s -C ../../build/");
	system("clang -g -O0 -c -emit-llvm hello-world.c -o tmp-hello-world.bc");
	system("opt  -reg2mem tmp-hello-world.bc -o hello-world.bc");

	system("opt -reg2mem tmp-hello-world.bc -S -o hello-world.ll");
	system("opt -load ../../build//Debug+Asserts/lib/libdgutility.so -load ../../build//Debug+Asserts/lib/libgiri.so -mergereturn -bbnum -lsnum -trace-giri -trace-file=hello-world.trace -remove-bbnum -remove-lsnum -stats -debug -reg2mem hello-world.bc -o hello-world.trace.bc");
	system("llc -asm-verbose=false -O0  hello-world.trace.bc -o hello-world.trace.s");
	system("clang++ -fno-strict-aliasing hello-world.trace.s -o hello-world.trace.exe  -L../../build//Debug+Asserts/lib -lrtgiri");

	
        string executed_file_name("./hello-world.trace.exe ");
        string executed_test_cmd = executed_file_name + " " + test_case;
	
	system(executed_test_cmd.c_str());
	
	system("opt -load ../../build//Debug+Asserts/lib/libdgutility.so -load ../../build//Debug+Asserts/lib/libgiri.so -mergereturn -bbnum -lsnum -dgiri -trace-file=hello-world.trace -slice-file=hello-world.slice -remove-bbnum -remove-lsnum -stats -debug -reg2mem hello-world.bc -o /dev/null");


	
	//system("opt -load ../../build//Debug+Asserts/lib/libdgutility.so -load ../../build//Debug+Asserts/lib/libgiri.so -mergereturn -bbnum -lsnum -dgiri -trace-file=hello-world.trace -slice-file=hello-world.slice -remove-bbnum -remove-lsnum -stats -debug -reg2mem hello-world.bc > my_tmp.txt");
	


 	
	//cumm_stmt_exec_features_from_curr_tc(stmt_info_vec);
	//record_stmt_exec_features_from_curr_tc(stmt_info_vec);
	//std::cout<<"------------yyy xxx -----------"<<std::endl;
	//add_test_result_of_each_tc_for_each_nli_stmt(stmt_info_vec,exec_result);


	//get_stmt_exec_features_and_exec_result_for_curr_tc(stmt_info_vec,exec_result);


	

	


	std::set<int>  rand_tc_exe_nli_lines;
	get_exec_nli_lines_for_curr_rand_tc(rand_tc_exe_nli_lines);
        display_exec_nli_lines_for_curr_rand_tc(rand_tc_exe_nli_lines);

	generate_mut_check_rslt(suite_file_name,newcompare_path,tested_file,rand_tc_exe_nli_lines, mut_kill_rslts_by_each_tc,test_case);

	

}



/*
void seven_factors_to_mysql(vector<nli_line_info> my_nli_line_infoes){
	MYSQL *conn;
	MYSQL_RES *res;
        MYSQL_ROW row;
	int r;
  	conn = mysql_init(NULL); 

  	if (conn == NULL) {
      		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      		exit(1);
  	} 

  	if (mysql_real_connect(conn, "localhost", "root", "tll1945", "impact_factor", 0, NULL, 0) == NULL) {
      		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      		exit(1);
  	} 

        //string query_stat_prefix("select * from mutants where nli_linLoc = ");
	for(int i =0 ; i < my_nli_line_infoes.size(); i++){
		nli_line_info my_nli_line_info = my_nli_line_infoes[i];
		int nli_line_number = my_nli_line_info.nli_line;
		
		
		char query_buff[2014];
		sprintf(query_buff, "select * from mutants where nli_linLoc = %d",nli_line_number);
        	
		int t = mysql_query(conn, query_buff);
  		if (t) {
      			printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      			exit(1);
  		}else{
			res = mysql_store_result(conn);
			if(res){
				int num_fields = mysql_num_fields(res);
			 	while((row = mysql_fetch_row(res))) {

					double whole_output_cumm_change_possb = my_nli_line_info.whole_output_cumm_change_possb;
					double whole_path_cumm_change_possb = my_nli_line_info.whole_path_cumm_change_possb ;
					

					double whole_exec_branch_output_cumm_change_possb = my_nli_line_info.whole_exec_branch_output_cumm_change_possb ;
					double whole_exec_branch_path_cumm_change_possb = my_nli_line_info.whole_exec_branch_path_cumm_change_possb;

					double dyn_exec_totoal_num = my_nli_line_info.dyn_exec_totoal_num ;
					double average_repeated_emergence_possb = my_nli_line_info.average_repeated_emergence_possb ;

					//double whole_not_exec_branch_path_change_length = my_nli_line_info.whole_not_exec_branch_path_change_length;
					double whole_not_exec_branch_path_change_length_ratio = my_nli_line_info.whole_not_exec_branch_path_change_length_ratio;
					
				 	double value_factor = atof(row[1]);



                                	value_factor =  whole_output_cumm_change_possb;
					
					//tanlili delete 2018.7.25
					//double path_factor = atof(row[2]);
					//path_factor = whole_path_cumm_change_possb ;


					//tanlili add 2018.7.25
					double path_factor = atof(row[2]);
					path_factor = whole_exec_branch_output_cumm_change_possb ;

					double un_related_fprintf_num = atof(row[3]);
					un_related_fprintf_num = dyn_exec_totoal_num;
					
					double generalized_value_factor= atof(row[6]);
                                	generalized_value_factor = average_repeated_emergence_possb;

					double generalized_path_factor = atof(row[7]);
                                	generalized_path_factor= whole_path_cumm_change_possb;

					double info_hidden_factor = atof(row[8]);
					info_hidden_factor= whole_exec_branch_path_cumm_change_possb;

					double calling_stmt_flag= atof(row[10]);
                                	//calling_stmt_flag = whole_not_exec_branch_path_change_length;
					calling_stmt_flag = whole_not_exec_branch_path_change_length_ratio;
                                	
					char update_path_and_output_and_execNum_factor_buff[2014];
					sprintf(update_path_and_output_and_execNum_factor_buff,
						"update mutants set value_factor = %f, path_factor = %f,execution_times = %f, generalized_value_factor =%f,generalized_path_factor = %f,info_hidden_factor= %f, calling_stmt_flag =%f where nli_linLoc = %d",					value_factor,path_factor,un_related_fprintf_num, generalized_value_factor, generalized_path_factor,info_hidden_factor, calling_stmt_flag,nli_line_number);
					int tt = mysql_query(conn,update_path_and_output_and_execNum_factor_buff);
					if (tt) {
      						printf("Error : update mysql 8\n");
						exit(1);
					}
				}	
			
			}
		}

	}
 
	mysql_free_result(res);
  	mysql_close(conn);

}
*/





void  build_mysql_table_and_init(int line_num)
{
  MYSQL *conn;
  conn = mysql_init(NULL); 

  if (conn == NULL) {
      printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      exit(1);
  } 

  if (mysql_real_connect(conn, "localhost", "root", "tll1945", NULL, 0, NULL, 0) == NULL) {
      printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      exit(1);
  } 

  if (mysql_query(conn,"drop database if exists impact_factor")){
  	printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      	exit(1);
  }

  if (mysql_query(conn, "create database impact_factor")) {
      printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      exit(1);
  }

  mysql_query(conn,"use impact_factor");

  mysql_query(conn, "CREATE TABLE mutants(nli_linLoc INT NOT NULL,value_factor DOUBLE NOT NULL,path_factor DOUBLE NOT NULL,execution_times DOUBLE NOT NULL,offset INT NOT NULL,record_len INT NOT NULL,generalized_value_factor DOUBLE NOT NULL,generalized_path_factor DOUBLE NOT NULL,info_hidden_factor DOUBLE NOT NULL,impacting_num INT NOT NULL, distance_factor INT NOT NULL,zext_factor INT NOT NULL,dynamic_cmpinst_factor INT NOT NULL,static_cmpinst_factor INT NOT NULL,av_factor DOUBLE NOT NULL, sd_factor DOUBLE NOT NULL, calling_stmt_flag INT NOT NULL)");
  //mysql_query(conn, "CREATE TABLE mutants(nli_linLoc INT NOT NULL,value_factor INT NOT NULL,path_factor INT NOT NULL,execution_times INT NOT NULL,offset INT NOT NULL,record_len INT NOT NULL)");
/*
  mysql_query(conn, "CREATE TABLE mutants(nli_linLoc INT NOT NULL,value_factor INT NOT NULL,path_factor INT NOT NULL,execution_times INT NOT NULL,offset INT NOT NULL,record_len INT NOT NULL,self_path_factor INT NOT NULL)");//path_factor is for other stmt,self_path_factor is about itself 
*/
  //read line num of tested .c program
  //int line_num = line_num_of_tested_nli();
  int i;
  
  for(i=1;i<=line_num;i++){
      strstream offset_ss;
      string offset_str;
      offset_ss << n3[i-1];
      offset_ss >> offset_str;

      strstream record_len_ss;
      string record_len_str;
      record_len_ss << n4[i-1];
      record_len_ss >> record_len_str;


      char c[1024];
      sprintf(c, "%d", i);
      //string str1("INSERT INTO mutants VALUES('");
      //str1=str1+c+"',0,0,0)";

      string str1("INSERT INTO mutants VALUES(");
      str1=str1+c+",0,0,0,"+offset_str+","+record_len_str+",0,0,0,0,0,0,0,0,0,0,0)";

      //str1=str1+c+",0,0,0,"+offset_str+","+record_len_str+",0)";
      //string str1("INSERT INTO mutants VALUES('0','0','0','0','0')");
      //string str1("INSERT INTO mutants VALUES('");
      //str1=str1+c+"',0,0,0)";
      mysql_query(conn, str1.c_str());
  }
  mysql_close(conn);
}


int obtain_stat_offset_form_nli(){
	char c;
    	int totalLineNumber = 0;
    	ifstream fin("/home/tll1945/Work/proteumIM2.0-master/LINUX/bin/__myprog.nli",ios::in);
    	if(!fin){
       		cout << "error !!!" <<endl;
       	return -1;
    	}

    	while(fin.get(c)){
		if(c=='\n')
		totalLineNumber++;
    	}

    	fin.close();
  
   	int toal_nli_line_num = totalLineNumber;
        
        vector<dependence_record *> dep_rec_vec;
        for(int i=0;i<totalLineNumber;i=i+1){
		//p = (dependence_record *)malloc(sizeof(dependence_record));
                struct dependence_record *p ;
                p = NULL;
        	dep_rec_vec.push_back(p);
        }
        

    	ifstream inFile;
    	int count; 
 
    	inFile.open("/home/tll1945/Work/proteumIM2.0-master/LINUX/bin/__myprog.nli");
 
        string x1;
        int x2;
        int x3;
        int x4;
        int x5;

    	for (count=0; count<totalLineNumber;count=count+1)    //读数据 
    	{
        	inFile>>x1>>x2>>x3>>x4>>x5;
                n1.push_back(x1);
		n2.push_back(x2);
		n3.push_back(x3);
		n4.push_back(x4);
		n5.push_back(x5);
    	}
    	inFile.close();
 
        cout << "obtain_stat_offset_form_nli() " << endl;
    	for (int i=0;i<totalLineNumber;i=i+1)         //显示数据
    	{
        	//cout<<setw(3)<<n1[i]<<setw(9)<<n2[i]<<setw(12)<<n3[i]<<setw(15)<<n4[i]<<setw(18)<<n5[i]<<endl;
    	}
        cout << "obtain_stat_offset_form_nli() is over " << endl;
    	return toal_nli_line_num;
}


void my_seven_factors_to_mysql(vector<stmt_info> stmt_info_vec){
	MYSQL *conn;
	MYSQL_RES *res;
        MYSQL_ROW row;
	int r;
  	conn = mysql_init(NULL); 

  	if (conn == NULL) {
      		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      		exit(1);
  	} 

  	if (mysql_real_connect(conn, "localhost", "root", "tll1945", "impact_factor", 0, NULL, 0) == NULL) {
      		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      		exit(1);
  	} 

       
	for(int i =0 ; i < stmt_info_vec.size(); i++){
		
		stmt_info my_nli_line_info = stmt_info_vec[i];
		
		
		char query_buff[2014];
		sprintf(query_buff, "select * from mutants where nli_linLoc = %d", my_nli_line_info.nli_line);
        	
		int t = mysql_query(conn, query_buff);
  		if (t) {
      			printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      			exit(1);
  		}else{
			res = mysql_store_result(conn);
			if(res){
				int num_fields = mysql_num_fields(res);
			 	while((row = mysql_fetch_row(res))) {
                                	
					char update_path_and_output_and_execNum_factor_buff[2014];
					sprintf(update_path_and_output_and_execNum_factor_buff,
						"update mutants set value_factor = %f, path_factor = %f,execution_times = %f, generalized_value_factor =%f,generalized_path_factor = %f,info_hidden_factor= %f, distance_factor =%d where nli_linLoc = %d",					(double)my_nli_line_info.sf.value_factor,  (double)my_nli_line_info.sf.path_factor, (double)my_nli_line_info.sf.execution_times, (double)my_nli_line_info.sf.generalized_value_factor, (double)my_nli_line_info.sf.generalized_path_factor, (double)my_nli_line_info.sf.info_hidden_factor, my_nli_line_info.sf.distance_factor, my_nli_line_info.nli_line);
					int tt = mysql_query(conn,update_path_and_output_and_execNum_factor_buff);
					if (tt) {
      						printf("Error : update mysql 8\n");
						exit(1);
					}
				}	
			
			}
		}

	}
 
	mysql_free_result(res);
  	mysql_close(conn);

}

void my_factors_to_mysql(vector<stmt_info> stmt_info_vec){
	MYSQL *conn;
	MYSQL_RES *res;
        MYSQL_ROW row;
	int r;
  	conn = mysql_init(NULL); 

  	if (conn == NULL) {
      		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      		exit(1);
  	} 

  	if (mysql_real_connect(conn, "localhost", "root", "tll1945", "impact_factor", 0, NULL, 0) == NULL) {
      		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      		exit(1);
  	} 

       
	for(int i =0 ; i < stmt_info_vec.size(); i++){
		
		stmt_info my_nli_line_info = stmt_info_vec[i];
		
		
		char query_buff[2014];
		sprintf(query_buff, "select * from mutants where nli_linLoc = %d", my_nli_line_info.nli_line);
        	
		int t = mysql_query(conn, query_buff);
  		if (t) {
      			printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      			exit(1);
  		}else{
			res = mysql_store_result(conn);
			if(res){
				int num_fields = mysql_num_fields(res);
			 	while((row = mysql_fetch_row(res))) {
                                	
					char update_path_and_output_and_execNum_factor_buff[2014];
					sprintf(update_path_and_output_and_execNum_factor_buff,
						"update mutants set value_factor = %f, path_factor = %f,execution_times = %f, generalized_value_factor =%f,generalized_path_factor = %f, distance_factor =%d,  zext_factor=%d, dynamic_cmpinst_factor=%d, static_cmpinst_factor=%d,av_factor = %f, sd_factor = %f, calling_stmt_flag = %d where nli_linLoc = %d",(double)my_nli_line_info.sf.value_factor,  (double)my_nli_line_info.sf.path_factor, (double)my_nli_line_info.sf.execution_times, (double)my_nli_line_info.sf.generalized_value_factor, (double)my_nli_line_info.sf.generalized_path_factor,  my_nli_line_info.sf.distance_factor,my_nli_line_info.sf.zext_factor, my_nli_line_info.sf.dynamic_cmpinst_factor, my_nli_line_info.sf.static_cmpinst_factor,my_nli_line_info.sf.av_factor, my_nli_line_info.sf.sd_factor, 
my_nli_line_info.sf.calling_stmt_flag, my_nli_line_info.nli_line);
					int tt = mysql_query(conn,update_path_and_output_and_execNum_factor_buff);
					if (tt) {
      						printf("Error : update mysql 8\n");
						exit(1);
					}
				}	
			
			}
		}

	}
 
	mysql_free_result(res);
  	mysql_close(conn);

}







void display_mut_kill_rslts_by_each_tc(std::map<int,vector<int>> mut_kill_rslts_by_each_tc){
	for(std::map<int,vector<int> >::iterator iter_mut = mut_kill_rslts_by_each_tc.begin();
		iter_mut != mut_kill_rslts_by_each_tc.end(); iter_mut++){
		std::cout << std::endl;
		std::cout << iter_mut->first << std::endl;
		for(int i = 0 ; i < iter_mut->second.size(); i++){
			std::cout << (iter_mut->second)[i] << std::endl;
		}
	}
}








int main(int argc, const char ** argv)
{	


	system("cp /home/tll1945/Work/proteumIM2.0-master/LINUX/bin/__myprog.c hello-world.c");

	clock_t startTime,endTime;
  	startTime = clock();
	
	

	int total_nli_line_num =  obtain_stat_offset_form_nli();
        build_mysql_table_and_init(total_nli_line_num);

	int at_least_exe_tc_num =5;



/*
	string tested_file = "totinfo";
	string suite_file_name = "/home/tll1945/Work/totinfo/testplans.alt/testplans-rand/suite182";
	string newcompare_path = "/home/tll1945/Work/totinfo/scripts/";
	#define my_tc_number 14
	string mytcs[my_tc_number]  = {" < /home/tll1945/Work/totinfo/inputs/universe/jkAAQ.mat", 
					"  < /home/tll1945/Work/totinfo/inputs/universe/jkADe.mat",
					" <  /home/tll1945/Work/totinfo/inputs/universe/test116.inc",
					" < /home/tll1945/Work/totinfo/inputs/universe/12new52",
					" <  /home/tll1945/Work/totinfo/inputs/universe/test138.inc",
					"  <  /home/tll1945/Work/totinfo/inputs/universe/test58.inc",
					" < /home/tll1945/Work/totinfo/inputs/universe/jkAD].mat",
					" <  /home/tll1945/Work/totinfo/inputs/universe/test137.inc",
					" < /home/tll1945/Work/totinfo/inputs/universe/jkABa.mat",
					"  < /home/tll1945/Work/totinfo/inputs/universe/jkAED.mat",
					" < /home/tll1945/Work/totinfo/inputs/universe/test91",
					" < /home/tll1945/Work/totinfo/inputs/universe/jkAEs.mat",
					" <  /home/tll1945/Work/totinfo/inputs/universe/test106.inc",
					" < /home/tll1945/Work/totinfo/inputs/universe/ntest50"};
      
*/

       

/*	
	string tested_file = "printtokens2";
	string suite_file_name = "/home/tll1945/Work/printtokens2/testplans.alt/testplans-rand/suite348";
	string newcompare_path = "/home/tll1945/Work/printtokens2/scripts/";
	#define my_tc_number 19
	string mytcs[my_tc_number]  = {" /home/tll1945/Work/printtokens2/inputs/uslin.1003", 
					"  < /home/tll1945/Work/printtokens2/inputs/tc157",
					" < /home/tll1945/Work/printtokens2/inputs/tc34",
					" /home/tll1945/Work/printtokens2/inputs/uslin.1069",
					" < /home/tll1945/Work/printtokens2/inputs/uslin.1170",
					" /home/tll1945/Work/printtokens2/inputs/uslin.1179 ",
					" < /home/tll1945/Work/printtokens2/inputs/tc363",
					" < /home/tll1945/Work/printtokens2/inputs/uslin.1984",
					" < /home/tll1945/Work/printtokens2/inputs/uslin.353",
					"  < /home/tll1945/Work/printtokens2/inputs/uslin.558",
					" /home/tll1945/Work/printtokens2/inputs/uslin.406",
					" < /home/tll1945/Work/printtokens2/inputs/tc315",
					" < /home/tll1945/Work/printtokens2/inputs/tc152",
					" < /home/tll1945/Work/printtokens2/inputs/ts587",
					" one doesntliketwo",
					" /home/tll1945/Work/printtokens2/inputs/uslin.664",
					" /home/tll1945/Work/printtokens2/inputs/uslin.532",
					" < /home/tll1945/Work/printtokens2/inputs/tst173",
					" < /home/tll1945/Work/printtokens2/inputs/ts652"};



*/

/*
string tested_file = "printtokens";
	string suite_file_name = "/home/tll1945/Work/printtokens/testplans.alt/testplans-rand/suite387";
	string newcompare_path = "/home/tll1945/Work/printtokens/scripts/";
	#define my_tc_number 14
	string mytcs[my_tc_number]  = {" < /home/tll1945/Work/printtokens/inputs/uslin.113",
					" < /home/tll1945/Work/printtokens/inputs/test380",
					" < /home/tll1945/Work/printtokens/inputs/uslin.811",
					" /home/tll1945/Work/printtokens/inputs/uslin.1450",
					" /home/tll1945/Work/printtokens/inputs/jk50",
					" /home/tll1945/Work/printtokens/inputs/uslin.1092",
					" < /home/tll1945/Work/printtokens/inputs/tc88",
					" < /home/tll1945/Work/printtokens/inputs/test351",
					" < /home/tll1945/Work/printtokens/inputs/newtst503.tst",
					" /home/tll1945/Work/printtokens/inputs/uslin.1815",
					" < /home/tll1945/Work/printtokens/inputs/tc127",
					" < /home/tll1945/Work/printtokens/inputs/test89",
					" /home/tll1945/Work/printtokens/inputs/uslin.242",
					" /home/tll1945/Work/printtokens/inputs/uslin.265"};

*/


/*
string tested_file = "hashing_code";
	string suite_file_name = "/home/tll1945/Work/hashing_code/testplans.alt/testplans-rand/suite1";
	string newcompare_path = "/home/tll1945/Work/hashing_code/scripts/";
	#define my_tc_number 17
	string mytcs[my_tc_number]  = {" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_0",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_1",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_2",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_3",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_4",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_5",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_6",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_7",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_8",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_9",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_10",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_11",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_12",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_13",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_14",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_15",
					" < /home/tll1945/Work/hashing_code/inputs/input/rand_tc_16"};

*/






string tested_file = "cervix_grades";
	string suite_file_name = "/home/tll1945/Work/cervix_grades/testplans.alt/testplans-rand/suite1";
	string newcompare_path = "/home/tll1945/Work/cervix_grades/scripts/";
	#define my_tc_number 14
	string mytcs[my_tc_number]  = {" < /home/tll1945/Work/cervix_grades/inputs/input/rand_tc_0",
					" < /home/tll1945/Work/cervix_grades/inputs/input/rand_tc_1",
					" < /home/tll1945/Work/cervix_grades/inputs/input/rand_tc_2",
					" < /home/tll1945/Work/cervix_grades/inputs/input/rand_tc_3",
					" < /home/tll1945/Work/cervix_grades/inputs/input/rand_tc_4",
					" < /home/tll1945/Work/cervix_grades/inputs/input/rand_tc_5",
					" < /home/tll1945/Work/cervix_grades/inputs/input/rand_tc_6",
					" < /home/tll1945/Work/cervix_grades/inputs/input/rand_tc_7",
					" < /home/tll1945/Work/cervix_grades/inputs/input/rand_tc_8",
					" < /home/tll1945/Work/cervix_grades/inputs/input/rand_tc_9",
					" < /home/tll1945/Work/cervix_grades/inputs/input/rand_tc_10",
					" < /home/tll1945/Work/cervix_grades/inputs/input/rand_tc_11",
					" < /home/tll1945/Work/cervix_grades/inputs/input/rand_tc_12",
					" < /home/tll1945/Work/cervix_grades/inputs/input/rand_tc_13"};

/*
	string tested_file = "tcas";
	string suite_file_name = "/home/tll1945/Work/tcas/testplans.alt/testplans-rand/suite540";
	string newcompare_path = "/home/tll1945/Work/tcas/scripts/";
	#define my_tc_number 10
	string mytcs[my_tc_number]  = {" 932 0 1 2219  415  189 0  641  500 1 1 0", 
					"  785 0 1 2802  447 2187 1  400  740 1 1 1",
					" 1062 1 1 1941 575 682 3 872 880 1 2 0",
					" 850 1 0 187 443 542 0 410 407 4 2 0",
					" 1001 1 1 737 -100 741 1 828 0 0 2 0",
					"  841 0 1 3618  311 1559 1  400  739 1 0 1",
					" 795 1 -1 538 325 601 2 702 634 0 1 1",
					" 1078 1 1 581 0 637 906 0 1 1",
					" 634 1 1 633 200 535 2 665 795 0 1 1",
					"  888 1 0 3648  302  215 1  501  399 1 1 0"};
      
*/



	vector<stmt_info> stmt_info_vec;
	stmt_info_vec.clear();
	init_stmt_info_vec(stmt_info_vec, total_nli_line_num);
	std::cout<<"stmt_info_vec.size: " << stmt_info_vec.size()<< std::endl;


	std::map<int,vector<int>>  mut_kill_rslts_by_each_tc;

	for(int i = 0 ; i < my_tc_number; i++){
		exec_rand_test_case(mytcs[i],stmt_info_vec,total_nli_line_num,i, suite_file_name, newcompare_path, tested_file, mut_kill_rslts_by_each_tc);
		//std::cout << "stmt_info_vec size: " << stmt_info_vec.size() << std::endl;

		
		//generate_mut_check_rslt(suite_file_name,newcompare_path,argv[1]);
		
	}



	display_mut_kill_rslts_by_each_tc(mut_kill_rslts_by_each_tc);
/*	
	char rand_tc_base_info[] = "/home/tll1945/Work/ml_for_stmt_mut_score/exe_general_muts/rand_tc_exe_nlil_lines_";
	char rand_tc_nli_lines_filename[1024];
	char* str_i = (char *)malloc(1024);
	strcpy(rand_tc_nli_lines_filename,rand_tc_base_info);
	sprintf(str_i, "%d", i);
	strcat(rand_tc_nli_lines_filename,str_i);
	strcat(rand_tc_nli_lines_filename,".csv");

	
	write_rand_tc_exec_nli_lines_to_csv_file(rand_tc_exe_nli_lines,rand_tc_nli_lines_filename);

	*/
	const char * mut_check_difficult_level_file = "/home/tll1945/Work/ml_for_stmt_mut_score/exe_general_muts/mut_check_difficult_level_file.csv";
	write_mut_check_difficult_level_to_csv_file(mut_kill_rslts_by_each_tc,mut_check_difficult_level_file,tested_file,at_least_exe_tc_num);

}




