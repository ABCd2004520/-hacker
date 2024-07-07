#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>

// �������ݽṹ
struct HealthData {
    int heartRate;
    int bloodPressure;
    std::string date;
};

// �ص����������ڴ���cURL��Ӧ
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

// ����AI API�ĺ���
std::string callAIService(const std::string& prompt) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.example.com/ai"); // �滻Ϊʵ�ʵ�AI����URL
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

// �������麯��
void displayHealthAdvice(const HealthData& data) {
    std::string prompt = "{ \"heartRate\": " + std::to_string(data.heartRate) + 
                         ", \"bloodPressure\": " + std::to_string(data.bloodPressure) + " }";
    std::string response = callAIService(prompt);

    std::cout << "��������:\n";
    std::cout << response << "\n";
}

// ��ʾ��������
void displayHealthRecords(const std::vector<HealthData>& records) {
    std::cout << "\n��������:\n";
    for (const auto& record : records) {
        std::cout << "����: " << record.date 
                  << ", ����: " << record.heartRate 
                  << ", Ѫѹ: " << record.bloodPressure << "\n";
    }
}

// ������
int main() {
    std::vector<HealthData> healthRecords;
    char choice;

    do {
        HealthData data;
        
        std::cout << "���������� (��ʽ: YYYY-MM-DD): ";
        std::cin >> data.date;
        std::cout << "���������� (��/����): ";
        std::cin >> data.heartRate;
        std::cout << "������Ѫѹ (mmHg): ";
        std::cin >> data.bloodPressure;

        healthRecords.push_back(data);
        displayHealthAdvice(data);

        std::cout << "\n�Ƿ�������뽡������? (y/n): ";
        std::cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    displayHealthRecords(healthRecords);

    return 0;
}

