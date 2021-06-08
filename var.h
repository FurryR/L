/*
L programming language was under the MIT license.
Copyright(c) 2021 nu11ptr team.
*/
/*
compile flags:
FORCE_UTF8 : converts Unicode to UTF-8 by force.
*/
#ifndef VAR_MODULE
#define VAR_MODULE
#include<vector>
#include<map>
#include<locale>
#include<string>
#ifdef FORCE_UTF8
#include<codecvt>
#endif
namespace Variable{
    int get_op_priority(const std::string& op){
        if(op=="+")return 1;
        if(op=="-")return 1;
        if(op=="*")return 2;
        if(op=="/")return 2;
        if(op=="%")return 2;
        if(op=="(")return 1;
        if(op==")")return 3;
        if(op=="==")return 1;
        if(op=="!=")return 1;
        if(op==">")return 1;
        if(op=="<")return 1;
        if(op==">=")return 1;
        if(op=="<=")return 1;
        if(op=="&&")return 0;
        if(op=="||")return 0;
        if(op=="!")return 0;
        if(op==">>")return 1;
        if(op=="<<")return 1;
        if(op=="^")return 1;
        if(op=="&")return 1;
        if(op=="~")return 0;
        if(op=="|")return 1;
        if(op==">>>")return 1;
        return -1;
    }
    std::string castExpression(const std::vector<std::string>& p){
        std::string ret;
        std::vector<std::string> temp;
        std::string last,temp2;
        std::string op;
        for(std::vector<std::string>::const_reverse_iterator i=p.crbegin();i!=p.crend();i++){
            op=(*i);
            if(get_op_priority(op)!=-1&&op!="!"&&op!="~"){
                temp2=temp[temp.size()-1];
                temp.pop_back();
                temp2+=op;
                temp2+=temp[temp.size()-1];
                temp.pop_back();
                if(get_op_priority(op)>=get_op_priority(last)){
                    temp.push_back("("+temp2+")");
                }else{
                    temp.push_back(temp2);
                }
                last=op;
            }else if(op=="!"){
                temp2=op+temp[temp.size()-1];
                temp.pop_back();
                temp.push_back(temp2);
                last=op;
            }else temp.push_back(op);
        }
        while(!temp.empty()){
            if(get_op_priority(temp[temp.size()-2])!=-2){
                ret=temp[temp.size()-1]+ret;
            }else{
                ret+=temp[temp.size()-1];
            }
            temp.pop_back();
        }
        return ret;
    }
    std::vector<std::string> genExpression(const std::vector<std::string>& p){
        std::vector<std::string> ret;
        std::vector<std::string> temp;
        std::vector<std::string> fin;
        std::string op;
        for(std::vector<std::string>::const_reverse_iterator i=p.crbegin();i!=p.crend();i++){
            op=(*i);
            if((get_op_priority(op)!=-1||op[0]=='.')&&op!=")"&&op!="("){
                while(!temp.empty()&&temp[temp.size()-1]!=")"&&get_op_priority(temp[temp.size()-1])>get_op_priority(op)){
                    ret.push_back(temp[temp.size()-1]);
                    temp.pop_back();
                }
                temp.push_back(op);
            }else if(op==")"){
                temp.push_back(op);
            }else if(op=="("){
                while(!temp.empty()){
                    if(temp[temp.size()-1]==")"){
                        temp.pop_back();
                        break;
                    }else{
                        ret.push_back(temp[temp.size()-1]);
                        temp.pop_back();
                    }
                }
            }else ret.push_back(op);
        }
        for(size_t i=0;i<temp.size();i++)ret.push_back(*(temp.crbegin()+i));
        for(size_t i=0;i<ret.size();i++)fin.push_back(*(ret.crbegin()+i));
        return fin;
    }
    std::vector<std::string> splitExpression(const std::string& p){
        short a=0;
        int f=0;
        bool x=false;
        std::vector<std::string> ret;
        std::string temp;
        for(size_t i=0;i<p.length();i++){
            switch(p[i]){
                case '\'':{
                    if(p[i-1]!='\\'||(p[i-1]=='\\'&&p[i-2]=='\\')){if(a==1)a=0;else if(a==0)a=1;}
                    temp+=p[i];
                    break;
                }
                case '\"':{
                    if(p[i-1]!='\\'||(p[i-1]=='\\'&&p[i-2]=='\\')){if(a==2)a=0;else if(a==0)a=2;}
                    temp+=p[i];
                    break;
                }
                case '!':
                case '>':
                case '=':
                case '&':
                case '|':
                case '~':
                case '<':{
                    if(a==0&&f==0&&(p[i-1]=='!'||p[i-1]=='>'||p[i-1]=='='||p[i-1]=='&'||p[i-1]=='|'||p[i-1]=='<'||((p[i-1]=='e'||p[i-1]=='E')&&p[i-2]>='0'&&p[i-2]<='9')))ret[ret.size()-1]+=p[i];
                    else if(a==0&&f==0&&p.substr(i>4?i-5:0,5)!="const"&&((!x)||p[i]!='>')){
                        if(temp!="")ret.push_back(temp);
                        temp="";
                        ret.push_back(std::string(1,p[i]));
                    }else if(p[i]=='<'){
                        x=true;
                        temp+=p[i];
                    }else if(p[i]=='>'&&x){
                        x=false;
                        temp+=p[i];
                    }
                    break;
                }
                case '-':{
                    if(a==0&&f==0&&p[i-1]==p[i])ret[ret.size()-1]+=p[i];
                    else if(a==0&&f==0&&(p[i+1]=='-'||(i>=1&&(p[i-1]!='+'&&p[i-1]!='-'&&p[i-1]!='*'&&p[i-1]!='/'&&p[i-1]!='%'&&((p[i-1]!='e'&&p[i-1]!='E')||((p[i-2]>='a'&&p[i-2]<='z')||(p[i-2]>='A'&&p[i-2]<='Z')||p[i-2]=='_')))))){
                        if(temp!=""){
                            ret.push_back(temp);
                            temp="";
                        }
                        ret.push_back(std::string(1,p[i]));
                    }else temp+=p[i];
                    break;
                }
                case '+':
                case '*':
                case '/':
                case '%':{
                    if(a==0&&f==0&&p[i-1]==p[i])ret[ret.size()-1]+=p[i];
                    else if(a==0&&f==0){
                        if(temp!=""){
                            ret.push_back(temp);
                            temp="";
                        }
                        ret.push_back(std::string(1,p[i]));
                    }else temp+=p[i];
                    break;
                }
                case '(':{
                    /*
                    if(f!=0&&a==0){
                        temp+=p[i];
                        f++;
                    }else if(a!=0)temp+=p[i];
                    */
                    if(a==0&&f==0){
                        if(temp!="")ret.push_back(temp);
                        temp="";
                        f++;
                        ret.push_back("(");
                    }else temp+="(";
                    break;
                }
                case ')':{
                    /*
                    if(f==1){
                        ret.push_back(temp);
                        temp="";
                        f--;
                    }else if(a!=0)temp+=p[i];
                    */
                    if(a==0&&f==1){
                        if(temp!="")ret.push_back(temp);
                        temp="";
                        f--;
                        ret.push_back(")");
                    }else temp+=")";
                    break;
                }
                case '{':
                case '[':{
                    if(a==0)f++;
                    temp+=p[i];
                    break;
                }
                case '}':
                case ']':{
                    if(a==0)f--;
                    temp+=p[i];
                    break;
                }
                default:{
                    temp+=p[i];
                    break;
                }
            }
        }
        if(f!=0)throw 0;
        if(a!=0)throw 0;
        if(temp!="")ret.push_back(temp);
        return ret;
    }
    bool isExpression(const std::string& p){
        bool flag=false;
        for(size_t i=0,j=0,a=0;i<p.length();i++){
            if(p[i]=='"'&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=1;else if(a==1)a=0;}
            if(p[i]=='\''&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=2;else if(a==2)a=0;}
            if((p[i]=='('||p[i]=='{'||p[i]=='[')&&a==0)j++;else if((p[i]==')'||p[i]=='}'||p[i]==']')&&a==0)j--;
            else if(get_op_priority(std::string(1,p[i]))!=-1&&a==0&&j==0)flag=true;
        }
        return flag;
    }
    int Hex2Dec(const std::string& m){
        int l;
        l=stoi(m,0,16);
        return l;
    }
    std::string Unicode2String(const std::string& str){
        if(str=="")return "";
        std::wstring m(1,Hex2Dec("0x"+str));
        #ifdef FORCE_UTF8
        return std::wstring_convert< std::codecvt_utf8<wchar_t> >().to_bytes(m);
        #else
        std::string ret;
        try{
            ret=std::string(m.length()*std::use_facet< std::codecvt<wchar_t,char,std::mbstate_t> >(std::locale("")).max_length(),'\0');
        }catch(...){
            return "?";
        }
        mbstate_t mb;
        const wchar_t* from_next;
        char* to_next;
        if(std::use_facet< std::codecvt<wchar_t,char,std::mbstate_t> >(std::locale("")).out(mb,&m[0],&m[m.size()],from_next,&ret[0],&ret[ret.size()],to_next)!=std::codecvt_base::result::ok)return "?";
        ret.resize(to_next-&ret[0]);
        return ret;
        #endif
    }
    std::string clearnull(const std::string& x){
        std::string tmp;
        for(size_t i=0,a=0;i<x.length();i++){
            if(x[i]=='"'&&(x[i-1]!='\\'||x[i-2]=='\\')){if(a==0)a=1;else if(a==1)a=0;}
            if(x[i]=='\''&&(x[i-1]!='\\'||x[i-2]=='\\')){if(a==0)a=2;else if(a==2)a=0;}
            if((x[i]=='\r'||x[i]=='\n'||x[i]=='\t')&&a==0)continue;
            else if(x[i]==' '&&a==0&&(i<=0||(tmp[i-1]=='('||tmp[i-1]=='['||tmp[i-1]=='{'||tmp[i-1]==' '))&&(i<=0||!((tmp[i-1]>='a'&&tmp[i-1]<='z')||(tmp[i-1]>='A'&&tmp[i-1]<='Z')||(tmp[i-1]>='0'&&tmp[i-1]<='9')))){
                //if(i<=0)continue;
                //if((x[i-1]>='a'||x[i-1]<='z')&&(x[i-1]>='A'||x[i-1]<='Z'))tmp+=x[i];
                continue;
            }else tmp+=x[i];
        }
        return tmp;
    }
    std::vector<std::string> code_split(const std::string& x){
        std::string p;
        for(size_t i=0,a=0;i<x.length();i++){
            if(x[i]=='"'&&(x[i-1]!='\\'||x[i-2]=='\\')){if(a==0)a=1;else if(a==1)a=0;}
            if(x[i]=='\''&&(x[i-1]!='\\'||x[i-2]=='\\')){if(a==0)a=2;else if(a==2)a=0;}
            if(x[i]=='\n'&&a==0&&i>=1&&x[i-1]!='['&&x[i-1]!='('&&x[i-1]!='{')p+=';';//p[i]=';';else p[i]=' ';
            if(x[i]!='\n')p+=x[i];
        }
        std::vector<std::string> ret;
        std::string temp;
        for(size_t i=0,j=0,a=0;i<p.length();i++){
            if(p[i]=='"'&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=1;else if(a==1)a=0;}
            if(p[i]=='\''&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=2;else if(a==2)a=0;}
            if((p[i]=='('||p[i]=='{'||p[i]=='[')&&a==0)j++;else if((p[i]==')'||p[i]=='}'||p[i]==']')&&a==0)j--;
            if(p[i]==';'&&a==0&&j==0)ret.push_back(temp),temp="";else temp+=p[i];
        }
        if(temp!="")ret.push_back(temp);
        for(size_t i=0;i<ret.size();i++){
            std::string w;
            for(size_t j=0,flag=0;j<ret[i].length();j++){
                if(ret[i][j]==' '&&flag==0)continue;else{
                    flag=1;
                    w+=ret[i][j];
                }
            }
            ret[i]=w;
        }
        std::vector<std::string> fin;
        for(size_t i=0;i<ret.size();i++){
            if(ret[i]!=""&&ret[i]!=";")fin.push_back(ret[i]);
        }
        return fin;
    }
    typedef enum var_tp{
        Null=0,
        Int=1,
        Boolean=2,
        String=3,
        Array=4,
        Object=5,
        Function=6,
        Expression=7,
    } var_tp;
    typedef struct Fn_temp{
        std::vector<std::string> value;
        Fn_temp(){}
        Fn_temp(std::vector<std::string> x){value=x;}
    } Fn_temp;
    typedef class var{
        bool needtoRemove;
        std::string replaceAll(std::string strBig,const std::string& Stringrc,const std::string& strdst) const{
            size_t pos=0,srclen=Stringrc.size(),dstlen=strdst.size();
            while((pos=strBig.find(Stringrc,pos))!=std::string::npos)strBig.replace(pos,srclen,strdst),pos+=dstlen;
            return strBig;
        }
        public:
        bool isConst;
        std::string StringValue;
        std::vector<var> ArrayValue;
        std::map<std::string,var> ObjectValue;
        Fn_temp FunctionValue;
        std::vector<std::string> ExpressionValue;
        var(){tp=Null;isConst=true;needtoRemove=false;}
        var(bool x,bool c=true){tp=Boolean;BooleanValue=x;isConst=c;needtoRemove=false;}
        var(std::nullptr_t x,bool c=true){tp=Null;isConst=c;needtoRemove=false;}
        var(const int& x,bool c=true){IntValue=x,tp=Int;isConst=c;needtoRemove=false;}
        var(const double& x,bool c=true){IntValue=x,tp=Int;isConst=c;needtoRemove=false;}
        var(const std::string& x,bool c=true){StringValue=x,tp=String;isConst=c;needtoRemove=false;}
        var(const char* const x,bool c=true){StringValue=std::string(x),tp=String;isConst=c;needtoRemove=false;}
        var(const std::vector<var>& x,bool c=true){ArrayValue=x,tp=Array;isConst=c;needtoRemove=false;}
        var(const std::map<std::string,var>& x,bool c=true){ObjectValue=x,tp=Object;isConst=c;needtoRemove=false;}
        var(const Fn_temp& x,bool c=true){FunctionValue=x,tp=Function;isConst=c;needtoRemove=false;}
        var(const std::vector<std::string>& x,bool c=true){ExpressionValue=x,tp=Expression;isConst=c;needtoRemove=false;}
        double IntValue;
        bool BooleanValue;
        var_tp tp;
        void remove(){
            needtoRemove=true;
        }
        void update(){
            switch(tp){
                case Array:{
                    std::vector<var> temp;
                    for(size_t i=0;i<ArrayValue.size();i++){
                        if(ArrayValue[i].needtoRemove==false)temp.push_back(ArrayValue[i]);
                    }
                    ArrayValue=temp;
                    return;
                }
                case Object:{
                    std::map<std::string,var> temp;
                    for(std::map<std::string,var>::const_iterator x=ObjectValue.cbegin();x!=ObjectValue.cend();x++){
                        if(x->second.needtoRemove==false)temp[x->first]=x->second;
                    }
                    ObjectValue=temp;
                    return;
                }
                default:return;
            }
        }
        std::string toString_nonconst() const{
            std::string tmp;
            switch(tp){
                case Null:return "null";
                case Int:{
                    if(IntValue==(int)IntValue)return std::to_string((int)IntValue);
                    return std::to_string(IntValue);
                }
                case Boolean:return BooleanValue?"true":"false";
                case String:{
                    tmp="\"";
                    for(size_t i=0;i<StringValue.length();i++){
                        switch(StringValue[i]){
                            case '\b':{
                                tmp+="\\b";
                                break;
                            }
                            case '\f':{
                                tmp+="\\f";
                                break;
                            }
                            case '\r':{
                                tmp+="\\r";
                                break;
                            }
                            case '\t':{
                                tmp+="\\t";
                                break;
                            }
                            case '\n':{
                                tmp+="\\n";
                                break;
                            }
                            case '\\':
                            case '\'':
                            case '"':{
                                tmp+="\\"+StringValue[i];
                                break;
                            }
                            default:{
                                tmp+=StringValue[i];
                                break;
                            }
                        }
                    }
                    return tmp+"\"";
                }
                case Array:{
                    tmp="[";
                    for(size_t i=0;i<ArrayValue.size();i++){
                        tmp+=ArrayValue[i].toString();
                        if(i+1<ArrayValue.size())tmp+=",";
                    }
                    return tmp+"]";
                }
                case Object:{
                    tmp="{";
                    for(std::map<std::string,var>::const_iterator it=ObjectValue.cbegin();it!=ObjectValue.cend();it++){
                        tmp+="\""+replaceAll(it->first,"\"","\\\"")+"\":";
                        tmp+=it->second.toString();
                        if((++std::map<std::string,var>::const_iterator(it))!=ObjectValue.cend())tmp+=",";
                    }
                    return tmp+"}";
                }
                case Function:{
                    tmp="(";
                    for(size_t i=0;i<FunctionValue.value.size();i++)tmp+=FunctionValue.value[i]+";";
                    return tmp+")";
                }
                case Expression:{
                    return castExpression(ExpressionValue);
                }
            }
            return "null";
        }
        std::string toString() const{
            if(isConst)return "const<"+toString_nonconst()+">";else return toString_nonconst();
        }
        var convert(const var_tp& type) const{
            if(tp==type)return *this;
            switch(tp){
                case Int:{
                    switch(type){
                        case Boolean:{
                            return var((bool)IntValue);
                        }
                        default:throw "failed";
                    }
                    break;
                }
                case Boolean:{
                    switch(type){
                        case Int:return var((int)BooleanValue);
                        default:throw "failed";
                    }
                }
                case String:{
                    switch(type){
                        case Array:{
                            std::vector<var> ret;
                            for(size_t i=0;i<StringValue.length();i++){
                                ret.push_back(var(std::string(1,StringValue[i])));
                            }
                            return var(ret);
                        }
                        default:throw "failed";
                    }
                    break;
                }
                case Array:{
                    switch(type){
                        case Object:{
                            std::map<std::string,var> ret;
                            for(size_t i=0;i<ArrayValue.size();i++){
                                ret[std::to_string(i)]=ArrayValue[i];
                            }
                            return var(ret);
                        }
                        default:throw "failed";
                    }
                    break;
                }
                case Null:
                case Function:
                case Object:{
                    throw "failed";
                }
                default:throw "failed";
            }
            return var();
        }
        var operator+(const var& opx){
            var ret;
            const var op=opx.convert(tp);
            switch(tp){
                case Int:{
                    return var(IntValue+op.IntValue);
                }
                case String:{
                    return var(StringValue+op.StringValue);
                }
                case Array:{
                    ret.tp=Array;
                    ret.ArrayValue=ArrayValue;
                    for(size_t i=0;i<op.ArrayValue.size();i++){
                        ret.ArrayValue.push_back(op.ArrayValue[i]);
                    }
                    return ret;
                }
                default:{
                    throw "failed";
                }
            }
        }
        var operator-(const var& opx){
            var ret;
            const var op=opx.convert(tp);
            switch(tp){
                case Int:{
                    return var(IntValue-op.IntValue);
                }
                default:{
                    throw "failed";
                }
            }
        }
        var operator*(const var& opx){
            var ret;
            const var op=opx.convert(tp);
            switch(tp){
                case Int:{
                    return var(IntValue*op.IntValue);
                }
                default:{
                    throw "failed";
                }
            }
        }
        var operator/(const var& opx){
            var ret;
            const var op=opx.convert(tp);
            switch(tp){
                case Int:{
                    return var(IntValue/op.IntValue);
                }
                default:{
                    throw "failed";
                }
            }
        }
        var operator%(const var& opx){
            var ret;
            const var op=opx.convert(tp);
            switch(tp){
                case Int:{
                    return var((int)IntValue%(int)op.IntValue);
                }
                default:{
                    throw "failed";
                }
            }
        }
        var operator&(const var& opx){
            var ret;
            const var op=opx.convert(tp);
            switch(tp){
                case Int:{
                    return var((int)IntValue&(int)op.IntValue);
                }
                default:{
                    throw "failed";
                }
            }
        }
        var operator|(const var& opx){
            var ret;
            const var op=opx.convert(tp);
            switch(tp){
                case Int:{
                    return var((int)IntValue|(int)op.IntValue);
                }
                default:{
                    throw "failed";
                }
            }
        }
        var operator^(const var& opx){
            var ret;
            const var op=opx.convert(tp);
            switch(tp){
                case Int:{
                    return var((int)IntValue^(int)op.IntValue);
                }
                default:{
                    throw "failed";
                }
            }
        }
        var operator~(){
            switch(tp){
                case Int:{
                    return var(~(int)IntValue);
                }
                default:{
                    throw "failed";
                }
            }
        }
        var leftmove(const var& opx){
            var ret;
            const var op=opx.convert(tp);
            switch(tp){
                case Int:{
                    if(op.IntValue>=32)return var((int)IntValue);else return var((int)IntValue<<(int)op.IntValue);
                }
                default:{
                    throw "failed";
                }
            }
        }
        var rightmove_signed(const var& opx){
            var ret;
            const var op=opx.convert(tp);
            switch(tp){
                case Int:{
                    if(op.IntValue>=32)return var((int)IntValue);else return var((int)IntValue>>(int)op.IntValue);
                }
                default:{
                    throw "failed";
                }
            }
        }
        var rightmove_unsigned(const var& opx){
            var ret;
            const var op=opx.convert(tp);
            switch(tp){
                case Int:{
                    return var(double((unsigned int)IntValue>>(unsigned int)op.IntValue));
                }
                default:{
                    throw "failed";
                }
            }
        }
        bool operator==(const var& opx) const{
            try{
                opx.convert(tp);
            }catch(...){
                return false;
            }
            const var op=opx.convert(tp);
            switch((size_t)op.tp){
                case Int:{
                    return op.IntValue==IntValue;
                }
                case Null:{
                    return true;
                }
                case Boolean:{
                    return op.BooleanValue==BooleanValue;
                }
                case String:{
                    return op.StringValue==StringValue;
                }
                case Array:{
                    if(op.ArrayValue.size()!=ArrayValue.size())return false;
                    for(size_t i=0;i<op.ArrayValue.size();i++){
                        if(var(op.ArrayValue[i])!=var(ArrayValue[i]))return false;
                    }
                    return true;
                }
                case Object:{
                    for(std::map<std::string,var>::const_iterator i=op.ObjectValue.begin();i!=op.ObjectValue.end();i++){
                        try{
                            if(var(((std::map<std::string,var>)ObjectValue).at(i->first))!=var(i->second))return false;
                        }catch(...){
                            return false;
                        }
                    }
                    return true;
                }
                case Function:{
                    if(FunctionValue.value.size()!=op.FunctionValue.value.size())return false;
                    for(size_t i=0;i<FunctionValue.value.size();i++){
                        if(FunctionValue.value[i]!=op.FunctionValue.value[i])return false;
                    }
                    return true;
                }
            }
            return false;
        }
        bool operator!=(const var& op){
            return !operator==(op);
        }
        bool operator>(const var& opx){
            try{
                opx.convert(tp);
            }catch(...){
                return false;
            }
            const var op=opx.convert(tp);
            switch(op.tp){
                case Int:{
                    return IntValue>op.IntValue;
                }
                case String:{
                    return StringValue>op.StringValue;
                }
                default:{
                    return false;
                }
            }
            return false;
        }
        bool operator<(const var& op){
            return (!operator>(op))&&(!operator==(op));
        }
        bool operator>=(const var& op){
            return operator>(op)||operator==(op);
        }
        bool operator<=(const var& op){
            return operator<(op)||operator==(op);
        }
        bool operator&&(const var& op) const{
            return (operator==(var(1)))&&(op==var(1));
        }
        bool operator||(const var& op) const{
            return (operator==(var(1)))||(op==var(1));
        }
        bool operator!(){
            if(tp==Boolean||tp==Int){
                if(tp==Boolean)return !BooleanValue;
                else return !(bool)IntValue;
            }
            return false;
        }
    } var;
    std::string getTypeStr(var_tp x){
        switch(x){
            case Null:return "null";
            case Int:return "int";
            case Boolean:return "boolean";
            case String:return "string";
            case Array:return "array";
            case Object:return "object";
            case Function:return "function";
            case Expression:return "null";//no use?
        }
        return "null";
    }
    var_tp getStrType(std::string x){
        if(x=="null")return Null;
        if(x=="int")return Int;
        if(x=="boolean")return Boolean;
        if(x=="string")return String;
        if(x=="array")return Array;
        if(x=="Object")return Object;
        if(x=="function")return Function;
        //if(x=="expression")return Expression;
        return Null;
    }
    var parse(std::string p,bool isc=false,bool forceisc=false){
        p=clearnull(p);
        if(p.substr(0,6)=="const<"&&p[p.length()-1]=='>'){
            if(forceisc)return parse(p.substr(6,p.length()-7),isc);
            return parse(p.substr(6,p.length()-7),true);
        }
        if(p=="")return var(nullptr,isc);
        try{
            if(isExpression(p)==false)throw 0;
        }catch(...){
            try{
                for(size_t i=0,a=0,j=0;i<p.length();i++){
                    if(p[i]=='"'&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=1;else if(a==1)a=0;}
                    if(p[i]=='\''&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=2;else if(a==2)a=0;}
                    if((p[i]=='('||p[i]=='{'||p[i]=='[')&&a==0)j++;else if((p[i]==')'||p[i]=='}'||p[i]==']')&&a==0)j--;
                    if(p[i]=='.'&&a==0&&j==0){
                        if(p[i+1]>='0'&&p[i+1]<='9')continue;
                        else throw 0;
                    }
                }
                if(p.find_first_of('.')!=std::string::npos)std::stod(p);
                else std::stoi(p,0,0);
            }catch(...){
                if(p=="null")return var(nullptr,isc);
                if(p=="true"||p=="false")return var(p=="true",isc);
                if((p[0]=='\"'&&p[p.length()-1]=='\"')||(p[0]=='\''&&p[p.length()-1]=='\'')){
                    std::string tmp=p.substr(1,p.length()-2),ret="";
                    for(size_t i=0;i<tmp.length();i++){
                        if(tmp[i]=='\\'){
                            i++;
                            switch(tmp[i]){
                                case '"':
                                case '\\':
                                case '\'':{
                                    ret+=tmp[i];
                                    break;
                                }
                                case 'b':{
                                    ret+='\b';
                                    break;
                                }
                                case 'f':{
                                    ret+='\f';
                                    break;
                                }
                                case 'r':{
                                    ret+='\r';
                                    break;
                                }
                                case 't':{
                                    ret+='\t';
                                    break;
                                }
                                case 'n':{
                                    ret+='\n';
                                    break;
                                }
                                case 'e':{
                                    ret+='\e';
                                    break;
                                }
                                case 'u':{
                                    ret+=Unicode2String(tmp.substr(i+1,4));
                                    i+=4;
                                    break;
                                }
                            }
                        }else ret+=tmp[i];
                    }
                    return var(ret,isc);
                }
                if((p[0]=='{'||p[0]=='[')&&(p[p.length()-1]=='}'||p[p.length()-1]==']')){
                    std::string key="",value="";
                    std::map<std::string,var> ret;
                    std::vector<var> ret2;
                    bool isobject=(p[0]=='{');
                    for(size_t i=1,a=0;i<p.length()-1;i++){
                        if(isobject){
                            for(;i<p.length()-1;i++){
                                if(p[i]=='"'&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=1;else if(a==1)a=0;}
                                if(p[i]=='\''&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=2;else if(a==2)a=0;}
                                if((p[i]=='"'||p[i]=='\'')&&(p[i-1]!='{'||p[i-1]!=',')&&a==0){
                                    key+=p[i++];
                                    break;
                                }
                                key+=p[i];
                            }
                            i++;//: token
                        }
                        for(size_t j=0;i<p.length()-1;i++){
                            if(p[i]=='"'&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=1;else if(a==1)a=0;}
                            if(p[i]=='\''&&(p[i-1]!='\\'||p[i-2]=='\\')){if(a==0)a=2;else if(a==2)a=0;}
                            if((p[i]=='('||p[i]=='{'||p[i]=='[')&&a==0)j++;else if((p[i]==')'||p[i]=='}'||p[i]==']')&&a==0)j--;
                            if(p[i]==','&&j==0&&a==0)break;else value+=p[i];
                        }
                        if(isobject)ret[parse(key).StringValue]=parse(value);else ret2.push_back(parse(value));
                        key="";
                        value="";
                    }
                    if(isobject)return var(ret,isc);else return var(ret2,isc);
                }
                if(p[0]=='('&&p[p.length()-1]==')')return var(Fn_temp(code_split(p.substr(1,p.length()-2))),isc);
                return var(genExpression(splitExpression(p)),isc);
            }
            if(p.find_first_of('.')==std::string::npos)return var(std::stoi(p,0,0),isc);else return var(std::stod(p),isc);
        }
        return var(genExpression(splitExpression(p)),isc);
    }
}
#endif
