/*
L programming language was under the MIT license.
copyright(c) 2021 nu11ptr team.
*/
#ifndef PARSE_MODULE
#define PARSE_MODULE
#include"./var.h"//if you don't wan't to use default variable library,please replace it with the following.
//#include<vector>
//#include<map>
namespace L{
    typedef struct L_base{
        std::vector<std::string> split_arg(const std::string& p) const{
            std::vector<std::string> ret(0);
            std::string temp="";
            for(size_t i=0,j=0,a=0;i<p.length();i++){
                if(p[i]=='"'&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=1;else if(a==1)a=0;}
                if(p[i]=='\''&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=2;else if(a==2)a=0;}
                if((p[i]=='('||p[i]=='{'||p[i]=='[')&&a==0)j++;else if((p[i]==')'||p[i]=='}'||p[i]==']')&&a==0)j--;
                if(p[i]==','&&j==0&&a==0)ret.push_back(temp),temp="";else temp+=p[i];
            }
            if(temp!="")ret.push_back(temp);
            return ret;
        }
        std::string arg2str(const std::vector<std::string>& arg) const{
            std::string x;
            for(size_t i=0;i<arg.size();i++){
                if(i+1==arg.size())x+=arg[i];else x+=arg[i]+",";
            }
            return x;
        }
        std::string name;
        std::vector<std::string> args;
        std::string toString() const{
            return name+(args.empty()?"":" ")+arg2str(args);
        }
        L_base(){}
        L_base(const std::string& x){
            std::string p;
            if(x==""||x[0]=='#')return;
            for(size_t i=0,a=0;i<x.length();i++){
                if(i==0)while(x[i]==' ')i++;
                if(x[i]=='"'&&(x[i-1]!='\\'||x[i-2]=='\\')){if(a==0)a=1;else if(a==1)a=0;}
                if(x[i]=='\''&&(x[i-1]!='\\'||x[i-2]=='\\')){if(a==0)a=2;else if(a==2)a=0;}
                if(x[i]=='#'&&a==0){
                    while(x[i]!='\n')i++;
                }else if(x[i]!='\n'&&x[i]!='\t')p+=x[i];
            }
            if(p==""||p[0]=='#')return;
            size_t i=0;
            for(size_t j=0,a=0;i<p.length();i++){
                if(p[i]=='"'&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=1;else if(a==1)a=0;}
                if(p[i]=='\''&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=2;else if(a==2)a=0;}
                if((p[i]=='('||p[i]=='{'||p[i]=='[')&&a==0)j++;else if((p[i]==')'||p[i]=='}'||p[i]==']')&&a==0)j--;
                if(p[i]==' '&&j==0&&a==0)break;else name+=p[i];
            }
            if(i==p.length())return;
            args=split_arg(p.substr(i+1));
        }
        bool operator==(const L_base& x) const{
            if(x.name==name&&x.args==args)return true;
            return false;
        }
        bool operator!=(const L_base& x) const{
            return !operator==(x);
        }
    } L_base;
    typedef struct L:public L_base{
        typedef enum Return_Type{
            Expression_Calc_Value=0,
            Function_Return_Value=1,
            Throw_Return_Value=2,
        } Return_Type;
        typedef struct Return_Value{
            Return_Type tp;
            L_base base;
            Variable::var value;
            Variable::var scope;
            Variable::var all_scope;
            Variable::var this_scope;
            Return_Value(){tp=Expression_Calc_Value;}
            Return_Value(L_base b,Return_Type t,Variable::var v,Variable::var l,Variable::var z,Variable::var q){base=b;tp=t;value=v;scope=l;all_scope=z;this_scope=q;}
        } Return_Value;
        Return_Value eval(Variable::var scope){
            scope.isConst=false;
            scope.tp=Variable::var_tp::Object;
            return eval(scope,scope,scope);
        }
        L(){}
        L(const std::string& x){
            std::string p;
            if(x==""||x[0]=='#')return;
            for(size_t i=0,a=0;i<x.length();i++){
                if(i==0)while(x[i]==' ')i++;
                if(x[i]=='"'&&(x[i-1]!='\\'||x[i-2]=='\\')){if(a==0)a=1;else if(a==1)a=0;}
                if(x[i]=='\''&&(x[i-1]!='\\'||x[i-2]=='\\')){if(a==0)a=2;else if(a==2)a=0;}
                if(x[i]=='#'&&a==0){
                    while(x[i]!='\n')i++;
                }else if(x[i]!='\n'&&x[i]!='\t')p+=x[i];
            }
            if(p==""||p[0]=='#')return;
            size_t i=0;
            for(size_t j=0,a=0;i<p.length();i++){
                if(p[i]=='"'&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=1;else if(a==1)a=0;}
                if(p[i]=='\''&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=2;else if(a==2)a=0;}
                if((p[i]=='('||p[i]=='{'||p[i]=='[')&&a==0)j++;else if((p[i]==')'||p[i]=='}'||p[i]==']')&&a==0)j--;
                if(p[i]==' '&&j==0&&a==0)break;else name+=p[i];
            }
            if(i==p.length())return;
            args=split_arg(p.substr(i+1));
        }
        private:
        typedef enum Object_Type{
            is_pointer=0,
            is_const_value=1,
            is_native_function=2,
        } Object_Type;
        typedef enum Object_Errors{
            member_cant_visit=0,
            member_not_exist=1,//only use for null,int,function,expression,array,boolean and native function/members.
        } Object_Errors;
        typedef class Return_Object{
            Variable::var* value;
            Variable::var const_value;
            Variable::var* parent;
            Variable::var const_parent;
            Variable::var* this_object;
            bool isConst;
            public:
            bool getIsConst(){return isConst;}
            Variable::var& getValue(){return *value;}
            Variable::var& getParent(){return *parent;}
            Variable::var getConstValue(){return const_value;}
            Variable::var getConstParent(){return const_parent;}
            Variable::var& getThis(){return *this_object;}
            Object_Type tp;
            Return_Object(){
                tp=is_const_value;
                value=nullptr;
                isConst=true;
                parent=nullptr;
            }
            Return_Object(Variable::var x,Variable::var* p,Variable::var* s){
                tp=is_const_value;
                const_value=x;
                isConst=true;
                parent=p;
                if(parent!=nullptr)const_parent=*p;
                this_object=s;
            }
            Return_Object(Variable::var x,Variable::var p,Variable::var* s){
                tp=is_const_value;
                const_value=x;
                isConst=true;
                const_parent=p;
                this_object=s;
            }
            Return_Object(Variable::var* x,Variable::var* p,Variable::var* s){
                tp=is_pointer;
                value=x;
                const_value=*x;
                isConst=false;
                parent=p;
                if(parent!=nullptr)const_parent=*p;
                this_object=s;
            }
            Return_Object(std::nullptr_t x,Variable::var w,Variable::var p,Variable::var* s){
                tp=is_native_function;
                const_value=w;
                isConst=true;
                const_parent=p;
                parent=nullptr;
                this_object=s;
            }
            Return_Object(std::nullptr_t x,Variable::var w,Variable::var* p,Variable::var* s){
                tp=is_native_function;
                const_value=w;
                isConst=false;
                parent=p;
                if(parent!=nullptr)const_parent=*p;
                this_object=s;
            }
        } Return_Object;
        std::vector<std::string> get_name_split(const std::string& p) const{
            std::vector<std::string> visit;
            std::string temp;
            for(size_t i=0,a=0,j=0;i<p.length();i++){
                if(p[i]=='"'&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=1;else if(a==1)a=0;}
                if(p[i]=='\''&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=2;else if(a==2)a=0;}
                if((p[i]=='('||p[i]=='{'||p[i]=='[')&&a==0)j++;else if((p[i]==')'||p[i]=='}'||p[i]==']')&&a==0)j--;
                if(p[i]=='['&&a==0&&j==1){
                    if(temp!="")visit.push_back(temp),temp="";
                }else if(p[i]==']'&&a==0&&j==0){
                    if(temp!="")visit.push_back(temp),temp="";
                }else if(p[i]=='.'&&j==0&&a==0){
                    i++;
                    while(p[i]!='['&&p[i]!='.'&&i<p.length()){
                        temp+=p[i];
                        i++;
                    }
                    visit.push_back("_"+temp+"_");
                    temp="";
                    i--;
                }else temp+=p[i];
            }
            if(temp!="")visit.push_back(temp);
            return visit;
        }
        bool isIdentifier(const std::string& x) const{
            if(x[0]=='_')return false;
            if(x=="arguments")return false;
            bool flag=false;
            for(size_t i=0;i<x.length();i++){
                if(i==1)flag=true;
                if(x[i]>='0'&&x[i]<='9'){
                    if(!flag)return false;
                }else if((x[i]>='a'&&x[i]<='z')||x[i]=='_');
                else return false;
            }
            return true;
        }
        std::string get_first_name(const std::string& p) const{
            std::string temp;
            for(size_t i=0,a=0,j=0;i<p.length();i++){
                if(p[i]=='"'&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=1;else if(a==1)a=0;}
                if(p[i]=='\''&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=2;else if(a==2)a=0;}
                if((p[i]=='('||p[i]=='{'||p[i]=='[')&&a==0)j++;else if((p[i]==')'||p[i]=='}'||p[i]==']')&&a==0)j--;
                if(p[i]=='.'&&a==0&&j==0)break;
                if(p[i]=='['&&a==0&&j==1){
                    if(temp==""&&(i<=0||p[i-1]!=']'))temp+=p[i];
                    else break;
                }else temp+=p[i];
            }
            return temp;
        }
        bool isKeyword(const std::string& x) const{
            static std::vector<std::string> keyword={
                "mov",
                "var",
                "if",
                "switch",
                "while",
                "for",
                "break",
                "continue",
                "call",
                "throw",
                "catch",
                "default",
                "const",
            };
            for(size_t i=0;i<keyword.size();i++){
                if(keyword[i]==x)return true;
            }
            return false;
        }
        Return_Object get_var_index(const std::string& p,Return_Object object,Variable::var& scope,Variable::var& all_scope,Variable::var& this_scope,const size_t& count_dont_parse=0,const bool& nonewobject=false,const bool& startwiththis=false) const{
            std::vector<std::string> visit=get_name_split(p);
            Object_Type fin=object.tp;
            Variable::var* now_object=&object.getValue();
            Variable::var now_const_object=object.getConstValue();
            Variable::var* parent_object=nullptr;
            Variable::var parent_const_object=nullptr;
            Variable::var* this_object=&this_scope;
            bool dont_return_nonconst=startwiththis;
            bool this_keep=startwiththis;
            bool isConst=false;//only for is_native_function
            for(size_t i=0;i<visit.size()-count_dont_parse;i++){
                if(visit[i][0]=='_'&&visit[i][visit[i].length()-1]=='_'){//native
                    if(visit[i]=="_type_"){
                        switch(fin){
                            case is_pointer:{
                                now_const_object=Variable::var(Variable::getTypeStr(now_object->tp));
                                break;
                            }
                            case is_const_value:{
                                now_const_object=Variable::var(Variable::getTypeStr(now_const_object.tp));
                                break;
                            }
                            case is_native_function:{
                                now_const_object=Variable::var(Variable::getTypeStr(Variable::var_tp::Function));
                                break;
                            }
                        }
                        fin=is_const_value;
                    }else if(visit[i]=="_keys_"){
                        now_const_object.ArrayValue.clear();
                        switch(fin){
                            case is_pointer:{
                                if(now_object->tp!=Variable::var_tp::Object)throw member_not_exist;
                                for(std::map<std::string,Variable::var>::iterator x=now_object->ObjectValue.begin();x!=now_object->ObjectValue.end();x++)if(x->first[0]!='_')now_const_object.ArrayValue.push_back(Variable::var(x->first,false));
                                break;
                            }
                            case is_const_value:{
                                if(now_const_object.tp!=Variable::var_tp::Object)throw member_not_exist;
                                for(std::map<std::string,Variable::var>::iterator x=now_const_object.ObjectValue.begin();x!=now_const_object.ObjectValue.end();x++)if(x->first[0]!='_')now_const_object.ArrayValue.push_back(Variable::var(x->first,false));
                                break;
                            }
                            case is_native_function:{
                                throw member_not_exist;
                            }
                        }
                        now_const_object.tp=Variable::var_tp::Array;
                        fin=is_const_value;
                    }else if(visit[i]=="_length_"){
                        switch(fin){
                            case is_pointer:{
                                if(now_object->tp!=Variable::var_tp::Array&&now_object->tp!=Variable::var_tp::String)throw member_not_exist;
                                if(now_object->tp==Variable::var_tp::Array){
                                    now_const_object=Variable::var((int)now_object->ArrayValue.size());
                                }else{
                                    now_const_object=Variable::var((int)now_object->StringValue.length());
                                }
                                break;
                            }
                            case is_const_value:{
                                if(now_const_object.tp!=Variable::var_tp::Array&&now_const_object.tp!=Variable::var_tp::String)throw member_not_exist;
                                if(now_const_object.tp==Variable::var_tp::Array){
                                    now_const_object=Variable::var((int)now_const_object.ArrayValue.size());
                                }else{
                                    now_const_object=Variable::var((int)now_const_object.StringValue.length());
                                }
                                break;
                            }
                            case is_native_function:{
                                throw member_not_exist;
                            }
                        }
                        fin=is_const_value;
                    }else if(visit[i]=="_isConst_"){
                        switch(fin){
                            case is_pointer:{
                                now_const_object=Variable::var(now_object->isConst);
                                break;
                            }
                            case is_const_value:{
                                now_const_object=now_const_object.isConst;//Variable::var(true);
                                break;
                            }
                            case is_native_function:{
                                now_const_object=Variable::var(true);
                                break;
                            }
                        }
                        fin=is_const_value;
                    }else if(visit[i]=="_eval_"){
                        switch(fin){
                            case is_pointer:{
                                if(now_object->tp!=Variable::var_tp::String)throw member_not_exist;
                                parent_object=now_object;
                                isConst=false;
                                break;
                            }
                            case is_const_value:{
                                if(now_const_object.tp!=Variable::var_tp::String)throw member_not_exist;
                                parent_const_object=now_const_object;
                                isConst=true;
                                break;
                            }
                            case is_native_function:{
                                throw member_not_exist;
                            }
                        }
                        now_const_object=Variable::parse("(default eval)");
                        fin=is_native_function;
                    }else if(visit[i]=="_parse_"){
                        switch(fin){
                            case is_pointer:{
                                if(now_object->tp!=Variable::var_tp::String)throw member_not_exist;
                                parent_object=now_object;
                                isConst=false;
                                break;
                            }
                            case is_const_value:{
                                if(now_const_object.tp!=Variable::var_tp::String)throw member_not_exist;
                                parent_const_object=now_const_object;
                                isConst=true;
                                break;
                            }
                            case is_native_function:{
                                throw member_not_exist;
                            }
                        }
                        now_const_object=Variable::parse("(default parse)");
                        fin=is_native_function;
                    }else if(visit[i]=="_join_"){
                        switch(fin){
                            case is_pointer:{
                                if(now_object->tp!=Variable::var_tp::Array)throw member_not_exist;
                                parent_object=now_object;
                                isConst=false;
                                break;
                            }
                            case is_const_value:{
                                if(now_const_object.tp!=Variable::var_tp::Array)throw member_not_exist;
                                parent_const_object=now_const_object;
                                isConst=true;
                                break;
                            }
                            case is_native_function:{
                                throw member_not_exist;
                            }
                        }
                        now_const_object=Variable::parse("(default join)");
                        fin=is_native_function;
                    }else if(visit[i]=="_pop_"){
                        switch(fin){
                            case is_pointer:{
                                if(now_object->tp!=Variable::var_tp::Array)throw member_not_exist;
                                parent_object=now_object;
                                isConst=false;
                                now_const_object=Variable::parse("(default pop)");
                                break;
                            }
                            case is_const_value:
                            case is_native_function:{
                                throw member_not_exist;
                            }
                        }
                        fin=is_native_function;
                    }else if(visit[i]=="_push_"){
                        switch(fin){
                            case is_pointer:{
                                if(now_object->tp!=Variable::var_tp::Array)throw member_not_exist;
                                parent_object=now_object;
                                isConst=false;
                                now_const_object=Variable::parse("(default push)");
                                break;
                            }
                            case is_const_value:
                            case is_native_function:{
                                throw member_not_exist;
                            }
                        }
                        fin=is_native_function;
                    }else if(visit[i]=="_insert_"){
                        switch(fin){
                            case is_pointer:{
                                if(now_object->tp!=Variable::var_tp::Array)throw member_not_exist;
                                parent_object=now_object;
                                isConst=false;
                                now_const_object=Variable::parse("(default insert)");
                                break;
                            }
                            case is_const_value:
                            case is_native_function:{
                                throw member_not_exist;
                            }
                        }
                        fin=is_native_function;
                    }else if(visit[i]=="_resize_"){
                        switch(fin){
                            case is_pointer:{
                                if(now_object->tp!=Variable::var_tp::Array)throw member_not_exist;
                                parent_object=now_object;
                                isConst=false;
                                now_const_object=Variable::parse("(default resize)");
                                break;
                            }
                            case is_const_value:
                            case is_native_function:{
                                throw member_not_exist;
                            }
                        }
                        fin=is_native_function;
                    }else if(visit[i]=="_convert_"){
                        switch(fin){
                            case is_pointer:{
                                parent_const_object=*now_object;
                                break;
                            }
                            case is_native_function:
                            case is_const_value:{
                                parent_const_object=now_const_object;
                                break;
                            }
                        }
                        isConst=true;
                        fin=is_native_function;
                        now_const_object=Variable::parse("(default convert)");
                    }else if(visit[i]=="_toString_"){
                        switch(fin){
                            case is_pointer:{
                                parent_const_object=*now_object;
                                break;
                            }
                            case is_native_function:
                            case is_const_value:{
                                parent_const_object=now_const_object;
                                break;
                            }
                        }
                        isConst=true;
                        fin=is_native_function;
                        now_const_object=Variable::parse("(default toString)");
                    }else throw member_not_exist;
                    continue;
                }else{//normal member
                    Variable::var visit_temp=exp_calc(Variable::parse(visit[i]),scope,all_scope,*this_object);
                    std::string find_str;
                    if(visit_temp.tp==Variable::var_tp::String)find_str=visit_temp.StringValue;
                    else find_str=visit_temp.toString_nonconst();
                    if(find_str[0]=='_'&&!this_keep)throw member_cant_visit;
                    if(find_str=="this"){
                        fin=is_pointer;
                        dont_return_nonconst=true;
                        now_object=this_object;
                        continue;
                    }else{
                        this_keep=false;
                        dont_return_nonconst=false;
                    }
                    switch(fin){
                        case is_pointer:{
                            this_object=now_object;
                            switch(now_object->tp){
                                case Variable::var_tp::String:{
                                    if(visit_temp.tp!=Variable::var_tp::Int){
                                        now_const_object=Variable::var();
                                    }else{
                                        now_const_object=Variable::var(std::string(1,now_object->StringValue[(size_t)visit_temp.IntValue]));
                                    }
                                    fin=is_const_value;
                                    break;
                                }
                                case Variable::var_tp::Object:{
                                    if(now_object->ObjectValue.find(find_str)==now_object->ObjectValue.end()){
                                        if(!nonewobject)now_object->ObjectValue[find_str]=Variable::var(nullptr,false);
                                        else throw member_not_exist;
                                    }
                                    now_object=&now_object->ObjectValue[find_str];
                                    break;
                                }
                                case Variable::var_tp::Array:{
                                    try{
                                        visit_temp=visit_temp.convert(Variable::var_tp::Int);
                                        if((size_t)visit_temp.IntValue>=now_object->ArrayValue.size()){
                                            if(!nonewobject){
                                                now_object->ArrayValue.resize((size_t)visit_temp.IntValue+1);
                                                for(size_t i=0;i<now_object->ArrayValue.size();i++)now_object->ArrayValue[i].isConst=false;
                                            }else throw member_not_exist;
                                        }
                                        now_object=&now_object->ArrayValue[(size_t)visit_temp.IntValue];
                                    }catch(...){
                                        fin=is_const_value;
                                        now_const_object=Variable::var();
                                    }
                                    break;
                                }
                                default:throw member_not_exist;
                            }
                            break;
                        }
                        case is_const_value:{
                            switch(now_const_object.tp){
                                case Variable::var_tp::String:{
                                    try{
                                        visit_temp=visit_temp.convert(Variable::var_tp::Int);
                                        if(now_const_object.StringValue.size()>=(size_t)visit_temp.IntValue)throw 0;
                                        now_const_object=Variable::var(std::string(1,now_const_object.StringValue[(size_t)visit_temp.IntValue]));
                                    }catch(...){
                                        now_const_object=Variable::var();
                                    }
                                    break;
                                }
                                case Variable::var_tp::Object:{
                                    if(now_const_object.ObjectValue.find(find_str)==now_const_object.ObjectValue.end())throw member_not_exist;
                                    now_const_object=now_const_object.ObjectValue[find_str];
                                    break;
                                }
                                case Variable::var_tp::Array:{
                                    try{
                                        visit_temp=visit_temp.convert(Variable::var_tp::Int);
                                        if((size_t)visit_temp.IntValue>=now_const_object.ArrayValue.size())throw 0;
                                        now_const_object=now_const_object.ArrayValue[(size_t)visit_temp.IntValue];
                                    }catch(...){
                                        now_const_object=Variable::var();
                                    }
                                    break;
                                }
                                default:throw member_not_exist;
                            }
                            break;
                        }
                        case is_native_function:{
                            throw member_not_exist;
                        }
                    }
                    dont_return_nonconst=false;
                }
            }
            switch(fin){
                case is_const_value:{
                    return Return_Object(now_const_object,parent_const_object,this_object);
                }
                case is_native_function:{
                    if(isConst)return Return_Object(nullptr,now_const_object,parent_const_object,this_object);else return Return_Object(nullptr,now_const_object,parent_object,this_object);
                }
                case is_pointer:{
                    if(!dont_return_nonconst)return Return_Object(now_object,parent_object,this_object);else return Return_Object(*now_object,parent_const_object,this_object);
                }
            }
            return Return_Object();
        }
        Return_Object get_object(const std::string& n,Variable::var& scope,Variable::var& all_scope,Variable::var& this_scope,const size_t& count_dont_parse=0,const bool& nonewobject=false) const{
            if(get_first_name(n)==n&&count_dont_parse==1){
                return Return_Object(&scope,&this_scope,&this_scope);
            }else if(get_first_name(n)==n){
                if(n=="this"){
                    return Return_Object(this_scope,&this_scope,&this_scope);
                }else if(scope.ObjectValue.find(n)!=scope.ObjectValue.end()){
                    return Return_Object(&scope.ObjectValue[n],&this_scope,&this_scope);
                }else if(all_scope.ObjectValue.find(n)!=all_scope.ObjectValue.end()){
                    return Return_Object(&all_scope.ObjectValue[n],&this_scope,&this_scope);
                }else{
                    try{
                        if(Variable::parse(n).tp==Variable::var_tp::Expression)throw 0;
                        return Return_Object(exp_calc(Variable::parse(n),scope,all_scope,this_scope),&scope,&scope);
                    }catch(...){
                        throw member_not_exist;
                    }
                }
            }else{
                std::string fst_nme=get_first_name(n);
                Variable::var tmp_obj;
                if(fst_nme=="this"){
                    return get_var_index(n.substr(fst_nme.length()),Return_Object(&this_scope,&this_scope,&this_scope),scope,all_scope,this_scope,count_dont_parse,nonewobject,true);
                }else if(scope.ObjectValue.find(fst_nme)!=scope.ObjectValue.end()){
                    return get_var_index(n.substr(fst_nme.length()),Return_Object(&scope.ObjectValue[fst_nme],&scope,&scope),scope,all_scope,this_scope,count_dont_parse,nonewobject);
                }else if(all_scope.ObjectValue.find(fst_nme)!=all_scope.ObjectValue.end()){
                    return get_var_index(n.substr(fst_nme.length()),Return_Object(&all_scope.ObjectValue[fst_nme],&all_scope,&all_scope),scope,all_scope,this_scope,count_dont_parse,nonewobject);
                }else return get_var_index(n.substr(fst_nme.length()),Return_Object(exp_calc(Variable::parse(fst_nme),scope,all_scope,this_scope),&scope,&scope),scope,all_scope,this_scope,count_dont_parse,nonewobject);
            }
        }
        Variable::var exp_calc(const Variable::var& exp,Variable::var scope,Variable::var all_scope,Variable::var this_scope) const{
            std::vector<std::string> p=exp.ExpressionValue;
            std::vector<Variable::var> st;
            std::string op;
            Variable::var res;
            if(exp.tp!=Variable::var_tp::Expression){
                if(exp.tp==Variable::var_tp::Object||exp.tp==Variable::var_tp::Array){
                        switch((size_t)exp.tp){
                            case Variable::var_tp::Object:{
                                std::map<std::string,Variable::var> x;
                                bool flag=false;
                                for(std::map<std::string,Variable::var>::const_iterator i=exp.ObjectValue.cbegin();i!=exp.ObjectValue.cend();i++){
                                    flag=i->second.isConst;
                                    x[i->first]=exp_calc(i->second,scope,all_scope,this_scope);//calc values of the object
                                    x[i->first].isConst=flag;
                                }
                                return Variable::var(x);
                            }
                            case Variable::var_tp::Array:{
                                std::vector<Variable::var> x(exp.ArrayValue.size());
                                for(size_t i=0;i<exp.ArrayValue.size();i++){
                                    x[i]=exp_calc(exp.ArrayValue[i],scope,all_scope,this_scope);//calc members of the array
                                    x[i].isConst=false;
                                }
                                return Variable::var(x);
                            }
                        }
                }else return exp;
            }//return var(exp);
            if(exp.ExpressionValue.size()==1){
                Return_Object o=get_object(exp.ExpressionValue[0],scope,all_scope,this_scope,0,true);
                if(o.tp==is_const_value||o.tp==is_native_function)return o.getConstValue();
                else return o.getValue();
            }//is a variable
            for(std::vector<std::string>::const_reverse_iterator i=p.crbegin();i!=p.crend();i++){
                if(Variable::get_op_priority(*i)!=-1){//operator
                    op=(*i);
                    if(op=="+")res=Variable::var(st[st.size()-1]+st[st.size()-2]);
                    else if(op=="-")res=Variable::var(st[st.size()-1]-st[st.size()-2]);
                    else if(op=="*")res=Variable::var(st[st.size()-1]*st[st.size()-2]);
                    else if(op=="/")res=Variable::var(st[st.size()-1]/st[st.size()-2]);
                    else if(op=="%")res=Variable::var(st[st.size()-1]%st[st.size()-2]);
                    else if(op=="==")res=Variable::var(st[st.size()-1]==st[st.size()-2]);
                    else if(op=="<")res=Variable::var(st[st.size()-1]<st[st.size()-2]);
                    else if(op==">")res=Variable::var(st[st.size()-1]>st[st.size()-2]);
                    else if(op=="<=")res=Variable::var(st[st.size()-1]<=st[st.size()-2]);
                    else if(op==">=")res=Variable::var(st[st.size()-1]>=st[st.size()-2]);
                    else if(op=="!=")res=Variable::var(st[st.size()-1]!=st[st.size()-2]);
                    else if(op=="&&")res=Variable::var(st[st.size()-1]&&st[st.size()-2]);
                    else if(op=="||")res=Variable::var(st[st.size()-1]||st[st.size()-2]);
                    else if(op=="!")res=Variable::var(!st[st.size()-1]);
                    else if(op=="&")res=Variable::var(st[st.size()-1]&st[st.size()-2]);
                    else if(op=="|")res=Variable::var(st[st.size()-1]|st[st.size()-2]);
                    else if(op=="^")res=Variable::var(st[st.size()-1]^st[st.size()-2]);
                    else if(op=="~")res=Variable::var(~st[st.size()-1]);
                    else if(op==">>")res=st[st.size()-1].rightmove_signed(st[st.size()-2]);
                    else if(op==">>>")res=st[st.size()-1].rightmove_unsigned(st[st.size()-2]);
                    else if(op=="<<")res=st[st.size()-1].leftmove(st[st.size()-2]);
                    if(op!="!"&&op!="~")st.pop_back();
                    st.pop_back();
                    st.push_back(res);
                }else{
                    if((*i)[0]=='.'){
                        try{
                            std::string b_op=st[st.size()-1].toString();
                            if(b_op.substr(0,6)=="const<"&&b_op[b_op.length()-1]=='>')b_op=b_op.substr(0,b_op.length()-1)+(*i)+">";
                            else b_op+=(*i);
                            res=exp_calc(Variable::parse(b_op),scope,all_scope,this_scope);
                        }catch(...){
                            res=(Variable::var(nullptr,false));
                        }
                        st.pop_back();
                        st.push_back(res);
                    }else{
                        try{
                            st.push_back(exp_calc(Variable::parse(*i),scope,all_scope,this_scope));
                        }catch(...){
                            st.push_back(Variable::var(nullptr,false));
                        }
                    }
                }
            }
            return st[0];
        }
        Variable::var update_scope(Variable::var scope,Variable::var temp_scope){
            for(std::map<std::string,Variable::var>::const_iterator x=scope.ObjectValue.cbegin();x!=scope.ObjectValue.cend();x++){
                if(temp_scope.ObjectValue.find(x->first)==temp_scope.ObjectValue.end())scope.ObjectValue[x->first].remove();
                else scope.ObjectValue[x->first]=temp_scope.ObjectValue[x->first];
            }
            scope.update();
            return scope;
        }
        Return_Value eval(Variable::var& scope,Variable::var& all_scope,Variable::var& this_scope){
            if(name=="return"){
                if(args.size()==0)return Return_Value(*this,Function_Return_Value,Variable::var(nullptr),scope,all_scope,this_scope);
                if(args.size()>1)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                Variable::var x;
                try{
                    x=exp_calc(Variable::parse(args[0]),scope,all_scope,this_scope);
                }catch(...){
                    return Return_Value(*this,Throw_Return_Value,Variable::var("VariableError"),scope,all_scope,this_scope);
                }
                return Return_Value(*this,Function_Return_Value,x,scope,all_scope,this_scope);
            }else if(name=="var"){
                if(args.size()<1||args.size()>2)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                if(isKeyword(args[0])||!isIdentifier(args[0]))return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                try{
                    if(Variable::parse(args[0]).tp!=Variable::var_tp::Expression)throw 0;
                }catch(...){
                    return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                }
                if(scope.ObjectValue.find(args[0])!=scope.ObjectValue.end())return Return_Value(*this,Throw_Return_Value,Variable::var("VariableError"),scope,all_scope,this_scope);
                if(args.size()==2){
                    Variable::var tmp;
                    try{
                        tmp=exp_calc(Variable::parse(args[1]),scope,all_scope,this_scope);
                    }catch(...){
                        return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                    }
                    if(args[1].substr(0,6)!="const<")tmp.isConst=false;
                    scope.ObjectValue[args[0]]=tmp;
                }else scope.ObjectValue[args[0]]=Variable::var(nullptr,false);
                return Return_Value(*this,Expression_Calc_Value,Variable::var(nullptr),scope,all_scope,this_scope);
            }else if(name=="mov"){
                if(args.size()!=2)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                Return_Object y;
                Variable::var x;
                try{
                    y=get_object(args[0],scope,all_scope,this_scope);
                    if(y.tp!=is_pointer||y.getValue().isConst==true)throw 0;
                }catch(...){
                    return Return_Value(*this,Throw_Return_Value,Variable::var("VariableError"),scope,all_scope,this_scope);
                }
                try{
                    x=exp_calc(Variable::parse(args[1]),scope,all_scope,this_scope);
                }catch(...){
                    return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                }
                x.isConst=false;
                y.getValue()=x;
                return Return_Value(*this,Expression_Calc_Value,x,scope,all_scope,this_scope);
            }else if(name=="delete"){
                if(args.size()!=1)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                Return_Object y;
                try{
                    y=get_object(args[0],scope,all_scope,this_scope);
                    if(y.tp==is_const_value||y.getValue().isConst)throw 0;
                }catch(...){
                    return Return_Value(*this,Expression_Calc_Value,Variable::var(false,false),scope,all_scope,this_scope);
                }
                y.getValue().remove();
                get_object(args[0],scope,all_scope,this_scope,1).getValue().update();
                return Return_Value(*this,Expression_Calc_Value,Variable::var(true,false),scope,all_scope,this_scope);
            }else if(name=="if"){
                if(args.size()<2||args.size()>3)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                Variable::var exp;
                Variable::var fn;
                Variable::var temp_scope=scope;
                Return_Value res;
                try{
                    exp=exp_calc(Variable::parse(args[0]),scope,all_scope,this_scope);
                }catch(...){
                    return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                }
                if(Variable::var(true)==exp){
                    try{
                        fn=exp_calc(Variable::parse(args[1]),scope,all_scope,this_scope);
                        if(fn.tp!=Variable::var_tp::Function)throw 0;
                    }catch(...){
                        return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                    }
                    for(size_t i=0;i<fn.FunctionValue.value.size();i++){
                        res=L(fn.FunctionValue.value[i]).eval(temp_scope,all_scope,this_scope);
                        if(res.tp!=Expression_Calc_Value){
                            res.scope=scope;
                            return res;
                        }
                        scope=update_scope(scope,temp_scope);
                    }
                }else if(args.size()==3){
                    try{
                        fn=exp_calc(Variable::parse(args[2]),scope,all_scope,this_scope);
                        if(fn.tp!=Variable::var_tp::Function)throw 0;
                    }catch(...){
                        return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                    }
                    for(size_t i=0;i<fn.FunctionValue.value.size();i++){
                        res=L(fn.FunctionValue.value[i]).eval(temp_scope,all_scope,this_scope);
                        if(res.tp!=Expression_Calc_Value){
                            res.scope=scope;
                            return res;
                        }
                        scope=update_scope(scope,temp_scope);
                    }
                }
                return Return_Value(*this,Expression_Calc_Value,Variable::var(Variable::var(true)==exp),scope,all_scope,this_scope);
            }else if(name=="switch"){
                if(args.size()<2)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                Variable::var x;
                Variable::var exp;
                Variable::var func;
                bool break_flag=false;
                for(size_t i=0;i<args.size();i++){
                    if(i+1!=args.size()){
                        try{
                            x=exp_calc(Variable::parse(args[i]),scope,all_scope,this_scope);
                            if(x.tp!=Variable::var_tp::Array||x.ArrayValue.size()!=2)throw 0;
                            exp=exp_calc(x.ArrayValue[0],scope,all_scope,this_scope);
                            func=exp_calc(x.ArrayValue[1],scope,all_scope,this_scope);
                            if(func.tp!=Variable::var_tp::Function)throw 0;
                        }catch(...){
                            return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                        }
                        if(exp==1){
                            Variable::var temp_scope=scope;
                            Return_Value res;
                            for(size_t i=0;i<func.FunctionValue.value.size();i++){
                                if(L(func.FunctionValue.value[i]).name=="break"){
                                    break_flag=1;
                                    break;
                                }else res=L(func.FunctionValue.value[i]).eval(temp_scope,all_scope,this_scope);
                                if(res.tp!=Expression_Calc_Value){
                                    res.scope=scope;
                                    return res;
                                }
                                scope=update_scope(scope,temp_scope);
                            }
                        }
                    }else{
                        try{
                            func=exp_calc(Variable::parse(args[i]),scope,all_scope,this_scope);
                            if(func.tp!=Variable::var_tp::Function)throw 0;
                        }catch(...){
                            return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                        }
                        Variable::var temp_scope=scope;
                        Return_Value res;
                        for(size_t i=0;i<func.FunctionValue.value.size();i++){
                            if(L(func.FunctionValue.value[i]).name=="break"){
                                break_flag=1;
                                break;
                            }
                            else res=L(func.FunctionValue.value[i]).eval(temp_scope,all_scope,this_scope);
                            if(res.tp!=Expression_Calc_Value){
                                res.scope=scope;
                                return res;
                            }
                            scope=update_scope(scope,temp_scope);
                        }
                    }
                    if(break_flag)break;
                }
                return Return_Value(*this,Expression_Calc_Value,Variable::var(nullptr),scope,all_scope,this_scope);
            }else if(name=="while"){
                if(args.size()!=2)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                Variable::var exp;
                Variable::var func;
                try{
                    exp=Variable::parse(args[0]);
                    func=exp_calc(Variable::parse(args[1]),scope,all_scope,this_scope);
                    if(func.tp!=Variable::var_tp::Function)throw 0;
                }catch(...){
                    return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                }
                while(true){
                    Variable::var exp_res;
                    try{
                        exp_res=exp_calc(exp,scope,all_scope,this_scope);
                    }catch(...){
                        return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                    }
                    if(Variable::var(true)==exp_res){
                        Variable::var temp_scope=scope;
                        Return_Value res;
                        bool break_flag=0;
                        for(size_t i=0;i<func.FunctionValue.value.size();i++){
                            L l=L(func.FunctionValue.value[i]);
                            if(l.name=="continue"){
                                break;
                            }else if(l.name=="break"){
                                break_flag=1;
                                break;
                            }else res=l.eval(temp_scope,all_scope,this_scope);
                            if(res.tp!=Expression_Calc_Value){
                                res.scope=scope;
                                return res;
                            }
                            scope=update_scope(scope,temp_scope);
                        }
                        if(break_flag)break;
                    }else break;
                }
                return Return_Value(*this,Expression_Calc_Value,Variable::var(nullptr),scope,all_scope,this_scope);
            }else if(name=="for"){
                if(args.size()!=2)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                Variable::var start;
                Variable::var exp;
                Variable::var routine;
                Variable::var func;
                Variable::var temp_scope;
                Return_Value res;
                Variable::var exp_res;
                bool flag=false;
                bool break_flag=false;
                try{
                    Variable::var x=Variable::parse(args[0]);
                    func=exp_calc(Variable::parse(args[1]),scope,all_scope,this_scope);
                    if(x.tp!=Variable::var_tp::Array||x.ArrayValue.size()!=3||func.tp!=Variable::var_tp::Function)throw 0;
                    start=x.ArrayValue[0];
                    exp=x.ArrayValue[1];
                    routine=x.ArrayValue[2];
                    if(start.tp!=Variable::var_tp::Function||exp.tp!=Variable::var_tp::Expression||routine.tp!=Variable::var_tp::Function)throw 0;
                }catch(...){
                    return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                }
                temp_scope=scope;
                while(true){
                    if(flag){
                        for(size_t i=0;i<routine.FunctionValue.value.size();i++){
                            res=L(routine.FunctionValue.value[i]).eval(temp_scope,all_scope,this_scope);
                            if(res.tp!=Expression_Calc_Value){
                                res.scope=scope;
                                return res;
                            }
                        }
                    }else{
                        for(size_t i=0;i<start.FunctionValue.value.size();i++){
                            res=L(start.FunctionValue.value[i]).eval(temp_scope,all_scope,this_scope);
                            if(res.tp!=Expression_Calc_Value){
                                res.scope=scope;
                                return res;
                            }
                        }
                        flag=1;
                    }
                    try{
                        exp_res=exp_calc(exp,temp_scope,all_scope,this_scope);
                    }catch(...){
                        return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                    }
                    if(Variable::var(true)==exp_res){
                        for(size_t i=0;i<func.FunctionValue.value.size();i++){
                            L l=L(func.FunctionValue.value[i]);
                            if(l.name=="continue"){
                                break;
                            }else if(l.name=="break"){
                                break_flag=1;
                                break;
                            }else res=l.eval(temp_scope,all_scope,this_scope);
                            if(res.tp!=Expression_Calc_Value){
                                res.scope=scope;
                                return res;
                            }
                            scope=update_scope(scope,temp_scope);
                        }
                        if(break_flag)break;
                    }else break;
                }
                return Return_Value(*this,Expression_Calc_Value,Variable::var(nullptr),scope,all_scope,this_scope);
            }else if(name=="call"){
                if(args.size()<1||args.size()>3)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                Return_Object fn_native;
                Variable::var func;
                Variable::var* parent;
                bool is_native=false;
                try{
                    func=exp_calc(Variable::parse(args[0]),scope,all_scope,this_scope);
                    if(func.tp!=Variable::var_tp::Function)throw 0;
                    try{
                        parent=&get_object(args[0],scope,all_scope,this_scope,0,true).getThis();
                        if(parent->tp!=Variable::var_tp::Object)parent=&scope;
                    }catch(...){
                        parent=&scope;
                    }
                    if(func.FunctionValue.value[0].substr(0,8)=="default "){
                        try{
                            fn_native=get_object(args[0],scope,all_scope,this_scope);
                            if(fn_native.tp!=is_native_function)throw 0;
                            is_native=true;
                        }catch(...){
                            is_native=false;
                        }
                    }
                }catch(...){
                    return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                }
                if(is_native){
                    Variable::var ret;
                    std::string member=func.FunctionValue.value[0].substr(8);
                    if(member=="eval"){
                        Return_Value res;
                        res=L(fn_native.getConstParent().StringValue).eval(scope,all_scope,this_scope);
                        if(res.tp!=Expression_Calc_Value){
                            return res;
                        }else ret=res.value;
                    }if(member=="parse"){
                        try{
                            ret=exp_calc(fn_native.getConstParent().StringValue,scope,all_scope,this_scope);
                        }catch(...){
                            return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                        }
                    }if(member=="join"){
                        try{
                            if(args.size()<2)throw 0;
                            Variable::var sp;
                            ret.tp=Variable::var_tp::String;
                            sp=exp_calc(args[1],scope,all_scope,this_scope);
                            if(sp.tp!=Variable::var_tp::String)throw 0;
                            for(size_t i=0;i<fn_native.getConstParent().ArrayValue.size();i++){
                                if(i+1!=fn_native.getConstParent().ArrayValue.size())ret.StringValue+=fn_native.getConstParent().ArrayValue[i].toString_nonconst()+sp.StringValue;
                                else ret.StringValue+=fn_native.getConstParent().ArrayValue[i].toString_nonconst();
                            }
                        }catch(...){
                            return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                        }
                    }else if(member=="pop"){
                        if(args.size()<2)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                        try{
                            if(fn_native.getParent().ArrayValue.empty())throw 0;
                            fn_native.getParent().ArrayValue.pop_back();
                        }catch(...){
                            return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                        }
                    }else if(member=="push"){
                        if(args.size()<2)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                        try{
                            Variable::var obj=exp_calc(Variable::parse(args[1]),scope,all_scope,this_scope);
                            obj.isConst=false;
                            fn_native.getParent().ArrayValue.push_back(obj);
                        }catch(...){
                            return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                        }
                    }else if(member=="convert"){
                        if(args.size()<2)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                        try{
                            if(exp_calc(Variable::var(args[0]),scope,all_scope,this_scope).tp!=Variable::var_tp::String)throw 0;
                            ret=exp_calc(Variable::parse(fn_native.getConstParent().StringValue),scope,all_scope,this_scope).convert(Variable::getStrType(exp_calc(Variable::parse(args[1]),scope,all_scope,this_scope).StringValue));;
                        }catch(...){
                            return Return_Value(*this,Throw_Return_Value,Variable::var("VariableError"),scope,all_scope,this_scope);
                        }
                    }else if(member=="toString"){
                        ret=fn_native.getConstParent().toString_nonconst();
                    }else if(member=="insert"){
                        if(args.size()<2)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                        try{
                            Variable::var op=exp_calc(Variable::parse(args[1]),scope,all_scope,this_scope);
                            if(op.tp!=Variable::var_tp::Array||op.ArrayValue.size()!=2||op.ArrayValue[0].tp!=Variable::var_tp::Int)throw 0;
                            if(fn_native.getParent().ArrayValue.size()<=(size_t)op.ArrayValue[0].IntValue)fn_native.getParent().ArrayValue.resize((size_t)op.ArrayValue[0].IntValue+1);
                            op.ArrayValue[1].isConst=false;
                            fn_native.getParent().ArrayValue.insert(fn_native.getParent().ArrayValue.begin()+((size_t)op.ArrayValue[0].IntValue),op.ArrayValue[1]);
                        }catch(...){
                            return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                        }
                    }else if(member=="resize"){
                        if(args.size()<2)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                        try{
                            Variable::var op=exp_calc(Variable::parse(args[1]),scope,all_scope,this_scope);
                            if(op.tp!=Variable::var_tp::Int)throw 0;
                            fn_native.getParent().ArrayValue.resize((size_t)op.IntValue);
                            for(size_t i=0;i<fn_native.getParent().ArrayValue.size();i++)if(fn_native.getParent().ArrayValue[i].tp==Variable::var_tp::Null&&fn_native.getParent().ArrayValue[i].isConst)fn_native.getParent().ArrayValue[i].isConst=false;
                        }catch(...){
                            return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                        }
                    }
                    if(args.size()==3){
                        ret.isConst=false;
                        try{
                            Return_Object x=get_object(args[2],scope,all_scope,this_scope);
                            if(x.tp!=is_pointer||x.getValue().isConst==true)throw 0;
                            x.getValue()=ret;
                        }catch(...){
                            return Return_Value(*this,Throw_Return_Value,Variable::var("VariableError"),scope,all_scope,this_scope);
                        }
                    }
                    return Return_Value(*this,Expression_Calc_Value,ret,scope,all_scope,this_scope);
                }else{
                    Variable::var temp_scope;
                    temp_scope.isConst=false;
                    temp_scope.tp=Variable::var_tp::Object;
                    if(args.size()>=2){
                        try{
                            temp_scope.ObjectValue["arguments"]=exp_calc(Variable::parse(args[1]),scope,all_scope,this_scope);
                            temp_scope.ObjectValue["arguments"].isConst=true;
                        }catch(...){
                            return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                        }
                    }else temp_scope.ObjectValue["arguments"]=Variable::var();
                    for(size_t i=0;i<func.FunctionValue.value.size();i++){
                        Return_Value res;
                        res=L(func.FunctionValue.value[i]).eval(temp_scope,all_scope,*parent);
                        if(res.tp==Function_Return_Value){
                            if(args.size()==3){
                                res.value.isConst=false;
                                try{
                                    Return_Object x=get_object(args[2],scope,all_scope,this_scope);
                                    if(x.tp!=is_pointer||x.getValue().isConst==true)throw 0;
                                    x.getValue()=res.value;
                                }catch(...){
                                    return Return_Value(*this,Throw_Return_Value,Variable::var("VariableError"),scope,all_scope,this_scope);
                                }
                            }
                            return Return_Value(*this,Expression_Calc_Value,res.value,scope,all_scope,this_scope);
                        }else if(res.tp==Throw_Return_Value){
                            res.this_scope=this_scope;
                            res.scope=scope;
                            return res;
                        }
                    }
                }
                return Return_Value(*this,Expression_Calc_Value,Variable::var(nullptr),scope,all_scope,this_scope);
            }else if(name=="throw"){
                if(args.size()!=1)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                Variable::var x;
                try{
                    x=exp_calc(Variable::parse(args[0]),scope,all_scope,this_scope);
                }catch(...){
                    return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                }
                return Return_Value(*this,Throw_Return_Value,x,scope,all_scope,this_scope);
            }else if(name=="catch"){
                if(args.size()!=2)return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
                Variable::var hook;
                Variable::var fn;
                Variable::var temp_scope=scope;
                Return_Value res;
                try{
                    hook=exp_calc(Variable::parse(args[0]),scope,all_scope,this_scope);
                    fn=exp_calc(Variable::parse(args[1]),scope,all_scope,this_scope);
                    if(hook.tp!=Variable::var_tp::Function||fn.tp!=Variable::var_tp::Function)throw 0;
                }catch(...){
                    return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
                }
                for(size_t i=0;i<hook.FunctionValue.value.size();i++){
                    res=L(hook.FunctionValue.value[i]).eval(temp_scope,all_scope,this_scope);
                    if(res.tp==Throw_Return_Value){
                        temp_scope.ObjectValue["arguments"]=res.value;
                        temp_scope.isConst=true;
                        for(size_t i=0;i<fn.FunctionValue.value.size();i++){
                            res=L(fn.FunctionValue.value[i]).eval(temp_scope,all_scope,this_scope);
                            if(res.tp!=Expression_Calc_Value){
                                res.scope=scope;
                                return res;
                            }
                            scope=update_scope(scope,temp_scope);
                        }
                        return Return_Value(*this,Expression_Calc_Value,Variable::var(nullptr),scope,all_scope,this_scope);
                    }else if(res.tp==Function_Return_Value){
                        res.scope=scope;
                        return res;
                    }
                    scope=update_scope(scope,temp_scope);
                }
                return Return_Value(*this,Expression_Calc_Value,Variable::var(nullptr),scope,all_scope,this_scope);
            }else if(name=="break"||name=="continue"||name=="default"){
                return Return_Value(*this,Throw_Return_Value,Variable::var("EvalError"),scope,all_scope,this_scope);
            }
            try{
                return Return_Value(*this,Expression_Calc_Value,exp_calc(Variable::parse(toString()),scope,all_scope,this_scope),scope,all_scope,this_scope);
            }catch(...){
                return Return_Value(*this,Throw_Return_Value,Variable::var("ExpressionError"),scope,all_scope,this_scope);
            }
        }
    } L;
};
#endif
