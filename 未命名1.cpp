#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>

// 健康数据结构
struct HealthData {
    int heartRate;
    int bloodPressure;
    std::string date;
};

// 回调函数，用于处理cURL响应
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch (std::bad_alloc& e) {
        // Handle memory problem
        return 0;
    }
    return newLength;
}

// 调用AI API的函数
std::string callAIService(const std::string& prompt) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.example.com/ai"); // 替换为实际的AI服务URL
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, prompt.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return readBuffer;
}

// 健康建议函数
void displayHealthAdvice(const HealthData& data) {
    std::string prompt = "{ \"heartRate\": " + std::to_string(data.heartRate) + 
                         ", \"bloodPressure\": " + std::to_string(data.bloodPressure) + " }";
    std::string response = callAIService(prompt);

    std::cout << "健康建议:\n";
    std::cout << response << "\n";
}

// 显示健康档案
void displayHealthRecords(const std::vector<HealthData>& records) {
    std::cout << "\n健康档案:\n";
    for (const auto& record : records) {
        std::cout << "日期: " << record.date 
                  << ", 心率: " << record.heartRate 
                  << ", 血压: " << record.bloodPressure << "\n";
    }
}

// 主函数
int main() {
    std::vector<HealthData> healthRecords;
    char choice;

    do {
        HealthData data;
        
        std::cout << "请输入日期 (格式: YYYY-MM-DD): ";
        std::cin >> data.date;
        std::cout << "请输入心率 (次/分钟): ";
        std::cin >> data.heartRate;
        std::cout << "请输入血压 (mmHg): ";
        std::cin >> data.bloodPressure;

        healthRecords.push_back(data);
        displayHealthAdvice(data);

        std::cout << "\n是否继续输入健康数据? (y/n): ";
        std::cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    displayHealthRecords(healthRecords);

    return 0;
}

