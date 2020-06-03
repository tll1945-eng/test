#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>

#include <mysql/mysql.h>
#include <strstream>
#include <map>
#include<cmath>

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

void exec_test_case(string test_case,vector<stmt_info> & stmt_info_vec, int total_nli_line,int exec_result){

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
	get_stmt_exec_features_and_exec_result_for_curr_tc(stmt_info_vec,exec_result);
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
int main(int argc, const char ** argv)
{	
	clock_t startTime,endTime;
  	startTime = clock();
	

	int total_nli_line_num =  obtain_stat_offset_form_nli();
        build_mysql_table_and_init(total_nli_line_num);

/*
//my first program under test

//tcas  /home/tll1945/Work/tcas/testplans.alt/testplans-cov-extended/suite14,ok
	#define my_tc_number 21
	string mytcs[my_tc_number] = {"  1062 1 0 1941 545 582 2 772 780 1 2 0",
				"  827 0 1 1691  437  776 1  641  399 1 0 1",
				"  867 1 1 1774  101 2204 0  499  499 1 0 1",
				" 934 1 1 2743  366 5463 2  739  399 0 1 1",
				" 1078 1 1 581 906 1 1",
				" 804 0 1 3514  152 1152 1  740  501 1 1 1",
				" 958 1 0 797  64 3253 0  399  100 1 0 1",
				" 934 1 0 433 400 433 0 445 350 1 0 0",
				" 691 0 0 0 548 737 1 410 364 1 1 0",
				" 598 0 1 2589  130  629 3  399  500 0 0 1",
				" 612 1 0 4829    4  200 1  399  500 1 1 1",
				" 661 1 1 1802  117 1355 3  400  499 0 1 0",
				" 1558 1 1 697  174 7253 3  929  740  1 0 1",
				" 611 0 0 1509   98  667 3  739  399 1 1 1",
				" 1005 1 1 601 394 601 1 717 0 0 2 0",
				" 808 1 0 581 492 448 2 452 0 0 2 1",
				" 780 0 1 669 562 387 1 0 951 0 2 1",
				" 718 1 0 717  64 1153 3  299  126 1 0 1",
				" 943 1 1 668 833 956 2 588 556 0 2 0",
				" 925 1 -1 650 432 655 0 859 891 0 2 0",
				" 1078 1 1 581 567 655 0 0 1 1"};
       
        int my_exec_outputs[my_tc_number] = {2,0,0,1,-1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,-1} ;

	int sn = atoi(argv[1]);
	#define tc_number 1
	string tcs[tc_number];
        
        tcs[0] = mytcs[sn]; 
        int exec_outputs[tc_number];
	exec_outputs[0] = my_exec_outputs[sn] ;

*/


/*
//my first program under test

	//tcas testplans-stmt suite122,ok
	#define my_tc_number 6
	string mytcs[my_tc_number] = {"619 1 0  197  287 4568 2  500  499 0 0 1",
				"927 1 1 4902  351 2093 0  739  740 1 0 0",
				"634 1 0 633 300 535 3 265 795 1 0 1",
				"901 1 1 502 200 450 1 801 740 0 1 0",
				"500 0 1 300 0 424 2 600 500 1 1 1",
				"1078 1 1 567 655 0 637 906 0 1 1"};
       
        int my_exec_outputs[my_tc_number] = {1, 2,  0,  0, 0, -1} ;

	int sn = atoi(argv[1]);
	#define tc_number 1
	string tcs[tc_number];
        
        tcs[0] = mytcs[sn]; 
        int exec_outputs[tc_number];
	exec_outputs[0] = my_exec_outputs[sn] ;
*/

/*
//tcas /home/tll1945/Work/tcas/testplans.alt/testplans-cov-extended/suite617,ok
	#define my_tc_number 11
	string mytcs[my_tc_number] = {" 643 1 1 5514  362 2288 1  400  741 1 1 0",
				" 1079 1 0 613 397 0 3 524 477 1 2 0",
				"  722 1 1 1415  245  316 0  401  739 0 0 1",
				" 716 1 1 618 341 767 1 0 0 0 2 1",
				" 1",
				" 653 1 -1 740 -1 702 -1 0 -1 2 1 1",
				" 780 1 0 5331   10 1209 3  399  640 1 0 1",
				" 954 1 0 672 625 744 2 732 810 2 2 1",
				" 785 0 1 4931  240  554 2  499  400 0 1 1",
				" 901 1 1 502 200 303 1 401 500 0 1 1",
				" 603 -1 0 654 507 11 3 611 600 0 1 1"};
       
        int my_exec_outputs[my_tc_number] = {0,0,2,1,-1,0,0,0,0,0,0} ;

	int sn = atoi(argv[1]);
	#define tc_number 1
	string tcs[tc_number];
        
        tcs[0] = mytcs[sn]; 
        int exec_outputs[tc_number];
	exec_outputs[0] = my_exec_outputs[sn] ;
*/

/*
//my first program under test

//tcas  a part of/home/tll1945/Work/tcas/testplans.alt/testplans.minstmt/suite783,ok
	#define my_tc_number 3
	string mytcs[my_tc_number] = {" 638 1 0 2578  156 3757 0  400  399 1 0 1 ",
				" 1119 1 1 586 386 221 2 830 979 0 2 0",
				" 1078 1 1 581 0 637 906 0 1 1"};
       
        int my_exec_outputs[my_tc_number] = {1,2,-1} ;

	int sn = atoi(argv[1]);
	#define tc_number 1
	string tcs[tc_number];
        
        tcs[0] = mytcs[sn]; 
        int exec_outputs[tc_number];
	exec_outputs[0] = my_exec_outputs[sn] ;


//my first program under test
/*
//tcas /home/tll1945/Work/tcas/testplans.alt/testplans.minstmt/suite783,ok
	#define my_tc_number 4
	string mytcs[my_tc_number] = {" 638 1 0 2578  156 3757 0  400  399 1 0 1 ",
				" 1119 1 1 586 386 221 2 830 979 0 2 0",
				" 1078 1 1 581 0 637 906 0 1 1",
				" 607 1 0 5014  531  310 1  739  501 0 0 1"};
       
        int my_exec_outputs[my_tc_number] = {1,2,-1,0} ;

	int sn = atoi(argv[1]);
	#define tc_number 1
	string tcs[tc_number];
        
        tcs[0] = mytcs[sn]; 
        int exec_outputs[tc_number];
	exec_outputs[0] = my_exec_outputs[sn] ;
*/

/*
//my second program under test	
	///home/tll1945/Work/totinfo/testplans.alt/testplans-cov/suite155
	//comment  /home/tll1945/Work/totinfo/inputs/universe/jkADX.mat fail
	//comment  /home/tll1945/Work/totinfo/inputs/universe/test65 ok
	//comment  /home/tll1945/Work/totinfo/inputs/universe/test88 ok
	//comment  /home/tll1945/Work/totinfo/inputs/universe/12new3 ok
        //comment  /home/tll1945/Work/totinfo/inputs/universe/new15 ok
	//comment  /home/tll1945/Work/totinfo/inputs/universe/tst115 ok
	//comment /home/tll1945/Work/totinfo/inputs/universe/test348.inc ok
	//comment /home/tll1945/Work/totinfo/inputs/universe/tst2g.mat fail

	#define my_tc_number 6 //
	string mytcs[my_tc_number] = {" < /home/tll1945/Work/totinfo/inputs/universe/test65",
				" < /home/tll1945/Work/totinfo/inputs/universe/test88",
				" < /home/tll1945/Work/totinfo/inputs/universe/12new3",
				" < /home/tll1945/Work/totinfo/inputs/universe/new15",
				" < /home/tll1945/Work/totinfo/inputs/universe/tst115",
				" < /home/tll1945/Work/totinfo/inputs/universe/test348.inc"};
       
        int my_exec_outputs[my_tc_number] = {1, 2, 3, 4, 5,6} ;

	int sn = atoi(argv[1]);
	#define tc_number 1
	string tcs[tc_number];
        
        tcs[0] = mytcs[sn]; 
        int exec_outputs[tc_number];
	exec_outputs[0] = my_exec_outputs[sn] ;
*/

/*
//https://www.thecrazyprogrammer.com/2017/06/hashing.html
	#define  my_tc_number 5
	string mytcs[my_tc_number] =  {" < /home/tll1945/Work/hashing_code/inputs/input/hashing_code_tc_1",
				" <  /home/tll1945/Work/hashing_code/inputs/input/hashing_code_tc_2",
				" <  /home/tll1945/Work/hashing_code/inputs/input/hashing_code_tc_3",
				" <  /home/tll1945/Work/hashing_code/inputs/input/hashing_code_tc_4",
				" <  /home/tll1945/Work/hashing_code/inputs/input/hashing_code_tc_5"}; 
	int my_exec_outputs[my_tc_number] = {1, 2, 3, 4, 5} ;
	int sn = atoi(argv[1]);
	#define tc_number 1
	string tcs[tc_number];
        
        tcs[0] = mytcs[sn]; 
        int exec_outputs[tc_number];
	exec_outputs[0] = my_exec_outputs[sn] ;
*/

/*
	//replace/testplans.alt/testplans-rand/suite659 a part, replace incldue complicate data structure so that it is cannot use.
	#define my_tc_number 7
	string mytcs[my_tc_number] = {" 'W' 'W=' < /home/tll1945/Work/replace/inputs/input/ruin.999",
				" '[^a-c--[^9-B][0-9]@[[^@@]-' '@%&a' < /home/tll1945/Work/replace/inputs/temp-test/2120.inp.899.1",
				" '[0-9]@*' 'a@n' < /home/tll1945/Work/replace/inputs/temp-test/300.inp.134.3",
				" '[^9-B] *' '&a@%' < /home/tll1945/Work/replace/inputs/temp-test/2075.inp.879.3",
				" '-[^@n]?-?' '&' < /home/tll1945/Work/replace/inputs/temp-test/1611.inp.685.2 ",
				" '[a-b]*' '&@t&@t&'  < /home/tll1945/Work/replace/inputs/moni/rr5.t",
				" '[a-c]*??[a-c]' '@%@&' < /home/tll1945/Work/replace/inputs/temp-test/371.inp.161.3"};
       
        int my_exec_outputs[my_tc_number] = {1, 2,  -1,  -1, 5, 6,-1} ;

	int sn = atoi(argv[1]);
	#define tc_number 1
	string tcs[tc_number];
        
        tcs[0] = mytcs[sn]; 
        int exec_outputs[tc_number];
	exec_outputs[0] = my_exec_outputs[sn] ;
*/
/*
	//replace/testplans.alt/testplans-cov/suite799
	#define my_tc_number 11
	string mytcs[my_tc_number] = {" '@[*-' '' < /home/tll1945/Work/replace/inputs/temp-test/2329.inp.988.3",
				"  '[a-?' 'NEW' < /home/tll1945/Work/replace/inputs/temp-test/734.inp.316.1",
				" '[a-c]?[0-9]^*' 'NEW' < /home/tll1945/Work/replace/inputs/temp-test/1822.inp.775.1",
				" '$' '%' < /home/tll1945/Work/replace/inputs/input/ruin.1122",
				"  '-[^9-B]' 'a&' < /home/tll1945/Work/replace/inputs/temp-test/990.inp.426.3",
				" '[^0-9Z-a]^_`a]' 'U' < /home/tll1945/Work/replace/inputs/input/ruin.1193",
				" '@;' 'p8-lf,m\V=UixymxlH&eP|nwsDhhN(Hcv4 CV$Gmbi' < /home/tll1945/Work/replace/inputs/input/ruin.1087",
				" '@@?*' 't+b%Ev`=tGgLKIdrp' < /home/tll1945/Work/replace/inputs/input/ruin.1596",
				" '@@' '3BLytQ69ILh[Q=v@' < /home/tll1945/Work/replace/inputs/input/ruin.608",
				" '%@n' 'v' < /home/tll1945/Work/replace/inputs/input/ruin.1561",
				" '-[^a-' 'a&' < /home/tll1945/Work/replace/inputs/temp-test/379.inp.165.1"};
       
        int my_exec_outputs[my_tc_number] = {} ;

	int sn = atoi(argv[1]);
	#define tc_number 1
	string tcs[tc_number];
        
        tcs[0] = mytcs[sn]; 
        int exec_outputs[tc_number];
	exec_outputs[0] = my_exec_outputs[sn] ;

	//comment '@[*-' '' < /home/tll1945/Work/replace/inputs/temp-test/2329.inp.988.3 ok
        //comment '[a-?' 'NEW' < /home/tll1945/Work/replace/inputs/temp-test/734.inp.316.1 fail
	//comment '[a-c]?[0-9]^*' 'NEW' < /home/tll1945/Work/replace/inputs/temp-test/1822.inp.775.1 fail
        //comment '$' '%' < /home/tll1945/Work/replace/inputs/input/ruin.1122 fail
	//comment '-[^9-B]' 'a&' < /home/tll1945/Work/replace/inputs/temp-test/990.inp.426.3 ok
	//comment '[^0-9Z-a]^_`a]' 'U' < /home/tll1945/Work/replace/inputs/input/ruin.1193 fail
	//comment '@;' 'p8-lf,m\V=UixymxlH&eP|nwsDhhN(Hcv4 CV$Gmbi' < /home/tll1945/Work/replace/inputs/input/ruin.1087 fail
	//comment '@@?*' 't+b%Ev`=tGgLKIdrp' < /home/tll1945/Work/replace/inputs/input/ruin.1596 fail
	//comment  '@@' '3BLytQ69ILh[Q=v@' < /home/tll1945/Work/replace/inputs/input/ruin.608 fail

*/
/*
	//toinfo testplans-stmt suite647
	#define my_tc_number 5 //comment < /home/tll1945/Work/totinfo/inputs/universe/12new47 is fail
	string mytcs[my_tc_number] = {" < /home/tll1945/Work/totinfo/inputs/universe/tst73",
				" < /home/tll1945/Work/totinfo/inputs/universe/test142.inc",
				" < /home/tll1945/Work/totinfo/inputs/universe/jkAAb.mat",
				" < /home/tll1945/Work/totinfo/inputs/universe/ntest38",
				" < /home/tll1945/Work/totinfo/inputs/universe/bnew2"};
       
        int my_exec_outputs[my_tc_number] = {1, 2, 3, 4, 5} ;

	int sn = atoi(argv[1]);
	#define tc_number 1
	string tcs[tc_number];
        
        tcs[0] = mytcs[sn]; 
        int exec_outputs[tc_number];
	exec_outputs[0] = my_exec_outputs[sn] ;
*/





        //schedule testplans-stmt suite290, fail when executing because there is a free function in schedule

	#define tc_number 6
	string tcs[tc_number] = {" 6 0 1 < /home/tll1945/Work/schedule/inputs/input/inp.26",
				" 1 1 1 < /home/tll1945/Work/schedule/inputs/input/ad.1",
				" 0 0 7 < /home/tll1945/Work/schedule/inputs/input/add.34",
				" 6  3  10  < /home/tll1945/Work/schedule/inputs/input/lu460",
				" 0 1 5 < /home/tll1945/Work/schedule/inputs/input/inp.hf.14",
				" 8 3 < /home/tll1945/Work/schedule/inputs/input/ft.25"};
	int exec_outputs[tc_number] = {1, 2, 3,  4, 5, 6} ;


/*	
	//toinfo testplans-stmt suite647, 
	#define tc_number 1
	//string tcs[tc_number] = {" < /home/tll1945/Work/totinfo/inputs/universe/tst73"};//ok
        //string tcs[tc_number] = {" < /home/tll1945/Work/totinfo/inputs/universe/test142.inc"};//ok
	//string tcs[tc_number] = {" < /home/tll1945/Work/totinfo/inputs/universe/jkAAb.mat"};//ok
	//string tcs[tc_number] = {" < /home/tll1945/Work/totinfo/inputs/universe/ntest38"};//ok
	//string tcs[tc_number] = {" < /home/tll1945/Work/totinfo/inputs/universe/bnew2"};//ok
	string tcs[tc_number] = {" < /home/tll1945/Work/totinfo/inputs/universe/12new47"};//fail
        int exec_outputs[tc_number] = {1} ;
*/

	//printtokens testplans-stmt suite261
/*
	#define my_tc_number 8
	string mytcs[my_tc_number]  = {" < /home/tll1945/Work/printtokens/inputs/tst218",
				" < /home/tll1945/Work/printtokens/inputs/newtst146.tst",
				" one doesntliketwo",
				" /home/tll1945/Work/printtokens/inputs/uslin.893",
				" /home/tll1945/Work/printtokens/inputs/garbage/nothing",
				" /home/tll1945/Work/printtokens/inputs/uslin.221",
				" /home/tll1945/Work/printtokens/inputs/uslin.1942",
				" < /home/tll1945/Work/printtokens/inputs/uslin.1702"};
       
        int my_exec_outputs[my_tc_number] =  {1, 2,  3,  4, 5, 6, 7, 8} ;

*/	

/*
//compound-if
	#define my_tc_number 3
	string mytcs[my_tc_number]  = {" -8", " 10"," 2"};
       
        int my_exec_outputs[my_tc_number] =  {-8,8,2} ;


	int sn = atoi(argv[1]);
	#define tc_number 1
	string tcs[tc_number];
        
        tcs[0] = mytcs[sn]; 
        int exec_outputs[tc_number];
	exec_outputs[0] = my_exec_outputs[sn] ;
*/







/*
//printtokens testplans.minstmt suite261, used in my big paper
	#define my_tc_number 5
	string mytcs[my_tc_number]  = {" < /home/tll1945/Work/printtokens/inputs/newtst146.tst",
				" one doesntliketwo",
				" /home/tll1945/Work/printtokens/inputs/uslin.221",
				" /home/tll1945/Work/printtokens/inputs/uslin.1942",
				" < /home/tll1945/Work/printtokens/inputs/uslin.1702"};
       
        int my_exec_outputs[my_tc_number] =  {1, 2,  3,  4, 5} ;


	int sn = atoi(argv[1]);
	#define tc_number 1
	string tcs[tc_number];
        
        tcs[0] = mytcs[sn]; 
        int exec_outputs[tc_number];
	exec_outputs[0] = my_exec_outputs[sn] ;
*/

/*
	//comment  /home/tll1945/Work/printtokens/inputs/tst218  fail
	//comment  < /home/tll1945/Work/printtokens/inputs/newtst146.tst ok
	//comment one doesntliketwo ok
	//comment /home/tll1945/Work/printtokens/inputs/uslin.893 ok
	//comment /home/tll1945/Work/printtokens/inputs/garbage/nothing fail
	//comment /home/tll1945/Work/printtokens/inputs/uslin.221 ok
	//comment /home/tll1945/Work/printtokens/inputs/uslin.1942 ok
	//comment  < /home/tll1945/Work/printtokens/inputs/uslin.1702 ok
*/

	//printtokens2 testplans-stmt suite261 the fist test case
/*
//ok
	#define tc_number 1
	string tcs[tc_number] = {" < /home/tll1945/Work/printtokens2/inputs/tc103"};
       
        int exec_outputs[tc_number] = {1} ;
*/
/*
 //ok
	#define tc_number 1
	string tcs[tc_number] = {" < /home/tll1945/Work/printtokens2/inputs/uslin.234"};       
        int exec_outputs[tc_number] = {2} ;
*/

// wrong
/*
	#define tc_number 1
	string tcs[tc_number] = {" < /home/tll1945/Work/printtokens2/inputs/uslin.379"};
       
        int exec_outputs[tc_number] = {3} ;
*/
/*
//printtokens2 modified suite460, use this 
	
	#define  my_tc_number 8
	string mytcs[my_tc_number] = {" < /home/tll1945/Work/printtokens2/inputs/newtst336.tst",
				" < /home/tll1945/Work/printtokens2/inputs/tc273",
				" < /home/tll1945/Work/printtokens2/inputs/uslin.1873",
				" < /home/tll1945/Work/printtokens2/inputs/uslin.607",
				" < /home/tll1945/Work/printtokens2/inputs/uslin.765",
				" /home/tll1945/Work/printtokens2/inputs/uslin.1864",
				" /home/tll1945/Work/printtokens2/inputs/uslin.373",
				" /home/tll1945/Work/printtokens2/inputs/uslin.55"}; 
	int my_exec_outputs[my_tc_number] = {1,2,3,4, 5, 6, 7, 8} ;
	int sn = atoi(argv[1]);
	#define tc_number 1
	string tcs[tc_number];
        
        tcs[0] = mytcs[sn]; 
        int exec_outputs[tc_number];
	exec_outputs[0] = my_exec_outputs[sn] ;

*/




/*
//suite460
	#define tc_number 10
	string tcs[tc_number] = {" < /home/tll1945/Work/printtokens2/inputs/newtst336.tst",
				" < /home/tll1945/Work/printtokens2/inputs/tc273",
				" < /home/tll1945/Work/printtokens2/inputs/uslin.1873",
				" < /home/tll1945/Work/printtokens2/inputs/uslin.607",
				" < /home/tll1945/Work/printtokens2/inputs/uslin.765",
				" /home/tll1945/Work/printtokens2/inputs/uslin.1864",
				" /home/tll1945/Work/printtokens2/inputs/uslin.373",
				" /home/tll1945/Work/printtokens2/inputs/uslin.55",
				" /home/tll1945/Work/printtokens2/inputs/garbage/nothing",
				" one doesntliketwo"}; 
	 int exec_outputs[tc_number] = {1,2,3,4, 5, 6, 7, 8,9,10} ;
*/

//ok,
/*
	#define tc_number 1
	string tcs[tc_number] = {" < /home/tll1945/Work/printtokens2/inputs/newtst336.tst"};       
        int exec_outputs[tc_number] = {1} ;
*/

/*ok
	#define tc_number 1
	string tcs[tc_number] = {" < /home/tll1945/Work/printtokens2/inputs/tc273"};       
        int exec_outputs[tc_number] = {2} ;
*/
/*
//ok
	#define tc_number 1
	string tcs[tc_number] = {" < /home/tll1945/Work/printtokens2/inputs/uslin.1873"};       
        int exec_outputs[tc_number] = {3} ;
*/


//
/*
//ok
	#define tc_number 1
	string tcs[tc_number] = {"  < /home/tll1945/Work/printtokens2/inputs/uslin.607"};       
        int exec_outputs[tc_number] = {4} ;
*/

//
/*
//ok
	#define tc_number 1
	string tcs[tc_number] = {"  < /home/tll1945/Work/printtokens2/inputs/uslin.765"};       
        int exec_outputs[tc_number] = {5} ;
*/
/*
//ok
	#define tc_number 1
	string tcs[tc_number] = {"  /home/tll1945/Work/printtokens2/inputs/uslin.1864"};       
        int exec_outputs[tc_number] = {6} ;
*/
/*
//ok
	#define tc_number 1
	string tcs[tc_number] = {" /home/tll1945/Work/printtokens2/inputs/uslin.373 "};       
        int exec_outputs[tc_number] = {7} ;
*/


/*
//ok
	#define tc_number 1
	string tcs[tc_number] = {" /home/tll1945/Work/printtokens2/inputs/uslin.55"};       
        int exec_outputs[tc_number] = {8} ;
*/
/*
//WRONG, there is a exist(0) in the execuiton path
	#define tc_number 1
	string tcs[tc_number] = {" /home/tll1945/Work/printtokens2/inputs/garbage/nothing"};       
        int exec_outputs[tc_number] = {9} ;
*/
/*
//WRONG, there is a exist(0) in the execuiton path	
	#define tc_number 1
	string tcs[tc_number] = {"  one doesntliketwo"};       
        int exec_outputs[tc_number] = {10} ;
*/

/*
	#define tc_number 1
	
	string tcs[tc_number] = {" "};
        int exec_outputs[tc_number] = {1} ;
*/
/*	
	//replace.c  testplants-stmt suite672
	#define tc_number 4
	string tcs[tc_number] = {" '%?[a--b]@[*[0-9][^a-c]?@*[0-9][- *[^@@][-z][9-B]- *-[a-c]$' 'NEW' < /home/tll1945/Work/replace/inputs/temp-test/1743.inp.741.10", " '[^a-c]?$' 'a@n' < /home/tll1945/Work/replace/inputs/temp-test/70.inp.30.9", " '[a--] *^a-c]?-[0-9]?--a-]@t*?@[[^0-9]' '@%&a' < /home/tll1945/Work/replace/inputs/temp-test/496.inp.217.1", " '[c-at]' 'AQ' < /home/tll1945/Work/replace/inputs/input/ruin.253"};
       
        int exec_outputs[tc_number] = {1,2,3,4} ;
*/


//cervix_grades
/*	
	#define  my_tc_number 5
	string mytcs[my_tc_number] =  {" < /home/tll1945/Work/cervix_grades/inputs/input/cervix_grades_tc_1",
				" <  /home/tll1945/Work/cervix_grades/inputs/input/cervix_grades_tc_2",
				" <  /home/tll1945/Work/cervix_grades/inputs/input/cervix_grades_tc_3",
				" <  /home/tll1945/Work/cervix_grades/inputs/input/cervix_grades_tc_4",
				" <  /home/tll1945/Work/cervix_grades/inputs/input/cervix_grades_tc_5"}; 
	int my_exec_outputs[my_tc_number] = {1, 2, 3, 4, 5} ;
	int sn = atoi(argv[1]);
	#define tc_number 1
	string tcs[tc_number];
        
        tcs[0] = mytcs[sn]; 
        int exec_outputs[tc_number];
	exec_outputs[0] = my_exec_outputs[sn] ;
*/


//
/*
	#define  my_tc_number 1
	string mytcs[my_tc_number] =  {" "}; 
	int my_exec_outputs[my_tc_number] = {1} ;
	int sn = atoi(argv[1]);
	#define tc_number 1
	string tcs[tc_number];
        
        tcs[0] = mytcs[sn]; 
        int exec_outputs[tc_number];
	exec_outputs[0] = my_exec_outputs[sn] ;
*/













	vector<stmt_info> stmt_info_vec;
	stmt_info_vec.clear();
	init_stmt_info_vec(stmt_info_vec, total_nli_line_num);
	std::cout<<"stmt_info_vec.size: " << stmt_info_vec.size()<< std::endl;
	for(int i = 0 ; i < tc_number; i++){
		exec_test_case(tcs[i],stmt_info_vec,total_nli_line_num,exec_outputs[i]);
		std::cout << "stmt_info_vec size: " << stmt_info_vec.size() << std::endl;
	}
	//calculate_info_hidden_factor(stmt_info_vec);
	//display_recorded_stmt_exec_features(stmt_info_vec);
        //my_seven_factors_to_mysql(stmt_info_vec);
	my_factors_to_mysql(stmt_info_vec);

	endTime = clock();
  	cout << "The run time  is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

	string copy_file("cp stmt_exec_features.txt  stmt_exec_features_");
        string copy_file_cmd = copy_file + argv[1]  + ".txt";
	
	system(copy_file_cmd.c_str());
}




