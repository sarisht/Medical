#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <numeric>
#include <time.h>
#include <iomanip>


// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

size_t split(const string &txt, vector<string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

private:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

public:
	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)
	{
		Node_Name=name;
	
		nvalues=n;
		values=vals;
		

	}
	string get_name()
	{
		return Node_Name;
	}
	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<float> get_CPT()
	{
		return CPT;
	}
	int get_nvalues()
	{
		return nvalues;
	}
	vector<string> get_values()
	{
		return values;
	}
	void set_CPT(vector<float> new_CPT)
	{
		CPT.clear();
		CPT=new_CPT;
	}
    void set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }



};


 // The whole network represted as a list of nodes
class network{
public:
	vector <Graph_Node> Pres_Graph;
	int addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		return 0;
	}
    
    
	int netSize()
	{
		return Pres_Graph.size();
	}
    // get the index of node with a given name
    int get_index(string val_name)
    {
        vector<Graph_Node>::iterator vectorIt;
        int count=0;
        for(vectorIt=Pres_Graph.begin();vectorIt!=Pres_Graph.end();vectorIt++)
        {
            if(vectorIt->get_name().compare(val_name)==0)
                return count;
            count++;
        }
        return -1;
    }
// get the node at nth index
    vector<Graph_Node>::iterator get_nth_node(int n)
    {
       vector<Graph_Node>::iterator vectorIt;
        int count=0;
        for(vectorIt=Pres_Graph.begin();vectorIt!=Pres_Graph.end();vectorIt++)
        {
            if(count==n)
                return vectorIt;
            count++;
        }
        return vectorIt; 
    }
    //get the iterator of a node with a given name
    vector<Graph_Node>::iterator search_node(string val_name)
    {
        vector<Graph_Node>::iterator vectorIt;
        for(vectorIt=Pres_Graph.begin();vectorIt!=Pres_Graph.end();vectorIt++)
        {
            if(vectorIt->get_name().compare(val_name)==0)
                return vectorIt;
        }
    
            cout<<"node not found\n";
        return vectorIt;
    }
	

};

network read_network(string v1)
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile(v1); 
  	string temp;
  	string name;
  	vector<string> values;
  	
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		
      		
      		ss.str(line);
     		ss>>temp;
     		
     		
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;
     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{
     					
     					ss2>>temp;
     					
     					
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					
     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values);
     				int pos=Alarm.addNode(new_node);

     				
     		}
     		else if(temp.compare("probability")==0)
     		{
                    
     				ss>>temp;
     				ss>>temp;
     				
                    vector<Graph_Node>::iterator vectorIt;
                    vector<Graph_Node>::iterator vectorIt1;
     				vectorIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        vectorIt1=Alarm.search_node(temp);
                        vectorIt1->add_child(index);
     					values.push_back(temp);
     					
     					ss>>temp;

    				}
                    vectorIt->set_Parents(values);
    				getline (myfile,line);
     				stringstream ss2;
                    
     				ss2.str(line);
     				ss2>> temp;
                    
     				ss2>> temp;
                    
     				vector<float> curr_CPT;
                    string::size_type sz;
     				while(temp.compare(";")!=0)
     				{
                        
     					curr_CPT.push_back(atof(temp.c_str()));
     					
     					ss2>>temp;
                       
                        

    				}
                    
                    vectorIt->set_CPT(curr_CPT);


     		}
            else
            {
                
            }
     		
     		

    		
    		
    	}
    	
    	if(find==1)
    	myfile.close();
  	}
  	
  	return Alarm;
}

int find(vector<string> vt, string val)
{
	for(int i = 0; i < vt.size();i++)
	{
		if(vt[i] == val)
		return i;
	}
	return -1;
}

int prob_random(vector<float> vct, int start, int number){
	int diff = vct.size()/number;
	double val = (double)rand() / RAND_MAX;
	float total = 0;
	float sum_of_elems =0;
	int v = start;
	while (v <vct.size()){
		if (vct[v] == -1)
		vct[v] = 0;
		sum_of_elems += vct[v];
		v+=diff;
	}
	v = start;
	if (sum_of_elems == 0)
	return 0;
	while (v <vct.size()){
		total += vct[v];
		if(total/sum_of_elems >= val){
			return v;
		}
		v+=diff;
	}
	return v;
}

int main(int argc, char *argv[])
{
	network Alarm;
	Alarm=read_network(argv[1]);
	vector<vector<string> > inp_data;
	string line;
	string filename = argv[2];
    ifstream input(filename.c_str());
    if(!input){
        cerr << "Error in opening file: " << filename << "\n";
        return 1;
    }

    while(getline(input, line)){
        stringstream ss;
        ss << line;
        vector<string> row;
        while (!ss.eof()) {
            string tempStr;
            ss >> tempStr;
            row.push_back(tempStr);
        }
        inp_data.push_back(row);
    }
	int total = 0;
	vector<float> counter[inp_data[0].size()];
	for (int x= 0;x< Alarm.netSize();x++){
		vector<float> temp((Alarm.Pres_Graph[x]).get_CPT().size());
		counter[x] = temp;
	}
	time_t timer;
	time(&timer);
	vector<vector<string> > inp_data_copy= inp_data;
	double diff = difftime(timer,timer);
    while (diff<500){
		time_t curr_timer ;
		cout<<"time so far: "<<diff<<endl;
		for (int y=0;y<inp_data.size();y++)
		{	
			for (int x=0;x<inp_data[y].size();x++)//imputation of ?
			{
				Graph_Node curr_node = ((Alarm.Pres_Graph[x]));
				vector<string> parents = curr_node.get_Parents();
				int ind= find(curr_node.get_values(),inp_data[y][x]);
				if(ind == -1)
				{
					int start = 0;
					for(int num = 0; num<parents.size();num++){
						int alarm_index = Alarm.get_index(parents[num]);
						Graph_Node par_node = (Alarm.Pres_Graph[alarm_index]);
						int ind_1 = find(par_node.get_values(), inp_data[y][alarm_index]);
						start = par_node.get_nvalues()*start + ind_1;
					}
					int end = start + curr_node.get_nvalues();
					int nval = curr_node.get_nvalues();
					ind = prob_random(counter[x],start,nval);
					inp_data[y][x] = curr_node.get_values()[(ind*nval)/counter[x].size()];
				}
			}
			for (int x=0;x<inp_data[y].size();x++)// first loop making the count martix
			{
				Graph_Node curr_node = ((Alarm.Pres_Graph[x]));
				vector<string> parents = curr_node.get_Parents();
				int ind = find(curr_node.get_values(),inp_data[y][x]);
				for(int num = 0; num<parents.size();num++){
					int alarm_index = Alarm.get_index(parents[num]);
					Graph_Node par_node = (Alarm.Pres_Graph[alarm_index]);
					int ind_1 = find(par_node.get_values(), inp_data[y][alarm_index]);
					ind = par_node.get_nvalues()*ind + ind_1;
				}
				
				if(counter[x][ind] == -1){
					counter[x][ind] = 1;
				}
				else{
					counter[x][ind]++;
				}
			}
		}
		time(&curr_timer);
		diff = difftime(curr_timer,timer);
		inp_data = inp_data_copy;

	}
	vector<float> probabilities[inp_data[0].size()];
	for (int i=0; i < inp_data[0].size(); i++)
	{
		int n = Alarm.Pres_Graph[i].get_nvalues();
		int k = counter[i].size();
		int skip = k/n;
		float totals[skip];
		for(int j = 0; j< skip; j++)
		totals[j] = 0;
		for(int j = 0; j < k; j++)
		totals[j%skip] += counter[i][j]+0.1;
		for(int j = 0; j < k; j++)
		probabilities[i].push_back(((counter[i][j]+0.1))/totals[j%skip]);
	}
	ifstream myfile(argv[1]);
	ofstream outfile;
	outfile.open("Solved_Alarm.bif");
	string temp;
	string name;
	vector<string> values;
     		
	if (myfile.is_open())
    {
		int ctr = 0;
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
			if(line.compare("")==0)
			continue;
      		ss.str(line);
     		ss>>temp;
     		if(temp.compare("probability")==0)
     		{
     			outfile<<line<<endl;
				getline(myfile,line);
				outfile<<"\t table ";
				for(int i = 0 ; i < probabilities[ctr].size() ;i++){
					outfile<<setprecision(4)<< probabilities[ctr][i]<<" ";
				}
				outfile<<";"<<endl;
				ctr++;
			}
			else
			{
				outfile<<line<<endl;
			}
		}
  	}
    return 0;
}
