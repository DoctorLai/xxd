#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <sys/stat.h>

using namespace std;

void help() {
    cout << "xxd:  make a hexdump" << endl;
    cout << "visit https://HelloACM.com" << endl;
    cout << "-u uppercase" << endl;
    cout << "-c column" << endl;
    cout << "-h help" << endl;
}

inline bool file_exists(const string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

string tohex(int x, int w = 8, bool upper = false) {
    stringstream stream;
    stream << std::hex << (x & 0xFF);
    string result(stream.str());
    if (upper) {
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    }
    int j = w - result.length();
    while (j-- > 0) { // zero padding
        result = '0' + result;
    }
    return result;
}

void print_xxd(string content, int w = 16, bool upper = false) {
    int len = static_cast<int>(content.size());
    int num = 0;
    string curline = "";
    int width = 9 + (w / 2) * 5 + 1;
    if (w & 1 == 1) {
        width += 3;
    }
    int cw = 1;
    int kk = 1;
    for (int i = 0; i < len ; i ++) {
        if ((i % w) == 0) {
            cout << tohex(num) << ": ";
            cw = 9;
        }
        char t = content[i];
        cout << tohex((int)t, 2, upper);
        cw += 2;
        curline += t;
        if ((i & 1) == kk) {
            cout << " ";
            cw += 1;
        }
        if ((i % w) == (w - 1)) {
            num += w;
            cout << "  ";
            cw += 2;
            int k = i - w + 1;
            for (int j = 0; j < w; j ++) {
                t = content[k ++];
                if ((int)t < 32) { // non-printable characters
                    t = '.';
                }
                cout << t;
                cw ++;
            }
            cout << "\n\r";
            curline = "";
            cw = 0;
            if (w & 1 == 1) {
                kk = kk == 1 ? 0 : 1;
            }
        }
    }
    // remaining characters;
    int j = width - cw;
    while (j-- > 0) {
        cout << " ";
    }
    for (int i = 0; i < static_cast<int>(curline.size()); i ++) {
        char t = content[i];
        if ((int)t < 32) { // non-printable characters
            t = '.';
        }
        cout << t;
    }
}

std::string get_file_contents(const char *filename)
{
      std::ifstream in(filename, std::ios::in | std::ios::binary);
      if (in) {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return(contents);
      }
      throw(errno);
}

void xxd(string filename, int w = 16, bool upper = false) {
    if (!file_exists(filename)) {
        cout << "xxd: " << filename << ": No such file or directory" << endl;
        return;
    }
    print_xxd(get_file_contents(filename.c_str()), w, upper);
}

int main(int argc, char ** argv)
{
    bool upper = false;
    int w = 16;
    bool io = true;
    for (int i = 2; i <= argc; i ++) {
        string cur = argv[i - 1];
        if (cur == "-h") {
            help();
            return 0;
        }
        if (cur == "-u") {
            upper = true;
        }
        else if ((cur[0] == '-') && (cur.length() > 1)) {
            if (cur[1] == 'c') {
                if (cur.length() > 2) {
                    string num = cur.substr(2);
                    std::stringstream str(num);
                    int x;
                    str >> x;
                    if (str) {
                        if (x > 0) {
                            w = x;
                        }
                    }
                } else {
                    if (i != argc) {
                        string num = string(argv[i]);
                        std::stringstream str(num);
                        int x;
                        str >> x;
                        if (str) {
                            if (x > 0) {
                                w = x;
                            }
                        }
                        i ++;
                    }
                }
            }
        }
        else {
            xxd(cur, w, upper);
            io = false;
        }
    }
    if (io) {
        // standard input
        std::string line;
        std::string all = "";
        while (std::getline(std::cin, line))
        {
            all += line + "\n";
        }
        print_xxd(all, w, upper);
		cout << endl;
    }
    return 0;
}
