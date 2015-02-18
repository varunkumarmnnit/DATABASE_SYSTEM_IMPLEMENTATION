#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

// stub file .. replace it with your own DBFile.cc

DBFile::DBFile () {

        readPage = NULL;
        writePage = NULL;
        myDBFile = NULL;
        write_page_marker = 0;
        read_page_marker = 0;



}
void DBFile::initialiseHeapFile(char*  f_path,int fileLen)
{
        myDBFile = new File();
        myDBFile->Open(fileLen,f_path);
}

int DBFile::Create (char *f_path, fType f_type, void *startup) {
	//create a new File with specified path.
       switch(f_type){
       case 0: 
        initialiseHeapFile(f_path,0);
         break;
        default :
        break;}
        if(myDBFile == NULL)
                return 0;
        else
                return 1;

}

void DBFile::Load (Schema &f_schema, char *loadpath) {

 //create instance from loadpath for file to read and read records from there.
        if(myDBFile != NULL)
        {
                FILE *txtFile = fopen(loadpath,"r");
                if(txtFile != NULL)
                {
                        Record rec;
                        while(rec.SuckNextRecord(&f_schema,txtFile) != 0)
                        {
                                Add(rec);
                        }
                }
                else
                {
                        cerr<<"No file with specified name "<< loadpath <<" exists. "<<"\n";
                        exit(1);
                }
        }else
        {
                //this will happen if load is called before the file is actually created.
                cerr<<"No DBFile Exists.\n";
        }

}

int DBFile::Open (char *f_path) {

//initialise write_buffer_size to size of file.
        initialiseHeapFile(f_path,1);
        if(myDBFile == NULL)
                return 0;
        return 1;

}

void DBFile::MoveFirst () {
read_page_marker = 0;
        if(myDBFile == NULL)
        {
                cerr << "BAD! No File Open." << "\n";
                exit (1);
        }



}
void DBFile::cleanUp()
{
        read_page_marker =0;
        write_page_marker = 0;
        readPage = NULL;
        writePage = NULL;
        myDBFile = NULL;
}

int DBFile::Close () {

//close dbfile after saving all the records in write page.
        if(myDBFile != NULL)
        {
                if(writePage != NULL)
                {
                        myDBFile->AddPage(writePage,write_page_marker);
                }
                myDBFile->Close();
                delete myDBFile;
                cleanUp();
        }else
        {
                cleanUp();
                return 0;
        }

}

void DBFile::Add (Record &rec) {
//will enter first time add function is called.
        if(writePage == NULL)
        {
                //read the last page of file in write buffer to add records to end of file and avoid spaces;
                writePage = new Page();
                off_t page_num = myDBFile->GetLength();
                if(page_num>0)
                {
                        write_page_marker = page_num;
                        myDBFile->GetPage(writePage,write_page_marker);
                }
        }
        //First try adding the record....if no more records can be added to current page then get a new page and try adding there 
        //before that add that page to file.
        if(writePage->Append(&rec) ==0)
        {
                //add the write page to file and and allocate new page here to add record.
                myDBFile->AddPage(writePage,write_page_marker);
                writePage->EmptyItOut();
                writePage->Append(&rec);
                write_page_marker++;
        }


}

int DBFile::GetNext (Record &fetchme) {
        int status = 0;
        //try reading from the current read page till you can then if page exhausted read a new page from file in readPage and then read.
        if(readPage == NULL)
        {
                readPage = new Page();
                if(read_page_marker < myDBFile->GetLength())
                {
                        myDBFile->GetPage(readPage,read_page_marker);
                }
        }
        if(readPage != NULL)
        {
                status = readPage->GetFirst(&fetchme);
                //if was unable to read the next record fetch next page from disk and read.
                if(status == 0)
                {
                        //empty the read page reinitialise and read the next page.
                        read_page_marker++;
                        readPage->EmptyItOut();
                        if(read_page_marker < myDBFile->GetLength()-1)
                        {
                                myDBFile->GetPage(readPage,read_page_marker);
                                if(readPage!=NULL)
                                        status = readPage->GetFirst(&fetchme);
                                else
                                        return 0;
                        }
                }
        }
        return status;


}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {

ComparisonEngine comp;
        while (GetNext(fetchme) == 1)
        {
                if (comp.Compare (&fetchme, &literal, &cnf) == 1)
                        return 1;
        }
        return 0;


}
