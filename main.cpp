#include<set>
#include<fstream>
#include<cassert>
#include<iostream>
#include "NWKTree.h"
using namespace std;

set<string>s;
NwkTree tree;

bool in_set(const string& str){return s.find(str)!=s.end();}

bool dfs(Node *now){
    Node *nex,*pre = nullptr;
    bool is_leaf = now->get_name().substr(0,3) == "EPI";
    if(is_leaf){
        if(!in_set(now->get_name()))return false;
        return true;
    }
    for(nex = now->get_son();nex;){
//        printf("%s -> %s, length = %.5lf\n",now->get_name().c_str(), nex->get_name().c_str(), nex->get_len());
        bool no_del = dfs(nex);
        Node *tmp = nex;
        if(no_del)pre = nex;
        nex = nex->get_nex();
        if(!no_del){
//            printf("delete it: %s\n",tmp->get_name().c_str());
//            if(!pre)printf("No pre!\n");
            now->rm_nex_clade(pre, tmp);
//            printf("delete complete!\n");
        }
    }
//    assert(!is_leaf);
    if(!now->get_son())return false;// leaf never touch here, so no need to check !is_leaf
    return true;
}

int main(){
//    freopen("sb.nwk","r",stdin);
//    freopen("new_tree.nwk","w",stdout);
//    string file_name = "sb.nwk";
    string dir = R"(D:\source\cpps\NWKTreeBuilder\files\)";
    string file_name = R"(global.nwk)";
    string output_name = R"(global_filtered.nwk)";
    string dic_name = R"(both_in_nextstrain_and_GISAIDtree.tsv)";

    ifstream dicfile;
    dicfile.open(dir + dic_name);
    if (!dicfile.is_open()) {
        cerr << "Could not open the file - '"
             << dic_name << "'" << endl;
        return EXIT_FAILURE;
    }
    string EPI_name;
    while(getline(dicfile, EPI_name))s.insert(EPI_name);
    printf("Dic loaded!\n");
    printf("time used: %.2lfs\n",(double)clock()/CLOCKS_PER_SEC);

    int result = tree.from_file(dir + file_name);
    assert(result==0);
    tree.build_tree();
    printf("build tree completed!\n");
    printf("time used: %.2lfs\n",(double)clock()/CLOCKS_PER_SEC);
//    tree.print_tree();

    dfs(tree.get_real_root());
    printf("dfs tree completed!\n");
    printf("time used: %.2lfs\n",(double)clock()/CLOCKS_PER_SEC);

    tree.to_file(dir + output_name);
    printf("new tree written to %s!\n",output_name.c_str());
    printf("time used: %.2lfs\n",(double)clock()/CLOCKS_PER_SEC);
    return 0;
}