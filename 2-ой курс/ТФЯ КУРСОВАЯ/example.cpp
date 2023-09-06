#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;

int hctoi(char c)
{
    if(isdigit(c))
        return c - 48;

    if(c == 'a' || c == 'A')
        return 10;

    if(c == 'b' || c == 'B')
        return 11;

    if(c == 'c' || c == 'C')
        return 12;

    if(c == 'd' || c == 'D')
        return 13;

    if(c == 'e' || c == 'E')
        return 14;

    if(c == 'f' || c == 'F')
        return 15;

    return 0;
}

int bstoi(char* buf)
{
    int res = 0;
    size_t buf_size = strlen(buf);
    int counter = 0;
    for(int i = buf_size - 1; i >= 0; i--)
        res |= (buf[i] - 48) << (counter++);
    
    return res;
}

int ostoi(char* buf)
{
    int res = 0;
    size_t buf_size = strlen(buf);
    int counter = 0;
    for(int i = buf_size - 1; i >= 0; i--)
        res |= (buf[i] - 48) << (3 * counter++);

    return res;
}

int hstoi(char* buf)
{
    int res = 0;
    size_t buf_size = strlen(buf);
    int counter = 0;
    for(int i = buf_size - 1; i >= 0; i--)
        res |= hctoi(buf[i]) << (4 * counter++);

    return res;
}

bool isbin(char c)
{
    return c == '0' || c == '1';
}

bool isbin(char* buf)
{
    for(int i = 0; buf[i]; i++)
        if(!isbin(buf[i]))
            return false;

    return true;
}

bool isoct(char c)
{
    return c >= '0' && c <= '7';
}

bool isoct(char* buf)
{
    for(int i = 0; buf[i]; i++)
        if(!isoct(buf[i]))
            return false;

    return true;
}

bool ishex(char c)
{
    return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

bool ishex(char* buf)
{
    for(int i = 0; buf[i]; i++)
        if(!ishex(buf))
            return false;

    return true;
}

// Типы лексем
enum lex_type
{
    LEX_NULL,       // 0
    // КЛЮЧЕВЫЕ СЛОВА
    LEX_AND,        // 1
    LEX_BEGIN,      // 2
    LEX_BOOL,       // 3
    LEX_DO,         // 4
    LEX_ELSE,       // 5
    LEX_END,        // 6
    LEX_IF,         // 7
    LEX_FALSE,      // 8
    LEX_INT,        // 9
    LEX_NOT,        // 10
    LEX_OR,         // 11
    LEX_PROGRAM,    // 12
    LEX_READ,       // 13
    LEX_THEN,       // 14
    LEX_TRUE,       // 15
    LEX_VAR,        // 16
    LEX_WHILE,      // 17
    LEX_WRITE,      // 18
    // МАРКЕР КОНЦА ТЕКСТА ПРОГРАММЫ
    LEX_FIN,        // 19
    // ОПЕРАТОРЫ И РАЗДЕЛИТЕЛИ
    LEX_SEMICOLON,  // 20
    LEX_COMMA,      // 21
    LEX_COLON,      // 22
    LEX_ASSIGN,     // 23
    LEX_LPAREN,     // 24
    LEX_RPAREN,     // 25
    LEX_EQ,         // 26
    LEX_LSS,        // 27
    LEX_GTR,        // 28
    LEX_PLUS,       // 29
    LEX_MINUS,      // 30
    LEX_TIMES,      // 31
    LEX_SLASH,      // 32
    LEX_LEQ,        // 33
    LEX_NEQ,        // 34
    LEX_GEQ,        // 35
    // ЧИСЛОВАЯ КОНСТАНТА
    LEX_NUM,        // 36
    // ИДЕНТИФИКАТОР
    LEX_ID,         // 37
    //ДОБАВЛЕННЫЕ ЛЕКСЕМЫ СОГЛАСНО ВАРИАНТУ
    LEX_REAL,       // 38
    LEX_FOR,        // 39
    LEX_TO,         // 40
    LEX_STEP,       // 41
    LEX_NEXT,       // 42
    LEX_NUMREAL,    // 43
    LEX_EXP,        //  44
};

// Класс "Лексема"
class Lex
{
    lex_type type;
    int value;

public:
    Lex(lex_type t = LEX_NULL, int v = 0)
    {
        type = t;
        value = v;
    }
    lex_type getType()
    {
        return type;
    }
    int getValue()
    {
        return value;
    }
    friend ostream& operator << (ostream & s, Lex l)
    {
        s << " (" << l.type << ',';
        if(l.type == LEX_NUMREAL)
            s << *(float*)&l.value << ") ";
        else
            s << l.value << ") ";
        return s;
    }

    string to_string()
    {
        stringstream ss;
        ss << *this;
        return ss.str();
    }
};

// Класс "Идентификатор"
class Id
{
    char * name;
    bool declared;
    bool assigned;
    lex_type type;
    int value;

public:
    Id()
    {
        declared = false;
        assigned = false;
    }
    char * getName()
    {
        return name;
    }
    void setName(const char * name)
    {
        this->name = new char[strlen(name) - 1];
        strcpy(this->name, name);
    }
    lex_type getType()
    {
        return type;
    }
    void setType(lex_type type)
    {
        this->type = type;
    }
    bool isDeclared()
    {
        return declared;
    }
    void setDeclared()
    {
        declared = true;
    }
    bool isAssigned()
    {
        return assigned;
    }
    void setAssigned()
    {
        assigned = true;
    }
    int getValue()
    {
        return value;
    }
    void setValue(int value)
    {
        this->value = value;
    }
};

// Класс "Таблица идентификаторов"
class TableId
{
    Id * p;
    int size;
    int top;

public:
    TableId(int max_size)
    {
        size = max_size;
        p = new Id[size];
        top = 1;
    }
    ~TableId()
    {
        delete [] p;
    }
    Id& operator [] (int k)
    {
        return p[k];
    }
    int put(const char * name);
};

int TableId::put(const char * name)
{
    for(int i = 1; i < top; ++i) {
        if (!strcmp(name, p[i].getName())) {
            return i;
        }
    }
    p[top].setName(name);
    top++;
    return top - 1;
}

////////////////////////////
// ЛЕКСИЧЕСКИЙ АНАЛИЗАТОР //
////////////////////////////

class Lexer
{
    enum state {
        H,
        ID,
        FLT,
        DEC,
        OCT,
        HEX,
        BIN,
        EXP,
        COM,
        ALE,
        DELIM,
    }; // состояния
    state CS;
    static const char * TW[]; // таблица ключевых слов
    static const char * TD[]; // таблица разделителей
    static lex_type words[]; // таблица типов ключевых слов
    static lex_type dlms[]; // таблица типов разделителей
    FILE * fp; // файл с кодом
    char c; // текущий символ
    char buf[80]; // буфер лексемы
    int buf_top; // индекс последнего символа в массиве buf

    int curline_num = 1;
    int curchar_num = 1;

    void clear() // метод очистки buf
    {
        buf_top = 0;
        for(int i = 0; i < 80; ++i) {
            buf[i] = '\0';
        }
    }

    void add() // метод добавления в buf текущего символа
    {
        buf[buf_top++] = c;
    }

    int look(const char * buf, const char ** list) // метод поиска строки buf в таблице лексем/разделителей
    {
        int i = 0;
        while(list[i]) {
            if (!strcmp(buf, list[i])) {
                return i;
            }
            i++;
        }
        return 0;
    }

    void gc() // метод получения следующего символа
    {
        c = (char) fgetc(fp);
        curchar_num++;
        if(c == '\n')
        {
            curline_num++;
            curchar_num = 1;
        }
    }

public:

    Lexer(const char * filename)
    {
        fp = fopen(filename, "r");
        if(!fp)
            throw string("#001 - File ") + filename + " was not found!";

        CS = H;
        clear();
        gc();
    }
    ~Lexer()
    {
        fclose(fp);
    }

    Lex getLex();
};

// Таблица ключевых слов
const char * Lexer::TW[] = {
    "",
    "dim",
    "integer",
    "real",
    "boolean",
    "begin",
    "end",
    "if",
    "else",
    "for",
    "to",
    "step",
    "next",
    "while",
    "readln",
    "writeln",
    "true",
    "false",
    NULL
};

// Таблица разделителей
const char * Lexer::TD[] = {
    "",
    ":=",
    "!=",
    "==",
    "<",
    "<=",
    ">",
    ">=",
    "+",
    "-",
    "||",
    "*",
    "/",
    "&&",
    "!",
    ":",
    ";",
    "(",
    ")",
    ",",
    NULL
};

// Таблица идентификаторов
TableId TID(100);

// Таблица типов ключевых слов
lex_type Lexer::words[] = {
    LEX_NULL,
    LEX_VAR,
    LEX_INT,
    LEX_REAL,
    LEX_BOOL,
    LEX_BEGIN,
    LEX_END,
    LEX_IF,
    LEX_ELSE,
    LEX_FOR,
    LEX_TO,
    LEX_STEP,
    LEX_NEXT,
    LEX_WHILE,
    LEX_READ,
    LEX_WRITE,
    LEX_TRUE,
    LEX_FALSE,
    LEX_NULL
};

// Таблица типов разделителей
lex_type Lexer::dlms[] = {
    LEX_NULL,
    LEX_ASSIGN,
    LEX_NEQ,
    LEX_EQ,
    LEX_LSS,
    LEX_LEQ,
    LEX_GTR,
    LEX_GEQ,
    LEX_PLUS,
    LEX_MINUS,
    LEX_OR,
    LEX_TIMES,
    LEX_SLASH,
    LEX_AND,
    LEX_NOT,
    LEX_COLON,
    LEX_SEMICOLON,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_COMMA,
    LEX_NULL
};

// Основная функция лексического разбора
Lex Lexer::getLex()
{
    if(feof(fp))
        throw string("#250 - Missing 'end' keyword in the end of program!");

    CS = H;
    do {
        switch(CS) {
            case H:
                if(c == ' ' || c == '\t' || c == '\n' || c == '\r')
                {
                    gc();
                }

                else if(c == '_' || isalpha(c))
                {
                    CS = ID;

                    clear();
                    add();
                    gc();
                }

                else if(isnumber(c))
                {
                    if(c == '0')
                        CS = BIN;
                    else
                        CS = DEC;

                    clear();
                    add();
                    gc();
                }

                else if(c == '.')
                {
                    CS = FLT;

                    clear();
                    add();
                    gc();
                }

                else if(c == '(')
                {
                    gc();
                    if(c == '*')
                    {
                        CS = COM;
                        gc();
                    }
                    else
                    {
                        int idx = look("(", TD);
                        return Lex(LEX_LPAREN, idx);
                    }
                }

                else if(c == ':' || c == '>' || c == '<' || c == '=' || c == '!')
                {
                    CS = ALE;

                    clear();
                    add();
                    gc();
                }

                else
                {   
                    CS = DELIM;

                    clear();
                    add();
                    gc();
                }
                break;

            case ID:
                if(c == '_' || isalpha(c) || isnumber(c))
                {
                    add();
                    gc();
                }
                else
                {
                    int idx = look(buf, TW);
                    if(idx)
                        return Lex(words[idx], idx);
                    else
                        return Lex(LEX_ID, TID.put(buf));
                }
                break;
            
            case BIN:
                if(isbin(c))
                {
                    add();
                    gc();
                }
                else if(isoct(c))
                {
                    CS = OCT;

                    add();
                    gc();
                }
                else if(ishex(c))
                {
                    CS = HEX;

                    add();
                    gc();
                }
                else
                {
                    return Lex(LEX_NUM, bstoi(buf));
                }
                break;

            case OCT:
                if(isoct(c))
                {
                    add();
                    gc();
                }
                else if(ishex(c))
                {
                    CS = HEX;

                    add();
                    gc();
                }
                else
                {
                    return Lex(LEX_NUM, ostoi(buf));
                }
                break;

            case DEC:
                if(isalpha(c))
                {
                    add();
                    gc();
                }
                else if(c == '.')
                {
                    CS = FLT;

                    add();
                    gc();
                }
                else if(ishex(c))
                {
                    CS = HEX;

                    add();
                    gc();
                }
                else
                {
                    return Lex(LEX_NUM, stoi(buf));
                }
                break;

            case HEX:
                if(ishex(c))
                {
                    add();
                    gc();
                }
                else
                {
                    return Lex(LEX_NUM, hstoi(buf));
                }
                break;

            case FLT:
                if(isnumber(c))
                {
                    add();
                    gc();
                }
                else if(c == 'e' || c == 'E')
                {
                    CS = EXP;

                    add();
                    gc();
                }
                else
                {
                    float temp = stof(buf);
                    return Lex(LEX_NUMREAL, *(int*)&temp);
                }
                break;

            case EXP:
                if(isnumber(c))
                {
                    add();
                    gc();
                }
                else
                {
                    float temp = stof(buf);
                    return Lex(LEX_NUMREAL, *(int*)&temp);
                }
                break;

            case COM:
                if(c == '*')
                {
                    gc();
                    if(c == ')')
                    {
                        gc();
                        CS = H;
                    }
                    else
                    {
                        throw string("#101 - Missing comment closing symbol ')'");
                    }
                }
                gc();
                break;

            case ALE:
                if(c == '=')
                {
                    add();
                    gc();

                    int idx = look(buf, TD);
                    return Lex(dlms[idx], idx);
                }
                else
                {   
                    int idx = look(buf, TD);
                    return Lex(dlms[idx], idx);
                }
                break;

            case DELIM:
                int idx = look(buf, TD);

                if(idx)
                {
                    return Lex(dlms[idx], idx);
                }
                else
                {
                    add();
                    gc();

                    idx = look(buf, TD);
                    if(idx)
                        return Lex(dlms[idx], idx);
                    
                    throw string("#102 - Unknown delimeter: ") + " " + buf;
                }

                break;
        }
    }
    while(true);
}

///////////////////////////////
// СИНТАКСИЧЕСКИЙ АНАЛИЗАТОР //
///////////////////////////////

class Parser
{
    Lex curr_lex;       // текущая лексема
    lex_type c_type;    // её тип
    int c_val;          // её значение
    Lexer lexer;

    int stack[100];     // стек переменных для контроля повторного объявления
    int top;

    // рекурсивные функции
    void P();
    void D();
    void S();
    void E();
    void E1();
    void T();
    void F();

    // Получить очередную лексему
    void gl()
    {
        curr_lex = lexer.getLex();
        c_type = curr_lex.getType();
        c_val = curr_lex.getValue();
    }

    void reset()
    {
        top = 0;
    }

    void push(int i)
    {
        stack[top] = i;
        ++top;
    }

    int pop()
    {
        --top;
        return stack[top];
    }

    void dec(lex_type type)
    {
        int i;
        while(top) {
            i = pop();
            if (TID[i].isDeclared())
                throw string("#301 - Variable ") + TID[i].getName() + " is decleared twice!";

            TID[i].setDeclared();
            TID[i].setType(type);
        }
    }

public:

    // Провести синтаксический разбор
    void analyze()
    {
        gl();
        P();
    }

    // Конструктор
    Parser(const char * filename) : lexer(filename), top(0) {}
};

void Parser::P()
{
    do
    {
        if(c_type == LEX_VAR)
            D();
        else
            S();
        
    } while (c_type != LEX_END);
}

void Parser::D()
{
    reset();
    do
    {
        gl();

        if(c_type != LEX_ID)
            throw string("#201 - Missing variable name!");
        
        push(c_val);
        gl();
    } while (c_type == LEX_COMMA);
    
    if(c_type != LEX_INT && c_type != LEX_REAL && c_type != LEX_BOOL)
        throw string("#202 - Missing { integer | real | boolean } type!");

    dec(c_type);

    gl();
}

void Parser::S()
{
    if (c_type == LEX_IF) {
        gl();

        if(c_type != LEX_LPAREN)
            throw string("#203 - Missing '(' after 'if' statement!");

        gl();
        E();

        if (c_type != LEX_RPAREN) 
            throw string("#210 - Missing ')' after expression!");

        gl();
        S();

        if (c_type == LEX_ELSE)
        {
            gl();
            S();
        }
    }
    else if (c_type == LEX_WHILE) {
        gl();

        if(c_type != LEX_LPAREN)
            throw string("#204 - Missing '(' after 'while' statement!");

        gl();
        E();

        if (c_type == LEX_RPAREN)
            throw string("#210 - Missing ')' after expression!");

        gl();
        S();
    }
    else if (c_type == LEX_READ) {
        do
        {
            gl();

            if(c_type != LEX_ID)
                throw string("#201 - Missing variable name!");

            if(!TID[c_val].isAssigned())
                throw string("#320 - Reading unassigned variable '") + TID[c_val].getName() + "' !";

        } while (c_type == LEX_COMMA);
    }
    else if (c_type == LEX_WRITE) {
        do
        {
            gl();
            E();

        } while (c_type == LEX_COMMA);
    }
    else if (c_type == LEX_ID) {
        auto& curr_id = TID[c_val];

        if(!curr_id.isDeclared())
            throw string("#302 - using undeclared variable '") + curr_id.getName() + "' !";

        gl();
        
        if (c_type != LEX_ASSIGN)
            throw string("#211 - Missing assignment symbol ':=' after variable!");
        
        gl();
        E();

        curr_id.setAssigned();
    }
    else if(c_type == LEX_BEGIN) {
        do
        {
            gl();
            S();
        } while (c_type == LEX_SEMICOLON);

        if(c_type != LEX_END)
            throw string("#250 - Missing 'end' keyword!");

        gl();
    }
    else
        throw string("#299 - Unkown operator!");
}

void Parser::E()
{
    E1();
    if (c_type == LEX_EQ ||
        c_type == LEX_LSS ||
        c_type == LEX_GTR ||
        c_type == LEX_LEQ ||
        c_type == LEX_GEQ ||
        c_type == LEX_NEQ)
    {
        gl();
        E1();
    }
}

void Parser::E1()
{
    T();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
        gl();
        T();
    }
}

void Parser::T()
{
    F();
    while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND) {
        gl();
        F();
    }
}

void Parser::F()
{
    if (c_type == LEX_ID) {
        if(!TID[c_val].isDeclared())
            throw string("#302 - using undeclared variable '") + TID[c_val].getName() + "' !";

        if(!TID[c_val].isAssigned())
            throw string("#320 - Reading unassigned variable '") + TID[c_val].getName() + "' !";
        
        gl();
    }
    else if (c_type == LEX_NUM || c_type == LEX_NUMREAL) {
        gl();
    }
    else if (c_type == LEX_TRUE || c_type == LEX_FALSE) {
        gl();
    }
    else if (c_type == LEX_NOT) {
        gl();
        F();
    }
    else if (c_type == LEX_LPAREN) {
        gl();
        E();
        if (c_type != LEX_RPAREN) {
            throw string("#210 - Missing symbol ')' after expression");
        }
        gl();
    }
    else {
        throw string("#212 - Missing expression!");
    }
}


int main()
{
    try {
        Parser parser("example.txt");
        parser.analyze();
        cout << "Ok" << endl;
    } catch ( const string& what )
    {
        std::cerr << what << endl;
    }
    return 0;
}