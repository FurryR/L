/*
L programming language was under the MIT license.
Copyright(c) 2021 nu11ptr team.
*/

// your compile flag here.
#include "./parse.h"
#include <fstream>
#include <iostream>
#define VERSION_INFO "1.3.1-r_release"
typedef class str_factory {
  std::string fmt;

public:
  str_factory() {}
  str_factory(std::string f) {
    fmt = f;
    return;
  }
  std::string format(std::vector<std::string> value = {}) {
    bool flag = false;
    std::string fin;
    for (size_t i = 0; i < fmt.length(); i++) {
      if (fmt[i] == '\\')
        flag = !flag;
      if (fmt[i] == '{' && !flag) {
        size_t j = i;
        while (j < fmt.length() && fmt[j] != '}')
          j++;
        fin += value.at(stoi(fmt.substr(i + 1, j - 1)));
        i = j;
        continue;
      }
      if (!flag)
        fin += fmt[i];
    }
    return fin;
  }
} str_factory;
std::map<std::string, str_factory> strmap;
std::vector<std::string> newline_split(std::string x) {
  std::vector<std::string> a;
  std::string temp;
  for (size_t i = 0; i < x.length(); i++) {
    if (x[i] == '\n') {
      a.push_back(temp);
      temp = "";
      continue;
    } else
      temp += x[i];
  }
  return a;
}
void lang_set(std::string fn) {
  strmap.clear();
  std::vector<std::string> s = newline_split(fn);
  for (size_t i = 0; i < s.size(); i++) {
    if (s[i][0] == '#')
      continue;
    std::string key = s[i].substr(0, s[i].find_first_of(":"));
    std::string value_preparse = s[i].substr(s[i].find_first_of(":") + 1);
    Variable::var value_parsing = Variable::parse(value_preparse);
    strmap[key] = str_factory(value_parsing.StringValue);
    // strmap[s[i].substr(0,s[i].find_first_of(":"))]=str_factory(Variable::parse().StringValue);
  }
}
void lang_init() {
  lang_set(
      "#en_US.C\n"
      "lpp_title:\"L interpreter Version {0} English Edition\"\n"
      "lpp_usage_information:\"using:lpp (lang=langfile) "
      "[--help/help/(filename)] (arguments)...\"\n"
      "lpp_usage_argument_langfile:\"lang : select language file.\"\n"
      "lpp_usage_argument_help:\"help/--help : display this help message.\"\n"
      "lpp_usage_argument_terminal:\"terminal : start L terminal.\"\n"
      "lpp_usage_argument_file:\"(filename) : run script.\"\n"
      "lpp_failed_open_file:\"failed to open {0} file.\"\n"
      "lpp_loading_terminal:\"loading terminal...\"\n"
      "lpp_terminal_welcome:\"welcome!use .debug to show the variables,.exit "
      "to exit.\"\n"
      "lpp_throw:\"    {0} -> {1}\\n[x]Unexcepted {2} Error.\"\n"
      "lpp_unknown_internal_stmt:\"Unknown internal statement.After checking "
      "spell,please try again.\"\n"
      "lpp_variable_list:\"variable list:\"\n");
}
// language module ends
int main(int argc, char **argv) {
  lang_init();
  std::vector<std::string> arg(argc - 1);
  std::vector<std::string> program_arg;
  std::string langfile;
  bool uselangfile = false;
  // if(arg.size()==0)arg.push_back("terminal");else arg[0]="terminal";//default
  // arguments for develop
  for (size_t i = 1; i < (size_t)argc; i++)
    arg[i - 1] = std::string(argv[i]);
  if (arg.size() > 0 && arg[0].substr(0, 5) == "lang=") {
    langfile = arg[0].substr(5);
    std::ifstream s(langfile);
    if (!s.is_open()) {
      std::cout << "failed to open language file." << std::endl;
      return 1;
    }
    std::string fin;
    while (!s.eof()) {
      std::string temp;
      std::getline(s, temp);
      fin += temp + "\n";
    }
    lang_set(fin);
    uselangfile = true;
  }
  if (arg.size() >= 2) {
    if (uselangfile) {
      // langfile=[0] --help[1] arguments...[2]
      program_arg = std::vector<std::string>(arg.begin() + 2, arg.end());
    } else {
      program_arg = std::vector<std::string>(arg.begin() + 1, arg.end());
    }
  }
  if (arg.size() == 0 || (uselangfile && arg.size() == 1) ||
      arg[uselangfile] == "--help" || arg[uselangfile] == "help") {
    std::cout << strmap["lpp_title"].format({VERSION_INFO}) << std::endl;
    std::cout << strmap["lpp_usage_information"].format() << std::endl;
    std::cout << strmap["lpp_usage_argument_langfile"].format() << std::endl;
    std::cout << strmap["lpp_usage_argument_help"].format() << std::endl;
    std::cout << strmap["lpp_usage_argument_terminal"].format() << std::endl;
    std::cout << strmap["lpp_usage_argument_file"].format() << std::endl;
    return 0;
  } else if (arg[uselangfile] == "terminal") {
    std::cout << strmap["lpp_loading_terminal"].format() << std::endl;
    std::cout << strmap["lpp_terminal_welcome"].format() << std::endl;
    Variable::var f;
    f.isConst = false;
    f.tp = Variable::Object;
    while (!std::cin.eof()) {
      std::string m, cm;
      std::cout << ">>> " << std::flush;
      std::cin.clear();
      std::getline(std::cin, m);
      if (m[0] == '.') { // REPL COMMAND
        if (m == ".exit")
          return 0;
        else if (m == ".debug") {
          std::cout << strmap["lpp_variable_list"].format() << std::endl;
          std::cout << f.toString() << std::endl;
        } else {
          std::cout << strmap["lpp_unknown_internal_stmt"].format()
                    << std::endl;
        }
        continue;
      } else {
        cm = m + "\n";
        while (!std::cin.eof()) {
          size_t a = 0, j = 0;
          std::cin.clear();
          for (size_t i = 0, z = 0; i < cm.length(); i++) {
            if (cm[i] == '\\')
              z = !z;
            else if (cm[i] == '\"' && !z) {
              if (a == 0)
                a = 1;
              else if (a == 1)
                a = 0;
            } else if (cm[i] == '\'' && !z) {
              if (a == 2)
                a = 1;
              else if (a == 2)
                a = 0;
            } else
              z = 0;
            if (cm[i] == '#' && a == 0) {
              while (i < cm.length() && cm[i] != '\n')
                i++;
              i++;
            }
            if (cm[i] == '\n' &&
                (i > 0 && cm[i - 1] != '[' && cm[i - 1] != '{' &&
                 cm[i - 1] != '(' && cm[i - 1] != ';'))
              cm[i] = ';';
            if ((cm[i] == '[' || cm[i] == '{' || cm[i] == '(') && a == 0)
              j++;
            if ((cm[i] == ']' || cm[i] == '}' || cm[i] == ')') && a == 0)
              j--;
          }
          if (a != 0 || j != 0 || cm[cm.length() - 1] == ',') {
            std::string temp;
            std::cout << "... " << std::flush;
            std::getline(std::cin, temp);
            cm += temp + "\n";
          } else
            break;
          a = 0;
          j = 0;
        }
        if (std::cin.eof())
          return 0;
      }
      std::vector<std::string> w = Variable::code_split(cm);
      L::L::Return_Value x;
      for (size_t i = 0; i < w.size(); i++) {
        x = L::L(w[i]).eval(f);
        f = x.scope;
        if (x.tp == L::L::Return_Type::Throw_Return_Value) {
          std::cout << strmap["lpp_throw"].format({Variable::clearnull(w[i]),
                                                   x.base.toString(),
                                                   x.value.toString()})
                    << std::endl;
          x.value = Variable::var(nullptr, false);
        }
      }
      std::cout << "> " << x.value.toString() << std::endl;
      m = "";
      cm = "";
    }
  } else if (arg.size() > uselangfile) {
    std::ifstream s(arg[uselangfile]);
    if (!s.is_open()) {
      std::cout << strmap["lpp_failed_open_file"].format({"script"})
                << std::endl;
      return 1;
    }
    Variable::var var_arg(nullptr, true);
    var_arg.tp = Variable::Array;
    var_arg.ArrayValue = std::vector<Variable::var>(program_arg.size());
    for (size_t i = 0; i < program_arg.size(); i++) {
      var_arg.ArrayValue[i] = Variable::var(program_arg[i], false);
    }
    Variable::var f;
    f.isConst = false;
    f.tp = Variable::Object;
    f.ObjectValue["arguments"] = var_arg;
    while (!s.eof()) {
      std::string m, cm;
      std::getline(s, m);
      cm = m + "\n";
      while (!s.eof()) {
        size_t a = 0, j = 0;
        for (size_t i = 0, z = 0; i < cm.length(); i++) {
          if (cm[i] == '\\')
            z = !z;
          else if (cm[i] == '\"' && !z) {
            if (a == 0)
              a = 1;
            else if (a == 1)
              a = 0;
          } else if (cm[i] == '\'' && !z) {
            if (a == 2)
              a = 1;
            else if (a == 2)
              a = 0;
          } else
            z = 0;
          if (cm[i] == '#' && a == 0) {
            while (i < cm.length() && cm[i] != '\n')
              i++;
            i++;
          }
          if (cm[i] == '\n' &&
              (i <= 0 || (cm[i - 1] != '[' && cm[i - 1] != '{' &&
                          cm[i - 1] != '(' && cm[i - 1] != ';')))
            cm[i] = ';';
          if ((cm[i] == '[' || cm[i] == '{' || cm[i] == '(') && a == 0)
            j++;
          if ((cm[i] == ']' || cm[i] == '}' || cm[i] == ')') && a == 0)
            j--;
        }
        if (a != 0 || j != 0 || cm[cm.length() - 1] == ',') {
          std::string temp;
          std::getline(s, temp);
          cm += temp + "\n";
        } else
          break;
      }
      if (s.eof())
        break;
      std::vector<std::string> w = Variable::code_split(cm);
      L::L::Return_Value x;
      for (size_t i = 0; i < w.size(); i++) {
        x = L::L(w[i]).eval(f);
        f = x.scope;
        if (x.tp == L::L::Return_Type::Throw_Return_Value) {
          // std::cout<<"    "<<w[i]<<" ->
          // "<<x.base.toString()<<std::endl<<"[x]Interpreter throws a(an)
          // "<<x.value.toString()<<" Error."<<std::endl;
          std::cout << strmap["lpp_throw"].format({Variable::clearnull(w[i]),
                                                   x.base.toString(),
                                                   x.value.toString()})
                    << std::endl;
          return 1;
        } else if (x.tp == L::L::Return_Type::Function_Return_Value) {
          try {
            x.value = x.value.convert(Variable::Int);
            return (int)x.value.IntValue;
          } catch (...) {
            return 0;
          }
        }
      }
    }
    return 0;
  }
  return 0;
}
