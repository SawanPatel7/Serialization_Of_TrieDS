///author : Sawan Patel
#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;

//This structure is used to represent a node of the trie.
struct trieNode
{
    //indicates if the word ending here is a valid word.
    bool isValidWord;
    //number of words have the prefix that is represented by this node.
    int wordsWithPrefix;
    //represent all child nodes of this node.
    map<char,trieNode*> children;

    trieNode()
    {
       isValidWord = false;
       wordsWithPrefix = 0;
    }

};

//This class represents a generic trie data structure
class TrieDS
{
private:

   trieNode *root;

   int totalWords;

   //helper function that traverse the trie and finds all word present in trie.
   void treeDepthTraversal(trieNode *currentNode,
                   vector<string>& trieToString,string prefix)
   {
   	 if(currentNode->isValidWord)
   	 {
   	 	trieToString.push_back(prefix);
   	 }
   	 for(auto p : currentNode->children)
   	 {
   	 	prefix.push_back(p.first);
   	 	treeDepthTraversal(p.second, trieToString, prefix);
   	 	prefix.pop_back();
   	 }
   }

   //helper function
   void buildSerialTrie(trieNode *root, string &serialTrie)
   {
       if(root->isValidWord)
          serialTrie.push_back(']');
       map<char, trieNode*> &children = root->children;
       for(auto child : children)
       {
           serialTrie.push_back(child.first);
           buildSerialTrie(child.second, serialTrie);
       }
       serialTrie.push_back('>');
   }

   //helper function
   int stringToTrie(trieNode *node, string &serialTrie, int &read)
   {
        int wordSeen = 0;
        if(serialTrie[read] == ']')
        {
           node->isValidWord = 1;
           wordSeen++;
           read++;
        }
        else node->isValidWord = 0;


        map<char, trieNode*> &childMap = node->children;
        while(serialTrie[read] != '>')
        {
            char ch = serialTrie[read++];
            childMap[ch] = new trieNode();
            wordSeen += stringToTrie(childMap[ch], serialTrie, read);
        }
        read++;
        node->wordsWithPrefix = wordSeen;
        return wordSeen;
   }

public:

   //used to construct an empty trie.
   TrieDS()
   {
      root = new trieNode();
      totalWords = 0;
   }

   //Used to construct a trie initialized with a list of words.
   TrieDS(vector<string> words)
   {
       totalWords = 0;
       root = new trieNode();
       for(string word : words)
       {
       	  insertWord(word);
       }
   }

   //Used to construct a trie using an existing trie given in serial format.
   TrieDS(string serialTrie)
   {
       root = new trieNode();
       int read = 0;
       totalWords = stringToTrie(root, serialTrie, read);
   }

   //used to insert a word in the trie.
   void insertWord(string word)
   {
      trieNode *currentNode = root;
      for(int i = 0; i < word.length(); i++)
      {
        currentNode->wordsWithPrefix++;
      	map<char,trieNode*>& childMap = (currentNode->children);
      	auto it = childMap.find(word[i]);
      	if(it == childMap.end())
      	{
      	   currentNode = new trieNode();
      	   childMap[word[i]] = currentNode;
      	}
      	else
      	{
      		currentNode = childMap[word[i]];
      	}
      }
      currentNode->wordsWithPrefix++;
      currentNode->isValidWord = true;
      totalWords++;
   }

   //gives the total number of words currently present in the trie.
   int numberOfWordsInTrie()
   {
       return totalWords;
   }

   //Used to convert the trie to a list of words(serialization).
   vector<string> trieToList()
   {
   	  vector<string> trieToString;
   	  treeDepthTraversal(root, trieToString, "");
   	  return trieToString;
   }

   //check to see if a word is present in the trie.
   bool findWord(string word)
   {
      int countOfWord = wordsWithThePrefixCount(word,0);
      if(countOfWord>0)
         return true;
      else return false;
   }

   //Number of words in the trie the have a given prefix.
   int wordsWithThePrefixCount(string prefix, int calledBy = 1)
   {
      if(root==NULL)
              return 0;
      trieNode *currentNode = root;
      for(int i = 0; i < prefix.length(); i++)
      {
        map<char,trieNode*>& childMap = (currentNode->children);
      	auto it = childMap.find(prefix[i]);
      	if(it == childMap.end())
      	   return 0;
      	else
      		currentNode = childMap[prefix[i]];
      }
      if(calledBy||currentNode->isValidWord)
           return currentNode->wordsWithPrefix;
      else return 0;
   }

   string trieToString()
   {
       string serializedTrie;
       buildSerialTrie(root, serializedTrie);
       return serializedTrie;
   }

  vector<string> wordsWithPrefixStrings(string prefix)
  {
    vector<string> words;
    if (root == NULL)
    return words ;
    trieNode * currentNode = root;
    for (int i = 0; i < prefix.length(); i++) {
      map < char, trieNode * > & childMap = (currentNode -> children);
      auto it = childMap.find(prefix[i]);
      if (it == childMap.end())
        return words ;
      else
        currentNode = childMap[prefix[i]];
    }
    treeDepthTraversal(currentNode,words,prefix);
    return words;
  }
};

int main()
{
    /*
     *THIS PROGRAM USES THE TEST DATA
     *TO BUILD A TRIE FOR THE GIVEN DATA
     *THEN IT CONVERTS THE TRIE INTO ITS
     *SERIALIZED FORM AND WRITES IT TO A FILE
     */
int ch;   
do{
  system("cls");
  TrieDS * myTrie = new TrieDS();
  string word;
  ifstream fin;
  ofstream fout;
  int n;
  bool isTestData=false;
  bool isSerializeData=false;
  char c;
  cout<<"\n\tDICTIONARY";
  cout<<"\n\t1.EXTRACTING DATA FROM DESERIALIZE FILE";
  cout<<"\n\t2.EXTRACTING DATA FROM SERIALIZE FILE ";
  cout<<"\n\t3.WORK ON SIMPLE TRIE ";
  cout<<"\n\tEnter your choice: ";
  cin>>ch;
  switch(ch)
  {
  case 1: fin.open("TestData.txt");
      isTestData=true;
      while(!fin.eof())
      {
        fin>>word;
        myTrie->insertWord(word);
      }
      fin.close();
      break;
  case 2: fin.open("serializeData.txt");
          isSerializeData=true;
          fin>>word;
          TrieDS * curTrie=new TrieDS(word);
          myTrie=curTrie;
          break;

  }
  cout<<"\n\tAdding words to the dictionary...";
  sleep(3);
  cout<<"\n\tAll the words have been suceesfully added to the dictionary."; 
 bool ischange=false;
 do
  {
    system("cls");
    cout<<"\n\t1.Search Word";
    cout<<"\n\t2.Insert Word";
    cout<<"\n\t3.Seach All Word with given prefix";
    cout<<"\n\t4.Serialize All the words in trie to String";
    cout<<"\n\tEnter your choice: ";
    cin>>ch;
    // cout<<"\n\tENTER WORD: ";
    // cin>>word;
    
    switch(ch)
    {
      case 1:
             cout<<"\n\tENTER WORD: ";
               cin>>word;
             if(myTrie->findWord(word))
             {
              cout<<"\n\t"<<word<<" is Present in Dictionary"<<endl;
             }
             else{
              cout<<"\n\t"<<word<<" is Not found in Dictionary"<<endl;
               vector<string>words=myTrie->wordsWithPrefixStrings(word);
                int  cnt=0;
                if(words.size()){
                 cout<<"\n\t Do you mean any of this word";
                for(string curWord:words)
                {
                  cnt++;
                  cout<<"\n\t  "<<curWord<<endl;
                }
               }
             }
           break;
      case 2:
               ischange=true;
               cout<<"\n\tENTER WORD: ";
               cin>>word;
               myTrie->insertWord(word);
               cout<<"\n\t Your word is successfully added";
               break;
      case 3:   
                {
                cout<<"\n\tENTER PREFIX: ";
                cin>>word;
                vector<string>words=myTrie->wordsWithPrefixStrings(word);
                int  cnt=0;
                for(string curWord:words)
                {
                  cnt++;
                  cout<<"\n\t"<<curWord<<endl;
                }
                if(cnt==0)
                {
                  cout<<"\n\tThere is No word this given prefix: ";
                }
                else{
                  cout<<"\n\tThere are : "<<cnt<<" words in Dictionary with given prefix";
                }
            }
                break;
      case 4:
            cout<<"\n\tENTER THE FILE NAME WHERE YOU WANT TO SAVE THE SERIALIZE STRING ";
            string fname;
            cin>>fname;
            string serializeData=myTrie->trieToString();
            fout.open(fname+".txt",ios::trunc);
            fout<<serializeData<<endl;
            fout.close();
            cout<<"\n\t Your file is successfully saved as " + fname +".txt";
        
    }
    cout<<"\n\n\twant to continue (enter 'y' or 'Y') ";
    cin>>c;
  }while(c=='y'||c=='Y');
  
  //Adding data back to file.
  if(isTestData&&ischange)
  {
    cout<<"\n\t Do you want to save the changes you make on file";
    cout<<"\n\t if yes than press (1)  ";
    cin>>ch;
    if(ch){
    vector<string>List=myTrie->trieToList();
    fout.open("TestData.txt",ios::trunc);
    for(string i:List)
    {
        fout<<i<<endl;
    }
    cout<<"\n\t Your changes are successfully saved.";
    }
    fout.close();
  }

  //Adding Data back to file.
  if(isSerializeData&&ischange)
  {
    cout<<"\n\t Do you want to save the changes you make on file";
    cout<<"\n\t if yes than press (1)  ";
    cin>>ch;
    if(ch)
    {
    string serializeData=myTrie->trieToString();
    fout.open("SerializeData.txt",ios::trunc);
    fout<<serializeData<<endl;
    fout.close();
    cout<<"\n\t Your changes are successfully saved.";
    }
  }

  cout<<"\n\t Press 1 to back on home screen else Exit ";
  cin>>ch;
}while(ch);

}