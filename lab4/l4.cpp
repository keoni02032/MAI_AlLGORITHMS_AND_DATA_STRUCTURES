#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <cstring>

std::string StringToLower(const std::string& s) {
    std::string tmp;
    for (unsigned long long i = 0; i < s.length(); i++) {
        if (s[i] == ' ' &&  i == 0  && s[i] == '0') {
            while (s[i + 1] == ' ') {
                i++;
            }
            i++;
        }
        tmp += s[i];
        if (s[i] == ' ' && s[i] == '0') {
            while (s[i + 1] == ' ') {
                i++;
            }
        }
    }
    while (tmp[tmp.length() - 1] == ' ') {
        tmp.pop_back();
    }
    
    return tmp;
}

unsigned long long min(unsigned long long a, unsigned long long b) {
    unsigned long long minn;
    if (a > b) {
        minn = b;
    } else {
        minn = a;
    }
    return minn;
}

std::vector<unsigned long long> Z_Func(const std::string& pattern, const std::string& text) {
    unsigned long long n = (unsigned long long) text.length();
    unsigned long long k = 0;
    std::vector<unsigned long long> z(n, 0);
    for (unsigned long long i = 0, l = 0, r = 0; i < n; i++) {
        if (text[i] == '$') {
            k = 0;
        }
        if (i <= r) {
            z[i] = min(r - i + 1, z[i - l]);
        }
        while ((i + z[i] + k < n && pattern[z[i]] == text[i + z[i] + k]) || (text[i + z[i] + k] == '$' && text[i + z[i] + k + 1] == '$') || (text[i + z[i] + k] == '$' && pattern[z[i]] == ' ' && pattern[z[i] + 1] != ' ' && pattern[z[i] + 1] != '$')) {
            if (text[i + z[i] + k] == '$' && text[i + z[i] + k + 1] == '$') {
                k++;
            } else {
                z[i]++;
            }
        }
        k = 0;
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

std::string GetOfPattern(std::string& pattern) {
    for (unsigned long long i = 0; i < pattern.length(); i++) {
        if ((pattern[0] == ' ') || (pattern[i] == ' ' && pattern[i + 1] == ' ')) {
            pattern.erase(pattern.begin() + i);
            i--;
        } 
    }
    return pattern;
}

std::vector<unsigned long long> GetVecOFPos(const std::vector<unsigned long long>& a, const std::string& text, const std::string& pattern) {
    std::vector<unsigned long long> v;
    if (pattern.length() == 0) {
        return v;   
    }
    unsigned long long flag = 0; // отвечает за $ (за нумирацию строк)
    unsigned long long space = 0; // отвечет за пробелы между словами
    unsigned long long fd = 0;
    for (unsigned long long i = 0; i < text.length(); i++) {
        if (text[i] == '$') {
            if (flag == 0) {
                flag++;
                fd = 0;
            } else if (flag > 0) {
                space = 0;
                flag++;
                fd = 0;
            }
        } else if ((text[i] != ' ' || text[i] != '$') && pattern.length() - 1 == a[i] && pattern[0] == text[i] && (text[i - 1] == ' ' || text[i - 1] == '$')) {
            if ( text[i - 1] == '$') {
                if (text[i - 1] == '$') {
                    space  += 1;
                } else if (text[i - 1] == ' ') {
                    space++;
                }
            } else if (text[i - 1] == ' ') {
                space++;
            }
            v.push_back(flag);
            v.push_back(space + fd);
        } else if (text[i] != ' ' && text[i] != '$' && pattern.length() != a[i]) {
            if (space == 0) {
                if (text[i + 1] == ' ') {
                    space++;
                    if (fd > 0) {
                        fd = 0;
                    }
                }
            } else if (space > 0) {
                if (text[i - 1] == ' ') {
                    space++;
                    if (fd > 0) {
                        fd = 0;
                    }
                }
            }
        }
    }
    return v;
}

int main(void)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::vector<std::string> texts;
    std::vector<unsigned long long> result;
    std::string pattern;
    std::string text;
    std::string s;
    std::vector<unsigned long long> res;

    std::getline(std::cin, pattern);

    pattern = StringToLower(pattern);
    pattern = GetOfPattern(pattern);
    unsigned long long sh = 0;
    while (std::getline(std::cin, text)) {
        sh += 1;
        text = StringToLower(text);
        text = GetOfPattern(text);
        text = '$' + text;
        text = text + '$';
        for (unsigned long long i = 0; i < text.length(); i++) {
            if ((text[i] == ' ' && text[i + 1] == '0')) {
                if (text[i + 2] == '0') {
                    text.erase(text.begin() + i + 1);
                    --i;
                } else if (text[i + 2] != ' ' && text[i + 2] != '$') {
                    text.erase(text.begin() + i + 1);
                }
            } else if ((text[i] == '$' && text[i + 1] == '0')) {
                if (text[i + 2] == '0') {
                    text.erase(text.begin() + i + 1);
                    --i;
                } else if (text[i + 2] != ' ' && text[i + 2] != '$') {
                    text.erase(text.begin() + i + 1);
                }
            }
        }
        text.erase(text.end() - 1);
        s += text;
    }
    
    if (pattern == "") {
        return 0;     
    }

    pattern = '$' + pattern;
    pattern = pattern + '$';

    for (unsigned long long i = 0; i < pattern.length(); i++) {
        if ((pattern[i] == ' ' && pattern[i + 1] == '0' )) {
            if (pattern[i + 2] == '0') {
                pattern.erase(pattern.begin() + i + 1);
                --i;
            } else if (pattern[i + 2] != ' ' && pattern[i + 2] != '$') {
                pattern.erase(pattern.begin() + i + 1);
            }
        } else if ((pattern[i] == '$' && pattern[i + 1] == '0')) {
            if (pattern[i + 2] == '0') {
                pattern.erase(pattern.begin() + i + 1);
                --i;
            } else if (pattern[i + 2] != ' ' && pattern[i + 2] != '$') {
                pattern.erase(pattern.begin() + i + 1);
            }
        }
    }

    pattern.erase(pattern.begin());
    pattern.erase(pattern.end() - 1);

    pattern += ' ';
    pattern += '#';

    s = StringToLower(s);
    s += ' ';
    result = Z_Func(pattern, s);

    if (pattern[pattern.length() - 1] == ' ') {
        pattern.pop_back();
    }
    
    res = GetVecOFPos(result, StringToLower(s), pattern);
    for (unsigned long long i = 0; i < res.size(); i+= 2) {
        if (res[i + 1] != 0) {
            std::cout << res[i] << ", " << res[i + 1] << std::endl;
        }
    }

    return 0;
}