
// #include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdio.h>
using namespace std;


struct Page {
	int tableid = 0 ;
	int timeStamp = 0 ;
	int count = 0 ;
};



struct Data {
	int id = 0 ;
	vector<Page> pageFrame;
	bool pageFault;
};

bool Comparetime( Page a, Page b ) {
	return a.timeStamp < b.timeStamp;
} // compare



class PageReplacement{
	
	public : 
		vector<int> Int_reference;	
		vector<Data> dataFIFO;
		vector<Data> dataLRU;
		vector<Data> dataLFUPR;
		vector<Data> dataMFUPR;
		vector<Data> dataLFULRU;
		vector<Data> dataMFULRU;
		string reference = "" ;
		int PageFrame = 0 ;
		int pageReplacesFIFO = 0 ;
		int pagefaultFIFO = 0 ;
		int pagefaultLRU = 0 ;
		int pageReplacesLRU = 0 ;
		int pageReplacesLFUPR = 0 ;
		int pagefaultLFUPR = 0 ;		
		int pageReplacesMFUPR  = 0 ;
		int pagefaultMFUPR  = 0 ;
		int pageReplacesLFULRU = 0 ;
		int pagefaultLFULRU = 0 ;
		int pageReplacesMFULRU = 0 ;
		int pagefaultMFULRU = 0 ;

bool CheckPageFault( int & PageFault, vector<Page> currentTable, int id, int & currentid ) {
	
	for (int i = 0 ; i < currentTable.size(); i++) {
		if ( currentTable[i].tableid == id ) {
			currentid = i;
			return true;
		} // if
	} // for
	
	PageFault++;
	return false;
} // bool	


int FindLeast( vector<Page> currentTable ) {
	int min = 0, count = 0 ;
	for ( int i = 1; i < currentTable.size(); i++) {
		if (currentTable[min].count > currentTable[i].count) min = i;
		else if (currentTable[min].count == currentTable[i].count) count++ ;	
		
	}// for
	
	if (count == currentTable.size()) min = 0 ;

	return min;
} // FindLeast




int FindMost(vector<Page> currentTable) {
	
	
	int max = 0;
	// int count = 0;
	for (int i = 1; i < currentTable.size(); i++) {
		
		if (currentTable[max].count < currentTable[i].count) {
			max = i;
		} // if
		else if (currentTable[max].count == currentTable[i].count) {
			 if (currentTable[max].timeStamp > currentTable[i].timeStamp) {
				max = i;
			 } // if
		} // else if

	}// for

	return max;
} // 






void Read( string & input) {
	fstream In_File;
	cout << "請輸入要打開的檔案名 :";
	cin >> input;
	input = input + ".txt";
	In_File.open(input.c_str(), ios::in);
	while ( !In_File) {
		cout << "未找到檔案，請再輸入一次檔案名 :";
		cin >> input;
		input = input + ".txt";
		In_File.open(input.c_str(), ios::in);
	} // while
	
	In_File >> PageFrame ;
	In_File >> reference ;
	StringToInt() ;
	In_File.close();
	
} // Read()


void StringToInt() {

	string str = "" ;
	int temp = 0 ;
	for( int i = 0 ; i < reference.size() ; i ++  ) {
		str = reference[i] ;
		temp = atoi( str.c_str() ) ;
		Int_reference.push_back( temp ) ;
	} // for
	
} // StringToInt


void PushPage( bool & fault, vector<Page> & currentTable, int i ){
	fault = true;
	Page temp ;
	temp.count++ ;
	temp.tableid = Int_reference[i];
	temp.timeStamp = i ;
	currentTable.push_back(temp);
	
} // Push



void PushData( bool fault, vector<Page> currentTable, vector<Data> & dataList, int i ){
	
	Data datatemp;
	datatemp.pageFault = fault;
	datatemp.id = Int_reference[i];
	datatemp.pageFrame = currentTable;
	dataList.push_back(datatemp);	
	
} // Push

void FIFO() {
	
	int PageFault = 0 ;
	int PageReplaces = 0;
	vector<Page> currentTable ;
	for (int i = 0; i < Int_reference.size() ; i++ ) {
		bool fault = false ;
		int currentid = 0 ;
		if ( currentTable.size() < PageFrame ) {   // 未滿的情況 
			if ( CheckPageFault( PageFault, currentTable, Int_reference[i], currentid ) == false ) { // 判斷再不再table裡面 
				PushPage( fault, currentTable,i ) ; 
			} // if
		} // if	
		else {   // 滿了的情況 
			
			if ( CheckPageFault( PageFault, currentTable, Int_reference[i], currentid ) == false ) {  // 判斷再不再table裡面 
				PageReplaces++ ;
				currentTable.erase( currentTable.begin() ) ; // 刪除最先進來的 
				PushPage( fault, currentTable,i ) ; 		
			} // if
			
		} // else
			
		PushData( fault, currentTable, dataFIFO,i ) ;
			
	} // for 
	
	pagefaultFIFO = PageFault;		
	pageReplacesFIFO = 	PageReplaces ;
	
} //  FIFO


void LRU() {  

	int PageFault = 0 ;
	int PageReplaces = 0;
	vector<Page> currentTable ;
	for (int i = 0; i < Int_reference.size() ; i++ ) {
		bool fault = false ;
		int currentid = 0 ;
		if ( currentTable.size() < PageFrame ) {   // 未滿的情況 
			if ( CheckPageFault( PageFault, currentTable, Int_reference[i], currentid ) == false ) { // 判斷再不再table裡面 
				PushPage( fault, currentTable,i ) ; 	
			} // if
			else currentTable[currentid].timeStamp = i ;
			
		} // if	
		else {   // 滿了的情況 
			
			if ( CheckPageFault( PageFault, currentTable, Int_reference[i], currentid ) == false ) {  // 判斷再不再table裡面 
				PageReplaces++ ;
				currentTable.erase( currentTable.begin() ) ; // 刪除最先進來的 
				PushPage( fault, currentTable,i ) ; 				
			} // if
			else currentTable[currentid].timeStamp = i ;
			
		} // else
		
		sort(currentTable.begin(), currentTable.end(), Comparetime);
		
		PushData( fault, currentTable, dataLRU,i ) ;	
	} // for 
	
	pagefaultLRU = PageFault;		
	pageReplacesLRU = PageReplaces ;

} // LRU


void LFUPR() {
	
	int PageFault = 0 ;
	int PageReplaces = 0;
	vector<Page> currentTable ;
	for (int i = 0; i < Int_reference.size() ; i++ ) {
		bool fault = false ;
		int currentid = 0 ;
		if ( currentTable.size() < PageFrame ) {   // 未滿的情況 
			if ( CheckPageFault( PageFault, currentTable, Int_reference[i], currentid ) == false ) { // 判斷再不再table裡面 
				PushPage( fault, currentTable, i ) ; // count = 1 次 
			} // if
			else {
				currentTable[currentid].count++ ; // 被參考count++  
			} // else
			
		} // if	
		else {   // 滿了的情況 
			if ( CheckPageFault( PageFault, currentTable, Int_reference[i], currentid ) == false ) {  // 判斷再不再table裡面 
				int min = FindLeast(currentTable) ;
				PageReplaces++ ;
				currentTable.erase( currentTable.begin() + min ) ; // 刪除最先進來的 
				PushPage( fault, currentTable, i ) ; //不再table 加一次		
			} // if
			else {
				currentTable[currentid].count++;
		    } // else

		} // else
		
		PushData( fault, currentTable, dataLFUPR,i ) ;	
	} // for 
	
	pagefaultLFUPR = PageFault;		
	pageReplacesLFUPR = PageReplaces ;


} // LFUPR




void MFUPR() {

	int PageFault = 0 ;
	int PageReplaces = 0;
	vector<Page> currentTable ;
	for (int i = 0; i < Int_reference.size() ; i++ ) {
		bool fault = false ;
		int currentid = 0 ;
		if ( currentTable.size() < PageFrame ) {   // 未滿的情況 
			if ( CheckPageFault( PageFault, currentTable, Int_reference[i], currentid ) == false ) { // 判斷再不再table裡面 
				PushPage( fault, currentTable, i ) ; // count = 1 次 
			} // if
			else {
				currentTable[currentid].count++ ;
			} // else
			
		} // if	
		else {   // 滿了的情況 
			if ( CheckPageFault( PageFault, currentTable, Int_reference[i], currentid ) == false ) {  // 判斷再不再table裡面 
				int max = FindMost(currentTable) ;
				PageReplaces++ ;
				currentTable.erase( currentTable.begin() + max ) ; // 刪除最先進來的 
				PushPage( fault, currentTable, i ) ; 			
			} // if
			else {
				currentTable[currentid].count++;
		    } // else

		} // else
		
		PushData( fault, currentTable, dataMFUPR,i ) ;	
	} // for 
	
	pagefaultMFUPR = PageFault;		
	pageReplacesMFUPR = PageReplaces ;

}


void LFULRU() {
	
	int PageFault = 0 ;
	int PageReplaces = 0;
	vector<Page> currentTable ;
	for (int i = 0; i < Int_reference.size() ; i++ ) {
		bool fault = false ;
		int currentid = 0 ;
		if ( currentTable.size() < PageFrame ) {   // 未滿的情況 
			if ( CheckPageFault( PageFault, currentTable, Int_reference[i], currentid ) == false ) { // 判斷再不再table裡面 
				PushPage( fault, currentTable, i ) ; // count = 1 次 
			} // if
			else {
				currentTable[currentid].timeStamp = i;
				currentTable[currentid].count++ ;
			} // else
			
		} // if	
		else {   // 滿了的情況 
			if ( CheckPageFault( PageFault, currentTable, Int_reference[i], currentid ) == false ) {  // 判斷再不再table裡面 
				int min = FindLeast(currentTable) ;
				PageReplaces++ ;
				currentTable.erase( currentTable.begin() + min ) ; // 刪除最先進來的 
				PushPage( fault, currentTable, i ) ; 				
			} // if
			else {
				currentTable[currentid].timeStamp = i ;
				currentTable[currentid].count++;
		    } // else

		} // else
		
		
		sort(currentTable.begin(), currentTable.end(), Comparetime);
		PushData( fault, currentTable, dataLFULRU,i ) ;	
	} // for 
	
	pagefaultLFULRU = PageFault;		
	pageReplacesLFULRU = PageReplaces ;	




}


void MFULRU(){

	int PageFault = 0 ;
	int PageReplaces = 0;
	vector<Page> currentTable ;
	for (int i = 0; i < Int_reference.size() ; i++ ) {
		bool fault = false ;
		int currentid = 0 ;
		if ( currentTable.size() < PageFrame ) {   // 未滿的情況 
			if ( CheckPageFault( PageFault, currentTable, Int_reference[i], currentid ) == false ) { // 判斷再不再table裡面 
				PushPage( fault, currentTable, i ) ; // count = 1 次 
			} // if
			else {
				currentTable[currentid].timeStamp = i;
				currentTable[currentid].count++ ;
			} // else
			
		} // if	
		else {   // 滿了的情況 
			if ( CheckPageFault( PageFault, currentTable, Int_reference[i], currentid ) == false ) {  // 判斷再不再table裡面 
				int max = FindMost(currentTable) ;
				PageReplaces++ ;
				currentTable.erase( currentTable.begin() + max ) ; // 刪除最先進來的 
				PushPage( fault, currentTable, i ) ; 			
			} // if
			else {
				currentTable[currentid].timeStamp = i ;
				currentTable[currentid].count++;
		    } // else

		} // else
		
		sort(currentTable.begin(), currentTable.end(), Comparetime);
		PushData( fault, currentTable, dataMFULRU,i ) ;	
	} // for 
	
	pagefaultMFULRU = PageFault;		
	pageReplacesMFULRU = PageReplaces ;

}




void Wirte( string input ) {
	
	input = "out_" + input;
	fstream Out_File;
	Out_File.open(input.c_str(), ios::out );
	Out_File << "--------------FIFO-----------------------" << endl;
	for (int i = 0; i < dataFIFO.size() ; i++) {
		
		Out_File << dataFIFO[i].id << "\t";
		for (int j = dataFIFO[i].pageFrame.size() -1 ; j >= 0 ; j--)
		//	cout << dataFIFO[i].pageFrame[j].id ;
			Out_File << dataFIFO[i].pageFrame[j].tableid;
		if (dataFIFO[i].pageFault) Out_File << "\t" << "F";
		Out_File << endl;
	} // for 

	Out_File << "Page Fault = " << pagefaultFIFO << "  Page Replaces = " << pageReplacesFIFO << "  Page Frames = " << PageFrame << endl << endl ;
    cout << "Page Fault = " << pagefaultFIFO << "  Page Replaces = " << pageReplacesFIFO << "  Page Frames = " << PageFrame << endl << endl ;
	Out_File << "--------------LRU-----------------------" << endl;
	for (int i = 0; i < dataLRU.size() ; i++) {
		Out_File << dataLRU[i].id << "\t";
		for (int j = dataLRU[i].pageFrame.size() - 1 ; j >= 0; j-- ) {
			Out_File << dataLRU[i].pageFrame[j].tableid;
	    } // for
		if (dataLRU[i].pageFault) Out_File << "\t" << "F";
		Out_File << endl;
	} // for 

	Out_File << "Page Fault = " << pagefaultLRU << "  Page Replaces = " << pageReplacesLRU << "  Page Frames = " << PageFrame << endl << endl;

	cout << "Page Fault = " << pagefaultLRU << "  Page Replaces = " << pageReplacesLRU << "  Page Frames = " << PageFrame << endl << endl;
	
	Out_File << "--------------Least Frequently Used Page Replacement-----------------------" << endl;

	for (int i = 0; i < dataLFUPR.size() ; i++) {
		
		Out_File << dataLFUPR[i].id << "\t";
		for (int j = dataLFUPR[i].pageFrame.size() -1 ; j >= 0 ; j--)
		//	cout << dataFIFO[i].pageFrame[j].id ;
			Out_File << dataLFUPR[i].pageFrame[j].tableid;
		if (dataLFUPR[i].pageFault) Out_File << "\t" << "F";
		Out_File << endl;
	} // for 

	Out_File << "Page Fault = " << pagefaultLFUPR << "  Page Replaces = " << pageReplacesLFUPR << "  Page Frames = " << PageFrame << endl << endl ;
    cout << "Page Fault = " << pagefaultLFUPR << "  Page Replaces = " << pageReplacesLFUPR << "  Page Frames = " << PageFrame << endl << endl ;
    
    
    	Out_File << "--------------Most Frequently Used Page Replacement -----------------------" << endl;

	for (int i = 0; i < dataMFUPR.size() ; i++) {
		
		Out_File << dataMFUPR[i].id << "\t";
		for (int j = dataMFUPR[i].pageFrame.size() -1 ; j >= 0 ; j--)
		//	cout << dataFIFO[i].pageFrame[j].id ;
			Out_File << dataMFUPR[i].pageFrame[j].tableid;
		if (dataMFUPR[i].pageFault) Out_File << "\t" << "F";
		Out_File << endl;
	} // for 

	Out_File << "Page Fault = " << pagefaultMFUPR << "  Page Replaces = " << pageReplacesMFUPR << "  Page Frames = " << PageFrame << endl ;
    cout << "Page Fault = " << pagefaultMFUPR << "  Page Replaces = " << pageReplacesMFUPR << "  Page Frames = " << PageFrame << endl ;
	
	Out_File << "--------------Least Frequently Used LRU Page Replacement-----------------------" << endl;

	for (int i = 0; i < dataLFUPR.size() ; i++) {
		
		Out_File << dataLFULRU[i].id << "\t";
		for (int j = dataLFULRU[i].pageFrame.size() -1 ; j >= 0 ; j--)
		//	cout << dataFIFO[i].pageFrame[j].id ;
			Out_File << dataLFULRU[i].pageFrame[j].tableid;
		if (dataLFULRU[i].pageFault) Out_File << "\t" << "F";
		Out_File << endl;
	} // for 

	Out_File << "Page Fault = " << pagefaultLFULRU << "  Page Replaces = " << pageReplacesLFULRU << "  Page Frames = " << PageFrame << endl <<endl  ;
    cout << "Page Fault = " << pagefaultLFULRU << "  Page Replaces = " << pageReplacesLFULRU << "  Page Frames = " << PageFrame << endl ;
    
	Out_File << "--------------Most Frequently Used LRU Page Replacement -----------------------" << endl;

	for (int i = 0; i < dataMFULRU.size() ; i++) {
		
		Out_File << dataMFULRU[i].id << "\t";
		for (int j = dataMFULRU[i].pageFrame.size() -1 ; j >= 0 ; j--)
		//	cout << dataFIFO[i].pageFrame[j].id ;
			Out_File << dataMFULRU[i].pageFrame[j].tableid;
		if (dataMFULRU[i].pageFault) Out_File << "\t" << "F";
		Out_File << endl;
	} // for 

	Out_File << "Page Fault = " << pagefaultMFULRU << "  Page Replaces = " << pageReplacesMFULRU << "  Page Frames = " << PageFrame << endl  ;
    
	cout << "Page Fault = " << pagefaultMFULRU << "  Page Replaces = " << pageReplacesMFULRU << "  Page Frames = " << PageFrame << endl  ;
    dataFIFO.clear();
	dataLFUPR.clear();
	dataMFUPR.clear();
	dataLFULRU.clear();
	dataMFULRU.clear();
    
    
    
    
	Out_File.close() ;
	
	PageFrame = 0 ;
	pageReplacesFIFO = 0 ;
	pagefaultFIFO = 0 ;
	pagefaultLRU = 0 ;
	pageReplacesLRU = 0 ;
	pageReplacesLFUPR = 0 ;
	pagefaultLFUPR = 0 ;		
	pageReplacesMFUPR  = 0 ;
	pagefaultMFUPR  = 0 ;
	pageReplacesLFULRU = 0 ;
	pagefaultLFULRU = 0 ;
	pageReplacesMFULRU = 0 ;
	pagefaultMFULRU = 0 ;
	
	
} // write
	
}; // PageReplacement 


int main(int argc, char** argv) {
	string input = "";
	PageReplacement user ;
	user.Read( input );
	user.FIFO();
	user.LRU();
	user.LFUPR() ;
	user.MFUPR() ;
	user.LFULRU() ;
	user.MFULRU() ;
	user.Wirte(input) ;

	system("pause");
    return 0;
} // int main




