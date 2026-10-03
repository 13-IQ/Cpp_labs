#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <thread>
#include <mutex>
#include <chrono>
#include <memory>
#include <iomanip>
#include <ctime>
#include <cctype>

using namespace std;

//Контекст выполнения
class Context {
    map<string, double> vars;
    mutable mutex mtx;
public:
    void setVar(const string& name, double val) {
        lock_guard<mutex> lock(mtx);
        vars[name] = val;
    }
    double getVar(const string& name) const {
        lock_guard<mutex> lock(mtx);
        auto it = vars.find(name);
        return (it != vars.end()) ? it->second : 0.0;
    }
};

mutex coutMutex;
mutex fileMutex;

//Базовый класс команды
class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Context& ctx) = 0;
};

//Парсер арифметических выражений
class ExprParser {
    string s;
    size_t pos;

    void skip() { while (pos < s.size() && s[pos] == ' ') pos++; }

    double factor(const Context& ctx) {
        skip();
        if (pos < s.size() && s[pos] == '(') {
            pos++;
            double v = expr(ctx);
            skip();
            if (pos < s.size() && s[pos] == ')') pos++;
            return v;
        }
        if (pos < s.size() && (isdigit(s[pos]) || s[pos] == '.' || s[pos] == '-')) {
            int start = pos;
            if (s[pos] == '-') pos++;
            while (pos < s.size() && (isdigit(s[pos]) || s[pos] == '.')) pos++;
            return stod(s.substr(start, pos - start));
        }
        if (pos < s.size() && (isalpha(s[pos]) || s[pos] == '_')) {
            int start = pos;
            while (pos < s.size() && (isalnum(s[pos]) || s[pos] == '_')) pos++;
            return ctx.getVar(s.substr(start, pos - start));
        }
        return 0.0;
    }

    double term(const Context& ctx) {
        double v = factor(ctx);
        while (true) {
            skip();
            if (pos < s.size() && s[pos] == '*') { pos++; v *= factor(ctx); }
            else if (pos < s.size() && s[pos] == '/') {
                pos++; double d = factor(ctx); 
                if (d != 0.0) v /= d; else { cerr << "Ошибка: деление на ноль\n"; v = 0; }
            }
            else break;
        }
        return v;
    }

    double expr(const Context& ctx) {
        double v = term(ctx);
        while (true) {
            skip();
            if (pos < s.size() && s[pos] == '+') { pos++; v += term(ctx); }
            else if (pos < s.size() && s[pos] == '-') { pos++; v -= term(ctx); }
            else break;
        }
        return v;
    }

public:
    double parse(const string& input, const Context& ctx) {
        s = input; pos = 0;
        return expr(ctx);
    }
};

//Конкретные команды

class SetCommand : public Command {
    string name, exprStr;
public:
    SetCommand(const string& n, const string& e) : name(n), exprStr(e) {}
    void execute(Context& ctx) override {
        ExprParser p;
        ctx.setVar(name, p.parse(exprStr, ctx));
    }
};

class PrintCommand : public Command {
    string exprStr;
public:
    PrintCommand(const string& e) : exprStr(e) {}
    void execute(Context& ctx) override {
        ExprParser p;
        double v = p.parse(exprStr, ctx);
        lock_guard<mutex> lock(coutMutex);
        cout << v << '\n';
    }
};

class FileCommand : public Command {
    string path, exprStr;
public:
    FileCommand(const string& p, const string& e) : path(p), exprStr(e) {}
    void execute(Context& ctx) override {
        ExprParser p;
        double v = p.parse(exprStr, ctx);
        lock_guard<mutex> lock(fileMutex);
        ofstream f(path, ios::app);
        if (f.is_open()) f << v << '\n';
        else {
            lock_guard<mutex> coutLock(coutMutex);
            cerr << "Ошибка открытия файла: " << path << '\n';
        }
    }
};

class ForCommand : public Command {
    string varName, fromStr, toStr;
    vector<shared_ptr<Command>> body;
public:
    ForCommand(const string& v, const string& f, const string& t)
        : varName(v), fromStr(f), toStr(t) {}
    
    void addCmd(shared_ptr<Command> c) { body.push_back(c); }
    
    void execute(Context& ctx) override {
        ExprParser p;
        int from = (int)p.parse(fromStr, ctx);
        int to = (int)p.parse(toStr, ctx);
        for (int i = from; i <= to; i++) {
            ctx.setVar(varName, i);
            for (auto& cmd : body) {
                cmd->execute(ctx);
            }
        }
    }
};

bool startsWith(const string& s, const string& prefix) {
    return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
}

string trim(const string& s) {
    size_t a = 0, b = s.size() - 1;
    while (a <= b && s[a] == ' ') a++;
    while (b >= a && s[b] == ' ') b--;
    return (a <= b) ? s.substr(a, b - a + 1) : "";
}

string currentTime() {
    static const auto programStart = chrono::steady_clock::now();
    auto now = chrono::steady_clock::now();
    auto ms = chrono::duration_cast<chrono::microseconds>(now - programStart).count();
    return to_string(ms) + " мс";
}

shared_ptr<Command> parseLine(const string& line) {
    string t = trim(line);
    if (t.empty()) return nullptr;

    string pVivod = "вывод ";
    if (startsWith(t, pVivod))
        return make_shared<PrintCommand>(trim(t.substr(pVivod.size())));

    // "файл "<путь>" <выражение>"
    string pFile = "файл ";
    if (startsWith(t, pFile)) {
        string rest = t.substr(pFile.size());
        size_t q1 = rest.find('"');
        size_t q2 = rest.find('"', q1 + 1);
        if (q1 != string::npos && q2 != string::npos) {
            string path = rest.substr(q1 + 1, q2 - q1 - 1);
            string expr = trim(rest.substr(q2 + 1));
            return make_shared<FileCommand>(path, expr);
        }
    }

    // "<имя> = <выражение>"
    size_t eq = t.find('=');
    if (eq != string::npos) {
        string name = trim(t.substr(0, eq));
        string expr = trim(t.substr(eq + 1));
        return make_shared<SetCommand>(name, expr);
    }

    return nullptr;
}

// Выполнение блока в потоке
void executeBlock(int blockNum, const vector<shared_ptr<Command>>& cmds, Context& ctx) {
    {
        lock_guard<mutex> lock(coutMutex);
        cout << "[Блок " << blockNum << "] Старт: " << currentTime() << '\n';
    }
    for (auto& cmd : cmds) {
        cmd->execute(ctx);
    }
    {
        lock_guard<mutex> lock(coutMutex);
        cout << "[Блок " << blockNum << "] Финиш: " << currentTime() << '\n';
    }
}

int main() {
    cout << " Мини-язык PPLang " << '\n';
    cout << "Команды: <имя> = <выражение> | вывод <выражение> | файл \"<путь>\" <выражение>" << '\n';
    cout << "Цикл: цикл <имя> от <начало> до <конец> ... конец" << '\n';
    cout << "Пустая строка = новый блок (поток). Команда 'запуск' = выполнение." << '\n';
    cout << "----------------------------------------" << '\n';

    vector<vector<shared_ptr<Command>>> blocks;
    vector<shared_ptr<Command>> currentBlock;
    vector<shared_ptr<ForCommand>> forStack;

    string line;
    while (getline(cin, line)) {
        string t = trim(line);

        if (t == "запуск") break;

        // "цикл <имя> от <начало> до <конец>"
        string pCikl = "цикл ";
        if (startsWith(t, pCikl)) {
            string rest = t.substr(pCikl.size());
            istringstream iss(rest);
            string var, ot, from, doStr, to;
            iss >> var >> ot >> from >> doStr >> to;
            forStack.push_back(make_shared<ForCommand>(var, from, to));
            continue;
        }

        if (t == "конец") {
            if (!forStack.empty()) {
                auto forCmd = forStack.back();
                forStack.pop_back();
                if (forStack.empty())
                    currentBlock.push_back(forCmd);
                else
                    forStack.back()->addCmd(forCmd);
            }
            continue;
        }

        auto cmd = parseLine(t);
        if (cmd) {
            if (!forStack.empty())
                forStack.back()->addCmd(cmd);
            else
                currentBlock.push_back(cmd);
        } else if (t.empty()) {
            if (!currentBlock.empty()) {
                blocks.push_back(currentBlock);
                currentBlock.clear();
            }
        }
    }
    if (!currentBlock.empty()) blocks.push_back(currentBlock);

    Context ctx;
    vector<thread> threads;
    for (size_t i = 0; i < blocks.size(); i++) {
        threads.emplace_back(executeBlock, i + 1, blocks[i], ref(ctx));
    }
    for (auto& t : threads) {
        t.join();
    }

    cout << "=== Все потоки завершены ===" << '\n';
    return 0;
}
