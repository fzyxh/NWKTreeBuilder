#include<fstream>
#include<cassert>
#include<iostream>
#include "NWKTree.h"
using namespace std;

NwkTree tree;

void dfs(Node *now){
    Node *nex, *pre = nullptr;
    bool is_leaf = now->get_name().substr(0,3) == "EPI";
    if(is_leaf){
        return;
    }
    assert(now->get_son());
    Node *minson = nullptr, *minpre = nullptr;
    string minson_name = now->get_son()->get_name();
    double min_len = 999.99999;
    for(nex = now->get_son();nex;nex = nex->get_nex()){
//        printf("%s -> %s, length = %.5lf\n",now->get_name().c_str(), nex->get_name().c_str(), nex->get_len());
        dfs(nex);
        if(nex->get_len() < min_len){
            min_len = nex->get_len();
            minson_name = nex->get_name();
            minson = nex;
            minpre = pre;
        }
        pre = nex;
    }
    for(nex = now->get_son();nex;nex = nex->get_nex()){
        if(minson_name == nex->get_name()){
            continue;
        }
        nex->set_len(nex->get_len() - min_len);
    }
    now->set_name(minson_name);
    now->set_len(now->get_len() + min_len);
    now->rm_only_nex_clade(minpre, minson);
}

int main(){
//    freopen("sb.nwk","r",stdin);
//    freopen("new_tree.nwk","w",stdout);
//    string file_name = "sb.nwk";
    string dir = R"(D:\SARS-Cov-2\nextstrain\NWKTreeBuilder\files\)";
    string file_name = R"(global_filtered.nwk)";
    string output_name = R"(global_filtered_Clade_Only.nwk)";

    int result = tree.from_file(dir + file_name);
    assert(result==0);
    tree.build_tree();
    printf("build tree completed!\n");
    printf("time used: %.2lfs\n",(double)clock()/CLOCKS_PER_SEC);
//    tree.print_tree();

    dfs(tree.get_real_root());
    cout<<tree.get_real_root()->get_name()<<endl;
    printf("dfs tree completed!\n");
    printf("time used: %.2lfs\n",(double)clock()/CLOCKS_PER_SEC);

    tree.to_file(dir + output_name);
    printf("new tree written to %s!\n",output_name.c_str());
    printf("time used: %.2lfs\n",(double)clock()/CLOCKS_PER_SEC);
    return 0;
}