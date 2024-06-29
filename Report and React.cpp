#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <curl/curl.h>

using namespace std;

// Struct to hold accident record details
struct AccidentRecord {
    string date; // Format: day/month/year
    string vehicleType;
    string roadCondition;
    string timeOfDay;
    string weatherCondition;
    int numVehiclesInvolved;
    int numCasualties;
    string dayOfWeek;
    
    // Function to convert date to a comparable format (YYYYMMDD)
    int dateToComparable() const {
        int day, month, year;
        char delimiter;
        stringstream dateStream(date);
        dateStream >> day >> delimiter >> month >> delimiter >> year;
        return year * 10000 + month * 100 + day;
    }
};

// Function to read accident records from a CSV file
vector<AccidentRecord> readAccidentRecordsFromCSV(const string& filename) {
    vector<AccidentRecord> records;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Could not open the file!" << endl;
        return records;
    }

    string line;
    bool isFirstLine = true; // Skip header
    while (getline(file, line)) {
        if (isFirstLine) {
            // Check for BOM and remove if present
            if (line.substr(0, 3) == "\xEF\xBB\xBF") {
                line = line.substr(3);
            }
            isFirstLine = false;
            continue; // Skip header row
        }

        stringstream ss(line);
        string date, vehicleType, roadCondition, timeOfDay, weatherCondition, dayOfWeek, numVehiclesStr, numCasualtiesStr;

        getline(ss, date, ',');
        getline(ss, vehicleType, ',');
        getline(ss, roadCondition, ',');
        getline(ss, timeOfDay, ',');
        getline(ss, weatherCondition, ',');
        getline(ss, numVehiclesStr, ',');
        getline(ss, numCasualtiesStr, ',');
        getline(ss, dayOfWeek, ',');

        int numVehiclesInvolved = stoi(numVehiclesStr);
        int numCasualties = stoi(numCasualtiesStr);

        AccidentRecord record = {date, vehicleType, roadCondition, timeOfDay, weatherCondition, numVehiclesInvolved, numCasualties, dayOfWeek};
        records.push_back(record);
    }

    file.close();
    return records;
}

// Function to display the main menu
void displayMenu() {
    cout << "\t\t\t\t\t===========================================" << endl;
    cout << "\t\t\t\t\t||                                       ||" << endl;
    cout << "\t\t\t\t\t||    Road Accident Report and React     ||" << endl;
    cout << "\t\t\t\t\t||              Bukit Aman               ||" << endl;
    cout << "\t\t\t\t\t||                                       ||" << endl;
    cout << "\t\t\t\t\t===========================================" << endl;
    cout << "\t\t\t\t\t||                                       ||" << endl;
    cout << "\t\t\t\t\t||             1. Log In                 ||" << endl;
    cout << "\t\t\t\t\t||             2. Exit                   ||" << endl;
    cout << "\t\t\t\t\t||                                       ||" << endl;
    cout << "\t\t\t\t\t===========================================" << endl;
    cout << "\t\t\t\t\t||        Please enter your choice:      ||" << endl;
}

// Function to display accident reports
void displayReports(const vector<AccidentRecord>& records) {
    cout << endl;
    cout << "\t\t\t\t\tDisplaying accident reports:" << endl;
    cout << endl;
    for (size_t i = 0; i < records.size(); ++i) {
        cout << endl;
        const auto& record = records[i];
        cout << "\t\t\t\t\tAccident Record " << i + 1 << ": " << endl;
        cout << "\t\t\t\t\tDate: " << record.date << endl;
        cout << "\t\t\t\t\tVehicle Type: " << record.vehicleType << endl;
        cout << "\t\t\t\t\tRoad Condition: " << record.roadCondition << endl;
        cout << "\t\t\t\t\tTime of Day: " << record.timeOfDay << endl;
        cout << endl;
        cout << "\t\t\t\t\t----------------------------" << endl;
    }
}

// Function to authenticate the BARSA employees
bool authenticateUser() {
    string username, password;
    cout << "\t\t\t\t\t*******************************************" << endl;
    cout << "\t\t\t\t\tEnter username: ";
    cin >> username;
    cout << "\t\t\t\t\tEnter password: ";
    cin >> password;

    bool validUsername = (username == "fatihah" || username == "nabylah" || username == "amar" || username == "imran" || username == "syahmi");

    bool validPassword = (password == "BARSA" || password == "group5");

    // Return true only if both the username and password are valid
    return (validUsername && validPassword);
}

void logIn(){
    cout << endl;
    cout << "\t\t\t\t\t\t====================" << endl;
    cout << "\t\t\t\t\t\t Log In Successful" << endl;
    cout << "\t\t\t\t\t\t====================" << endl;
    cout << endl;
}

// Function to prompt user for sorting the reports
bool promptForSorting() {
    char choice;
    cout << "\t\t\t\tDo you want to sort the reports by date? (y/n): ";
    cin >> choice;
    return choice == 'y' || choice == 'Y';
}

void displayReportDetails(const AccidentRecord& record) {
    cout << endl;
    cout << "\t\t\t===================================== REPORT DETAILS =====================================" << endl;
    cout << "\t\t\t\t\t\tDate of Accident: " << record.date << " (" << record.dayOfWeek << ")" << endl;
    cout << "\t\t\t\t\t\tTime of Day: " << record.timeOfDay << endl;
    cout << "\t\t\t------------------------------------------------------------------------------------------" << endl;
    cout << "\t\t\t\t\t\tVehicle Involved: " << record.vehicleType << endl;
    cout << "\t\t\t\t\t\tNumber of Vehicles Involved: " << record.numVehiclesInvolved << endl;
    cout << "\t\t\t\t\t\tNumber of Casualties: " << record.numCasualties << endl;
    cout << "\t\t\t------------------------------------------------------------------------------------------" << endl;
    cout << "\t\t\t\t\t\tRoad Condition: " << record.roadCondition << endl;
    cout << "\t\t\t\t\t\tWeather Condition: " << record.weatherCondition << endl;
    cout << "\t\t\t------------------------------------------------------------------------------------------" << endl;
    cout << "\t\t\tNarrative Description: " << endl;
    cout << "\t\t\tOn " << record.date << " (" << record.dayOfWeek << "), at " << record.timeOfDay << ", a " << record.vehicleType << " was involved in a road accident." << endl;
    cout << "\t\t\tThe road condition at that time was " << record.roadCondition << ", and the weather condition was " << record.weatherCondition << "." << endl; 
    cout << "\t\t\tThere were " << record.numVehiclesInvolved << " vehicles involved in the accident, resulting in " << record.numCasualties << " casualties." << endl;
    cout << "\t\t\t------------------------------------------------------------------------------------------" << endl;
    cout << "\t\t\tThis accident can be reported to higher authorities such as Jabatan Siasatan dan "<< endl;
    cout << "\t\t\tPenguatkuasaan Trafik (JSPT), Dewan Bandaraya Kuala Lumpur (DBKL), and JKR Wilayah "<<endl;
    cout << "\t\t\tPersekutuan Kuala Lumpur for further investigation and action." << endl;
    cout << endl;
    cout << "\t\t\tAdditional informations! " << endl;
    cout << "\t\t\t1. JSPT:" << endl;
    cout << "\t\t\t- Responsible for the investigation of traffic accidents" << endl;
    cout << "\t\t\t- Responsible for the enforcement of traffic laws" << endl;
    cout << "\t\t\t2. DBKL:" << endl;
    cout << "\t\t\t- Responsible for public infrastructure in Kuala Lumpur, including roads." << endl;
    cout << "\t\t\t- Responsible for the inspection of roads throughout Kuala Lumpur." << endl;
    cout << "\t\t\t- Perform preventative measures to extend their lifespan and prevent further damage." << endl;
    cout << "\t\t\t3. JKR:" << endl;
    cout << "\t\t\t- Responsible for planning, constructing, and maintaining roads in Kuala Lumpur." << endl;
    cout << "\t\t\t- Provide technical support and consultancy for government infrastructure projects." << endl;
    cout << "\t\t\t- Ensure all infrastructure projects comply with regulatory and quality standards." << endl;
    cout << "\t\t\t==========================================================================================" << endl;
    cout << endl;
}

// Email sending function
struct upload_status {
    int lines_read;
    vector<string> payload;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
    struct upload_status *upload_ctx = (struct upload_status *)userp;
    const char *data;
    
    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
        return 0;
    }

    if (upload_ctx->lines_read >= upload_ctx->payload.size()) {
        return 0;
    }

    data = upload_ctx->payload[upload_ctx->lines_read].c_str();

    if (data) {
        size_t len = strlen(data);
        memcpy(ptr, data, len);
        upload_ctx->lines_read++;
        return len;
    }

    return 0;
}

string getRecipientEmail(const string& authority) {
    if (authority == "JSPT (Jabatan Siasatan dan Penguatkuasaan Trafik)") {
        return "jspt@example.com";
    } else if (authority == "DBKL (Dewan Bandaraya Kuala Lumpur)") {
        return "dbkl@example.com";
    } else if (authority == "JKR (Jabatan Kerja Raya, Wilayah Persekutuan Kuala Lumpur)") {
        return "jkr@example.com";
    } else {
        return "";
    }
}

int send_email(const AccidentRecord& record, const string& authority) {
    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;
    struct upload_status upload_ctx = { 0 };

    // Create the payload
    upload_ctx.payload.push_back("To: " + getRecipientEmail(authority) + "\r\n");
    upload_ctx.payload.push_back("From: sender@example.com\r\n");
    upload_ctx.payload.push_back("Subject: Complaint Regarding Road Accident at Bukit Aman\r\n");
    upload_ctx.payload.push_back("\r\n");
    upload_ctx.payload.push_back("Dear "+ authority + ", \r\n");
    upload_ctx.payload.push_back("\r\n");
    upload_ctx.payload.push_back("We hope this email finds you well. We are writing to express our concern regarding the frequent accidents occurring in the Bukit Aman area.\r\n");
    upload_ctx.payload.push_back("As an organisation responsible for monitoring and improving road safety, we have analyzed the accidents that pose a significant risk to the safety and well-being of all road users in this area.\r\n");
    upload_ctx.payload.push_back("\r\n");
    upload_ctx.payload.push_back("Here are the details about the accidents:.\r\n");
    upload_ctx.payload.push_back("Date: " + record.date + "\r\n");
    upload_ctx.payload.push_back("Vehicle Type: " + record.vehicleType + "\r\n");
    upload_ctx.payload.push_back("Road Condition: " + record.roadCondition + "\r\n");
    upload_ctx.payload.push_back("Time of Day: " + record.timeOfDay + "\r\n");
    upload_ctx.payload.push_back("Weather Condition: " + record.weatherCondition + "\r\n");
    upload_ctx.payload.push_back("Number of Vehicles Involved: " + to_string(record.numVehiclesInvolved) + "\r\n");
    upload_ctx.payload.push_back("Number of Casualties: " + to_string(record.numCasualties) + "\r\n");
    upload_ctx.payload.push_back("Day of the Week: " + record.dayOfWeek + "\r\n");
    upload_ctx.payload.push_back("\r\n");
    upload_ctx.payload.push_back("If these issues are not addressed promptly and effectively, many more accidents will occur. It is imperative that we take immediate action to prevent further incidents and ensure the safety of all road users in the Bukit Aman area. We believe that addressing these concerns can significantly reduce the number of accidents and improve road safety for all users in Bukit Aman.\r\n");
    upload_ctx.payload.push_back("\r\n");
    upload_ctx.payload.push_back("Thank you for your attention to this matter.\r\n");
    upload_ctx.payload.push_back("\r\n");
    upload_ctx.payload.push_back("“YOUR SAFETY IS OUR PRIORITY”\r\n");
    upload_ctx.payload.push_back("\r\n");
    upload_ctx.payload.push_back("Sincerely,\r\n");
    upload_ctx.payload.push_back("Bukit Aman Road Safety Association (BARSA)\r\n");
    upload_ctx.payload.push_back("+603 - 2613 4699 \r\n");


    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_USERNAME, "sender@example.com");

        // Use your actual app-specific password or less secure app access enabled password here
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "sender's app password");

        curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");

        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "<sender@example.com>");
        
        recipients = curl_slist_append(recipients, ("<" + getRecipientEmail(authority) + ">").c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        // Bypass SSL certificate verification (for development purposes only)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        // Alternatively, specify the CA certificate bundle path
        // curl_easy_setopt(curl, CURLOPT_CAINFO, "/path/to/cacert.pem");

        upload_ctx.lines_read = 0;

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
    return (int)res;
}

// Function to notify the authorities
void notifyAuthority(const string& authority, const AccidentRecord& record) {
    cout << "\t\t\t*************************************************************************************" << endl;
    cout << "\t\t\tNotifying " << authority << " about this road accident:" << endl;
    cout << endl; 
    cout << "\t\t\t================================== Accident Report ==================================" << endl;
    cout << "\t\t\t\t\t\t\tDate: " << record.date << endl;
    cout << "\t\t\t\t\t\t\tVehicle Type: " << record.vehicleType << endl;
    cout << "\t\t\t\t\t\t\tRoad Condition: " << record.roadCondition << endl;
    cout << "\t\t\t\t\t\t\tTime of Day: " << record.timeOfDay << endl;
    cout << "\t\t\t*************************************************************************************" << endl;

    // Send email notification
    send_email(record, authority);
}

// Function to prompt user for notifying authorities
void notifyAuthorities(const AccidentRecord& record) {
    string choice;
    cout << "\t\t\t==========================================================================================" << endl;
    cout << "\t\t\t\tSelect an authority to notify:" << endl;
    cout << endl;
    cout << "\t\t\t\t1. Notify JSPT (Jabatan Siasatan dan Penguatkuasaan Trafik)" << endl;
    cout << "\t\t\t\t2. Notify DBKL (Dewan Bandaraya Kuala Lumpur)" << endl;
    cout << "\t\t\t\t3. Notify JKR (Jabatan Kerja Raya, Wilayah Persekutuan Kuala Lumpur)" << endl;
    cout << "\t\t\t\t4. Notify all" << endl;
    cout << "\t\t\t\t5. Notify any two authorities" << endl;
    cout << "\t\t\t\tX. Return to main menu" << endl;
    cout << "\t\t\t\t**************************************************************" << endl;
    cout << "\t\t\t\tEnter your choice: ";
    cin >> choice;

    if (choice == "X" || choice == "x") return;
    
    switch (stoi(choice)) {
        case 1:
            notifyAuthority("JSPT (Jabatan Siasatan dan Penguatkuasaan Trafik)", record);
            break;
        case 2:
            notifyAuthority("DBKL (Dewan Bandaraya Kuala Lumpur)", record);
            break;
        case 3:
            notifyAuthority("JKR (Jabatan Kerja Raya, Wilayah Persekutuan Kuala Lumpur)", record);
            break;
        case 4:
            notifyAuthority("JSPT (Jabatan Siasatan dan Penguatkuasaan Trafik)", record);
            notifyAuthority("DBKL (Dewan Bandaraya Kuala Lumpur)", record);
            notifyAuthority("JKR (Jabatan Kerja Raya)", record);
            break;
        case 5:
        {
            int firstChoice, secondChoice;
            cout << "\t\t\t\tEnter the first authority to notify (1 for JSPT, 2 for DBKL, 3 for JKR): ";
            cin >> firstChoice;
            cout << "\t\t\t\tEnter the second authority to notify (1 for JSPT, 2 for DBKL, 3 for JKR): ";
            cin >> secondChoice;

            if (firstChoice == secondChoice || firstChoice < 1 || firstChoice > 3 || secondChoice < 1 || secondChoice > 3) {
                cout << "Invalid choices. No notifications sent." << endl;
            } else {
                if (firstChoice == 1 || secondChoice == 1) notifyAuthority("JSPT (Jabatan Siasatan dan Penguatkuasaan Trafik)", record);
                if (firstChoice == 2 || secondChoice == 2) notifyAuthority("DBKL (Dewan Bandaraya Kuala Lumpur)", record);
                if (firstChoice == 3 || secondChoice == 3) notifyAuthority("JKR (Jabatan Kerja Raya)", record);
            }
        }
            break;
        default:
            cout << "Invalid choice. No notification sent. Returning to main menu." << endl;
    }
}

// Function to access report details
void accessReportDetails(const AccidentRecord& record) {
    displayReportDetails(record);
    notifyAuthorities(record);
}

// Function to filter reports
void filterReports(vector<AccidentRecord>& accidentRecords) {
    while (true) {
        string userChoice;
        cout << endl;
        cout << "\t\t\t\t\t1. Access any report" << endl;
        cout << "\t\t\t\t\t2. Sort reports further by year" << endl;
        cout << "\t\t\t\t\tX. Return to main menu" << endl;
        cout << "\t\t\t\t\tEnter your choice: ";
        cin >> userChoice;

        if (userChoice == "X" || userChoice == "x") break;

        if (userChoice == "1") {
            string reportNumber;
            cout << "\t\t\t\t\tEnter the number of the report you want to access: ";
            cin >> reportNumber;
            if (stoi(reportNumber) > 0 && stoi(reportNumber) <= accidentRecords.size()) {
                accessReportDetails(accidentRecords[stoi(reportNumber) - 1]);
            } else {
                cout << "\t\t\t\t\tInvalid report number." << endl;
            }
            break;
        } else if (userChoice == "2") {
            string year;
            cout << "\t\t\t\t\tEnter the year to sort by: ";
            cin >> year;
            vector<AccidentRecord> filteredRecords;
            for (const auto& record : accidentRecords) {
                if (record.date.substr(record.date.size() - 4) == year) {
                    filteredRecords.push_back(record);
                }
            }
            if (filteredRecords.empty()) {
                cout << "\t\t\t\t\tNo records found for the year " << year << "." << endl;
            } else {
                displayReports(filteredRecords);
                string furtherChoiceVehicles;
                cout << "\t\t\t\t\t1. Access any of the year sorted reports" << endl;
                cout << "\t\t\t\t\t2. Sort reports further by vehicle type" << endl;
                cout << "\t\t\t\t\tX. Return to main menu" << endl;
                cout << "\t\t\t\t\tEnter your choice: ";
                cin >> furtherChoiceVehicles;

                if (furtherChoiceVehicles == "X" || furtherChoiceVehicles == "x") break;

                if (furtherChoiceVehicles == "1") {
                    string reportNumber;
                    cout << "\t\t\t\t\tEnter the number of the report you want to access: ";
                    cin >> reportNumber;
                    if (stoi(reportNumber) > 0 && stoi(reportNumber) <= filteredRecords.size()) {
                        accessReportDetails(filteredRecords[stoi(reportNumber) - 1]);
                    } else {
                        cout << "\t\t\t\t\tInvalid report number." << endl;
                    }
                    break;
                } else if (furtherChoiceVehicles == "2") {
                    string vehicleType;
                    cout << "\t\t\t\t\tRemember: This section is Case Sensitive!" << endl;
                    cout << "\t\t\t\tEnter the vehicle type to sort by (Car/Motorcycle/Truck/Pedestrian/Bicycle): ";
                    cin >> vehicleType;
                    vector<AccidentRecord> vehicleFilteredRecords;
                    for (const auto& record : filteredRecords) {
                        if (record.vehicleType == vehicleType) {
                            vehicleFilteredRecords.push_back(record);
                        }
                    }
                    if (vehicleFilteredRecords.empty()) {
                        cout << "\t\t\t\t\tNo records found for vehicle type " << vehicleType << "." << endl;
                    } else {
                        displayReports(vehicleFilteredRecords);

                        string furtherChoiceRoad;
                        cout << "\t\t\t\t\t1. Access any of the vehicle type sorted reports" << endl;
                        cout << "\t\t\t\t\t2. Sort reports further by road condition" << endl;
                        cout << "\t\t\t\t\tX. Return to main menu" << endl;
                        cout << "\t\t\t\t\tEnter your choice: ";
                        cin >> furtherChoiceRoad;

                        if (furtherChoiceRoad == "X" || furtherChoiceRoad == "x") break;

                        if (furtherChoiceRoad == "1") {
                            string reportNumber;
                            cout << "\t\t\t\t\tEnter the number of the report you want to access: ";
                            cin >> reportNumber;
                            if (stoi(reportNumber) > 0 && stoi(reportNumber) <= vehicleFilteredRecords.size()) {
                                accessReportDetails(vehicleFilteredRecords[stoi(reportNumber) - 1]);
                            } else {
                                cout << "\t\t\t\t\tInvalid report number." << endl;
                            }
                            break;
                        } else if (furtherChoiceRoad == "2") {
                            string roadCondition;
                            cout << "\t\t\t\t\tEnter the road condition to sort by (Good/Bad): ";
                            cin >> roadCondition;
                            vector<AccidentRecord> roadFilteredRecords;
                            for (const auto& record : vehicleFilteredRecords) {
                                if (record.roadCondition == roadCondition) {
                                    roadFilteredRecords.push_back(record);
                                }
                            }
                            if (roadFilteredRecords.empty()) {
                                cout << "\t\t\t\t\tNo records found for road condition " << roadCondition << "." << endl;
                            } else {
                                displayReports(roadFilteredRecords);

                                string furtherChoiceTime;
                                cout << "\t\t\t\t\t1. Access any of the road condition sorted reports" << endl;
                                cout << "\t\t\t\t\t2. Sort reports further by time of day" << endl;
                                cout << "\t\t\t\t\tX. Return to main menu" << endl;
                                cout << "\t\t\t\t\tEnter your choice: ";
                                cin >> furtherChoiceTime;

                                if (furtherChoiceTime == "X" || furtherChoiceTime == "x") break;

                                if (furtherChoiceTime == "1") {
                                    string reportNumber;
                                    cout << "\t\t\t\t\tEnter the number of the report you want to access: ";
                                    cin >> reportNumber;
                                    if (stoi(reportNumber) > 0 && stoi(reportNumber) <= roadFilteredRecords.size()) {
                                        accessReportDetails(roadFilteredRecords[stoi(reportNumber) - 1]);
                                    } else {
                                        cout << "\t\t\t\t\tInvalid report number." << endl;
                                    }
                                    break;
                                } else if (furtherChoiceTime == "2") {
                                    string timeOfDay;
                                    cout << "\t\t\t\t\tEnter the time of day to sort by (Day/Night): ";
                                    cin >> timeOfDay;
                                    vector<AccidentRecord> timeFilteredRecords;
                                    for (const auto& record : roadFilteredRecords) {
                                        if (record.timeOfDay == timeOfDay) {
                                            timeFilteredRecords.push_back(record);
                                        }
                                    }
                                    if (timeFilteredRecords.empty()) {
                                        cout << "\t\t\t\t\tNo records found for time of day " << timeOfDay << "." << endl;
                                    } else {
                                        displayReports(timeFilteredRecords);

                                        string reportNumber;
                                        cout << "\t\t\t\t\tEnter the number of the report you want to access: ";
                                        cin >> reportNumber;
                                        if (stoi(reportNumber) > 0 && stoi(reportNumber) <= timeFilteredRecords.size()) {
                                            accessReportDetails(timeFilteredRecords[stoi(reportNumber) - 1]);
                                        } else {
                                            cout << "\t\t\t\t\tInvalid report number." << endl;
                                        }
                                        break;
                                    }
                                } else {
                                    cout << "\t\t\t\t\tInvalid choice. Please try again." << endl;
                                }
                            }
                        } else {
                            cout << "\t\t\t\t\tInvalid choice. Please try again." << endl;
                        }
                    }
                } else {
                    cout << "\t\t\t\t\tInvalid choice. Please try again." << endl;
                }
            }
        } else {
            cout << "\t\t\t\t\tInvalid choice. Please try again." << endl;
        }
    }
}

// Partition function for Quick Sort
int partition(vector<AccidentRecord>& records, int low, int high) {
    AccidentRecord pivot = records[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (records[j].dateToComparable() > pivot.dateToComparable()) {
            i++;
            swap(records[i], records[j]);
        }
    }
    swap(records[i + 1], records[high]);
    return (i + 1);
}

void quickSort(vector<AccidentRecord>& records, int low, int high) {
    if (low < high) {
        int pi = partition(records, low, high);

        quickSort(records, low, pi - 1);
        quickSort(records, pi + 1, high);
    }
}

int main() {
    string filename = "Data Sets.csv";
    vector<AccidentRecord> accidentRecords = readAccidentRecordsFromCSV(filename);
    
    if (accidentRecords.empty()) {
        cerr << "No records found. Exiting the program." << endl;
        return 1;
    }
    
    int choice;
    while (true) {
        displayMenu();
        cout << "\t\t\t\t\t\t   User choice -> " ;
        cin >> choice;
        
        switch (choice) {
            case 1:
                if (authenticateUser()){ {
                    logIn();
                    displayReports(accidentRecords);
                    if (promptForSorting()) {
                        cout << endl;
                        cout <<"\t\t\t\tDisplaying sorted reports (From Latest to Oldest): " << endl;
                        cout <<"\t\t\t\t-------------------------------------------------- " << endl;
                        quickSort(accidentRecords, 0, accidentRecords.size() - 1);
                    }
                    displayReports(accidentRecords);}
                    filterReports(accidentRecords);
                } else {
                    cout << "\t\t\t\t\tAuthentication failed. Returning to main menu." << endl;
                }
                break;
            case 2:
                cout << "\t\t\t\t\t\t=======================" << endl;
                cout << "\t\t\t\t\t\t Exiting the program..." << endl;
                cout << "\t\t\t\t\t\t=======================" << endl;
                return 0;
            default:
                cout << "\t\t\t\t\t===================================" << endl;
                cout << "\t\t\t\t\t Invalid choice. Please try again." << endl;
                cout << "\t\t\t\t\t===================================" << endl;
        }
    }
    return 0;
}

