# HTML Entities Decoder
A simple header-only HTML Entities String Decoder for modern C++ (≥C++17).

## Usage
```
#include <iostream>
#include "html_entities_decoder.hpp"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");  // Optional, only needed if you want to print wstring on screen
    html_entities_decoder::html_entities_decoder hed;
 
    string html_text_s = "&quot;html&nbsp;entities&nbsp;decoder&quot;";
    wstring html_text_ws = L"&#x54c8;&#x58eb;&#x5947;";
    wchar_t html_text[] = L"&amp; 5 &lt; 9 &gt; 2, &#101;&#110;&#100;";

    cout << hed.decode_html_entities(html_text_s) << endl;
    cout << "-------------" << endl;    
    wcout << hed.decode_html_entities(html_text_ws) << endl;
    wcout << L"-------------" << endl;
    wcout << hed.decode_html_entities(html_text, sizeof(html_text)) << endl;

    return 0;
}
```

Output:
```
"html entities decoder"
-------------
哈士奇
-------------
& 5 < 9 > 2, end
```

## Note
All of the decoded strings (return values) are STL `basic_string` (`string` / `wstring` / `u16string` / `u32string` ; and `u8string` in C++20)
