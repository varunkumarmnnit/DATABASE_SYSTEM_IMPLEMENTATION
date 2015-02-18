#include <iostream>
#include "DBFile.h"
#include "test.h"

// make sure that the file path/dir information below is correct
char *dbfile_dir = "./"; // dir where binary heap files should be stored
char *tpch_dir ="../src/DATA/10M/"; // dir where dbgen tpch files (extension *.tbl) can be found
char *catalog_path = "../src/catalog"; // full path of the catalog file

using namespace std;

relation *rel;

// load from a tpch file
void test1 () {

	DBFile dbfile;
	cout << " DBFile will be created at " << rel->path () << endl;
	dbfile.Create (rel->path(), heap, NULL);

	char tbl_path[100]; // construct path of the tpch flat text file
	sprintf (tbl_path, "%s%s.tbl", tpch_dir, rel->name()); 
	cout << " tpch file will be loaded from " << tbl_path << endl;

	dbfile.Load (*(rel->schema ()), tbl_path);
	dbfile.Close ();
}

// sequential scan of a DBfile 
void test2 () {

	DBFile dbfile;
	dbfile.Open (rel->path());
	dbfile.MoveFirst ();

	Record temp;

	int counter = 0;
	while (dbfile.GetNext (temp) == 1) {
		counter += 1;
		temp.Print (rel->schema());
		if (counter % 10000 == 0) {
			cout << counter << "\n";
		}
	}
	cout << " scanned " << counter << " recs \n";
	dbfile.Close ();
}

// scan of a DBfile and apply a filter predicate
void test3 () {

	cout << " Filter with CNF for : " << rel->name() << "\n";

	CNF cnf; 
	Record literal;
	rel->get_cnf (cnf, literal);

	DBFile dbfile;
	dbfile.Open (rel->path());
	dbfile.MoveFirst ();

	Record temp;

	int counter = 0;
	while (dbfile.GetNext (temp, cnf, literal) == 1) {
		counter += 1;
		temp.Print (rel->schema());
		if (counter % 10000 == 0) {
			cout << counter << "\n";
		}
	}
	cout << " selected " << counter << " recs \n";
	dbfile.Close ();
}

void test4 ()
{
	DBFile dbfile;
	dbfile.Open (rel->path());
	dbfile.MoveFirst ();

	Record temp;

	if (dbfile.GetNext (temp) == 1)
	{
		cout<<"\t \n Adding the first record \n";
		dbfile.Add(temp);
		cout<<"\t\n The record has been added \n";



	}

	dbfile.Close ();

}

void test_suit1()
{
	int counter1=0;
	CNF cnf;
        // DBFile dbfile;
          //      dbfile.Open (rel->path());

	Record literal;
	FILE * pFile;
	pFile=fopen("input1","r");
	char mystring[50];
	while(fgets(mystring ,100,pFile)!=NULL){
		rel->Get_Cnf (cnf, literal,mystring);
		counter1++;
		DBFile dbfile;
		dbfile.Open (rel->path());
		dbfile.MoveFirst ();

		int counter = 0;
		Record temp;
		switch(counter1){
			case 1: 
				while (dbfile.GetNext (temp, cnf, literal) == 1) {
					counter += 1;
					//temp.Print (rel->schema());
					if (counter % 10000 == 0) {
						cout << counter << "\n";
					}
				}
				if( counter ==0)
					cout << "PASS"<<endl;
				else 
					cout<<"FAIL "<<endl;
				break;
			case 2:
				while (dbfile.GetNext (temp, cnf, literal) == 1) {
					counter += 1;
					//temp.Print (rel->schema());
					if (counter % 10000 == 0) {
						cout << counter << "\n";
					}
				}
				if( counter ==1)
					cout << "PASS"<<endl;
				else
					cout<<"FAIL "<<endl;

				break;
			default:
				break;


dbfile.Close();		}
	}

}

void test_suit2()
{

}

void test_suit3()
{
}
void test_suit4()
{
}
void test_suit5()
{
}
void test_suit6()
{

}
void test_suit7()
{

	CNF cnf;
	Record literal;
	rel->Get_Cnf (cnf, literal,"input5");

	DBFile dbfile;
	dbfile.Open (rel->path());
	dbfile.MoveFirst ();

	Record temp;

	int counter = 0;
	while (dbfile.GetNext (temp, cnf, literal) == 1) {
		counter += 1;
		//temp.Print (rel->schema());
		if (counter % 10000 == 0) {
			cout << counter << "\n";
		}
	}
	cout << " selected " << counter << " recs \n";
	dbfile.Close ();

	/*
	   DBFile dbfile;
	   dbfile.Open (rel->path());
	   dbfile.MoveFirst ();

	   Record temp;

	   if (dbfile.GetNext (temp) == 1)
	   {
	   cout<<"\t \n No of Attributes  \n";
	   int num= ((int *)(temp.bits)[1])-1/sizeof(int);
	   cout<<"\t\n num\n";



	   }

	   dbfile.Close (); 
	 */

}
/*
//Check if the file path is correct
void checkFilePath(){
DBFile dbfile;
if(!dbfile.open)
cout<<"Checked for file path correctness;Test case passed\n";
else
cout<<"checked for file pasth correctnes;Test case failed\n";
}
//Test cases to check CNF output's
void checkCNFoutput(char *c){

}*/
int main () {

	setup (catalog_path, dbfile_dir, tpch_dir);

	void (*test) ();
	relation *rel_ptr[] = {n, r, c, p, ps, o, li};
	void (*test_ptr[]) () = {&test1, &test2, &test3,&test4};  
	void (*test_suit[]) () = {&test_suit1, &test_suit2, &test_suit3,&test_suit4,&test_suit5,test_suit6,test_suit7};
	int tindx = 0;
	while (tindx < 1 || tindx > 5) {
		cout << " select test: \n";
		cout << " \t 1. load file \n";
		cout << " \t 2. scan \n";
		cout << " \t 3. scan & filter \n \t ";
		cout << " \t 4. Add record \n \t ";
		cout << " \t 5. Sample Unit Test cases results :Please see ReadMe File for more deatil \n \t ";
		cin >> tindx;
	}

	int findx = 0;
	while (findx < 1 || findx > 7) {
		cout << "\n select table: \n";
		cout << "\t 1. nation \n";
		cout << "\t 2. region \n";
		cout << "\t 3. customer \n";
		cout << "\t 4. part \n";
		cout << "\t 5. partsupp \n";
		cout << "\t 6. orders \n";
		cout << "\t 7. lineitem \n \t ";
		cin >> findx;
	}

	rel = rel_ptr [findx - 1];
	if(tindx!=5)
		test = test_ptr [tindx - 1];
	else
		test=test_suit[findx-1];

	test ();

	cleanup ();
}
