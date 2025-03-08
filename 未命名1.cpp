#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

// д��ص����������ڴ�����������ص�����
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
        return newLength;
    } catch (...) {
        return 0;
    }
}

int main() {
    CURL* curl;
    CURLcode res;

    string url = "https://api.siliconflow.cn/v1/chat/completions";

    // ����JSON payload
    json payload = {
        {"model", "Qwen/QwQ-32B"},
        {"messages", {{"role", "user"}, {"content", "What opportunities and challenges will the Chinese large model industry face in 2025?"}}},
        {"stream", false},
        {"max_tokens", 512},
        {"stop", nullptr},
        {"temperature", 0.7},
        {"top_p", 0.7},
        {"top_k", 50},
        {"frequency_penalty", 0.5},
        {"n", 1},
        {"response_format", {{"type", "text"}}},
        {"tools", {{"type", "function"}, {"function", {{"description", ""}, {"name", ""}, {"parameters", {}}, {"strict", false}}}}}
    };

    string token = "<��Կ>";

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        string response_string;

        // ����URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // ��������ͷ
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + token).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // ����POST����
        string post_data = payload.dump();
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());

        // ����д��ص�����
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        // ִ������
        res = curl_easy_perform(curl);

        // ��������Ƿ�ɹ�
        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        } else {
            cout << "Response: " << response_string << endl;
        }

        // ����
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}
