//NOTE: Steam API key is replaced with X's to keep it private

#include <iostream>
#include <vector>
#include <string>
#include <curl/curl.h>
using namespace std;


//idk how exactly the whole http thing here works bc i copied and pasted it..BUT IT WORKS and thats all that matters
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

int main(void)
{
  CURL* curl;
  CURLcode res;
  std::string readBuffer;
  int counter = 0;
  int insertGame = 0;
  vector<string> games;
  char findGames[50];

  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);  // redirects
  // bonus:
  curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, 1L); // corp. proxies etc.
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, 
      "http://api.steampowered.com/IPlayerService/GetOwnedGames/v0001/?key=XXXXXXXXXXXXXXX&steamid=76561199061343514&include_appinfo=true&format=json");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    //std::cout << readBuffer << std::endl;

    //loop that iterates for every char in buffer
    for (int i = 0; i < readBuffer.length(); i++) { 

      //if "name" is found in the buffer
      if (readBuffer[i] == 'n' && readBuffer[i + 1] == 'a' 
        && readBuffer[i + 2] == 'm' && readBuffer[i + 3] == 'e') {
        
        //counter is set to the first char of the game name
        counter = i + 7;

        //inserts each char into an array until the end of the name is found by a "
        while (readBuffer[counter] != '"') {
          findGames[insertGame] = readBuffer[counter];
          counter++;
          insertGame++;
        }

        //inserts game name into vector
        games.push_back(findGames);
        counter = 0;
        insertGame = 0;

        //NULL's the char array so that longer game names are still not present on the next iteration
        for (int i = 0; i < 50; i++) {
          findGames[i] = '\0';
        }


      }

    }


  }

  //outputs the vector of game names
  for (int i = 0; i < games.size(); i++) {
    cout << games[i] << endl;
  }

  return 0;
}