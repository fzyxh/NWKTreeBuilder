//
// Created by phinney on 2023/04/05.
//

#include<fstream>
#include<iomanip>
#include<iostream>
#ifndef NWKTREEBUILDER_NWKTREE_H
#define NWKTREEBUILDER_NWKTREE_H

#endif //NWKTREEBUILDER_NWKTREE_H

class Node{
private:
    std::string name;
    double length;
    Node *son = nullptr, *nex = nullptr;
public:
    explicit Node(std::string nam="",double lengt=0.0){
        this->name = std::move(nam);
        this->length = lengt;
    }
    ~Node()= default;
    std::string get_name() const{
        return this->name;
    }
    double get_len() const{
        return this->length;
    }
    void set_son(Node *node){
        this->son = node;
    }
    void set_nex(Node *node){
        this->nex = node;
    }
    Node* get_son(){
        return this->son;
    }
    Node* get_nex(){
        return this->nex;
    }
    void set_name(std::string nam){
//        cout<<"set name:"<<name<<endl;
        this->name = std::move(nam);
    }
    void set_len(double lengt){
        this->length = lengt;
    }
    void add_clade_at_tail(Node *new_clade){
        if(!new_clade)return;
        new_clade->set_nex(this->nex);
        this->nex=new_clade;
    }
    void rm_nex_clade(Node *pre, Node *del_clade){
        if(del_clade == this->son){
            this->son = del_clade->get_nex();
            delete del_clade;
            return;
        }
        pre->set_nex(del_clade->get_nex());
        delete del_clade;// dangerous behavior
    }
};

class NwkTree{
private:
    std::ifstream infile;
    std::ofstream outfile;
    Node *root= nullptr;
    char get_buf(){
        char ch;
        if(!this->infile.get(ch)){
            ch = '\0';
            fprintf(stderr, "get NULL!\n");
            return ch;
        }
        return ch;
    }
    void putback_buf(char ch){this->infile.putback(ch);}
    void write_dfs(Node *node){
        Node *nex = node->get_son();
        if(node->get_son()) outfile<<'(';
        for(;nex;nex = nex->get_nex()){
//            printf("%s -> %s, length = %.5lf\n",node->get_name().c_str(), nex->get_name().c_str(), nex->get_len());
            write_dfs(nex);
            if(nex->get_nex()) outfile<<',';
        }
        if(node->get_son()) outfile<<')';
        outfile<<std::setprecision(6)<<node->get_name()<<':'<<node->get_len();
        outfile.flush();
    }
    static Node* get_new(std::string name="",double length=0.0){
        Node *node = new Node(std::move(name),length);
        return node;
    }
public:
    int from_file(const std::string& file_name){
        this->infile.open(file_name);
        if (!this->infile.is_open()) {
            std::cerr << "Could not open the file - '"
                 << file_name << "'" << std::endl;
            return EXIT_FAILURE;
        }
        return 0;
    }
    int to_file(const std::string& file_name){
        this->outfile.open(file_name);
        if (!this->outfile.is_open()) {
            std::cerr << "Could not open the file - '"
                 << file_name << "'" << std::endl;
            return EXIT_FAILURE;
        }
        this->write_dfs(this->root);
        outfile.close();
        return 0;
    }
    Node get_root(){return *this->root;}
    Node* get_real_root(){return this->root;}
    Node* dfs_file(){
        Node *node = NwkTree::get_new();
        char ch = this->get_buf();
        std::string tmp;
        bool colon_flag = false;
        if(ch==')')return nullptr;
        while(ch){
            if(ch == '('){
                Node *clade = this->dfs_file();
                if(!node->get_son())node->set_son(clade);
                else node->get_son()->add_clade_at_tail(clade);
//                cout<<"node name: "<<node->get_name()<<endl;
//                cout<<"clade name: "<<clade->get_name()<<endl;
//                if(clade->get_name()!="" || abs(clade->get_len()-0.0)>1e-6)
            }
            else if(ch == ')' || ch == ';'){
                if(colon_flag) node->set_len(stod(tmp));
                else node->set_name(tmp);
                return node;
            }
            else if(ch == ','){
                if(colon_flag) node->set_len(stod(tmp));
                else node->set_name(tmp);
                this->putback_buf('(');
                return node;
            }
            else if(ch == ':'){
                colon_flag = true;
                node->set_name(tmp);
                tmp = "";
            }
            else{
                tmp += ch;
            }
            ch = this->get_buf();
        }
        return node;
    }
    void build_tree(){
        root = dfs_file();
        if(root->get_name().empty())root->set_name("root");
        infile.close();
    }
    void print_tree(Node *node = nullptr){
        if(!node)node = this->root;
        Node *nex;
        for(nex = node->get_son();nex;nex = nex->get_nex()){
            printf("%s -> %s, length = %.5lf\n",node->get_name().c_str(), nex->get_name().c_str(), nex->get_len());
            print_tree(nex);
        }
    }
};