#include<bits/stdc++.h>
#include<conio.h>
#include<windows.h>
using namespace std;


//define colors------------------------------------------------------------------------------------------------------------------------------------
#define RESET        "[0m"
#define BLACK        "[30m"
#define RED          "[31m"
#define GREEN        "[32m"
#define YELLOW       "[33m"
#define BLUE         "[34m"
#define MAGENTA      "[35m"
#define CYAN         "[36m"
#define WHITE        "[37m"
#define BRIGHT_BLACK "[90m"
#define BRIGHT_RED   "[91m"
#define BRIGHT_GREEN "[92m"
#define BRIGHT_YELLOW "[93m"
#define BRIGHT_BLUE  "[94m"
#define BRIGHT_MAGENTA "[95m"
#define BRIGHT_CYAN  "[96m"
#define BRIGHT_WHITE "[97m"

string color(string s, string color){
    return "\033"+color+s+"\033[0m";
}








//take input------------------------------------------------------------------------------------------------------------------------------------
string input(){
    string content;
    int choice;
    cout << "\n\n\t\tInput options\n";
    cout << "\t\t=============\n\n";
    cout << "[1] Manual input\n";
    cout << "[2] File input\n";
    cout << "\nChoose an option: ";
    try {
        cin >> choice;

        if (cin.fail() || choice<1 || choice>2) {
            throw invalid_argument(color("Invalid input! Please enter a valid option.\n\n", RED));
        }
    }
    catch (const invalid_argument& e) {
        cout << e.what() << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if(choice == 1) {
        string str1="";
        string line="";
        cout<<"[write 'end' on a new line to stop input]\n\n";
        cout<<"Enter input: ";
        while(1)
        {
            cin.ignore();
            getline(cin, line);
            if (line=="end"){
                break;
            }
            str1=str1+line+"\n";
        }
        content = str1;       
    }
    else if(choice == 2){
        string path = "";
        string newpath = "";
        
        cout<<"Enter file path: ";
        cin.ignore();
        getline(cin, path);
        for (int i = 0; i < path.length(); i++)
        {
            if (path.length() >= 2 && path.front() == '"' && path.back() == '"')
            path = path.substr(1, path.length() - 2);

            if (path[i] == '\\')
            {
                newpath = newpath + path[i] + '\\';
            }
            else
            {
                newpath = newpath + path[i];
            }
        }
        
        ifstream inputfile(newpath);
        string str;
        if (inputfile.is_open())
        {
            while (getline(inputfile, str))
            {
                content = content + str + "\n";
            }
            inputfile.close();
        }
        else
        {
            cout << "error" << endl;
        }
    }
    return content;
}






//remove comments------------------------------------------------------------------------------------------------------------------------------------
string remove_comments(string str1) {
    string str2 = "";
    bool single = false;
    bool multi = false;

    for (int i = 0; i < str1.length(); i++) {
        if (multi) {
            if (str1[i] == '*' && str1[i + 1] == '/') {
                multi = false;
                i++;
                if(str1[i] == '\n') i++;
            }
        } else if (single) {
            if (str1[i] == '\n') {
                single = false;
                i++;
            }
        } else {
            if (str1[i] == '/' && str1[i + 1] == '*') {
                multi = true;
                i++;
            } else if (str1[i] == '/' && str1[i + 1] == '/') {
                single = true;
                i++;
            } else {
                str2 = str2 + str1[i];
            }
        }
    }
    return str2;
}










//parse lexemes from given code------------------------------------------------------------------------------------------------------------------------------------
vector<string> get_lexemes(string a){
    string lexeme = "";
    int n = a.length();
    vector<string>lexemes;

    for(int i=0;i<n;i++){
        if(isalnum(a[i]) || a[i] == '_') lexeme+=a[i];
        else if(a[i] == ' ' || a[i] == 10){
            if(lexeme.length()>0) lexemes.push_back(lexeme);
            lexeme = "";
        }
        else if(a[i] == '\"'){
            lexeme += a[i];
            i++;
            while(i < n && a[i] != '\"'){
                lexeme += a[i];
                i++;
            }
            if(i < n) {
                lexeme += a[i];
            }
            lexemes.push_back(lexeme);
            lexeme = "";
        }
        else if(a[i] == '\''){
            lexeme = a.substr(i, 3);
            lexemes.push_back(lexeme);
            i+=2;
            lexeme = "";
        }
        else{
            if(lexeme.length()>0) lexemes.push_back(lexeme);
            lexeme = a[i];
            lexemes.push_back(lexeme);
            lexeme = "";
        }
    }
    return lexemes;
}
























//check category of lexeme------------------------------------------------------------------------------------------------------------------------------------
bool is_keyword(string s){
    static const unordered_set<string> keywords = {
        "alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel", "atomic_commit",
        "atomic_noexcept", "auto", "bitand", "bitor", "bool", "break", "case", "catch",
        "char", "char8_t", "char16_t", "char32_t", "class", "compl", "concept", "const",
        "consteval", "constexpr", "constinit", "const_cast", "continue", "co_await",
        "co_return", "co_yield", "decltype", "default", "delete", "do", "double", "dynamic_cast",
        "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend",
        "goto", "if", "include", "inline", "int", "long", "mutable", "namespace", "new", "noexcept",
        "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected",
        "public", "register", "reinterpret_cast", "requires", "return", "short", "signed",
        "sizeof", "static", "static_assert", "static_cast", "string", "struct", "switch", "synchronized",
        "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid",
        "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t",
        "while", "xor", "xor_eq"
    };
    return keywords.find(s) != keywords.end();
}


bool is_identifier(string s) {
    if (!std::isalpha(s[0]) && s[0] != '_') {
        return false;
    }
    for (size_t i = 1; i < s.size(); ++i) {
        if (!std::isalnum(s[i]) && s[i] != '_') {
            return false;
        }
    }
    return true;
}

bool is_sliteral(string s){
    if(s[0] == '\"') return true;
    return false;
}

bool is_cliteral(string s){
    if(s[0] == '\'') return true;
    return false;
}

bool is_numeric(const string& str) {
    if (str.empty()) return false;
    try {
        stod(str);
        return true;
    } 
    catch (const invalid_argument& e) {
        return false;
    }
}

bool is_operator(string s) {
    static const unordered_set<string> operators = {
        "+", "-", "*", "/", "%", "=", "<", ">","&", "|", "^", "~","!", ":", "?", "#"
    };
    return operators.find(s) != operators.end();
}

bool is_punctuation(string s) {
    static const unordered_set<string> operators = {
        "(", ")", "{", "}", "[", "]", ",", ";", "."
    };
    return operators.find(s) != operators.end();
}









//classify lexeme------------------------------------------------------------------------------------------------------------------------------------
string classify_lexeme(string s){
    if(is_keyword(s)) return "Keyword";
    if(is_identifier(s)) return "Identifier";
    if(is_sliteral(s)) return "String";
    if(is_cliteral(s)) return "Char";
    if(is_numeric(s)) return "Numeric";
    if(is_operator(s)) return "Operator";
    if(is_punctuation(s)) return "Punctuation";
    return "Unknown";
}






//classify tokens------------------------------------------------------------------------------------------------------------------------------------
vector<pair<string, string>> classify_tokens(vector<string>lexemes){
    vector<pair<string, string>>classified_tokens;
    pair<string,string> token;
    for(auto lexeme: lexemes){
        token.first = lexeme;
        token.second = classify_lexeme(lexeme);
        classified_tokens.push_back(token);
    }
    return classified_tokens;
}



//set colors------------------------------------------------------------------------------------------------------------------------------------
unordered_map<string, string> colors {
    {"Keyword", BLUE},
    {"Identifier", RED},
    {"String", GREEN},
    {"Char", CYAN},
    {"Numeric", BLACK},
    {"Operator", MAGENTA},
    {"Punctuation", BRIGHT_YELLOW}
};





//syntax highlighter------------------------------------------------------------------------------------------------------------------------------------
string syntax_highlighter(string a){
    string lexeme = "";
    int n = a.length();
    string result = "";
    string category = "";

    for(int i=0;i<n;i++){
        if(isalnum(a[i]) || a[i] == '_') lexeme+=a[i];
        else if(a[i] == ' ' || a[i] == 10){
            if(!lexeme.empty()){
                category = classify_lexeme(lexeme);
                result+=color(lexeme, colors[category]);
            }
            result+=a[i];
            lexeme = "";
        }
        else if(a[i] == '\"'){
            lexeme += a[i];
            i++;
            while(i < n && a[i] != '\"'){
                lexeme += a[i];
                i++;
            }
            if(i < n) {
                lexeme += a[i];
            }
            category = classify_lexeme(lexeme);
            result+=color(lexeme, colors[category]);
            lexeme = "";
        }
        else if(a[i] == '\''){
            lexeme = a.substr(i, 3);
            category = classify_lexeme(lexeme);
            result+=color(lexeme, colors[category]);
            i+=2;
            lexeme = "";
        }
        else{
            if(!lexeme.empty()){
                category = classify_lexeme(lexeme);
                result+=color(lexeme, colors[category]);
            }
            lexeme = a[i];
            category = classify_lexeme(lexeme);
            result+=color(lexeme, colors[category]);
            lexeme = "";
        }
    }
    return result;
}











//main app menu------------------------------------------------------------------------------------------------------------------------------------
void app() {
    for(;;) {
        int choice;
        cout << "\n\n\t\tProject Zero\n";
        cout << "\t\t============\n\n";
        cout << "[1] Remove comment\n";
        cout << "[2] Lexical analyzer\n";
        cout << "[3] Token classifier\n";
        cout << "[4] Syntax highlighter\n";
        cout << "[0] Exit\n";
        cout << "\nChoose an option: ";

        try {
            cin >> choice;

            if (cin.fail() || choice<0 || choice>4) {
                throw invalid_argument(color("Invalid input! Please enter a valid option.\n\n", RED));
            }
        }
        catch (const invalid_argument& e) {
            cout << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if(choice == 1) {//remove comments
            string s = input();
            cout<<color("\n\nComments removed successfully!\n\n\n", GREEN);
            Sleep(1000);
            cout<<remove_comments(s);
        }
        else if(choice == 2) {//lexical analysis
            string s = input();
            vector<string>lexemes = get_lexemes(s);
            cout<<"\n\n"<<lexemes.size()<<color(" Lexemes found!\n\n\n", GREEN);
            Sleep(1000);
            cout<<"Lexemes:\n";
            cout<<"========\n";
            for(auto i:lexemes) cout<<i<<endl;
        }
        else if(choice == 3) {//classify tokens
            string s = input();
            vector<string>lexemes = get_lexemes(s);
            vector<pair<string, string>>tokens = classify_tokens(lexemes);
            cout<<color("\n\nClassified tokens successfully!\n\n\n", GREEN);
            Sleep(1000);
            cout << left <<setw(15)<<"Class"<<" -\t"<<"Token"<< endl;
            cout <<"=============================="<< endl;
            for(auto token: tokens){
                cout << left <<setw(15)<<token.second<<" -\t"<<token.first<< endl;
            }
        }
        else if(choice == 4) {//syntax highlighter
            string s = input();
            string highlighted = syntax_highlighter(s);
            cout<<color("\n\nHighlighting code complete!\n\n\n", GREEN);
            Sleep(1000);
            cout<<highlighted<<endl;
        }
        else{
            cout<<color("\n\nThank you for using the app. Exiting...", GREEN);
            Sleep(1000);
            cout<<"\n\n";
            return;
        }
        std::cout << "\n\nPress any key to continue...";
    
        while (!_kbhit()) {
        }
        
        _getch();
        system("cls");
    }
}

int main() {
    app();
}