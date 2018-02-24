#include <iostream>
using namespace std;
#include <string>
#include <fstream>

struct Node{    Node * prev;    char n;     Node * next;    };
void printList(string **List, int counter, int item){
    for (int i =0; i < counter; i++){
        for (int j =0; j < item; j++){
            cout<<List[i][j]<<" ";
        }cout<<endl;
    }
    cout<<endl;
}
// READ FROM FILE
string **readFile (ifstream &IFS, int &counter, int &Items){
    Node *head;
    Node *tail;
    Node *cu = new Node;
    head = cu;
    //load everything into a doubly Linked List
    while (IFS.get(cu->n)){
        if (cu->n == '{'){
            counter++;
        }
        if (cu->n == ',' && counter == 1){
            Items++;
        }
        cu->next = new Node;
        cu->next->prev = cu;
        cu = cu->next;
    }
    //go back to the last one
    cu = cu->prev;
    //reset for putting it in post
    cu->next = nullptr;
    tail = cu;
    cu = head;
    //putting things into pointer string
    string **Listay= new string *[counter];
    for(int i = 0; i < counter;i++){
        Listay[i] = new string[Items];
    }
    int index1 = 0; // for counter
    int index2 = 0; // for Items
    while (cu->next != nullptr){
        cu = cu->next;
        if (cu->n == '{'){
            continue;
        }
        if (cu->n == '}'){
            index1++;
            index2 = 0;
            cu = cu->next;
            continue;
        }
        //if next item
        if (cu->n == ','){
            index2++;
            continue;
        }
        Listay[index1][index2].push_back(cu->n);
        
    }
    return Listay;
}

string *readAttribute(ifstream & Attr, int &counter){
    //read file into 1 line first, the check for how many words
    string Line;
    getline(Attr,Line);
    
    //find how many attributes are there by : spaces + 1
    for (int i = 0; i < Line.length();i++){
        if (Line[i] == ' '){    counter++;      }
    }
    counter++;
    
    //writting it to a list
    string *line2 = new string [counter];
    int index = 0;
    for (int i = 0; i < Line.length();i++){
        if (Line[i] == ' '){    index++;    continue;       }
        else{   line2[index].push_back(Line[i]);    }
    }
    return line2;
}

void outputFile ( string **Listay, int counter,int Items){
    ofstream ofs;
    ofs.open("output.txt");
    
    for(int i = 0; i < counter; i++){
        ofs<<'{';
        for (int j = 0; j < Items;j++){
            if (j+1 == Items){
                ofs<<Listay[i][j]<<'}';
                continue;
            }
            else{
                ofs<<Listay[i][j]<<',';
            }
        }
        ofs<<endl;
    }
    ofs.close();
}
//other functions
void swapTwoString(string **A,string **B){
    string *temp = *A;
    *A = *B;
    *B = temp;
}


//chekcing values, I got help from Sarah B
string checkGreater(string A[],string B[],string Attr[],int AttrIndex, int maxAttrIndex,int itemIndex){
    if (AttrIndex >= maxAttrIndex){
        return "Equal";
    }
    else{
        int AttrLen = Attr[AttrIndex].size();
        int ListIndex = 0;
        for (int i = 0; i < itemIndex;i++){
            if (A[i].substr(0,AttrLen) == Attr[AttrIndex].substr(0,AttrLen)){
                ListIndex = i;
                break;
            }
        }
        int i = 0;
        while (i < A[ListIndex].size() and i < B[ListIndex].size()){
            if (A[ListIndex][i] > B[ListIndex][i]){         return "Greater";}
            else if (A[ListIndex][i] == B[ListIndex][i]){   i++;            }
            else if (A[ListIndex][i] < B[ListIndex][i]){    return "Less";  }
        }
        return checkGreater(A,B,Attr,AttrIndex+1,maxAttrIndex,itemIndex);
    }
}












void sortSelect(string **List, string sortWhatType[],int Attr_num,int Listlength, int ListItems){
    int Lowest_num_Index;
    string temp;
    string checkGreaterOutput;
    int AttributeIndex = 0;
    for (int i = 0; i < Listlength; i++){
        Lowest_num_Index = i;
        for (int j = i+1; j < Listlength; j++){
            checkGreaterOutput =  checkGreater(List[Lowest_num_Index], List[j], sortWhatType,AttributeIndex,Attr_num,ListItems);
            if (checkGreaterOutput == "Greater"){
                Lowest_num_Index = j;
            }
        }
        swapTwoString(&List[i], &List[Lowest_num_Index]);
    }
}

void sortInsert(string **List, string sortWhatType[],int Attr_num, int Listlength, int ListItems){
    string *temp;
    int AttributeIndex = 0;
    //string checkGreaterOutput = "Less";
    int j;
    for (int i = 1; i < Listlength; i++){
        temp = List[i];
        j = i-1;
        while (j >= 0 and (checkGreater(temp, List[j], sortWhatType,AttributeIndex,Attr_num,ListItems) == "Less")){
            swapTwoString(&List[j+1], &List[j]);
            j--;
        }
    }
}

//Quick sort
int partition (string **List,string sortWhatType[],int Attr_num, int left, int right, int ListItems){
    string *ListPivot;
    ListPivot = List[right];
    int AttributeIndex = 0;
    int indexSmall = left-1;
    for (int j = left; j <= right-1;j++){
        if (checkGreater(ListPivot, List[j], sortWhatType,AttributeIndex, Attr_num, ListItems) == "Greater" or
            checkGreater(ListPivot, List[j], sortWhatType, Attr_num, right, ListItems) =="Equal"){
            indexSmall++;
            swapTwoString(&List[indexSmall], &List[j]);
        }
    }
    swapTwoString(&List[indexSmall+1], &List[right]);
    return indexSmall + 1;
}
void sortQuick (string **List, string sortWhatType[],int Attr_num,int left, int right, int ListItems){
    if (left<right){
        int Partition_index = partition(List,sortWhatType,Attr_num, left, right,ListItems);
        sortQuick(List, sortWhatType, Attr_num, left, Partition_index - 1,ListItems);
        sortQuick(List, sortWhatType, Attr_num, Partition_index + 1, right,ListItems);
    }
}

















void Appending_lists(string **List,string sortWhatType[],int Attr_num, int left, int middle, int right, int ListItems){
    
    //initiating
    int List1Len = middle-left + 1;
    int List2Len = right-middle;
    string **List1 = new string *[List1Len];
    for (int i = 0; i < List1Len;i++){
        List1[i] = new string [ListItems];
        for(int j = 0; j < ListItems;j++){
            //cout<<"list 1: "<<i<<" "<<j<<endl;
            List1[i][j] = List[left+i][j];
        }
    }
    string **List2 = new string *[List2Len];
    for (int i = 0; i < List2Len;i++){
        List2[i] = new string [ListItems];
        for(int j = 0; j < ListItems;j++){
            //cout<<"list 2: "<<i<<" "<<j<<endl;
            List2[i][j] = List[middle+1+i][j];
        }
    }
    int List1Index = 0;
    int List2Index = 0;
    int MainListIndex = left;
    
    while (List1Index < List1Len and List2Index < List2Len){
        
        if (checkGreater( List1[List1Index],List2[List2Index],sortWhatType, 0, Attr_num, ListItems) == "Less"){
            List[MainListIndex] = List1[List1Index];
            List1Index++;
        }
        else{
            List[MainListIndex] = List2[List2Index];
            List2Index++;
        }
        MainListIndex++;
    }
    
    while (List1Index < List1Len){
        List[MainListIndex] = List1[List1Index];
        List1Index++;
        MainListIndex++;
    }
    while (List2Index < List2Len){
        List[MainListIndex] = List2[List2Index];
        List2Index++;
        MainListIndex++;
    }
}

//got Merge sort from a bit of help from google
void sortMerge (string **List, string sortWhatType[],int Attr_num,int left, int right, int ListItems){
    if (left<right){
        int middle = left+((right-left)/2);
        sortMerge(List, sortWhatType, Attr_num, left, middle, ListItems);
        sortMerge(List, sortWhatType, Attr_num, middle+1, right, ListItems);
        Appending_lists(List,sortWhatType,Attr_num,left, middle, right, ListItems);
        //printList(List, right-left, ListItems);
    }
}






































int main() {
    ifstream ifs("input.txt");
    ifstream Atr("attributes1.txt");
    
    int counter = 0;
    int Item = 1;
    string **A = readFile(ifs, counter, Item);
    
    //attributes
    int AtrCounter = 0;
    string *B = readAttribute(Atr, AtrCounter);
    /*for (int i =0; i < AtrCounter; i++){
        cout<<B[i]<<endl;
    }*/
    
    //sortSelect(A, B, AtrCounter, counter, Item);
    //sortInsert(A, B, AtrCounter, counter, Item);
    //sortMerge (A, B, AtrCounter, 0, counter-1, Item);
    sortQuick   (A, B, AtrCounter, 0, counter-1, Item);
    printList(A, counter, Item);
    
    /*printList(A, counter, Item);
    swapTwoString(A[1], A[3]);
    printList(A, counter, Item);*/
    
    outputFile(A, counter ,Item);
    
    return 0;
}
